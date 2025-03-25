#pragma once

#include <opencv2/opencv.hpp>

enum ComputeColorSimilarityMethod
{
    // 仅计算颜色三通道的距离平方和。
    CCS_NORMAL,
    // 根据人眼对三原色的灵敏度不同进行一定程度的修正。
    CCS_MODIFIED1,
    // 根据人眼对三原色的灵敏度不同进行一定程度的修正。
    CCS_MODIFIED2,
};

/// @brief 判断给定点是否位于给定矩阵之外。
bool isOverEdge(const cv::Mat& mat, const cv::Point& point);

/// @brief 计算两个颜色的相似度，相似度越高，返回值越小。
/// @param color1 三通道double型颜色数据。
/// @param color2 三通道double型颜色数据。
/// @param ccsMethod 颜色相似度的计算方法。
/// @return 颜色相似度。
double computeColorSimilarity(const cv::Vec3d& color1, const cv::Vec3d& color2, ComputeColorSimilarityMethod ccsMethod);

/// @brief 计算图像中给定点的能量大小（给定点与周围各点的相似度均值）
/// @param img 图像。
/// @param point 坐标点。
/// @param ccsMethod 颜色相似度的计算方法。
/// @param isAllDirection 若为true，则计算给定点与其周围八方位上的点，否则只计算上下左右四方位上的点。
/// @return 给定点能量值，能量值越高代表此处细节越丰富（相似度越低）。
double computePointEnergy(const cv::Mat& img, const cv::Point& point,
                          ComputeColorSimilarityMethod ccsMethod, bool isAllDirection);
