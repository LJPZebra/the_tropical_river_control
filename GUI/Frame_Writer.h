#ifndef FRAME_WRITER_H
#define FRAME_WRITER_H

#include <QObject>
#include <QThread>
#include <QString>
#include <queue>
#include <QDir>
#include <QImageWriter>
#include "Camera_FLIR.h"
#include "mutex"

using namespace std;

/**
 * \class Frame_Writer
 *
 * \brief The class Frame_Writer implements an image writer with a buffer to save image on
          the disk.
 *
 * \detailed This class implements a QImageWriter object to write images on the  
              disk, and a buffer allowing a constant speed acquisition image.      
              The Frame_Writer object needs to be run in it own thread. Image saving
              is thread safe but adding image in the buffer have to be done in a   
              thread safe manner (see below Frame_Writer_Wrapper that manages thread
              buffering and writing).
 *
 * \note This class need to be run in a separate thread.
 *
 * \author Benjamin Gallois
 *
 * \date Date: 2018/10/21 
 *
 */
class Frame_Writer : public QObject {

  Q_OBJECT

public:
  /**
    *\brief Constructs the Frame_Writer object  with the path to the folder where to write frame      s. 
    *\arg QString path to where to write frames
  */
  Frame_Writer(QString);


  bool m_isProcess;
  int m_bufferSize;
  mutex m_mutex;
  queue<Image_FLIR> m_buffer;

private:
  QString m_runPath;
  QImageWriter *ImageWriter;
  int m_nFrame;

signals:
  /**
    *\brief This signal is triggered when a frame is added in the buffer and send the size
            of the buffer.
    *\arg - int - Number of frames in the buffer.
  */
  void bufferSize(int);

  /**
    *\brief This signal is triggered when a frame is written in the disk.
    *\arg - int - Number of frames written on the disk.
  */
  void savedFrames(int);

  /**
    *\brief This signal is triggerred when the writing of frames are finished. This triggered
            the destruction of the Frame_Writer object. 
  */
  void finished();

public slots:
  /**
    *\brief Starts writing images to the disk.
  */
  void processBuffer();

  /**
    *\brief Displays in the QDebug the thread id where the Frame_Writer object lives. 
  */
  void displayInfo();
};



/**
 * \class Frame_Writer_Wrapper
 *
 * \brief This class implements a wrapper to use the Frame_Writer object and manages
          thread safety.
 *
 * \detailed This class implements a wrapper for the Frame_Writer object. It allows 
             to create and manage easily the creation and destruction of a 
             Frame_Writer object in a thread safe manner.
 *
 * \author Benjamin Gallois
 *
 *
 * \date Date: 2018/10/21 
 *
 */
class Frame_Writer_Wrapper : public QObject {

  Q_OBJECT

public:
  Frame_Writer_Wrapper();
  bool isSaving;

private:
  Frame_Writer *Writer;
  QThread *ImageWriterThread;

public slots:
  /**
    *\brief Adds frames to the buffer in a thread safe manner.
    *\arg - Image_FLIR structure - Image and its informations.
  */
  void addFrame(Image_FLIR);

  /**
    *\brief Starts writing frame to the disk.
    *\arg - QString - Path to the folder where to save frames.
  */
  void startSaving(QString);

  /**
    *\brief Stops writing frames to the disk and destroys the object.
  */
  void stopSaving();

  /**
    *\brief Creates a new Frame_Writer object and its connections 
    *\arg - QString - Path to the folder where to save frames.
  */
  void newWriter(QString);

signals:
  /**
    *\brief This signal is triggered when a frame is added in the buffer and send the size
            of the buffer.
    *\arg - int - Number of frames in the buffer.
  */
  void bufferSize(int);

  /**
    *\brief This signal is triggered when a frame is written in the disk.
    *\arg - int - Number of frames written on the disk.
  */
  void savedFrames(int);
};

#endif
