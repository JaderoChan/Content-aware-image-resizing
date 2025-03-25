#pragma once

#include <qwidget.h>
#include <qevent.h>
#include <qstring.h>
#include <qmovie.h>

#include "ui_mainwindow.h"
#include "work_thread.h"
#include "content_aware_resizing.hpp"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    double lightEnchancePercent() const;

    void setSourceImageByFile(const QString& srcImgPath);
    void setDestinationImageByFile(const QString& dstImgPath);

    void startLoading();
    void endLoading();

protected slots:
    void onOpenImageButtonClicked();
    void onSaveImageButtonClicked();
    void onLightnessSliderChanged(int value);
    void onRemoveLinesSliderChanged(int value);
    void onGenerateEnergyImageButtonClicked();
    void onGenerateRemovedImageButtonClicked();
    void onGenerateFinished();

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    static void setLabelImage_(QLabel* label, const QPixmap& image);
    void updateParameterPack() const;

    Ui::MainWindowClass ui;
    QMovie* loading_;

    QString defaultDir_;
    QString srcImgPath_;

    WorkThread* workTh_;
    qint64 beginSec_;
};
