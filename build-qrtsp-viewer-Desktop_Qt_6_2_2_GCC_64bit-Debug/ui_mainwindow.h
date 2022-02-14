/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget;
    QWidget *tab4Channels;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout4Channel;
    QWidget *tab16Channels;
    QHBoxLayout *horizontalLayout_3;
    QGridLayout *gridLayout16Channels;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QComboBox *comboBoxDomainNames;
    QPushButton *pushButtonExit;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(773, 602);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/default/images/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab4Channels = new QWidget();
        tab4Channels->setObjectName(QString::fromUtf8("tab4Channels"));
        horizontalLayout = new QHBoxLayout(tab4Channels);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        gridLayout4Channel = new QGridLayout();
        gridLayout4Channel->setSpacing(1);
        gridLayout4Channel->setObjectName(QString::fromUtf8("gridLayout4Channel"));
        gridLayout4Channel->setSizeConstraint(QLayout::SetDefaultConstraint);

        horizontalLayout->addLayout(gridLayout4Channel);

        tabWidget->addTab(tab4Channels, QString());
        tab16Channels = new QWidget();
        tab16Channels->setObjectName(QString::fromUtf8("tab16Channels"));
        horizontalLayout_3 = new QHBoxLayout(tab16Channels);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        gridLayout16Channels = new QGridLayout();
        gridLayout16Channels->setSpacing(1);
        gridLayout16Channels->setObjectName(QString::fromUtf8("gridLayout16Channels"));

        horizontalLayout_3->addLayout(gridLayout16Channels);

        tabWidget->addTab(tab16Channels, QString());

        verticalLayout_2->addWidget(tabWidget);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        comboBoxDomainNames = new QComboBox(centralWidget);
        comboBoxDomainNames->setObjectName(QString::fromUtf8("comboBoxDomainNames"));

        horizontalLayout_2->addWidget(comboBoxDomainNames);

        pushButtonExit = new QPushButton(centralWidget);
        pushButtonExit->setObjectName(QString::fromUtf8("pushButtonExit"));
        pushButtonExit->setMaximumSize(QSize(128, 16777215));

        horizontalLayout_2->addWidget(pushButtonExit);


        verticalLayout_2->addLayout(horizontalLayout_2);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "QRTSP\346\237\245\347\234\213\345\231\250 / QRTSP Viewer", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab4Channels), QCoreApplication::translate("MainWindow", "4\351\200\232\351\201\223 / 4 Channels", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab16Channels), QCoreApplication::translate("MainWindow", "16\351\200\232\351\201\223 / 16 Channels", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\345\237\237\345\220\215 / Select Domain Name:", nullptr));
        pushButtonExit->setText(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272 / Exit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
