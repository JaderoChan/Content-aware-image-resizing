#include "content_aware_resizing.hpp"

#include <cfloat>
#include <utility>

#include "aux_tools.hpp"
#include "parameter_pack.hpp"

static std::pair<double, signed char> minimalEnergyAndDirection(const cv::Mat& mat, const cv::Point& point);

cv::Mat makeEnergyImage(const cv::Mat& img)
{
    cv::Mat energyImg = cv::Mat(img.rows, img.cols, CV_64FC1, cv::Scalar(0.0));
    double maxEnergy = 0;

    for (int row = 0; row < img.rows; ++row)
    {
        for (int col = 0; col < img.cols; ++col)
        {
            double energy = computePointEnergy(img, cv::Point(col, row),
                                               globalCcsMethod(), globalIsAllDirectForEnergy());
            maxEnergy = maxEnergy > energy ? maxEnergy : energy;
            energyImg.at<double>(row, col) = energy;
        }
    }

    double scale = 255.0 / maxEnergy;
    energyImg *= scale;

    return energyImg;
}

cv::Mat removeMinimumEnergyCloumn(const cv::Mat& img)
{
    if (img.rows <= 1)
        return cv::Mat();

    cv::Mat dpEnergy = cv::Mat(img.rows, img.cols, CV_64FC1, cv::Scalar(0.0));
    cv::Mat dpTrack = cv::Mat(img.rows, img.cols, CV_8SC1, cv::Scalar(0));

    // 计算每个点的能量值，并利用动态规划记录最小能量路径。
    double lastRowMinEnergy = DBL_MAX;
    int lastRowMinEnergyCol = 0;
    for (int row = 0; row < img.rows; ++row)
    {
        for (int col = 0; col < img.cols; ++col)
        {
            double energy = computePointEnergy(img, cv::Point(col, row),
                                               globalCcsMethod(), globalIsAllDirectForEnergy());
            if (row != 0)
            {
                auto med = minimalEnergyAndDirection(dpEnergy, cv::Point(col, row));
                energy += med.first;
                dpTrack.at<signed char>(row, col) = med.second;
            }

            dpEnergy.at<double>(row, col) = energy;

            if (row == img.rows - 1)
            {
                if (lastRowMinEnergy > energy)
                {
                    lastRowMinEnergy = energy;
                    lastRowMinEnergyCol = col;
                }
            }
        }
    }

    // 重建图像。
    int needSkipCol = lastRowMinEnergyCol;
    int cursor = 0;
    cv::Mat rtn(img.rows, img.cols - 1, img.type());
    for (int row = img.rows - 1; row >= 0; --row)
    {
        cursor = 0;

        for (int col = 0; col < img.cols; ++col)
        {
            if (col != needSkipCol)
            {
                rtn.at<cv::Vec3d>(row, cursor) = img.at<cv::Vec3d>(row, col);
                ++cursor;
            }
        }

        needSkipCol = needSkipCol + dpTrack.at<signed char>(row, needSkipCol);
    }

    return rtn;
}

std::pair<double, signed char> minimalEnergyAndDirection(const cv::Mat& mat, const cv::Point& point)
{
    cv::Point leftTop(point.x - 1, point.y - 1);
    cv::Point top(point.x, point.y - 1);
    cv::Point rightTop(point.x + 1, point.y - 1);

    std::pair<double, char> rtn = { -1.0, 0 };

    if (!isOverEdge(mat, leftTop))
    {
        double energy = mat.at<double>(leftTop);
        rtn = { energy, -1 };
    }

    if (!isOverEdge(mat, top))
    {
        double energy = mat.at<double>(top);
        if (rtn.first == -1 || energy < rtn.first)
            rtn = { energy, 0 };
    }

    if (!isOverEdge(mat, rightTop))
    {
        double energy = mat.at<double>(rightTop);
        if (rtn.first == -1 || energy < rtn.first)
            rtn = { energy, 1 };
    }

    return rtn;
}
