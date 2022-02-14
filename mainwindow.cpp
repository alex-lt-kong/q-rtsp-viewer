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
        myVideoPlayers[i].setVideoOutput(&myVideoWidgets[i]);
    }
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
    for (int i = 0; i < 20; i ++) {
        myVideoPlayers[i].stop();
    }
    delete[] myVideoPlayers;
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
        for (int i = 0; i < 2; i ++) {
            if (myVideoPlayers[4+i].playbackState() == QMediaPlayer::PlayingState) {
                myVideoPlayers[4+i].stop();
            }
        }
    }
}

void MainWindow::playStreams(int tabIndex) {

    if (tabIndex == 0) {
        for (int i = 0; i < 4; i ++) {
            myVideoPlayers[i].setSource(myUrls4Channels[i].replace("[domain-name]", this->ui->comboBoxDomainNames->currentText()));
            if (myVideoPlayers[i].playbackState() != QMediaPlayer::PlayingState) {
                myVideoPlayers[i].play();
                cout << myVideoPlayers[i].errorString().toStdString() << endl;
            }
        }
    } else {
        for (int i = 0; i < 16; i ++) {
            myVideoPlayers[4+i].setSource(myUrls16Channels[i].replace("[domain-name]", this->ui->comboBoxDomainNames->currentText()));
            if (myVideoPlayers[4+i].playbackState() != QMediaPlayer::PlayingState) {
                myVideoPlayers[4+i].play();
                cout << myVideoPlayers[i].errorString().toStdString() << endl;
            }
        }
    }
}


void MainWindow::on_pushButton_2_clicked()
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
    if (this->ui->tabWidget->currentIndex() == 0) {
        stopStreams(1);
        playStreams(0);
    }
    else {
        stopStreams(0);
        playStreams(1);
    }
}

