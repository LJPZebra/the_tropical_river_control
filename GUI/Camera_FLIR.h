#ifndef CAMERA_FLIR_H
#define CAMERA_FLIR_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <math.h>

#include <QTime>
#include <QTimer>

#include "Spinnaker.h"
#include "SpinGenApi/SpinnakerGenApi.h"

#include "MsgHandler.h"

using namespace Spinnaker;
using namespace Spinnaker::GenApi;
using namespace Spinnaker::GenICam;
using namespace std;


/**
  * Image_FLIR structure to represent the image and all its informations
  */
struct Image_FLIR {
    /*@{*/
    QString CameraName; /**< the camera name */
    qint64 frameId; /**< Id of the frame */
    qint64 timestamp; /**< Timestamp in nanoseconds */
    qint64 gain;  /**< Value of the gain when capturing the image */
    QImage Img; /**< Image in type QImage */
    /*@}*/
};


Q_DECLARE_METATYPE(Image_FLIR)

/* =================================================================== *\
|    LowLevel_FLIR Class                                                |
\* =================================================================== */
/**
 * \class LowLevel_FLIR
 *
 * \brief This class allow to set the parameter of the FLIR camera and to grab frame and frame information in a struture.
 *
 *
 * \note As to be reformated: variable name and method, to have the standart.
 *
 * \author Raphaël Candelier & Benjamin Gallois
 *
 * \version $Revision: 2.0
 *
 * \date Date: 2018/10 
 *
 */
class LowLevel_FLIR : public QObject {

    Q_OBJECT

public:

    // Constructor and destructor
    LowLevel_FLIR(int);
    ~LowLevel_FLIR();

    // Camera parameters
    int CamId;
    QString CamName;
    int Exposure;
    int64_t OffsetX;
    int64_t OffsetY;
    int64_t Width;
    int64_t Height;
    float frameRate;
    bool grabState;
    float readFrameRate;

public slots:

    /**
      *@brief Get the camera information (Id, name, availability) and output it with qInfo
      * in the QDebug framework  
    */
    void display_info();


    /**
      *@brief Frame grabber, set all the parameter and grab frames
    */
    void grab();



signals:

  /**
    *@brief Emit a signal with the framerate when a frame is grabbed 
    *@note To be implemented and tested
    */
    void checkFrameRate(float);

  /**
    *@brief Emit a signal with the Image_FLIR structure when a frame is grabbed 
    */
    void newImage(Image_FLIR);
private:

    // --- Internal FLIR properties
    SystemPtr FLIR_system;
    CameraList FLIR_camList;
    CameraPtr pCam;

};


/* =================================================================== *\
|    Camera_FLIR Class                                                  |
\* =================================================================== */
/**
 * \class Camera_FLIR
 *
 * \brief 
 *
 *
 * \note As to be reformated: variable name and method, to have the standart.
 *
 * \author Raphaël Candelier & Benjamin Gallois
 *
 * \version $Revision: 2.0
 *
 * \date Date: 2018/10 
 *
 */
class Camera_FLIR : public QObject {

    Q_OBJECT

public:


    // Constructor and destructor
    Camera_FLIR(int);
    ~Camera_FLIR();

    void newCamera(); // Low_Level_FLIR camera object
    
    // Camera parameters
    float DisplayRate;
    float Exposure;
    int X1, X2, Y1, Y2;
    float frameRate;

    //Camera nformations
    int CamId;
    QString CamName;

public slots:

    /**
      *@brief Method that call the display_info() method of the Low-Level_FLIR camera object   
    */
    void display_info();

    /**
      *@brief Get the Qimage from the structure Image_FLIR and convert it to Pixmap to sent it to display
      *@note To be implemented (saving displaying and timestamp
      *@arg[in] structure Image_FLIR that contains the image and the image information 
    */
    void newImage(Image_FLIR);

    /**
      *@brief Stop the camera
    */
    void stopCamera();

signals:

    /**
      *@brief Send a QPixmap image to be displayed
    */
    void newImageForDisplay(QPixmap);

private:

    LowLevel_FLIR *Camera;
    QThread *t_Cam;

    Image_FLIR Image;
    qint64 tRefDisp;
    qint64 tRefSave;

};

#endif
