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
    void setChannelId(int id);
    void setTargetFPS(float fps);

protected:
    void run();

private:    
    static const int maxCapOpenAttempt = 10;    
    string url = "";
    int channelId;
    bool stopSignal;
    const Mat emptyFrame;
    Mat frame;
    int capOpenAttempts;
    float targetFps;

    string getVideoCaptureBackend(VideoCapture vc);

signals:
    void sendNewFrame(int channelId, cv::Mat frame, long long int msSinceEpoch);
    void sendTextMessage(int channelId, std::string message);
};
#endif // RTSPREADER_H
