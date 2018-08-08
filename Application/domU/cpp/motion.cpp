/*
* To compile: g++ opencv_test2.cpp -o opencv_test2 $(pkg-config --cflags --libs opencv)
*/
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <unistd.h>
#include "pacer.h"

using namespace cv;
using namespace std;
 
int main(int argc, char **argv) {
Pacer mypacer;
mypacer.setWindowSize(10);
mypacer.setItem("box_entry");
mypacer.setItem("frame_number_entry");
mypacer.setItem("frame_size");

char * item;
printf("My Dom ID: %d\n",mypacer.getDomid() );
printf("waiting for dom0...\n");
while (strcmp("init",item)!=0)
{
    item = mypacer.readItem("frame_number_entry");
} 

item = mypacer.readItem("box_entry");
string init_video_data = string(item);
istringstream iss(item);
vector<string> init_video_data_vec(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());

int hw_size,lw_size;
if (init_video_data_vec[0]=="init")
{
    hw_size=stoi(init_video_data_vec[1]);
    lw_size=stoi(init_video_data_vec[2]);
}
vector<int> vidarray_binary;
std::stringstream ss(init_video_data_vec[3]);
int i;
while (ss >> i)
{
    vidarray_binary.push_back(i);
    if (ss.peek() == ',') ss.ignore();
}



vector<Mat> car;
vector<Mat> no_car;
for (int k = 0; k < 200; ++k)
{
    Mat frame;
    cap >> frame;
    if (k>=20 and k <50)
    {
        car.push_back(frame);
    }
    if (k >= 140)
    {
        no_car.push_back(frame);
    }
}
vector<Mat> flipcar = car; 
reverse(flipcar.begin(),flipcar.end());
for (int k=0;k<flipcar.size();k++)
{
    car.push_back(flipcar[k]);
}

std::vector<Mat> vidarray;
for (i=0; i< vidarray_binary.size(); i++)
{
    if (vidarray_binary.at(i))
    {
        for (int k=0; k< car.size(); k++) vidarray.push_back(car.at(k));
    }
    else
    {
        for (int k=0; k< no_car.size(); k++) vidarray.push_back(no_car.at(k));
    }
}

string box_coords = "0 0 0 0";
mypacer.writeItem("box_entry",box_coords.c_str());
mypacer.writeItem("frame_number_entry","ready");
printf("ready...\n");


int frame_num;
int prev_frame_num = -1;
int frame_size = vidarray_binary[0];
int detect_car = vidarray_binary[0];
int prev_frame_size = 0;

Mat frame, gray, frameDelta, thresh, firstFrame;
vector<vector<Point> > cnts;
while (strcmp("done",item)!=0)
{

item=mypacer.readItem("frame_number_entry");
frame_num = atoi(item);
if (frame_num > prev_frame_num) 
{

    prev_frame_num = frame_num;
    frame = vidarray[frame_num];
    if (detect_car) frame_size = hw_size;
    else frame_size = lw_size;
    if(frame_num == 0)
    {
        //convert to grayscale and set the first frame
        cvtColor(frame, firstFrame, COLOR_BGR2GRAY);
        GaussianBlur(firstFrame, firstFrame, Size(21, 21), 0);
    }
    else
    {
        //convert to grayscale
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        GaussianBlur(gray, gray, Size(21, 21), 0);

        //compute difference between first frame and current frame
        absdiff(firstFrame, gray, frameDelta);
        threshold(frameDelta, thresh, 25, 255, THRESH_BINARY);
        
        dilate(thresh, thresh, Mat(), Point(-1,-1), 2);
        findContours(thresh, cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        for(int i = 0; i< cnts.size(); i++) {
            if(contourArea(cnts[i]) < 500) {
                continue;
            }
            // putText(frame, "Motion Detected", Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
            printf("Motion Detected at frame%d\n",frame_num );
        }
        mypacer.beat();
        mypacer.writeInstantHeartRate();

        // imshow("Camera", frame);
        
        // if(waitKey(1) == 27){
        //     //exit if ESC is pressed
        //     break;
        // }
    
    }

    return 0;
}
// /*
// * To compile: g++ opencv_test2.cpp -o opencv_test2 $(pkg-config --cflags --libs opencv)
// */
// #include <opencv2/opencv.hpp>
// #include <opencv2/tracking.hpp>
// #include <opencv2/core/ocl.hpp>
// #include <unistd.h>

// using namespace cv;
// using namespace std;
 
// int main(int argc, char **argv) {

//     Mat frame, gray, frameDelta, thresh, firstFrame;
//     vector<vector<Point> > cnts;
//     VideoCapture camera(0); //open camera
    
//     //set the video size to 512x288 to process faster
//     camera.set(3, 512);
//     camera.set(4, 288);

//     sleep(3);
//     camera.read(frame);

//     //convert to grayscale and set the first frame
//     cvtColor(frame, firstFrame, COLOR_BGR2GRAY);
//     GaussianBlur(firstFrame, firstFrame, Size(21, 21), 0);

//     while(camera.read(frame)) {

//         //convert to grayscale
//         cvtColor(frame, gray, COLOR_BGR2GRAY);
//         GaussianBlur(gray, gray, Size(21, 21), 0);

//         //compute difference between first frame and current frame
//         absdiff(firstFrame, gray, frameDelta);
//         threshold(frameDelta, thresh, 25, 255, THRESH_BINARY);
        
//         dilate(thresh, thresh, Mat(), Point(-1,-1), 2);
//         findContours(thresh, cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

//         for(int i = 0; i< cnts.size(); i++) {
//             if(contourArea(cnts[i]) < 500) {
//                 continue;
//             }

//             putText(frame, "Motion Detected", Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
//         }
    
//         imshow("Camera", frame);
        
//         if(waitKey(1) == 27){
//             //exit if ESC is pressed
//             break;
//         }
    
//     }

//     return 0;
// }