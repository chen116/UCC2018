#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>

#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main()
{

    VideoCapture cap;
    bool update_bg_model = true;

    cap.open("rollcar.3gp");
    cv::BackgroundSubtractorMOG2 bg;//(100, 3, 0.3, 5);
    bg.set ("nmixtures", 3);
    std::vector < std::vector < cv::Point > >contours;

    cv::namedWindow ("Frame");
    cv::namedWindow ("Background");

    Mat frame, fgmask, fgimg, backgroundImage;

    for(;;)
    {
        cap >> frame;
        bg.operator()(frame, fgimg);
        bg.getBackgroundImage (backgroundImage);
        cv::erode (fgimg, fgimg, cv::Mat ());
        cv::dilate (fgimg, fgimg, cv::Mat ());

        cv::findContours (fgimg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
        cv::drawContours (frame, contours, -1, cv::Scalar (0, 0, 255), 2);

        cv::imshow ("Frame", frame);
        cv::imshow ("Background", backgroundImage);


        char k = (char)waitKey(30);
        if( k == 27 ) break;

    }

    return 0;
}