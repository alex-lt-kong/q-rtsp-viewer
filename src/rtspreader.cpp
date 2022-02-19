#include "rtspreader.h"
#include <QDebug>
#include <string>
#include <QLabel>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cmath>
#include <QThread>
#include <ctime>

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

void rtspReader::setChannelId(int id) {
    this->channelId = id;
}

void rtspReader::setTargetFPS(float fps) {
    this->targetFps = fps;
}

void rtspReader::setQueueDepthPointer(int* depthPtr) {
    this->globalQueueDepth = depthPtr;
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
        emit sendTextMessage(this->channelId, "this->url is EMPTY, returned;");
        return;
    }
    bool readResult = false;

    VideoCapture cap = VideoCapture();

    cap.set(CV_CAP_PROP_BUFFERSIZE, this->targetFps * 5);
    // internal buffer stores only 2 frames to minimize loading
    emit sendTextMessage(this->channelId, "URL [" + this->url + "] opening");
    cap.open(this->url);
    emit sendTextMessage(this->channelId, "URL [" + this->url + "], open() result: " +
                         to_string(cap.isOpened()) + ", backedend: " + this->getVideoCaptureBackend(cap));

    while (this->stopSignal == false) {

        if (this->capOpenAttempts >= rtspReader::maxCapOpenAttempt) {
            emit sendTextMessage(this->channelId, "Too many failed attempts, leaving the message loop...");
            emit sendNewFrame(this->channelId, this->emptyFrame);
            // here we emit an empty Mat to indicate the loop is about to end;
            break; // break quits only the innermost loop
        }

        readResult = cap.grab();
        readResult = readResult && cap.retrieve(this->frame);

        if (readResult == false || this->frame.empty() || cap.isOpened() == false) {
            emit sendTextMessage(this->channelId,
                                 "(readResult == false || this->frame.empty() || cap.isOpened() == false) triggered: waiting for 10 sec and then trying to re-open() cv::VideoCapture (" +
                                 to_string(++this->capOpenAttempts) +  "/" + to_string(rtspReader::maxCapOpenAttempt) + ")");
            QThread::sleep(10);
            // cap.release();
            // seems we cannot call release() here because OpenCV's document says:
            // "[it] also deallocates memory and clears *capture pointer". Appears to me that
            // release() makes my cap a nullptr...
            cap.open(this->url);
            emit sendTextMessage(this->channelId, "cv::VideoCapture reopen result: " + to_string(cap.isOpened()));

            continue;            
        }

        this->capOpenAttempts = 0;
        emit sendNewFrame(this->channelId, this->frame.clone());
        *(this->globalQueueDepth) = *(this->globalQueueDepth) + 1;

        // this->frame.clone(): if emit is asynchronous, is it possible that this->frame is re-written
        // before it is fully consumed by GUI thread? Is this the cause of random segmentation fault?
    }    
    // cap.release();
    // The method is automatically called by subsequent VideoCapture::open and by VideoCapture destructor.
    emit sendTextMessage(this->channelId, "RTSP stream loop stopped gracefully");
}
