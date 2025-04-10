#include "work_thread.h"

#include "image_rw.hpp"
#include "temp_file_path.h"

WorkThread::WorkThread(QObject* parent) :
    QThread(parent)
{}

WorkThread::~WorkThread()
{}

void WorkThread::setTaskType(TaskType taskType)
{
    assignValue_(taskType_, taskType);
}

void WorkThread::setInPath(const QString& path)
{
    assignValue_(inPath_, path);
}

void WorkThread::setOutPath(const QString& path)
{
    assignValue_(outPath_, path);
}

void WorkThread::setRemovedLineCount(int count)
{
    assignValue_(removedLineCount_, count);
}

void WorkThread::run()
{
    shouldExit_ = false;

    switch (taskType_)
    {
        case TT_GEN_REMOVED_LINES_IMAGE:
        {
            auto img = loadImage(inPath_.toLocal8Bit().toStdString());
            for (int i = 0; i < removedLineCount_; ++i)
            {
                if (shouldExit_)
                    return;
                img = removeMinimumEnergyCloumn(img);
            }
            saveImage(img, tempFilePath1().toLocal8Bit().toStdString());
            break;
        }
        case TT_GEN_ENERGY_IMAGE:
        {
            auto img = loadImage(inPath_.toLocal8Bit().toStdString());
            img = makeEnergyImage(img);
            saveImage(img, tempFilePath1().toLocal8Bit().toStdString());

            break;
        }
        default:
            break;
    }
}
