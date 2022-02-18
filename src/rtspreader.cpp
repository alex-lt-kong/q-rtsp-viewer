#include "rtspreader.h"
#include <QDebug>
#include <string>
#include <QLabel>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cmath>
#include <QThread>

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
    this->labelName = this->label->toolTip().toStdString();
}

void rtspReader::stop() {
    this->stopSignal = true;
}

string rtspReader::getVideoCaptureBackend(VideoCapture vc) {
    int backendId = vc.get(cv::CAP_PROP_BACKEND);

    if (backendId == cv::CAP_MSMF)
        return "MSMF";
    if (backendId == cv::CAP_FFMPEG)
        return "FFMPEG";
    return "unknown backendID " + to_string(backendId);
}


void rtspReader::run()
{
    this->stopSignal = false;
    this->emptyFrameCount = 0;
    this->emptyFrameWarningThrottle = 0;
    this->capOpenCount = 0;
    if (this->url == ""){
        cout << "this->url is EMPTY, returned;\n";
        return;
    }
    VideoCapture cap = VideoCapture();
    cap.set(CV_CAP_PROP_BUFFERSIZE, 2);
    // internal buffer stores only 2 frames to minimize loading
    cout << this->labelName << ": URL " << this->url << ": opening\n";
    cap.open(this->url);
    this->capOpenCount ++;
    cout <<this->labelName << ": URL " << this->url << ": opened, Backend: " << this->getVideoCaptureBackend(cap) << endl;
    // need to have a flush here, otherwise, sometimes the opened line won't be flushed

    while (this->stopSignal == false) {

        if (this->capOpenCount > 1) {
            Mat m;
            cout << this->labelName << ": Too many failed attempts, leaving the loop..." << endl;
            emit sendNewFrame(m, this->label);
            // here we emit an empty Mat to indicate the loop is about to end;
            break;
        }

        cap.read(this->frame);

        if(this->frame.empty()) {
            this->emptyFrameCount ++;
            if (this->emptyFrameCount % (long long int)pow(10, this->emptyFrameWarningThrottle) == 0) {
                this->emptyFrameWarningThrottle ++;
                cout << this->labelName << ": empty frame received (" << this->emptyFrameCount << " in total, stdout throttled to " <<
                        (long long int)pow(10, this->emptyFrameWarningThrottle) << " messages), " <<
                       "vc::isOpened(): " << cap.isOpened() << endl;

                if (cap.isOpened() == false) {
                    cout << this->labelName << ": cap unexpectedly closed, reopening...";
                    cap.open(this->url);
                    this->capOpenCount ++;
                } else if (this->emptyFrameCount % 100) {
                    QThread::sleep(10);
                    cout << this->labelName << ": cap says it is still opened, but who cares, let's release() and open() it again!" << endl;
                  //  cap.release();
                  //  cap.open(this->url);
                    this->capOpenCount ++;
                }
            }
            continue;
            this->capOpenCount = 0;
        }
        emit sendNewFrame(frame, this->label);
    }
    cap.release();
    cout << this->labelName << ": RTSP stream loop stopped gracefully!\n";
}
