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

void rtspReader::setFpsThrottle(float fpsUpperLimit) {
    if (fpsUpperLimit <= 0) {
        return;
    }
    this->fpsUpperLimit = fpsUpperLimit;
    this->frameIntervalInMs = 1000 * (1 / this->fpsUpperLimit);
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
    long long int prevMsSinceEpoch = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    long long int currMsSinceEpoch = 0;

    VideoCapture cap = VideoCapture();

    cap.set(CV_CAP_PROP_BUFFERSIZE, this->fpsUpperLimit);
    emit sendTextMessage(this->channelId, "URL [" + this->url + "] opening");
    cap.open(this->url);
    emit sendTextMessage(this->channelId, "URL [" + this->url + "], open() result: " +
                         to_string(cap.isOpened()) + ", backedend: " + this->getVideoCaptureBackend(cap));

    while (this->stopSignal == false) {

        if (this->capOpenAttempts >= rtspReader::maxCapOpenAttempt) {
            emit sendTextMessage(this->channelId, "Too many failed attempts, leaving the message loop...");
          //  emit sendNewFrame(this->channelId, this->emptyFrame, -1);
            break;
        }

        readResult = cap.grab();
        currMsSinceEpoch = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
        if ((currMsSinceEpoch - prevMsSinceEpoch) < this->frameIntervalInMs) { continue; }
        prevMsSinceEpoch = currMsSinceEpoch;
        readResult = readResult && cap.retrieve(this->frame);

        if (readResult == false || this->frame.empty() || cap.isOpened() == false) {
            emit sendTextMessage(this->channelId,
                                 "(readResult == false || this->frame.empty() || cap.isOpened() == false) triggered: waiting for 10 sec and then trying to re-open() cv::VideoCapture (" +
                                 to_string(++this->capOpenAttempts) +  "/" + to_string(rtspReader::maxCapOpenAttempt) + ")");
            QThread::sleep(10);
            // cap.release(); -> seems we cannot call release() here because OpenCV's document says:
            // "[it] also deallocates memory and clears *capture pointer". Appears to me that release() makes cap a nullptr...
            cap.open(this->url);
            emit sendTextMessage(this->channelId, "cv::VideoCapture reopen result: " + to_string(cap.isOpened()));

            continue;            
        }

        this->capOpenAttempts = 0;
        QImage image = QImage(
                    (uchar*)this->frame.data,
                    this->frame.cols,
                    this->frame.rows,
                    this->frame.step,
                    QImage::Format_BGR888);
        QPixmap qp = QPixmap::fromImage(image);
        emit sendNewFrame(this->channelId, QPixmap::fromImage(image), chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count());
        cv::imshow(to_string(this->channelId), frame);
    }    
    // cap.release();
    // The method is automatically called by subsequent VideoCapture::open and by VideoCapture destructor.
    emit sendTextMessage(this->channelId, "RTSP stream loop stopped gracefully");
}
