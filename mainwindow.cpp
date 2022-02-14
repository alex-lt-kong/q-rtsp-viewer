#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGridLayout>
#include <QMediaPlayer>
#include <QNetworkRequest>
#include <QVideoWidget>
#include <QSettings>
#include <iostream>
#include<QDebug>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadSettings();

    int idx = 0;
    this->ui->gridLayout4Channel->addWidget(&myVideoWidgets[idx++],0,0,1,1);
    this->ui->gridLayout4Channel->addWidget(&myVideoWidgets[idx++],0,1,1,1);
    this->ui->gridLayout4Channel->addWidget(&myVideoWidgets[idx++],1,0,1,1);
    this->ui->gridLayout4Channel->addWidget(&myVideoWidgets[idx++],1,1,1,1);

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

    delete ui;
}

void MainWindow::stopStreams(int tabIndex) {

    if (tabIndex == 0) {
        for (int i = 0; i < 4; i ++) {
            if (myVideoPlayers[i].playbackState() == QMediaPlayer::PlayingState) {
                myVideoPlayers[i].stop();
            }

        }
    } else {
        for (int i = 0; i < 16; i ++) {
            if (myVideoPlayers[4+i].playbackState() == QMediaPlayer::PlayingState) {
                myVideoPlayers[4+i].stop();
            }
        }
    }
}

void MainWindow::playStreams(int tabIndex) {

    if (tabIndex == 0) {
        for (int i = 0; i < 4; i ++) {
            cout << this->ui->comboBoxDomainNames->currentText().toStdString() << endl;
            myVideoPlayers[i].setSource(QString(myUrls4Channels[i]).replace("[domain-name]", this->ui->comboBoxDomainNames->currentText()));
            if (myVideoPlayers[i].playbackState() != QMediaPlayer::PlayingState) {
                myVideoPlayers[i].play();
                cout << "Start playing: " << myVideoPlayers[i].source().toString().toStdString() << endl;
            }
            // QString is replace()'ed inplace, therefore, we need to do QString(QString).replace()
            // so that we only manipulate its copy.
        }
    } else {
        for (int i = 0; i < 16; i ++) {
            myVideoPlayers[4+i].setSource(QString(myUrls16Channels[i]).replace("[domain-name]", this->ui->comboBoxDomainNames->currentText()));
            if (myVideoPlayers[4+i].playbackState() != QMediaPlayer::PlayingState) {
                myVideoPlayers[4+i].play();
                cout << "Start playing: " << myVideoPlayers[4+i].source().toString().toStdString() << endl;
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
