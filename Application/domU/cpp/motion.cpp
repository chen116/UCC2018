/*
* To compile: g++ opencv_test2.cpp -o opencv_test2 $(pkg-config --cflags --libs opencv)
*/
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <unistd.h>

using namespace cv;
using namespace std;
 
int main(int argc, char **argv) {

    Mat frame, gray, frameDelta, thresh, firstFrame;
    vector<vector<Point> > cnts;
    VideoCapture camera(0); //open camera
    
    //set the video size to 512x288 to process faster
    camera.set(3, 512);
    camera.set(4, 288);

    sleep(3);
    camera.read(frame);

    //convert to grayscale and set the first frame
    cvtColor(frame, firstFrame, COLOR_BGR2GRAY);
    GaussianBlur(firstFrame, firstFrame, Size(21, 21), 0);

    while(camera.read(frame)) {

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

            putText(frame, "Motion Detected", Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
        }
    
        imshow("Camera", frame);
        
        if(waitKey(1) == 27){
            //exit if ESC is pressed
            break;
        }
    
    }

    return 0;
}

// #include <opencv2/opencv.hpp>
// #include <opencv2/tracking.hpp>
// #include <opencv2/core/ocl.hpp>
 
// using namespace cv;
// using namespace std;
 
// // Convert to string
// #define SSTR( x ) static_cast< std::ostringstream & >( \
// ( std::ostringstream() << std::dec << x ) ).str()
 
// int main(int argc, char **argv)
// {
//     // List of tracker types in OpenCV 3.4.1
//     string trackerTypes[6] = {"BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "GOTURN"};
//     // vector <string> trackerTypes(types, std::end(types));
 
//     // Create a tracker
//     string trackerType = trackerTypes[2];
 
//     Ptr<Tracker> tracker;
 
//     #if (CV_MINOR_VERSION < 3)
//     {
//         tracker = Tracker::create(trackerType);
//     }
//     #else
//     {
//         if (trackerType == "BOOSTING")
//             tracker = TrackerBoosting::create();
//         if (trackerType == "MIL")
//             tracker = TrackerMIL::create();
//         if (trackerType == "KCF")
//             tracker = TrackerKCF::create();
//         if (trackerType == "TLD")
//             tracker = TrackerTLD::create();
//         if (trackerType == "MEDIANFLOW")
//             tracker = TrackerMedianFlow::create();
//         if (trackerType == "GOTURN")
//             tracker = TrackerGOTURN::create();

//     }
//     #endif
//     // Read video
//     VideoCapture video("rollcar.3gp");
     
//     // Exit if video is not opened
//     if(!video.isOpened())
//     {
//         cout << "Could not read video file" << endl; 
//         return 1; 
//     } 
 
//     // Read first frame 
//     Mat frame; 
//     bool ok = video.read(frame); 
 
//     // Define initial bounding box 
//     Rect2d bbox(287, 23, 86, 320); 
 
//     // Uncomment the line below to select a different bounding box 
//     // bbox = selectROI(frame, false); 
//     // Display bounding box. 
//     rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 ); 
 
//     imshow("Tracking", frame); 
//     tracker->init(frame, bbox);
     
//     while(video.read(frame))
//     {     
//         // Start timer
//         double timer = (double)getTickCount();
         
//         // Update the tracking result
//         bool ok = tracker->update(frame, bbox);
         
//         // Calculate Frames per second (FPS)
//         float fps = getTickFrequency() / ((double)getTickCount() - timer);
         
//         if (ok)
//         {
//             // Tracking success : Draw the tracked object
//             rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );
//               printf("motion detected\n");
//         }
//         else
//         {
//             // Tracking failure detected.
//             putText(frame, "Tracking failure detected", Point(100,80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
//         }
         
//         // Display tracker type on frame
//         putText(frame, trackerType + " Tracker", Point(100,20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50),2);
         
//         // Display FPS on frame
//         putText(frame, "FPS : " + SSTR(int(fps)), Point(100,50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50), 2);
 
//         // Display frame.
//         imshow("Tracking", frame);
         
//         // Exit if ESC pressed.
//         int k = waitKey(1);
//         if(k == 27)
//         {
//             break;
//         }
 
//     }
// }