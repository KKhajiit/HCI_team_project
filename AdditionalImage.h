#pragma once 
#include <opencv2/opencv.hpp>


class AdditionalImage {
public:
    static cv::Mat Add_Image(const cv::Mat &src, const cv::Mat &add);
};