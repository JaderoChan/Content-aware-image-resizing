#include "mainwindow.h"

#include <qapplication.h>
#include <qdatetime.h>
#include <qdir.h>
#include <qfiledialog.h>
#include <qpixmap.h>
#include <qmimedata.h>

#include "image_process.hpp"
#include "image_rw.hpp"
#include "parameter_pack.hpp"
#include "temp_file_path.h"

MainWindow::MainWindow(QWidget* parent) :
    QWidget(parent), loading_(new QMovie(":/resource/image/loading.gif")), workTh_(new WorkThread())
{
    ui.setupUi(this);

    ui.loadingLbl->setMovie(loading_);

    ui.srcImgLbl->installEventFilter(this);
    ui.dstImgLbl->installEventFilter(this);

    connect(ui.openSrcBtn, &QPushButton::clicked, this, &MainWindow::onOpenImageButtonClicked);
    connect(ui.saveDstBtn, &QPushButton::clicked, this, &MainWindow::onSaveImageButtonClicked);
    connect(ui.lightnessSldr, &QSlider::valueChanged, this, &MainWindow::onLightnessSliderChanged);
    connect(ui.removeLinesSldr, &QSlider::valueChanged, this, &MainWindow::onRemoveLinesSliderChanged);
    connect(ui.generateEnergyImgBtn, &QPushButton::clicked, this, &MainWindow::onGenerateEnergyImageButtonClicked);
    connect(ui.generateRemovedImgBtn, &QPushButton::clicked, this, &MainWindow::onGenerateRemovedImageButtonClicked);

    connect(workTh_, &QThread::finished, this, &MainWindow::onGenerateFinished);
}

MainWindow::~MainWindow()
{
    workTh_->exit();
    QFile(tempFilePath1()).remove();
    QFile(tempFilePath2()).remove();
}

double MainWindow::lightEnchancePercent() const
{
    return static_cast<double>(ui.lightnessSldr->value() * 10) / 100.0 + 1.0;
}

void MainWindow::setSourceImageByFile(const QString& srcImgPath)
{
    QPixmap img;
    if (img.load(srcImgPath))
    {
        setLabelImage_(ui.srcImgLbl, img);
        ui.srcImgPathLbl->setText(srcImgPath);

        ui.removeLinesSldr->setMaximum(img.width() * 0.8);
        ui.removeLinesSldr->setValue(ui.removeLinesSldr->maximum() * 0.5);
        ui.removeLinesSldr->setEnabled(true);
    }
}

void MainWindow::setDestinationImageByFile(const QString& dstImgPath)
{
    QPixmap pixmap;
    if (pixmap.load(tempFilePath1()))
    {
        auto img = loadImage(dstImgPath.toLocal8Bit().toStdString());
        img = lightnessEnhance(img, lightEnchancePercent());
        saveImage(img, tempFilePath2().toLocal8Bit().toStdString());

        if (pixmap.load(tempFilePath2()))
            setLabelImage_(ui.dstImgLbl, pixmap);
    }
}

void MainWindow::startLoading()
{
    ui.loadingLbl->show();
    loading_->start();

    beginSec_ = QDateTime::currentDateTime().toSecsSinceEpoch();
}

void MainWindow::endLoading()
{
    loading_->stop();
    ui.loadingLbl->hide();

    qint64 endSecs = QDateTime::currentDateTime().toSecsSinceEpoch();
    qint64 elapsedSec = endSecs - beginSec_;
    ui.elapsedTimeLbl->setNum(static_cast<int>(elapsedSec));
}

void MainWindow::showAlert()
{
    qApp->alert(this);
}

void MainWindow::onOpenImageButtonClicked()
{
    if (defaultDir_.isEmpty())
        defaultDir_ = QDir::home().path();

    QString path = QFileDialog::getOpenFileName(this,
                                                tr("Open the image"),
                                                defaultDir_,
                                                "*.jpg; *.png; *.jpeg");
    defaultDir_ = QFileInfo(path).dir().path();

    QPixmap srcImg;
    if (srcImg.load(path))
    {
        srcImgPath_ = path;
        setSourceImageByFile(srcImgPath_);
    }
}

void MainWindow::onSaveImageButtonClicked()
{
    QPixmap dstImage;
    if (dstImage.load(tempFilePath2()))
    {
        QString savePath = QFileDialog::getSaveFileName(this,
                                                        tr("Save the image"),
                                                        srcImgPath_,
                                                        "*.jpg *.png *.jpeg");
        dstImage.save(savePath);
    }
}

void MainWindow::onLightnessSliderChanged(int value)
{
    setDestinationImageByFile(tempFilePath1());
    ui.lightnessValueLbl->setNum(value);
}

void MainWindow::onRemoveLinesSliderChanged(int value)
{
    ui.removeLinestValueLbl->setNum(value);
}

void MainWindow::onGenerateEnergyImageButtonClicked()
{
    if (srcImgPath_.isEmpty())
        return;

    updateParameterPack();

    workTh_->setInPath(srcImgPath_);
    workTh_->setOutPath(tempFilePath1());
    workTh_->setTaskType(WorkThread::TT_GEN_ENERGY_IMAGE);
    workTh_->start();

    ui.generateWgt->setEnabled(false);
    startLoading();
}

void MainWindow::onGenerateRemovedImageButtonClicked()
{
    if (srcImgPath_.isEmpty())
        return;

    updateParameterPack();

    workTh_->setInPath(srcImgPath_);
    workTh_->setOutPath(tempFilePath1());
    workTh_->setRemovedLineCount(ui.removeLinesSldr->value());
    workTh_->setTaskType(WorkThread::TT_GEN_REMOVED_LINES_IMAGE);
    workTh_->start();

    ui.generateWgt->setEnabled(false);
    startLoading();
}

void MainWindow::onGenerateFinished()
{
    endLoading();
    showAlert();
    ui.generateWgt->setEnabled(true);
    setDestinationImageByFile(tempFilePath1());
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui.srcImgLbl || watched == ui.dstImgLbl)
    {
        switch (event->type())
        {
            // 当QLabel接收到Resize事件时，更改显示图片的大小以适应QLabel。
            case QEvent::Type::Resize:
            {
                if (watched == ui.srcImgLbl)
                {
                    setSourceImageByFile(srcImgPath_);
                }
                else if (watched == ui.dstImgLbl)
                {
                    setDestinationImageByFile(tempFilePath1());
                }

                break;
            }
            case QEvent::Type::DragEnter:
            {
                if (watched != ui.srcImgLbl)
                    break;

                auto e = dynamic_cast<QDragEnterEvent*>(event);
                auto data = e->mimeData();
                data->hasUrls() ? e->acceptProposedAction() : e->ignore();

                break;
            }
            case QEvent::Type::Drop:
            {
                if (watched != ui.srcImgLbl)
                    break;

                auto e = dynamic_cast<QDropEvent*>(event);
                auto data = e->mimeData();

                if (data->hasUrls())
                {
                    QPixmap pixmap;
                    auto path = data->urls()[0].toLocalFile();
                    if (pixmap.load(path))
                    {
                        srcImgPath_ = path;
                        setSourceImageByFile(srcImgPath_);
                    }
                }

                break;
            }
            default:
                break;
        }
    }

    return QWidget::eventFilter(watched, event);
}

void MainWindow::setLabelImage_(QLabel* label, const QPixmap& image)
{
    QPixmap img = image.scaled(label->size(), Qt::KeepAspectRatio);
    label->setPixmap(img);
}

void MainWindow::updateParameterPack() const
{
    ComputeColorSimilarityMethod ccsMethod = CCS_NORMAL;
    if (ui.normalRdoBtn->isChecked())
        ccsMethod = CCS_NORMAL;
    else if (ui.mod1RdoBtn->isChecked())
        ccsMethod = CCS_MODIFIED1;
    else if (ui.mod2RdoBtn->isChecked())
        ccsMethod = CCS_MODIFIED2;

    bool isAllDirectionForEnergy = ui.trueRdoBtn->isChecked();

    setGlobalCcsMethod(ccsMethod);
    setGlobalIsAllDirectForEnergy(isAllDirectionForEnergy);
}
