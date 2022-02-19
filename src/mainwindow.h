#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <rtspreader.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonExit_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_comboBoxDomainNames_currentIndexChanged1(int index);


    void on_pushButtonSaveScreenshots_clicked();

private:
    Ui::MainWindow *ui;
    const static int channelCount = 20;
    QString *myUrls4Channels = new QString[4];
    QString *myUrls16Channels = new QString[16];
    bool isMainWindowInitialized = false;
    QLabel *frameLabels[channelCount];
    Mat rawFrames[channelCount];
    int* globalQueueDepthPtr = new int;

    rtspReader *myRtspReaders = new rtspReader[20];
    void showEvent(QShowEvent* event);
    void playStreams(int tabIndex);
    void stopStreams(int tabIndex, bool wait);
    void loadSettings();

public slots:
    void onNewFrameReceived(int channelId, cv::Mat frame);
    void onNewTextMessageReceived(int channelId, std::string message);
};

#endif // MAINWINDOW_H
