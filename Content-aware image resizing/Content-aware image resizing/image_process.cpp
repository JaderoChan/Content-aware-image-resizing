#include "image_process.hpp"

cv::Mat enhanceLightness(const cv::Mat& img, double multiple)
{
    return img * multiple;
}

cv::Mat limitSize(const cv::Mat& img, int maxWidth, int maxHeight)
{
    // 原图像宽高。
    int oWidth = img.cols;
    int oHeight = img.rows;

    // 若原图宽高均在指定大小内，则不作处理。
    if (oWidth <= maxWidth && oHeight <= maxHeight)
        return cv::Mat(img);

    // 获得限定宽高与原图像宽高比值，取小者。
    double r1 = static_cast<double>(maxWidth) / oWidth;
    double r2 = static_cast<double>(maxHeight) / oHeight;
    double ratio = r1 < r2 ? r1 : r2;

    cv::Mat result;
    cv::resize(img, result, cv::Size(oWidth * ratio, oHeight * ratio), 0.0, 0.0, cv::InterpolationFlags::INTER_AREA);
    return result;
}
