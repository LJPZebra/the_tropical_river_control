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

/* =================================================================== *\
|    Frame_Writer Class                                                |
\* =================================================================== */
/**
 * \class Frame_Writer
 *
 * \brief This class implementes a QImageWriter with a buffer. This allow to save all frames independant of disk speed of writing.
 *
 *
 * \note This class need to be run in a separate thread.
 *
 * \author Benjamin Gallois
 *
 *
 * \date Date: 2018/10/21 
 *
 */
class Frame_Writer : public QObject {

  Q_OBJECT

public:

  /**
    *\brief Constructor with the path to the folder where to write frames. 
    *\arg QString path to where to write frames
  */
  Frame_Writer(QString);

  int nFrame;
  bool processStatus;
  mutex mtx;
  queue<Image_FLIR> buffer;

private:
  QString RunPath;
  QImageWriter *ImageWriter;

signals:
  /**
    *\brief Signal emitted at each adding to the queue that contains the number of frames in the buffer 
  */
  void bufferSize(int);

  /**
    *\brief Signal emitted at each adding to the queue.
    *\arg int Number of frames written.
  */
  void savedFrames(int);

  /**
    *\brief Signal emitted when the saving of frames are finished to destroy the Frame-Writer object. 
  */
  void finished();

public slots:
  /**
    *\brief Start writing images to the disk.
  */
  void processBuffer();

  /**
    *\brief Display in the QDebug the thread Id where the Frame_Writer object live. 
  */
  void displayInfo();
};



/* =================================================================== *\
|    Frame_Writer_Wrapper Class                                                |
\* =================================================================== */
/**
 * \class Frame_Writer
 *
 * \brief This class implementes a wrapper to use Frame_Writer and manage thread safety.
 *
 *
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
  bool saveStatus;

private:
  Frame_Writer *Writer;
  QThread *ImageWriterThread;


public slots:
  /**
    *\brief Add frame thread safe to the buffer of the Writer. 
    *\arg Image_FLIR structure containing image and its informations
  */
  void addFrame(Image_FLIR);

  /**
    *\brief Start writing frame to the disk
    *\arg QString Path to where to save frames
  */
  void startSaving(QString);

  /**
    *\brief Stop saving frames and destroy Writer.
  */
  void stopSaving();

  /**
    *\brief Create a new Frame_Writer object and its connections 
    *\arg QString Path to where to save frames
  */
  void newWriter(QString);

signals:
  /**
    *\brief Signal emitted at each adding to the queue.
    *\arg int Number of frames in the buffer.
  */
  void bufferSize(int);

  /**
    *\brief Signal emitted at each adding to the queue.
    *\arg int Number of frames written.
  */
  void savedFrames(int);
};

#endif
