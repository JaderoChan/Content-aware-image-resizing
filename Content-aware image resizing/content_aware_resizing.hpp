#pragma once

#include <string>

#include <opencv2/opencv.hpp>

cv::Mat makeEnergyImage(const cv::Mat& img);

cv::Mat removeMinimumEnergyCloumn(const cv::Mat& img);
