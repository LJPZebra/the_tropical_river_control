#include "Frame_Writer.h"

/* =================================================================== *\
|                       Frame_Writer Class                             |
| This class implementes a QImageWriter object to write images on the  |
| disk and a buffer allowing a constant speed acquision of image.      |
| The Frame_Writer object need to be run in is own thread. Image saving|
| is thread safe but adding image in the buffer have to be done in a   |
| thread safe manner (see below Frame_Writer_Wrapper that manage thread|
| buffering and writing. See Frame_Writer.h for full documentation.    |
\* =================================================================== */

Frame_Writer::Frame_Writer(QString runPath){
  m_runPath = runPath;
  m_nFrame = 0;
  m_bufferSize = 0;
  ImageWriter = new QImageWriter;
  ImageWriter->setFormat("pgm");
}


void Frame_Writer::processBuffer(){
  
  Image_FLIR frame;
  QPixmap pix;
  bool isFrameToSave = false; // Necessary to minimize the buffer lock time
  int timestamp;

  while(m_isProcess || m_bufferSize > 0){ // Process the buffer when the Frame_Writer is started until it is stopped and the buffer is empty

    // Thread safe access to the buffer to take a frame
    m_mutex.lock();
    if(m_buffer.size() > 0){
      frame = m_buffer.front();
      m_buffer.pop();
      m_bufferSize = m_buffer.size();
      isFrameToSave = true;
}
    m_mutex.unlock();

    if(isFrameToSave){

      // Write frame to disk
      pix = QPixmap::fromImage(frame.Img);
      ImageWriter->setFileName(QString(m_runPath + "Frame_%1.pgm").arg(m_nFrame, 6, 10, QLatin1Char('0')));
      ImageWriter->write(pix.toImage());

      // Append metadata
      timestamp = frame.timestamp;
      QFile ImgFile(ImageWriter->fileName());
      if (ImgFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
          QTextStream tmp(&ImgFile);
          tmp << endl << "#" + QString("Timestamp:%1;Temperature:%2").arg(timestamp).arg(frame.meta);
      }

      emit bufferSize(m_bufferSize);
      emit savedFrames(m_nFrame);
      m_nFrame++;
      isFrameToSave = false;
    }
  }
  emit finished(); // Destroy the Frame_Writer object after the saving is completed
}

void Frame_Writer::displayInfo(){
  // Thread info
  qInfo().nospace() << THREAD << "Frame writer lives in thread: " << QThread::currentThreadId();
}


Frame_Writer_Wrapper::Frame_Writer_Wrapper(){
  isSaving = false;
}





/* =================================================================== *\
|                     Frame_Writer_Wrapper Class                       |
| This class implementes a wrapper to the Frame_Writer object.It allows|
| to create and manage easily the creation and destruction of a        |
| Frame_Writer object and the thread safety. 
\* =================================================================== */

void Frame_Writer_Wrapper::newWriter(QString runPath){

    // Frame_Writer object have to live in is own thread
    Writer = new Frame_Writer(runPath);
    ImageWriterThread = new QThread;
    Writer->moveToThread(ImageWriterThread);

    connect(ImageWriterThread, SIGNAL(started()), Writer, SLOT(displayInfo()));
    connect(ImageWriterThread, SIGNAL(started()), Writer, SLOT(processBuffer()));
    connect(Writer, SIGNAL(bufferSize(int)), this, SIGNAL(bufferSize(int)));
    connect(Writer, SIGNAL(savedFrames(int)), this, SIGNAL(savedFrames(int)));
    connect(Writer, SIGNAL(finished()), ImageWriterThread, SLOT(quit()));
    connect(Writer, SIGNAL(finished()), Writer, SLOT(deleteLater()));
    connect(ImageWriterThread, SIGNAL(finished()), ImageWriterThread, SLOT(deleteLater()));
}

void Frame_Writer_Wrapper::addFrame(Image_FLIR frame){
    if(isSaving){ // Thread safe access to put frame in the Frame_Writer buffer
        Writer->m_mutex.lock();
        Writer->m_buffer.push(frame);
        Writer->m_mutex.unlock();
      }
}

void Frame_Writer_Wrapper::startSaving(QString path){
    newWriter(path);
    Writer->m_isProcess = true;
    isSaving = true;
    ImageWriterThread->start();
}

void Frame_Writer_Wrapper::stopSaving(){
    isSaving = false;
    Writer->m_isProcess = false;
}
