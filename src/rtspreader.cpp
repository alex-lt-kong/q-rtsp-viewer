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
        if (this->label->width() < 30 || this->label->height() < 30)
            continue;

        cv::resize(this->frame, this->frame, cv::Size(this->label->width() - 3, this->label->height() - 3));


        QPixmap pixmap = QPixmap::fromImage(QImage((uchar*)this->frame.data, this->frame.cols, this->frame.rows, this->frame.step, QImage::Format_BGR888));
        cout << thread()->currentThreadId() <<
                ": pixmap.isNull(): " << pixmap.isNull() <<                
                ", pixmap.height(): " << pixmap.height() << ", pixmap.width(): " << pixmap.width() <<
                ", pixmap.isQBitmap(): " << pixmap.isQBitmap() <<
                ", this->label->height(): " << this->label->height() <<
                ", this->label->width(): " << this->label->width() << endl;
        this->label->setPixmap(pixmap);
        //this->label->setPixmap(pixmap.scaled(this->frame.cols, this->frame.rows, Qt::AspectRatioMode::IgnoreAspectRatio));
        // Using Qt's native scaled() function appears to be buggy--occasionally causes segmentation fault
    }
}
