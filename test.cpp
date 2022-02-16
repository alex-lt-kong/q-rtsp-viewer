#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    String rtsp_addr = "rtsp://kxw_laptop:16863!!!@mamsds.xicp.net:55544/cam/realmonitor?channel=8&subtype=1";
    //rtsp_addr = "/home/mamsds/Documents/data2/speech-by-grandpa.mp4";
    VideoCapture cap(rtsp_addr);

    Mat frame;
    Mat resize;
    cout << "OpenCV version : " << CV_VERSION << endl;
    cout << "Major version : " << CV_MAJOR_VERSION << endl;
    cout << "Minor version : " << CV_MINOR_VERSION << endl;
    cout << "Subminor version : " << CV_SUBMINOR_VERSION << endl;
    for(;;) {
        cap >> frame;
        if(frame.empty())
            break;

        cv::resize(frame, resize, cv::Size(800, 240));
        imshow("Video Stream", resize);

        if (waitKey(10) == 'q')
            break;
    }
}