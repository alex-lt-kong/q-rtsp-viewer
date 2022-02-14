#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>

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
    void on_comboBoxDomainNames_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    int channelCount = 20;
    QVideoWidget *myVideoWidgets = new QVideoWidget[20];
    QMediaPlayer *myVideoPlayers = new QMediaPlayer[20];
    QString *myUrls4Channels = new QString[4];
    QString *myUrls16Channels = new QString[16];

    void playStreams(int tabIndex);
    void stopStreams(int tabIndex);
    void loadSettings();
};

#endif // MAINWINDOW_H
