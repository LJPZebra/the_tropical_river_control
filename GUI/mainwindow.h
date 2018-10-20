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


    /**
      *@brief parse the signal emit by the Camera to update the GUI
    */
    void parsingRefreshedParameters(int, int);

    /**
      *@brief open window to select the protocol file
    */
    void browseProtocol();

    /**
      *@brief read protocol file and start the protocol
    */
    void readingProtocolFile(bool);

    /**
      *@brief parse and execute protocol instructions
      *Note bool not used to check
    */
    void parsingProtocolInstructions();

    /**
      *@brief display the protocol time in the GUI 
    */
    void displayingProtocolTime();

    /**
      *@brief reading protocol file ans start the protocol
    */
    void updateAge(QDate);

    // Serial communication
    void checkSerial();
    void readSerial();

    /**
      *@brief create a new object camera with the input parameters
    */
    void initCamera();

    /**
      *@brief set the camera parameters and start the acquisition
    */
    void armCamera();
    
    /**
      *@brief stop the running Camera and start a new Camera with the new input Parameters
    */
    void updateCamera();
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
    int saveRate;
    QTimer *timerGrab;
    bool saveFrame;
    int nRun;
    qint64 nFrame;
    QString RunPath;
    QImageWriter *ImgWriter;
    QString ImgComment;

    // Protocols
    QVector<QString> protocolInstructions;
    QTime protocolTime;
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
