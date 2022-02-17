#include "rtspreader.h"
#include <QDebug>
#include <string>
#include <QLabel>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

rtspReader::rtspReader()
{
}

rtspReader::~rtspReader(){

}

void rtspReader::setRtspUrl(string url) {
    this->url = url;
}

void rtspReader::setLabel(QLabel *label) {
    this->label = label;

}

void rtspReader::stop() {
    this->stopSignal = true;
}

void rtspReader::run()
{
    this->stopSignal = false;
    if (this->url == ""){
        cout << "this->url is EMPTY, returned;" << endl;
        return;
    }
    VideoCapture vc = VideoCapture();
    vc.set(CV_CAP_PROP_BUFFERSIZE, 1);
    // internal buffer will now store only 1 frames to minimize loading
    vc.open(this->url);

    while (this->stopSignal == false) {
        vc >> this->frame;

        if(this->frame.empty())
            continue;
        emit sendNewFrame(frame, this->label);
    }
    cout << "RTSP stream loop stopped gracefully!" << endl;
}
