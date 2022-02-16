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

    while (this->stopSignal == false) {
        cap >> this->frame;

        if(this->frame.empty())
            continue;

//        this->resizedFrame = this->frame;
        this->frame.copyTo(this->resizedFrame);

        cout << "frame.cols: " << frame.cols << ", frame.rows " << frame.rows << endl;
        //cv::resize(this->frame, this->resizedFrame, cv::Size(640, 480));

        cout << "resized.cols: " << this->resizedFrame.cols << ", resized.rows " << this->resizedFrame.rows << endl;

        QPixmap pixmap = QPixmap::fromImage(QImage((uchar*)this->resizedFrame.data, this->resizedFrame.cols, this->resizedFrame.rows, this->resizedFrame.step, QImage::Format_BGR888));
        cout << thread()->currentThreadId() <<
                ": pixmap.isNull(): " << pixmap.isNull() <<                
                ", pixmap.height(): " << pixmap.height() << ", pixmap.width(): " << pixmap.width() <<
                ", pixmap.isQBitmap(): " << pixmap.isQBitmap() <<
                ", this->label->height(): " << this->label->height() <<
                ", this->label->width(): " << this->label->width() << endl;
        this->label->setPixmap(pixmap);
     //  this->label->setPixmap(pixmap.scaled(this->label->width(), this->label->width(), Qt::AspectRatioMode::IgnoreAspectRatio));
    }
}
