#pragma once

#include <string>

#include <opencv2/opencv.hpp>

/// @brief 从文件读取图像。
/// @param filename 图像文件路径。
/// @return CV_64FC3类型图像。
cv::Mat loadImage(const std::string& filename);

/// @brief 保存图像。
/// @param img CV_64FC3类型图像。
/// @param filename 保存文件路径。
void saveImage(const cv::Mat& img, const std::string& filename);
