#include "Frame_Writer.h"

Frame_Writer::Frame_Writer(QString RunPat){

  nFrame = 0;
  RunPath = RunPat;
  ImageWriter = new QImageWriter;
  ImageWriter->setFormat("pgm");
}


void Frame_Writer::processBuffer(){
  // CAN BE REIMPLEMENTED TO MINIMIZED MUTEX LOCK TIME
  while(processStatus|buffer.size() > 0){
    mtx.lock();
    if(buffer.size() != 0){
      Image_FLIR frame = buffer.front();
      buffer.pop();
      QPixmap pix = QPixmap::fromImage(frame.Img);
      int timestamp = frame.timestamp;
      ImageWriter->setFileName(QString(RunPath + "Frame_%1.pgm").arg(nFrame, 6, 10, QLatin1Char('0')));
      ImageWriter->write(pix.toImage());
    emit bufferSize(buffer.size());
    emit savedFrames(nFrame);
    // Append metadata
      QFile ImgFile(ImageWriter->fileName());
      if (ImgFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
          QTextStream tmp(&ImgFile);
    //      tmp << endl << "#" + SetupName + " " + Version;
          tmp << endl << "#" + QString("Timestamp:%1;TempLeft:%2;TempRight:%3").arg(timestamp);
      }
    }
    mtx.unlock();
    nFrame++;
  }
  emit finished();
}

void Frame_Writer::displayInfo(){
  // Thread info
  qInfo().nospace() << THREAD << "Frame writer lives in thread: " << QThread::currentThreadId();
}


Frame_Writer_Wrapper::Frame_Writer_Wrapper(){
  saveStatus = false;
}


void Frame_Writer_Wrapper::newWriter(QString runPath){

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
    if(saveStatus){
        Writer->mtx.lock();
        Writer->buffer.push(frame);
        Writer->mtx.unlock();
      }
}

void Frame_Writer_Wrapper::startSaving(QString path){
    newWriter(path);
    Writer->processStatus = true;
    saveStatus = true;
    ImageWriterThread->start();
}

void Frame_Writer_Wrapper::stopSaving(){
    saveStatus = false;
    Writer->processStatus = false;
}
