#include "aux_tools.hpp"

#define SQUARE(x) ((x) * (x))

bool isOverEdge(const cv::Mat& mat, const cv::Point& point)
{
    return point.x < 0 || point.y < 0 ||
           point.x >= mat.cols || point.y >= mat.rows;
}

double computeColorSimilarity(const cv::Vec3d& color1, const cv::Vec3d& color2, ComputeColorSimilarityMethod ccsMethod)
{
    switch (ccsMethod)
    {
        case CCS_NORMAL:
            return SQUARE(color1[0] - color2[0]) +
                   SQUARE(color1[1] - color2[1]) +
                   SQUARE(color1[2] - color2[2]);
        case CCS_MODIFIED1:
            return SQUARE(color1[0] - color2[0]) * 0.16 +
                   SQUARE(color1[1] - color2[1]) * 0.52 +
                   SQUARE(color1[2] - color2[2]) * 0.32;
        case CCS_MODIFIED2:
            return SQUARE(color1[0] - color2[0]) * 0.114 +
                   SQUARE(color1[1] - color2[1]) * 0.587 +
                   SQUARE(color1[2] - color2[2]) * 0.299;
        default:
            return 0;
    }
}

double computePointEnergy(const cv::Mat& img, const cv::Point& point,
                          ComputeColorSimilarityMethod ccsMethod, bool isAllDirection)
{
    int count = 0;
    double energy = 0.0;

    auto& centerColor = img.at<cv::Vec3d>(point);
    cv::Point pt;
    cv::Vec3d clr;
    if (isAllDirection)
    {
        // Left top.
        pt = cv::Point(point.x - 1, point.y - 1);
        if (!isOverEdge(img, pt))
        {
            clr = img.at<cv::Vec3d>(pt);
            energy += computeColorSimilarity(centerColor, clr, ccsMethod);
            ++count;
        }

        // Right top.
        pt = cv::Point(point.x + 1, point.y - 1);
        if (!isOverEdge(img, pt))
        {
            clr = img.at<cv::Vec3d>(pt);
            energy += computeColorSimilarity(centerColor, clr, ccsMethod);
            ++count;
        }

        // Right bottom.
        pt = cv::Point(point.x + 1, point.y + 1);
        if (!isOverEdge(img, pt))
        {
            clr = img.at<cv::Vec3d>(pt);
            energy += computeColorSimilarity(centerColor, clr, ccsMethod);
            ++count;
        }

        // Left bottom.
        pt = cv::Point(point.x - 1, point.y + 1);
        if (!isOverEdge(img, pt))
        {
            clr = img.at<cv::Vec3d>(pt);
            energy += computeColorSimilarity(centerColor, clr, ccsMethod);
            ++count;
        }
    }

    // Left
    pt = cv::Point(point.x - 1, point.y);
    if (!isOverEdge(img, pt))
    {
        clr = img.at<cv::Vec3d>(pt);
        energy += computeColorSimilarity(centerColor, clr, ccsMethod);
        ++count;
    }

    // Top.
    pt = cv::Point(point.x, point.y - 1);
    if (!isOverEdge(img, pt))
    {
        clr = img.at<cv::Vec3d>(pt);
        energy += computeColorSimilarity(centerColor, clr, ccsMethod);
        ++count;
    }

    // Right.
    pt = cv::Point(point.x + 1, point.y);
    if (!isOverEdge(img, pt))
    {
        clr = img.at<cv::Vec3d>(pt);
        energy += computeColorSimilarity(centerColor, clr, ccsMethod);
        ++count;
    }

    // Bottom.
    pt = cv::Point(point.x, point.y - 1);
    if (!isOverEdge(img, pt))
    {
        clr = img.at<cv::Vec3d>(pt);
        energy += computeColorSimilarity(centerColor, clr, ccsMethod);
        ++count;
    }

    return energy / count;
}
