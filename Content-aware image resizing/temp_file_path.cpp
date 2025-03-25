#include "temp_file_path.h"

#include <qdir.h>

QString tempFilePath1()
{
    return QDir::temp().filePath("content-aware_image_resizing_temp1.jpg");
}

QString tempFilePath2()
{
    return QDir::temp().filePath("content-aware_image_resizing_temp2.jpg");
}
