#pragma once

#include <opencv2/opencv.hpp>

/// @brief 增强图像亮度。
/// @param img 图像。
/// @param multiple 用于乘以图像矩阵的倍数。
cv::Mat lightnessEnhance(const cv::Mat& img, double multiple);

/// @brief 将图像缩放在指定大小内，且保留长宽比。
/// @param img 图像。
/// @param maxWidth 最大宽度。
/// @param maxHeight 最大高度。
cv::Mat limitedSize(const cv::Mat& img, int maxWidth, int maxHeight);
