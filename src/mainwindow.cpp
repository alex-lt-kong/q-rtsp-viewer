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

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadSettings();

    /*int idx = 0;

    this->ui->gridLayout16Channels->addWidget(&myVideoWidgets[idx++],0,0,1,1);
    this->ui->gridLayout16Channels->addWidget(&myVideoWidgets[idx++],0,1,1,1);
    this->ui->gridLayout16Channels->addWidget(&myVideoWidgets[idx++],0,2,1,1);
    this->ui->gridLayout16Channels->addWidget(&myVideoWidgets[idx++],0,3,1,1);
    this->ui->gridLayout16Channels->addWidget(&myVideoWidgets[idx++],1,0,1,1);
    this->ui->gridLayout16Channels->addWidget(&myVideoWidgets[idx++],1,1,1,1);
    this->ui->gridLayout16Channels->addWidget(&myVideoWidgets[idx++],1,2,1,1);
    this->ui->gridLayout16Channels->addWidget(&myVideoWidgets[idx++],1,3,1,1);
    this->ui->gridLayout16Channels->addWidget(&myVideoWidgets[idx++],2,0,1,1);
    this->ui->gridLayout16Channels->addWidget(&myVideoWidgets[idx++],2,1,1,1);
    this->ui->gridLayout16Channels->addWidget(&myVideoWidgets[idx++],2,2,1,1);
    this->ui->gridLayout16Channels->addWidget(&myVideoWidgets[idx++],2,3,1,1);
    this->ui->gridLayout16Channels->addWidget(&myVideoWidgets[idx++],3,0,1,1);
    this->ui->gridLayout16Channels->addWidget(&myVideoWidgets[idx++],3,1,1,1);
    this->ui->gridLayout16Channels->addWidget(&myVideoWidgets[idx++],3,2,1,1);
    this->ui->gridLayout16Channels->addWidget(&myVideoWidgets[idx++],3,3,1,1);

    for (int i = 0; i < channelCount; i ++) {
        myVideoWidgets[i].setAspectRatioMode(Qt::AspectRatioMode::IgnoreAspectRatio);
        myVideoPlayers[i].setPlaybackRate(0);
        // Without this, fream will freeze if we play() after stop()
        myVideoPlayers[i].setVideoOutput(&myVideoWidgets[i]);
    }
*/

  /*  ui->label4Channel00->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    ui->label4Channel01->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    ui->label4Channel10->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    ui->label4Channel11->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );*/
/*
    ui->label4Channel00->setScaledContents(true);
    ui->label4Channel01->setScaledContents(true);
    ui->label4Channel10->setScaledContents(true);
    ui->label4Channel11->setScaledContents(true);
 Using Qt's native scaled() function appears to be buggy--occasionally causes segmentation fault*/


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

    srand (time(NULL));
    this->ui->comboBoxDomainNames->setCurrentIndex(rand() % this->ui->comboBoxDomainNames->count());
    on_tabWidget_currentChanged(0);
}

void MainWindow::loadSettings() {

    QSettings settings("ak-studio", "qrtsp-viewer");

    int size = settings.beginReadArray("DomainNames");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        this->ui->comboBoxDomainNames->addItem(settings.value("name").toString());
    }
    settings.endArray();

    size = settings.beginReadArray("4ChannelUrls");
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
}

MainWindow::~MainWindow()
{
    stopStreams(0);
    stopStreams(1);

    // Although the heap will be cleared by OS anyway,
    // it is still a good practice to delete[] then yourself!
    // (And not doing so results in a SegmentFault...)
    delete[] myVideoPlayers;
    delete[] myVideoWidgets;
    delete[] myUrls4Channels;
    delete[] myUrls16Channels;
    delete[] myRtspReaders;

    delete ui;
}

void MainWindow::stopStreams(int tabIndex) {

    if (tabIndex == 0) {
        for (int i = 0; i < 4; i ++) {
            myRtspReaders[i].stop();
        }
    } else {
        for (int i = 0; i < 16; i ++) {
            myRtspReaders[4+i].stop();
        }
    }
    return;
}

void MainWindow::playStreams(int tabIndex) {


    if (tabIndex == 0) {
        for (int i = 0; i < 4; i ++) {
            myRtspReaders[i].setRtspUrl(
                        QString(myUrls4Channels[i]).replace("[domain-name]", this->ui->comboBoxDomainNames->currentText()).toStdString());
            if (myRtspReaders[i].isRunning() == false) {
                myRtspReaders[i].start();
            }
        }
    } else {
        for (int i = 0; i < 16; i ++) {
            myRtspReaders[4+i].setRtspUrl(
                        QString(myUrls16Channels[i]).replace("[domain-name]", this->ui->comboBoxDomainNames->currentText()).toStdString());
            if (myRtspReaders[4+i].isRunning() == false) {
                myRtspReaders[4+i].start();
            }
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
        stopStreams(1);
        playStreams(0);
    }
    else {
        stopStreams(0);
        playStreams(1);
    }
}


void MainWindow::on_comboBoxDomainNames_currentIndexChanged(int index)
{
    return;
    cout << this->ui->comboBoxDomainNames->currentText().toStdString() << endl;
    if (this->ui->tabWidget->currentIndex() == 0) {
        stopStreams(0);
        stopStreams(1);

        playStreams(0);
    }
    else {
        stopStreams(0);
        stopStreams(1);

        playStreams(1);
    }
}
