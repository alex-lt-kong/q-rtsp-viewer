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
    this->capOpenAttempts = 0;
    if (this->url == "") {
        emit sendTextMessage(this->labelName, "this->url is EMPTY, returned;");
        return;
    }
    bool readResult = false;
    VideoCapture cap = VideoCapture();
    cap.set(CV_CAP_PROP_BUFFERSIZE, 2);
    // internal buffer stores only 2 frames to minimize loading
    emit sendTextMessage(this->labelName, "URL [" + this->url + "] opening");
    cap.open(this->url);
    emit sendTextMessage(this->labelName, "URL [" + this->url + "] opened, backedend: " + this->getVideoCaptureBackend(cap));
    // need to have a manual flush here, otherwise, sometimes the opened line won't be dispalyed

    while (this->stopSignal == false) {

        if (this->capOpenAttempts >= this->maxCapOpenAttempt) {
            emit sendTextMessage(this->labelName, "Too many failed attempts, leaving the message loop...");
            emit sendNewFrame(this->emptyFrame, this->label);
            // here we emit an empty Mat to indicate the loop is about to end;
            break; // break quits only the innermost loop
        }

        readResult = cap.read(this->frame);

        if (readResult == false || this->frame.empty() || cap.isOpened() == false) {
            emit sendNewFrame(this->emptyFrame, this->label);
            // here we cannot assume this->frame is empty--if cap is closed, it may
            // simply skip cap.read() and this->frame keeps its existing non-empty data.
            emit sendTextMessage(this->labelName,
                                 "(readResult == false || this->frame.empty() || cap.isOpened() == false) triggered: trying to reopen cv::VideoCapture after wait for 10 sec (" +
                                 to_string(++this->capOpenAttempts) +  "/" + to_string(this->maxCapOpenAttempt) + ")");
            QThread::sleep(10);
            // cap.release();
            // seems we cannot call release() here because OpenCV's document says:
            // "[it] also deallocates memory and clears *capture pointer". Appears to me that
            // release() makes my cap a nullptr...
            cap.open(this->url);
            emit sendTextMessage(this->labelName, "cv::VideoCapture reopen result: " + to_string(cap.isOpened()));
            continue;            
        }
        this->capOpenAttempts = 0;
        emit sendNewFrame(frame, this->label);
    }    
    // cap.release();
    // The method is automatically called by subsequent VideoCapture::open and by VideoCapture destructor.
    emit sendTextMessage(this->labelName, "RTSP stream loop stopped gracefully");
}
