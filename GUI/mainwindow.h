#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QDir>
#include <QRegExp>
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

#include "plotwindow.h"
#include "MsgHandler.h"
#include "Camera_FLIR.h"
#include "Frame_Writer.h"
#include "Serial_Master.h"
#include "Pid.h"
#include "Neslab_Rte.h"

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

signals:
    void startSavingFrame();
    void sendFrame(Image_FLIR);
    void serialTerminalOutput(QString);
    void temperatureUpdate(double);
    void plotTemperature(double);
public slots:
    // Messages
    void UpdateMessage();
    void receivedSerialTerminalDialogue(QString, QString);
    void receivedTemperatureUpdate(QString, QString); 
    // Project directories
    void BrowseProject();
    void autoset();
    void sendSerialTerminalDialogue(); 
    
    void appendMetadata(double);
    void emergencyStop();
    
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
    void readingProtocolFile();

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
    //void checkSerial();
    //void readSerial();

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
    void updateDisplay(Image_FLIR);

    // Images
    void snapshot();

    void openProtocol();
    void saveProtocol(const QString &fileName);
    void loadProtocol(const QString &fileName);
    void saveAsProtocol();

private:

    QElapsedTimer *counter;
    qint64 tmp;
    // --- Properties ---------------------------

    // Files and folders
    QString filesep, progPath, projPath;

    // Windows management
    Ui::MainWindow *ui;
    QShortcut *s_Close;

    // Plots temperature window
    PlotWindow *plotWindow;;

    // Temperature sensor
    double temperature;

    //Pid
    Pid *temperaturePid;


    // Camera
    Camera_FLIR *Camera;

    //Frame writer
    Frame_Writer_Wrapper *Writer;
    QThread *ImageWriterThread;

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
    QString currentProtocol;


    // --- Methods ------------------------------

    // Serial communication
    Serial_Master *serial;
    Neslab_Rte *heater;
    QString idSerialWaitingAnswer;            // In the terminal block the answer of all serial that are not asked 
    QTimer *timer = new QTimer(this);
    // Directories
    void updatePath();


};

#endif // MAINWINDOW_H
