#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QDir>
#include <QShortcut>
#include <QScrollBar>
#include <QVector>
#include <QDesktopWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QTime>
#include <QFile>
#include <QFontDatabase>
#include <QHashIterator>
#include <QThread>
#include <QImageWriter>
#include <QFileDialog>
#include <QVector>

#include "qcustomplot.h"
#include "MsgHandler.h"
#include "Camera_FLIR.h"

// === Mainwindow class ====================================================

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {

    Q_OBJECT

public:

    // --- Properties ---------------------------

    QString SetupName;

    // --- Methods ------------------------------

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:

    // Messages
    void UpdateMessage();

    // Project directories
    void BrowseProject();
    void autoset();

    // Protocols
    void BrowseProtocol();
    void toggleProtocol(bool);
    void ProtoLoop();
    void ProtocolLoop();
    void updateAge(QDate);

    // Serial communication
    void checkSerial();
    void readSerial();

    // Camera
    void InitCamera();
    void ArmCamera();
    void UpdateCamera();
    void updateDisplay(QPixmap);

    // Images
    void snapshot();
    void GrabLoop();


private:

    // --- Properties ---------------------------

    // Files and folders
    QString filesep, progPath, projPath;

    // Windows management
    Ui::MainWindow *ui;
    QShortcut *s_Close;

    // Plots
    QVector<double> Time, TempLeft, TempRight, TargetLeft, TargetRight;
    double TargetLeftValue, TargetRightValue;

    // Serial communication
    QSerialPort *Serial;
    bool skipSerial;

    // Camera
    Camera_FLIR *Camera;

    // Run
    int SaveRate;
    QTimer *timerGrab;
    int nRun;
    qint64 nFrame;
    QString RunPath;
    QImageWriter *ImgWriter;
    QString ImgComment;

    // Protocols
    QVector<QString> Protocol;
    QTime ProtocolTime;
    QTimer *timerProtocol;
    QString comment;

    // --- Methods ------------------------------

    // Serial communication
    void send(QString);
    const char* str(QString);

    // Directories
    void updatePath();

};

#endif // MAINWINDOW_H
