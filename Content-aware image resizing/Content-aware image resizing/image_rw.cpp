#include "image_rw.hpp"

cv::Mat loadImage(const std::string& filename)
{
    cv::Mat img = cv::imread(filename);
    img.convertTo(img, CV_64FC3);
    return img;
}

void saveImage(const cv::Mat& img, const std::string& filename)
{
    cv::Mat convertedImg;
    img.convertTo(convertedImg, CV_8UC3);
    cv::imwrite(filename, convertedImg);
}
