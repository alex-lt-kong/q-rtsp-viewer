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
    myRtspReaders[idx++].setLabel(ui->label4Channel00);
    myRtspReaders[idx++].setLabel(ui->label4Channel01);
    myRtspReaders[idx++].setLabel(ui->label4Channel10);
    myRtspReaders[idx++].setLabel(ui->label4Channel11);

    myRtspReaders[idx++].setLabel(ui->label16Channel00);
    myRtspReaders[idx++].setLabel(ui->label16Channel01);
    myRtspReaders[idx++].setLabel(ui->label16Channel02);
    myRtspReaders[idx++].setLabel(ui->label16Channel03);
    myRtspReaders[idx++].setLabel(ui->label16Channel10);
    myRtspReaders[idx++].setLabel(ui->label16Channel11);
    myRtspReaders[idx++].setLabel(ui->label16Channel12);
    myRtspReaders[idx++].setLabel(ui->label16Channel13);
    myRtspReaders[idx++].setLabel(ui->label16Channel20);
    myRtspReaders[idx++].setLabel(ui->label16Channel21);
    myRtspReaders[idx++].setLabel(ui->label16Channel22);
    myRtspReaders[idx++].setLabel(ui->label16Channel23);
    myRtspReaders[idx++].setLabel(ui->label16Channel30);
    myRtspReaders[idx++].setLabel(ui->label16Channel31);
    myRtspReaders[idx++].setLabel(ui->label16Channel32);
    myRtspReaders[idx++].setLabel(ui->label16Channel33);

    //on_tabWidget_currentChanged(0);
    cout << "cv::getBuildInformation():\n" <<  getBuildInformation();

    for (int i = 0; i < 20; i ++) {
        // https://stackoverflow.com/questions/14545961/modify-qt-gui-from-background-worker-thread
        connect(&myRtspReaders[i], SIGNAL(sendTextMessage(string,string)), SLOT(onNewTextMessageReceived(string,string)));
        connect(&myRtspReaders[i], SIGNAL(sendNewFrame(Mat,QLabel*)), SLOT(onNewFrameReceived(Mat,QLabel*)));        
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

void MainWindow::onNewFrameReceived(Mat frame, QLabel *label) {

    if (frame.empty() == false) {
        QImage image = QImage((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888);
        QPixmap pixmap = QPixmap::fromImage(image);
        label->setPixmap(pixmap.scaled(label->width() > 1 ? label->width() - 1: 1,
                                       label->height() > 1 ? label->height() - 1: 1,
                                       Qt::IgnoreAspectRatio));
    } else {
        label->clear();
        label->setText("无法从RTSP源读取画面/Failed to read frame from RTSP stream");
    }
}

void MainWindow::onNewTextMessageReceived(string labelName, string message) {
    // This slot is a must--if we allow worker threads to directly write
    // to stdout, it may be safe, but the format could be broken by possible
    // concurrent writing.

     QDateTime dateTime = dateTime.currentDateTime();
     cout << "[" << dateTime.toString("yyyy-MM-dd HH:mm:ss").toStdString() << "] "
          << labelName << ": " << message << endl;
    // Have to flush, otherwise a line may be partially shown ¯\_(ツ)_/¯
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

