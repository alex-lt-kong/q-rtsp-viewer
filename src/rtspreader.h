#ifndef RTSPREADER_H
#define RTSPREADER_H

#include <QObject>
#include <QThread>
#include <string>
#include <QLabel>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

class rtspReader: public QThread
{
    Q_OBJECT

public:
    rtspReader();
    ~rtspReader();
    void stop();
    void setRtspUrl(string url);
    void setLabel(QLabel *label);

protected:
    void run();

private:
    string url = "";
    string labelName = "";
    QLabel* label;
    bool stopSignal;
    Mat frame;
    const Mat emptyFrame;
    QPixmap pixmap;
    int capOpenAttempts;
    const int maxCapOpenAttempt = 10;

    string getVideoCaptureBackend(VideoCapture vc);


signals:
    void sendNewFrame(Mat frame, QLabel *label);
    void sendTextMessage(string labelName, string message);
};
#endif // RTSPREADER_H
