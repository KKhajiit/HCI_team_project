#include <iostream>
#include <math.h>
#include <vector>
#include <string>
/// opencv
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgproc.hpp>
#define _WIN32 // if Your OS is Window
#ifdef _WIN32
#include <Windows.h>
#define sleep Sleep
#define TOSECONDS 1000
#else
#include <unistd.h>
#define TOSECONDS 1
#endif
using namespace cv;

const std::string CURRENT_DIR("C:/image/");
const std::vector<std::string> AddImagesFile({CURRENT_DIR + "board.jpg"});
std::vector<Mat> AddImages;

Mat mask;
Mat AddImageMask;

Mat GetBackgroundImage(VideoCapture& vc) {
    Mat ret;
    sleep(3 * TOSECONDS);
    vc.read(ret);
    return ret;
}

uchar sub(const uchar& a, const uchar& b) {
    if (a > b) {
        return a - b;
    }
    else {
        return b - a;
    }
}

Mat RemoveBackground(const Mat& background, const Mat& input, const uchar& thres, const bool& isGray) {
    Mat bg, ret, gray;
    bg = background.clone();
    ret = input.clone();
    int channel = 3;
    if (isGray) {
        channel = 1;
        gray = input.clone();
    }
    else {
        cvtColor(ret, gray, COLOR_BGR2GRAY);
    }

    for (int y = 0; y < bg.size().height; ++y) {
        uchar* pointer_bg = bg.ptr<uchar>(y);
        uchar* pointer_gray = gray.ptr<uchar>(y);
        uchar* pointer_ret = ret.ptr<uchar>(y);

        for (int x = 0; x < bg.size().width; ++x) {
            uchar c_bg = pointer_bg[x];
            uchar c_gray = pointer_gray[x];

            if (sub(c_bg, c_gray) < thres) {
                for (int c = 0; c < channel; ++c) {
                    pointer_ret[x * channel + c] = 0;
                }
            }
        }
    }
    return ret;
}

Mat RemoveBackground2(const Mat& background, const Mat& input, const uchar& thres)
{
    Mat bg, ret;
    bg = background.clone();
    ret = input.clone();

    bool check[3] = { false, false, false };

    for (int y = 0; y < bg.size().height; ++y) {
        uchar* pointer_bg = bg.ptr<uchar>(y);
        uchar* pointer_ret = ret.ptr<uchar>(y);
        uchar* pointer_mask = mask.ptr<uchar>(y);

        for (int x = 0; x < bg.size().width; ++x) {

            for (int c = 0; c < 3; ++c)
            {
                uchar c_bg = pointer_bg[x * 3 + c];
                uchar c_ret = pointer_ret[x * 3 + c];
                if (abs(c_bg - c_ret) < thres)
                {
                    check[c] = true;
                }
            }
            if (check[0] == true && check[1] == true && check[2] == true)
            {
                pointer_mask[x] = 0;
            }
            else
            {
                pointer_mask[x] = 255;
            }
            /*pointer_ret[x * 3 + 0] *= pointer_mask[x * 3 + 0];
            pointer_ret[x * 3 + 1] *= pointer_mask[x * 3 + 1];
            pointer_ret[x * 3 + 2] *= pointer_mask[x * 3 + 2];*/
            check[0] = false;
            check[1] = false;
            check[2] = false;
        }
    }

    return ret;
}

void Erode(const Mat& input, Mat& output, const int size = 3, const int type = MORPH_RECT) {
    erode(input, output, getStructuringElement(type, Size(2 * size + 1, 2 * size + 1), Point(1, 1)));
}
void Dilate(const Mat& input, Mat& output, const int size = 3, const int type = MORPH_RECT) {
    dilate(input, output, getStructuringElement(type, Size(2 * size + 1, 2 * size + 1), Point(1, 1)));
}
void EraseNoise(const Mat& input, Mat& output) {
    int size = 2;
    // int type = MORPH_CROSS;
    int type = MORPH_ELLIPSE;
    Erode(input, output, size, type);
    Dilate(output, output, size, type);
}

Mat mult(const Mat& mask, const Mat& result)
{
    Mat maskC = mask.clone();
    Mat resultC = result.clone();
    for (int y = 0; y < mask.size().height; ++y)
    {
        uchar* init = maskC.ptr<uchar>(y);
        uchar* pointer_ret = resultC.ptr<uchar>(y);
        for (int x = 0; x < mask.size().width; ++x)
        {
            pointer_ret[x * 3 + 0] *= init[x]/255;
            pointer_ret[x * 3 + 1] *= init[x]/255;
            pointer_ret[x * 3 + 2] *= init[x]/255;
        }
    }
    return resultC;
}
// Input: cam video
// Output: only person

int main() {
	// Load Image
	for (int i = 0; i < AddImagesFile.size(); i++) AddImages.push_back(imread(AddImagesFile[i].c_str()));
	// End Load Image
    VideoCapture vc(0);
    Mat backgroundImage;
    std::cout << "Getting Background..\n";
    backgroundImage = GetBackgroundImage(vc);
    //cvtColor(backgroundImage, backgroundImage, COLOR_BGR2GRAY);
    // Erode(backgroundImage, backgroundImage);
    //EraseNoise(backgroundImage, backgroundImage);
    Mat getFrame;
    Mat result;
    Mat grayFrame;

    cvtColor(backgroundImage.clone(), mask, CV_BGR2GRAY);
    std::cout << mask.size().width << mask.size().height << "\n";
    std::cout << mask.channels() << backgroundImage.channels() << "\n";

    for (int y = 0; y < mask.size().height; ++y)
    {
        uchar* init = mask.ptr<uchar>(y);
        for (int x = 0; x < mask.size().width; ++x)
        {
            init[x] = 0;
        }
    }

    /*cvtColor(backgroundImage, backgroundImage, COLOR_BGR2HSV);*/

    std::cout << "Done\n\nVideo Input Start\n";
    while (true) {
        vc.read(getFrame);

        //   result = RemoveBackground(backgroundImage, getFrame, uchar(20), false);
        //cvtColor(getFrame, grayFrame, COLOR_BGR2GRAY);
        //EraseNoise(grayFrame, grayFrame);
        //result = RemoveBackground(backgroundImage, getFrame, uchar(16), true);

        //result = RemoveBackground(backgroundImage, getFrame, uchar(20), false);
        //result = grayFrame - backgroundImage;
        //cv::Canny(result, result, 100, 200);

        //EraseNoise(result, result);
        /*cvtColor(getFrame, getFrame, COLOR_BGR2HSV);*/

        imshow("black", mask);
        result = RemoveBackground2(backgroundImage, getFrame, 65);
        /*result = mult(mask, result);*/
        imshow("ASD1", result);

        Dilate(mask, mask, 10);
        Dilate(mask, mask, 10);
        Dilate(mask, mask, 10);
        Erode(mask, mask, 10);
        Erode(mask, mask, 10);
        result = mult(mask, result);
        //Mat gimori = result * mask;
        //imshow("GiMoRing", gimori);
        /*cvtColor(result, result, COLOR_HSV2BGR);*/

        //imshow("add,", AddImages[0]);
	    AddImageMask = ~mask;
		//-------------------------------------------------- Add Image ---------------------------------------------//
		for (size_t i = 0; i < AddImages.size(); i++) {
			cv::resize(AddImages[i], AddImages[i], mask.size());

			Mat toAdd = mult(AddImageMask, AddImages[i]);

			result = result + toAdd;
		}



        imshow("ASD2", result);

        if (waitKey(27) == 27) break;
    }
	return 0;
}