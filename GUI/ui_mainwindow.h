/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tabMain;
    QLabel *Image;
    QLabel *label_6;
    QLabel *label_7;
    QSpinBox *LightValue;
    QPushButton *ProtocolRun;
    QPushButton *Snapshot;
    QWidget *gridLayoutWidget_3;
    QGridLayout *gridLayout_3;
    QLineEdit *DataPath;
    QPushButton *ProtocolPathButton;
    QPushButton *ProjectPathButton;
    QLineEdit *ProtocolPath;
    QPushButton *Autoset;
    QLineEdit *ProjectPath;
    QLabel *ProtocolTime;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLineEdit *TargetRight;
    QLabel *label;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *TempRight;
    QLabel *TempLeft;
    QLineEdit *TargetLeft;
    QLabel *label_2;
    QPushButton *setTargets;
    QPushButton *Regulation;
    QCustomPlot *PlotLeft;
    QCustomPlot *PlotRight;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QLabel *StainLabel;
    QLabel *DateLabel;
    QLineEdit *Strain;
    QLabel *label_9;
    QLineEdit *Age;
    QDateEdit *SpawningDate;
    QWidget *tabSettings;
    QTextBrowser *Output;
    QPushButton *CheckSerial;
    QPushButton *UpdateCamera;
    QWidget *gridLayoutWidget_4;
    QGridLayout *gridLayout_4;
    QSpinBox *Exposure;
    QLabel *label_8;
    QLabel *label_10;
    QSpinBox *X1;
    QSpinBox *Y1;
    QSpinBox *X2;
    QSpinBox *Y2;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *Label_P;
    QLabel *Label_I;
    QLabel *Label_D;
    QDoubleSpinBox *Pcoeff;
    QDoubleSpinBox *Icoeff;
    QDoubleSpinBox *Dcoeff;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1300, 750);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(10, 10, 1280, 670));
        tabMain = new QWidget();
        tabMain->setObjectName(QStringLiteral("tabMain"));
        Image = new QLabel(tabMain);
        Image->setObjectName(QStringLiteral("Image"));
        Image->setGeometry(QRect(10, 10, 640, 300));
        Image->setStyleSheet(QStringLiteral("background: lightgrey;"));
        Image->setScaledContents(true);
        Image->setAlignment(Qt::AlignCenter);
        label_6 = new QLabel(tabMain);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(745, 137, 21, 21));
        label_7 = new QLabel(tabMain);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(660, 110, 121, 20));
        label_7->setAlignment(Qt::AlignCenter);
        LightValue = new QSpinBox(tabMain);
        LightValue->setObjectName(QStringLiteral("LightValue"));
        LightValue->setGeometry(QRect(680, 135, 61, 27));
        LightValue->setMaximum(100);
        LightValue->setValue(50);
        ProtocolRun = new QPushButton(tabMain);
        ProtocolRun->setObjectName(QStringLiteral("ProtocolRun"));
        ProtocolRun->setGeometry(QRect(920, 110, 201, 41));
        ProtocolRun->setCheckable(true);
        Snapshot = new QPushButton(tabMain);
        Snapshot->setObjectName(QStringLiteral("Snapshot"));
        Snapshot->setGeometry(QRect(810, 110, 91, 41));
        gridLayoutWidget_3 = new QWidget(tabMain);
        gridLayoutWidget_3->setObjectName(QStringLiteral("gridLayoutWidget_3"));
        gridLayoutWidget_3->setGeometry(QRect(660, 10, 611, 95));
        gridLayout_3 = new QGridLayout(gridLayoutWidget_3);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        DataPath = new QLineEdit(gridLayoutWidget_3);
        DataPath->setObjectName(QStringLiteral("DataPath"));
        DataPath->setEnabled(false);

        gridLayout_3->addWidget(DataPath, 1, 0, 1, 1);

        ProtocolPathButton = new QPushButton(gridLayoutWidget_3);
        ProtocolPathButton->setObjectName(QStringLiteral("ProtocolPathButton"));

        gridLayout_3->addWidget(ProtocolPathButton, 2, 1, 1, 1);

        ProjectPathButton = new QPushButton(gridLayoutWidget_3);
        ProjectPathButton->setObjectName(QStringLiteral("ProjectPathButton"));

        gridLayout_3->addWidget(ProjectPathButton, 0, 1, 1, 1);

        ProtocolPath = new QLineEdit(gridLayoutWidget_3);
        ProtocolPath->setObjectName(QStringLiteral("ProtocolPath"));

        gridLayout_3->addWidget(ProtocolPath, 2, 0, 1, 1);

        Autoset = new QPushButton(gridLayoutWidget_3);
        Autoset->setObjectName(QStringLiteral("Autoset"));

        gridLayout_3->addWidget(Autoset, 1, 1, 1, 1);

        ProjectPath = new QLineEdit(gridLayoutWidget_3);
        ProjectPath->setObjectName(QStringLiteral("ProjectPath"));

        gridLayout_3->addWidget(ProjectPath, 0, 0, 1, 1);

        ProtocolTime = new QLabel(tabMain);
        ProtocolTime->setObjectName(QStringLiteral("ProtocolTime"));
        ProtocolTime->setGeometry(QRect(1130, 110, 141, 41));
        QFont font;
        font.setFamily(QStringLiteral("DejaVu Sans"));
        font.setPointSize(20);
        ProtocolTime->setFont(font);
        ProtocolTime->setAlignment(Qt::AlignCenter);
        gridLayoutWidget = new QWidget(tabMain);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(660, 170, 311, 101));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        TargetRight = new QLineEdit(gridLayoutWidget);
        TargetRight->setObjectName(QStringLiteral("TargetRight"));
        TargetRight->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(TargetRight, 2, 2, 1, 1);

        label = new QLabel(gridLayoutWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_3, 0, 2, 1, 1);

        label_4 = new QLabel(gridLayoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        label_5 = new QLabel(gridLayoutWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_5, 2, 0, 1, 1);

        TempRight = new QLabel(gridLayoutWidget);
        TempRight->setObjectName(QStringLiteral("TempRight"));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        TempRight->setFont(font1);
        TempRight->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(TempRight, 1, 2, 1, 1);

        TempLeft = new QLabel(gridLayoutWidget);
        TempLeft->setObjectName(QStringLiteral("TempLeft"));
        TempLeft->setFont(font1);
        TempLeft->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(TempLeft, 1, 1, 1, 1);

        TargetLeft = new QLineEdit(gridLayoutWidget);
        TargetLeft->setObjectName(QStringLiteral("TargetLeft"));
        TargetLeft->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(TargetLeft, 2, 1, 1, 1);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_2, 0, 1, 1, 1);

        setTargets = new QPushButton(gridLayoutWidget);
        setTargets->setObjectName(QStringLiteral("setTargets"));

        gridLayout->addWidget(setTargets, 2, 3, 1, 1);

        Regulation = new QPushButton(gridLayoutWidget);
        Regulation->setObjectName(QStringLiteral("Regulation"));
        Regulation->setCheckable(true);
        Regulation->setChecked(false);
        Regulation->setAutoDefault(false);

        gridLayout->addWidget(Regulation, 1, 3, 1, 1);

        PlotLeft = new QCustomPlot(tabMain);
        PlotLeft->setObjectName(QStringLiteral("PlotLeft"));
        PlotLeft->setGeometry(QRect(10, 320, 621, 311));
        PlotRight = new QCustomPlot(tabMain);
        PlotRight->setObjectName(QStringLiteral("PlotRight"));
        PlotRight->setGeometry(QRect(640, 320, 621, 311));
        gridLayoutWidget_2 = new QWidget(tabMain);
        gridLayoutWidget_2->setObjectName(QStringLiteral("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(990, 170, 231, 101));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        StainLabel = new QLabel(gridLayoutWidget_2);
        StainLabel->setObjectName(QStringLiteral("StainLabel"));

        gridLayout_2->addWidget(StainLabel, 0, 0, 1, 1);

        DateLabel = new QLabel(gridLayoutWidget_2);
        DateLabel->setObjectName(QStringLiteral("DateLabel"));

        gridLayout_2->addWidget(DateLabel, 1, 0, 1, 1);

        Strain = new QLineEdit(gridLayoutWidget_2);
        Strain->setObjectName(QStringLiteral("Strain"));

        gridLayout_2->addWidget(Strain, 0, 1, 1, 1);

        label_9 = new QLabel(gridLayoutWidget_2);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_2->addWidget(label_9, 2, 0, 1, 1);

        Age = new QLineEdit(gridLayoutWidget_2);
        Age->setObjectName(QStringLiteral("Age"));
        Age->setEnabled(false);

        gridLayout_2->addWidget(Age, 2, 1, 1, 1);

        SpawningDate = new QDateEdit(gridLayoutWidget_2);
        SpawningDate->setObjectName(QStringLiteral("SpawningDate"));

        gridLayout_2->addWidget(SpawningDate, 1, 1, 1, 1);

        tabWidget->addTab(tabMain, QString());
        tabSettings = new QWidget();
        tabSettings->setObjectName(QStringLiteral("tabSettings"));
        Output = new QTextBrowser(tabSettings);
        Output->setObjectName(QStringLiteral("Output"));
        Output->setGeometry(QRect(10, 10, 451, 621));
        Output->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        CheckSerial = new QPushButton(tabSettings);
        CheckSerial->setObjectName(QStringLiteral("CheckSerial"));
        CheckSerial->setGeometry(QRect(470, 10, 99, 27));
        UpdateCamera = new QPushButton(tabSettings);
        UpdateCamera->setObjectName(QStringLiteral("UpdateCamera"));
        UpdateCamera->setGeometry(QRect(580, 10, 121, 27));
        gridLayoutWidget_4 = new QWidget(tabSettings);
        gridLayoutWidget_4->setObjectName(QStringLiteral("gridLayoutWidget_4"));
        gridLayoutWidget_4->setGeometry(QRect(490, 50, 281, 128));
        gridLayout_4 = new QGridLayout(gridLayoutWidget_4);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        Exposure = new QSpinBox(gridLayoutWidget_4);
        Exposure->setObjectName(QStringLiteral("Exposure"));
        Exposure->setAlignment(Qt::AlignCenter);
        Exposure->setMaximum(1000000);
        Exposure->setValue(40);

        gridLayout_4->addWidget(Exposure, 0, 1, 1, 1);

        label_8 = new QLabel(gridLayoutWidget_4);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_4->addWidget(label_8, 0, 0, 1, 1);

        label_10 = new QLabel(gridLayoutWidget_4);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout_4->addWidget(label_10, 0, 2, 1, 1);

        X1 = new QSpinBox(gridLayoutWidget_4);
        X1->setObjectName(QStringLiteral("X1"));
        X1->setAlignment(Qt::AlignCenter);
        X1->setMaximum(1280);
        X1->setSingleStep(4);

        gridLayout_4->addWidget(X1, 2, 0, 1, 1);

        Y1 = new QSpinBox(gridLayoutWidget_4);
        Y1->setObjectName(QStringLiteral("Y1"));
        Y1->setAlignment(Qt::AlignCenter);
        Y1->setMaximum(1024);
        Y1->setSingleStep(2);
        Y1->setValue(140);

        gridLayout_4->addWidget(Y1, 1, 1, 1, 1);

        X2 = new QSpinBox(gridLayoutWidget_4);
        X2->setObjectName(QStringLiteral("X2"));
        X2->setAlignment(Qt::AlignCenter);
        X2->setMaximum(1280);
        X2->setSingleStep(4);
        X2->setValue(1280);

        gridLayout_4->addWidget(X2, 2, 2, 1, 1);

        Y2 = new QSpinBox(gridLayoutWidget_4);
        Y2->setObjectName(QStringLiteral("Y2"));
        Y2->setAlignment(Qt::AlignCenter);
        Y2->setMaximum(1024);
        Y2->setValue(740);

        gridLayout_4->addWidget(Y2, 3, 1, 1, 1);

        formLayoutWidget = new QWidget(tabSettings);
        formLayoutWidget->setObjectName(QStringLiteral("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(830, 50, 122, 95));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        Label_P = new QLabel(formLayoutWidget);
        Label_P->setObjectName(QStringLiteral("Label_P"));

        formLayout->setWidget(0, QFormLayout::LabelRole, Label_P);

        Label_I = new QLabel(formLayoutWidget);
        Label_I->setObjectName(QStringLiteral("Label_I"));

        formLayout->setWidget(1, QFormLayout::LabelRole, Label_I);

        Label_D = new QLabel(formLayoutWidget);
        Label_D->setObjectName(QStringLiteral("Label_D"));

        formLayout->setWidget(2, QFormLayout::LabelRole, Label_D);

        Pcoeff = new QDoubleSpinBox(formLayoutWidget);
        Pcoeff->setObjectName(QStringLiteral("Pcoeff"));
        Pcoeff->setAlignment(Qt::AlignCenter);
        Pcoeff->setDecimals(2);
        Pcoeff->setMaximum(500);
        Pcoeff->setValue(75);

        formLayout->setWidget(0, QFormLayout::FieldRole, Pcoeff);

        Icoeff = new QDoubleSpinBox(formLayoutWidget);
        Icoeff->setObjectName(QStringLiteral("Icoeff"));
        Icoeff->setAlignment(Qt::AlignCenter);
        Icoeff->setDecimals(2);
        Icoeff->setValue(0.55);

        formLayout->setWidget(1, QFormLayout::FieldRole, Icoeff);

        Dcoeff = new QDoubleSpinBox(formLayoutWidget);
        Dcoeff->setObjectName(QStringLiteral("Dcoeff"));
        Dcoeff->setAlignment(Qt::AlignCenter);
        Dcoeff->setDecimals(2);
        Dcoeff->setValue(50);

        formLayout->setWidget(2, QFormLayout::FieldRole, Dcoeff);

        tabWidget->addTab(tabSettings, QString());
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1300, 25));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        Image->setText(QString());
        label_6->setText(QApplication::translate("MainWindow", "%", nullptr));
        label_7->setText(QApplication::translate("MainWindow", "Lighting intensity", nullptr));
        ProtocolRun->setText(QApplication::translate("MainWindow", "Run Protocol", nullptr));
        Snapshot->setText(QApplication::translate("MainWindow", "Snapshot", nullptr));
        ProtocolPathButton->setText(QApplication::translate("MainWindow", "Protocol ...", nullptr));
        ProjectPathButton->setText(QApplication::translate("MainWindow", "Project ...", nullptr));
        Autoset->setText(QApplication::translate("MainWindow", "Autoset", nullptr));
        ProtocolTime->setText(QApplication::translate("MainWindow", "00:00:00", nullptr));
        TargetRight->setText(QApplication::translate("MainWindow", "28", nullptr));
        label->setText(QString());
        label_3->setText(QApplication::translate("MainWindow", "Right", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "Measured", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "Target", nullptr));
        TempRight->setText(QApplication::translate("MainWindow", "-", nullptr));
        TempLeft->setText(QApplication::translate("MainWindow", "-", nullptr));
        TargetLeft->setText(QApplication::translate("MainWindow", "28", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "Left", nullptr));
        setTargets->setText(QApplication::translate("MainWindow", "Set Target", nullptr));
        Regulation->setText(QApplication::translate("MainWindow", "Regulation", nullptr));
        StainLabel->setText(QApplication::translate("MainWindow", "Strain", nullptr));
        DateLabel->setText(QApplication::translate("MainWindow", "Spawning date", nullptr));
        Strain->setText(QApplication::translate("MainWindow", "WT", nullptr));
        label_9->setText(QApplication::translate("MainWindow", "Age", nullptr));
        Age->setText(QApplication::translate("MainWindow", "0", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabMain), QApplication::translate("MainWindow", "Main", nullptr));
        CheckSerial->setText(QApplication::translate("MainWindow", "Check Serial", nullptr));
        UpdateCamera->setText(QApplication::translate("MainWindow", "Update Camera", nullptr));
        label_8->setText(QApplication::translate("MainWindow", "Exposure", nullptr));
        label_10->setText(QApplication::translate("MainWindow", "ms", nullptr));
        Label_P->setText(QApplication::translate("MainWindow", "P", nullptr));
        Label_I->setText(QApplication::translate("MainWindow", "I", nullptr));
        Label_D->setText(QApplication::translate("MainWindow", "D", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabSettings), QApplication::translate("MainWindow", "Settings", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
