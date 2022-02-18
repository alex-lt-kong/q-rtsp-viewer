#include "rtspreader.h"
#include <QDebug>
#include <string>
#include <QLabel>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cmath>

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
    this->emptyFrameCount = 0;
    this->emptyFrameWarningThrottle = 0;
    if (this->url == ""){
        cout << "this->url is EMPTY, returned;" << endl;
        return;
    }
    VideoCapture vc = VideoCapture();
    vc.set(CV_CAP_PROP_BUFFERSIZE, 1);
    // internal buffer will now store only 1 frames to minimize loading
    cout << this->url << ": opening" << endl;
    vc.open(this->url);
    cout << this->url << ": opened" << endl;
    while (this->stopSignal == false) {
        vc >> this->frame;

        if(this->frame.empty()) {
            this->emptyFrameCount ++;
            if (this->emptyFrameCount % (long)pow(10, this->emptyFrameWarningThrottle) == 0) {
                this->emptyFrameWarningThrottle ++;
                cout << ": empty frame received (" << this->emptyFrameCount << " in total, stdout throttled to " << (long)pow(10, this->emptyFrameWarningThrottle) << " messages)" << endl;
            }
            continue;
        }
        emit sendNewFrame(frame, this->label);
    }
    cout << "RTSP stream loop stopped gracefully!" << endl;
}
