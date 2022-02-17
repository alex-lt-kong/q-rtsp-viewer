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
    cout << "rtspReader opening: " << this->url << endl;
    if (this->url == ""){
        cout << "returned??" << endl;
        return;
    }
    VideoCapture cap(this->url);
    cap.set(CV_CAP_PROP_BUFFERSIZE, 2); // internal buffer will now store only 3 frames

    while (this->stopSignal == false) {
        cap >> this->frame;

        if(this->frame.empty())
            continue;
        if (this->label->width() < 30 || this->label->height() < 30)
            continue;

        cv::resize(this->frame, this->frame, cv::Size(this->label->width() - 3, this->label->height() - 3));
        //this->mutex.lock();
        //QImage image = QImage(
         //           (uchar*)this->frame.data,
          //          this->frame.cols, this->frame.rows, this->frame.step, QImage::Format_BGR888,
           //         nullptr);
        //image.scanLine(0);
        qDebug() << "ThreadID from the caller: " << thread()->currentThreadId();
        emit newFrameReceived(frame, this->label);
        //image.bits();
        //this->pixmap = QPixmap::fromImage(image.copy());
        //this->label->setPixmap(this->pixmap);
        //this->mutex.unlock();
        //this->label->setPixmap(pixmap.scaled(this->frame.cols, this->frame.rows, Qt::AspectRatioMode::IgnoreAspectRatio));
        // Using Qt's native scaled() function appears to be buggy--occasionally causes segmentation fault
    }
}
