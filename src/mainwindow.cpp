#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGridLayout>
#include <QMediaPlayer>
#include <QNetworkRequest>
#include <QVideoWidget>
#include <QSettings>
#include <iostream>
#include <QDebug>
#include <rtspreader.h>
#include <QThread>
#include <QDialog>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>

using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->label4Channel00->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    ui->label4Channel01->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    ui->label4Channel10->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    ui->label4Channel11->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

    ui->label4Channel00->setScaledContents(true);
    ui->label4Channel01->setScaledContents(true);
    ui->label4Channel10->setScaledContents(true);
    ui->label4Channel11->setScaledContents(true);
 /*Using Qt's native scaled() function appears to be buggy--occasionally causes segmentation fault*/


    int idx = 0;
    this->frameLabels[idx++] = ui->label4Channel00;
    this->frameLabels[idx++] = ui->label4Channel01;
    this->frameLabels[idx++] = ui->label4Channel10;
    this->frameLabels[idx++] = ui->label4Channel11;

    this->frameLabels[idx++] = ui->label16Channel00;
    this->frameLabels[idx++] = ui->label16Channel01;
    this->frameLabels[idx++] = ui->label16Channel02;
    this->frameLabels[idx++] = ui->label16Channel03;
    this->frameLabels[idx++] = ui->label16Channel10;
    this->frameLabels[idx++] = ui->label16Channel11;
    this->frameLabels[idx++] = ui->label16Channel12;
    this->frameLabels[idx++] = ui->label16Channel13;
    this->frameLabels[idx++] = ui->label16Channel20;
    this->frameLabels[idx++] = ui->label16Channel21;
    this->frameLabels[idx++] = ui->label16Channel22;
    this->frameLabels[idx++] = ui->label16Channel23;
    this->frameLabels[idx++] = ui->label16Channel30;
    this->frameLabels[idx++] = ui->label16Channel31;
    this->frameLabels[idx++] = ui->label16Channel32;
    this->frameLabels[idx++] = ui->label16Channel33;

    this->globalQueueDepth = 0;
    for (int i = 0; i < MainWindow::channelCount; i ++) {
        myRtspReaders[i].setChannelId(i);
        myRtspReaders[i].setTargetFPS(100.0);
        myRtspReaders[i].setQueueDepthPointer(ref(globalQueueDepth));
    }

    cout << "cv::getBuildInformation():\n" <<  getBuildInformation();

    for (int i = 0; i < MainWindow::channelCount; i ++) {
        // https://stackoverflow.com/questions/14545961/modify-qt-gui-from-background-worker-thread
        connect(&myRtspReaders[i], SIGNAL(sendTextMessage(int,std::string)), SLOT(onNewTextMessageReceived(int,std::string)));
        connect(&myRtspReaders[i], SIGNAL(sendNewFrame(int,cv::Mat)), SLOT(onNewFrameReceived(int,cv::Mat)));
    }
}

void MainWindow::showEvent( QShowEvent* event ) {

    QWidget::showEvent( event );
    // the code below will be executed AFTER the MainWindow is shown
    if (isMainWindowInitialized == false) {
        loadSettings();
        srand (time(NULL));
        this->ui->comboBoxDomainNames->setCurrentIndex(rand() % this->ui->comboBoxDomainNames->count());
        connect(this->ui->comboBoxDomainNames, &QComboBox::currentIndexChanged, this, &MainWindow::on_comboBoxDomainNames_currentIndexChanged1);
        on_comboBoxDomainNames_currentIndexChanged1(0);
        /*
            Note this design:
            1. No slot is predefined in Qt Designer;
            2. Load comboBoxDomainNames values from settings file--no slot event will be triggered;
            3. then we pick a random item, so that it triggers the slot only once.
            4. Enable slot;
            How about we swap the order of 3 and 4? No, if the same value is randomly picked, then the event
            will NOT be triggered!
        */
        this->isMainWindowInitialized = true;
    }
}

void MainWindow::loadSettings() {

    QSettings settings("ak-studio", "qrtsp-viewer");

    int size = settings.beginReadArray("4ChannelUrls");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        myUrls4Channels[i] = settings.value("Url").toString();
    }
    settings.endArray();

    size = settings.beginReadArray("16ChannelUrls");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        myUrls16Channels[i] = settings.value("Url").toString();
    }
    settings.endArray();

    size = settings.beginReadArray("DomainNames");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        this->ui->comboBoxDomainNames->addItem(settings.value("name").toString());
    }
    settings.endArray();
}

MainWindow::~MainWindow()
{
    stopStreams(0, true);
    stopStreams(1, true);

    // Although the heap will be cleared by OS anyway,
    // it is still a good practice to delete[] then yourself!
    // (And not doing so results in a SegmentFault...)
    delete[] myUrls4Channels;
    delete[] myUrls16Channels;
    delete[] myRtspReaders;
    delete ui;
}

void MainWindow::onNewFrameReceived(int channelId, Mat frame) {

    if (frame.empty() == false) {
        rawFrames[channelId] = frame;
        QImage image = QImage(
                    (uchar*)rawFrames[channelId].data,
                    rawFrames[channelId].cols,
                    rawFrames[channelId].rows,
                    rawFrames[channelId].step,
                    QImage::Format_BGR888);
        QPixmap pixmap = QPixmap::fromImage(image);
        frameLabels[channelId]->setPixmap(pixmap.scaled(frameLabels[channelId]->width() > 1 ? frameLabels[channelId]->width() - 1: 1,
                                       frameLabels[channelId]->height() > 1 ? frameLabels[channelId]->height() - 1: 1,
                                       Qt::IgnoreAspectRatio));
        this->globalQueueDepth = this->globalQueueDepth -1;
        cout << this->globalQueueDepth << endl;
    } else {
        frameLabels[channelId]->clear();
        frameLabels[channelId]->setText("无法从RTSP源读取画面/Failed to read frame from RTSP stream");
    }
}

void MainWindow::onNewTextMessageReceived(int channelId, string message) {
    // This slot is a must--if we allow worker threads to directly write
    // to stdout, it may be safe, but the format could be broken by possible
    // concurrent writing.

     QDateTime dateTime = dateTime.currentDateTime();
     cout << "[" << dateTime.toString("yyyy-MM-dd HH:mm:ss").toStdString() << "] "
          << frameLabels[channelId]->toolTip().toStdString() << ": " << message << endl;
    // Have to flush by calling endl, otherwise a line may be partially shown ¯\_(ツ)_/¯
}

void MainWindow::stopStreams(int tabIndex, bool wait) {

    if (tabIndex == 0) {
        for (int i = 0; i < 4; i ++) {
            myRtspReaders[i].stop();
        }
        if (wait) {
            for (int i = 0; i < 4; i ++) {
                myRtspReaders[i].wait();
            }
        }
    } else {
        for (int i = 0; i < 16; i ++) {
            myRtspReaders[4+i].stop();
        }
        if (wait) {
            for (int i = 0; i < 16; i ++) {
                myRtspReaders[4+i].wait();
            }
        }
    }
    /*
    wait() Blocks the thread until either of these conditions is met:
        * The thread associated with this QThread object has finished execution (i.e. when it returns from run()).
          This function will return true if the thread has finished. It also returns true if the thread has not been started yet.
        * The deadline is reached. This function will return false if the deadline is reached.

    */
}

void MainWindow::playStreams(int tabIndex) {
    if (tabIndex == 0) {
        for (int i = 0; i < 4; i ++) {
            myRtspReaders[i].setRtspUrl(QString(myUrls4Channels[i]).replace("[domain-name]", this->ui->comboBoxDomainNames->currentText()).toStdString());
            myRtspReaders[i].start();
            // according to this link: https://www.qtcentre.org/threads/6548-Multiple-start()-on-same-QThread-Object
            // if you start() a running thread, it does nothing...
        }
    } else {
        for (int i = 0; i < 16; i ++) {
            myRtspReaders[4+i].setRtspUrl(QString(myUrls16Channels[i]).replace("[domain-name]", this->ui->comboBoxDomainNames->currentText()).toStdString());
            myRtspReaders[4+i].start();
        }
    }
}


void MainWindow::on_pushButtonExit_clicked()
{
    QApplication::quit();
}



void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index == 0) {
        stopStreams(1, false);
        playStreams(0);
    }
    else {
        stopStreams(0, false);
        playStreams(1);
    }
}


void MainWindow::on_comboBoxDomainNames_currentIndexChanged1(int index)
{
    stopStreams(0, this->ui->tabWidget->currentIndex() == 0);
    stopStreams(1, this->ui->tabWidget->currentIndex() == 1);
    // we want to stop both since if the user change the selected domain name
    // the program should reload the RTSP stream from the new domain name

    if (this->ui->tabWidget->currentIndex() == 0) {
        playStreams(0);
    }
    else {
        playStreams(1);
    }
}


void MainWindow::on_pushButtonSaveScreenshots_clicked()
{
    QString destDirectory = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/cctv/";
    bool writeResult = false;
    QString destPath;

    QDir dir(destDirectory);
    if (!dir.exists())
        dir.mkpath(".");

    QDateTime dateTime = dateTime.currentDateTime();
    for (int i = 0; i < MainWindow::channelCount; i ++) {
        if (rawFrames[i].empty())
            continue;
        destPath = destDirectory + QString::fromStdString("channel" + to_string(i+1) + "_") + dateTime.toString("yyyyMMdd-HHmmss") + QString::fromStdString(".jpg");
        writeResult = imwrite(destPath.toStdString(), rawFrames[i]);
        cout << "Written screenshot to " << destPath.toStdString() << ", result: " << writeResult << endl;
    }
}

