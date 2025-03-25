#pragma once

#include <qstring>
#include <qthread.h>

#include "content_aware_resizing.hpp"

class WorkThread : public QThread
{
    Q_OBJECT

public:
    enum TaskType
    {
        TT_GEN_REMOVED_LINES_IMAGE,
        TT_GEN_ENERGY_IMAGE
    };

    WorkThread(QObject* parent = nullptr);
    ~WorkThread();

    void setTaskType(TaskType taskType);
    void setInPath(const QString& path);
    void setOutPath(const QString& path);
    void setRemovedLineCount(int count);

    void run() override;

private:
    template <typename T>
    bool assignValue_(T& target, const T& value)
    {
        if (!isRunning())
        {
            target = value;
            return true;
        }

        return false;
    }

    TaskType taskType_              = TT_GEN_REMOVED_LINES_IMAGE;
    QString inPath_;
    QString outPath_;
    int removedLineCount_           = 1;
};
