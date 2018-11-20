#include "mainwindow.h"
#include "ui_mainwindow.h"



/**
Constructor of the main class of the program that implementes the Ui
and its connections with other objects of the program.
*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {

    SetupName = "TheTropicalRiver";

    // Paths definition of the project
    // NOTE TO DEV: it is working but not readable, to clean
    filesep = QString(QDir::separator());
    progPath = QDir::currentPath() + filesep;
    projPath = QDir::currentPath();
    projPath = projPath.mid(0, projPath.toStdString().find_last_of(filesep.toStdString()));
    projPath = projPath.mid(0, projPath.toStdString().find_last_of(filesep.toStdString()));
    projPath = projPath.mid(0, projPath.toStdString().find_last_of(filesep.toStdString())) + filesep;

    // Variables initialization
    nRun = 0;
    nFrame = 0;
    ImgComment = QString();


    // Ui setup from mainwindow.ui
    ui->setupUi(this);
    this->setWindowTitle(SetupName);
    this->showFullScreen();
    ui->emergencyStop->setStyleSheet("background-color: red");
    ui->ProjectPath->setText(projPath);
    ui->SpawningDate->setDate(QDate::currentDate());
    connect(ui->emergencyStop, SIGNAL(clicked()), this, SLOT(emergencyStop()));
    updatePath();


    // Defines shortcuts
    s_Close = new QShortcut(Qt::Key_Escape, this);
    connect(s_Close, SIGNAL(activated()), QApplication::instance(), SLOT(quit()));


    // Information console
    // NOTE TO DEV: Trigerred an error in the qDebug console
    QFile File("output.css");
    File.open(QFile::ReadOnly);
    QTextDocument *OutDoc = new QTextDocument;
    OutDoc->setDefaultStyleSheet(File.readAll());
    OutDoc->setDefaultFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    ui->Output->setDocument(OutDoc);

    QTimer *t_Msg = new QTimer();
    connect(t_Msg, SIGNAL(timeout()), this, SLOT(UpdateMessage()));
    t_Msg->start(50);

    qInfo() << TITLE_1 << "Initialization";
    qInfo().nospace() << THREAD << "Mainwindow lives in thread: " << QThread::currentThreadId();
    qInfo() << TITLE_2 << "Camera";


/*************************************************************************************
                      CAMERA OBJECT
*************************************************************************************/

    // Camera initialization and connections
    initCamera();

    // From camera to ui
    connect(Camera, SIGNAL(refreshParameters(int, int)), this, SLOT(parsingRefreshedParameters(int, int)));

    // From ui to camera
    connect(ui->ProjectPathButton, SIGNAL(clicked()), this, SLOT(BrowseProject()));
    connect(ui->Autoset, SIGNAL(clicked()), this, SLOT(autoset()));
    connect(ui->ProtocolPathButton, SIGNAL(clicked()), this, SLOT(browseProtocol()));
    connect(ui->ProtocolRun, SIGNAL(toggled(bool)), this, SLOT(readingProtocolFile()));
    connect(ui->Snapshot, SIGNAL(clicked()), this, SLOT(snapshot()));
    connect(ui->SpawningDate, SIGNAL(dateChanged(QDate)), this, SLOT(updateAge(QDate)));

    // From objects to camera
    connect(this, SIGNAL(temperatureUpdate(double)), this, SLOT(appendMetadata(double)));

/*************************************************************************************
                      FRAME_WRITER OBJECT
*************************************************************************************/
    // Movie writer
    Writer = new Frame_Writer_Wrapper();
    connect(Camera, SIGNAL(newImageForDisplay(Image_FLIR)), Writer, SLOT(addFrame(Image_FLIR)));

    // Snapshot writer
    ImgWriter = new QImageWriter;
    ImgWriter->setFormat("pgm");


/*************************************************************************************
                      PROTOCOL
*************************************************************************************/

    timerProtocol = new QTimer(this);
    connect(timerProtocol, SIGNAL(timeout()), this, SLOT(displayingProtocolTime()));



/*****************************************************************
                      SERIAL OBJECTS
*****************************************************************/

    // Serial_Master object initialization. Manages connections with Arduino devices.
    serial = new Serial_Master;
    connect(ui->CheckSerial, SIGNAL(clicked()), serial, SLOT(checkSerialConnection()));
    connect(ui->serialTerminalInput, SIGNAL(returnPressed()), this, SLOT(sendSerialTerminalDialogue()));
    connect(serial, SIGNAL(newMessage(QString, QString)), this, SLOT(receivedSerialTerminalDialogue(QString, QString)));
    connect(this, SIGNAL(serialTerminalOutput(QString)), ui->serialTerminalOutput, SLOT(appendHtml(QString)));
   


    // Manages Neslab_Rte bath heater cooler 
    heater = new Neslab_Rte("/dev/ttyUSB0");
    heater->start();
    heater->setTemperature(28.00);
    connect(ui->startHeater, SIGNAL(released()), heater, SLOT(start()));
    connect(ui->stopHeater, SIGNAL(released()), heater, SLOT(stop()));



    // Manages temperature sensor from Arduino device connected throught Serial_Master
    connect(serial, SIGNAL(newMessage(QString, QString)), this, SLOT(receivedTemperatureUpdate(QString, QString)));
    QTimer::singleShot(2000, [=] {                           // Start temperature sensor
      serial->sendSerialCommand("river", "startTemperature");
    });



    // Manages PID controller to regulate temperature
    // Initialized with (min, max) temperature of the Neslab bath heater cooler
    temperaturePid = new Pid(-24, 90);
    temperaturePid->setP(33.94);
    temperaturePid->setI(1.80);
    temperaturePid->setD(0.08);

    // Ui to PID object
    connect(ui->P_pid, SIGNAL(valueChanged(double)), temperaturePid, SLOT(setP(double))); 
    connect(ui->I_pid, SIGNAL(valueChanged(double)), temperaturePid, SLOT(setI(double))); 
    connect(ui->P_pid, SIGNAL(valueChanged(double)), temperaturePid, SLOT(setP(double))); 
    connect(ui->temperatureSet, SIGNAL(valueChanged(double)), temperaturePid, SLOT(setTarget(double)));

    // Send actual temperature and set point and received new set point
    connect(this, SIGNAL(temperatureUpdate(double)), temperaturePid, SLOT(computeSetPoint(double)));
    connect(temperaturePid, SIGNAL(newSetPoint(double)), heater, SLOT(setTemperature(double))); 
   
    
   
    // Manages the plot window to visualize the temperature 
    plotWindow = new PlotWindow();
    connect(ui->openPlot, &QPushButton::clicked, [this]() {
      plotWindow->show();
    });
    connect(this, SIGNAL(plotTemperature(double)), plotWindow, SLOT(appendData(double)));
  
   
  
    // Manages flow controller inialized throught Serial_Master  
    connect(ui->flowRate, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [this]() { // Set flow rate
      QString command = "setFlow:" + QString::number(ui->flowRate->value());
      serial->sendSerialCommand("fl1", command);
    });
    connect(ui->stopFlow, &QPushButton::clicked, [this]() { // Stop flow
      serial->sendSerialCommand("fl1", "stop");
    });



    // Manages peristaltic pump initialized throught Serial_Master
    connect(ui->speedPump, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this]() { // Set pump speed
      QString speed =  QString("setSpeed:" + QString::number(ui->speedPump->value()));
      serial->sendSerialCommand("river", speed);
    });
    connect(ui->stopPump, &QPushButton::clicked, [this]() { // Stop pump
      serial->sendSerialCommand("river", "stop");
      ui->speedPump->setValue(0);
    });


/*****************************************************************
                      PROTOCOL EDITOR
*****************************************************************/
    connect(ui->openProtocol, SIGNAL(clicked()), this, SLOT(openProtocol())); 
    connect(ui->ProtocolPath, SIGNAL(textChanged(QString)), this, SLOT(loadProtocol(QString))); 
    connect(ui->newProtocol, &QPushButton::clicked, [this]() { // Clear protocol editor and protocol path
      ui->protocolEditor->clear();
      ui->protocolFile->clear();
      currentProtocol.clear();
    });
    connect(ui->saveProtocol, &QPushButton::clicked, [this]() {
      saveProtocol(currentProtocol);
    });
    connect(ui->saveAsProtocol, SIGNAL(clicked()), this, SLOT(saveAsProtocol())); 

}









/*****************************************************************
                      SERIAL
*****************************************************************/

void MainWindow::sendSerialTerminalDialogue() {

  QString commandInput = ui->serialTerminalInput->text();
  emit(serialTerminalOutput("<b>You</b> say: <b>@</b><i>" + commandInput +"</i>"));
  ui->serialTerminalInput->clear();
 
  // Parse input commands from the serialTerminal console. 
  if(commandInput == "ls serial") { // Special command
    QVector<QString> commandOutputList = serial->listSerials();
    for(auto i: commandOutputList) {
      emit(serialTerminalOutput(i));
    }
  }
  else { // Direct access to the serial of the Arduino
    QStringList parsedCommand = commandInput.split(QRegularExpression("::"), QString::SkipEmptyParts);
    if(parsedCommand.length() == 2){
      serial->sendSerialCommand(parsedCommand.at(0), parsedCommand.at(1));
      idSerialWaitingAnswer = parsedCommand.at(0);
    }
    else {
      emit(serialTerminalOutput("<b>The Tropical River</b> says: <i>not a valid command</i>"));
    }
  }
}


void MainWindow::receivedSerialTerminalDialogue(QString serialId, QString message) {
  // Display in the console only the answer of the serial asked in sendSerialTerminalDialogue method
  if (serialId == idSerialWaitingAnswer){
    emit(serialTerminalOutput("Serial <b>" + serialId + "</b> says: <i>" + message + "</i>"));
    idSerialWaitingAnswer.clear();
  }
}


void MainWindow::receivedTemperatureUpdate(QString serialId, QString message) {
  QStringList parsedMessage = message.split(":",QString::SkipEmptyParts);
  if (serialId == "river" &&  parsedMessage.length() > 1 &&  parsedMessage.at(0) == "temperature" ){
    temperature = parsedMessage.at(1).toDouble();
    ui->temperatureMeasured->setText(parsedMessage.at(1));
    emit(temperatureUpdate(temperature));
    emit(plotTemperature(temperature));
  }
}



/* ====================================================================== *\
|    MESSAGES                                                              |
\* ====================================================================== */

void MainWindow::parsingRefreshedParameters(int param1, int param2) {
    ui->Exposure->setValue(param1);
    ui->fps->setValue(param2);
    saveRate = param2;
}


void MainWindow::UpdateMessage() {

    while (Messages.length()) {

        Message MSG = Messages.takeFirst();
        QString S;

        switch (MSG.type) {
        case QtDebugMsg:
           // cout << MSG.text.toStdString() << endl;
            break;
        case QtInfoMsg:
            S = "<" + MSG.css + ">" + MSG.text + "</p>" ;
            break;
        case QtWarningMsg:
            S = "<p class='warning'>" + MSG.text + "</p>";
            break;
        case QtCriticalMsg:
            S= "<p class='critical'>" + MSG.text + "</p>";
            break;
        case QtFatalMsg:
            S = "<p class='fatal'>" + MSG.text + "</p>";
            break;
        }

        ui->Output->setHtml(ui->Output->toHtml().append(S));
        ui->Output->verticalScrollBar()->setValue(ui->Output->verticalScrollBar()->maximum());
    }
}

/* ====================================================================== *\
|    DIRECTORIES                                                           |
\* ====================================================================== */

void MainWindow::BrowseProject() {

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly);
    if (dir.length()) {
        ui->ProjectPath->setText(dir + filesep);
        updatePath();
    }

}

void MainWindow::updatePath() {

    QDateTime now = QDateTime::currentDateTime();
    ui->DataPath->setText(ui->ProjectPath->text() + "Data" + filesep + now.toString("yyyy-MM-dd") + filesep);
    autoset();

}


void MainWindow::autoset() {

    // Create data folder?
    if (!QDir(ui->DataPath->text()).exists()) { QDir().mkpath(ui->DataPath->text()); }

    // Find last run
    nRun = 0;
    QDir DataDir(ui->DataPath->text());
    QFileInfoList FIL = DataDir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot, QDir::DirsFirst);
    foreach(const QFileInfo &elm, FIL) {
        if(QDir(elm.absoluteFilePath()).exists()) {

            QStringList tmp = elm.fileName().split(" ", QString::SkipEmptyParts);
            if (tmp.count()==2 || !tmp.at(0).compare("Run")) {
                nRun = tmp.at(1).toInt();
            }
        }
    }

    if (nRun) {
        ui->statusBar->showMessage(QString("Last run: %1").arg(nRun, 2, 10, QLatin1Char('0')));
    } else {
        ui->statusBar->showMessage(QString("No run for today"));
    }
}

void MainWindow::updateAge(QDate) {

    ui->Age->setText(QString::number(ui->SpawningDate->date().daysTo(QDate::currentDate())));

}


/* ====================================================================== *\
|    CAMERA                                                                |
\* ====================================================================== */

void MainWindow::initCamera() {

    Camera = new Camera_FLIR(0);

    connect(ui->UpdateCamera, SIGNAL(released()), this, SLOT(updateCamera()));
    connect(Camera, SIGNAL(newImageForDisplay(Image_FLIR)), this, SLOT(updateDisplay(Image_FLIR)));

    this->armCamera();

}

void MainWindow::armCamera() {

    Camera->Exposure = ui->Exposure->text().toFloat();
    Camera->X1 = ui->X1->text().toFloat();
    Camera->X2 = ui->X2->text().toFloat();
    Camera->Y1 = ui->Y1->text().toFloat();
    Camera->Y2 = ui->Y2->text().toFloat();
    Camera->frameRate = ui->fps->text().toFloat();

    Camera->newCamera();
}

void MainWindow::updateCamera() {

    Camera->stopCamera();
    this->armCamera();
}

void MainWindow::updateDisplay(Image_FLIR FImg) {
  QPixmap pix = QPixmap::fromImage(FImg.Img);
  ui->Image->setPixmap(pix);
}



void MainWindow::snapshot() {

    // Create snapshot directory?
    QString SnapPath(ui->DataPath->text() + "Snapshots" + filesep);
    if (!QDir(SnapPath).exists()) { QDir().mkpath(SnapPath); }

    // Get last image number
    int nSnap = 0;
    QDir SnapDir(SnapPath);
    QStringList flist = SnapDir.entryList(QDir::NoDotAndDotDot | QDir::Files);
    foreach(QString file, flist) {
        QStringList tmp = file.split("_", QString::SkipEmptyParts);
        tmp = tmp.at(1).split(".");
        if (tmp.at(0).toInt()>nSnap) { nSnap = tmp.at(0).toInt(); }
    }
    nSnap++;

    // Save snapshot
    const QPixmap* pix = ui->Image->pixmap();
    ImgWriter->setFileName(QString(SnapPath + "Image_%1.pgm").arg(nSnap, 6, 10, QLatin1Char('0')));
    ImgWriter->write(pix->toImage());

    // Update status message
    ui->statusBar->showMessage(QString("Last image: %1").arg(nSnap, 6, 10, QLatin1Char('0')));

}


void MainWindow::appendMetadata(double parameter) {
  Camera->metadata = QString::number(parameter);
}
/*****************************Protocol Editor*****************************/

void MainWindow::openProtocol() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open protocol file"), ui->ProjectPath->text() + "Data" + filesep + "Protocols", "Protocol files (*.protocol);; All files (*.*)");
    if (!fileName.isEmpty()) {
            loadProtocol(fileName);
            currentProtocol = fileName;
    }
}

void MainWindow::loadProtocol(const QString &fileName) {
    QFile file(fileName);
    ui->protocolFile->setText(fileName);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
      QTextStream document(&file);
      ui->protocolEditor->setPlainText(document.readAll());
    }
}

void MainWindow::saveProtocol(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName),
                                  file.errorString()));
    }

    QTextStream out(&file);
    out << ui->protocolEditor->toPlainText();
}

void MainWindow::saveAsProtocol() {

    QString fileName = QFileDialog::getSaveFileName(this, tr("Open protocol file"), ui->ProjectPath->text() + "Data" + filesep + "Protocols", "Protocol files (*.protocol);; All files (*.*)");
    if (!fileName.isEmpty()) {
            saveProtocol(fileName);
    }
}

/* ====================================================================== *\
|    PROTOCOLS                                                             |
\* ====================================================================== */

void MainWindow::browseProtocol() {

    QString pfile = QFileDialog::getOpenFileName(this, tr("Open protocol file"), ui->ProjectPath->text() + "Data" + filesep + "Protocols", "Protocol files (*.protocol);; All files (*.*)");
    if (pfile.length()) { ui->ProtocolPath->setText(pfile); }

}

void MainWindow::readingProtocolFile() {

    if (ui->ProtocolRun->isChecked()) { // If button pressed one time to trigger the protocol start

        QFile *PFile = new QFile(ui->ProtocolPath->text());

        if (!PFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Unable to load \"" << PFile->fileName() << "\"";
            return;
        };

        QTextStream stream(PFile);
        protocolInstructions.clear();
        QString line;
        while (stream.readLineInto(&line)) { // Remove empty lines
            if (line.isEmpty() || line.left(1) == "#"){
               continue; 
            }
            protocolInstructions.append(line);
        }

        // --- Start protocol
        ui->ProtocolTime->setStyleSheet("QLabel { color: firebrick;}");
        protocolTime.start();
        timerProtocol->start(1000);
        parsingProtocolInstructions();
    } 
    
    else { // If button pressed a second time when the protocol is running

        ui->ProtocolTime->setText("00:00:00");
        ui->ProtocolTime->setStyleSheet("QLabel { color: black;}");
        protocolInstructions.clear();
        timerProtocol->stop();
    }

}

void MainWindow::displayingProtocolTime() {
    // Synchronous loop (every 1s) for display

    // Ellapsed time
    int m = protocolTime.elapsed();
    int hours = floor(m/3600000); m -= hours*3600000;
    int minutes = floor(m/60000); m -= minutes*60000;
    int seconds = floor(m/1000);

    ui->ProtocolTime->setText(QString("%1").arg(hours, 2, 10, QChar('0')) + ":" +
                              QString("%1").arg(minutes, 2, 10, QChar('0')) + ":" +
                              QString("%1").arg(seconds, 2, 10, QChar('0')));

}

void MainWindow::parsingProtocolInstructions() {
// Asynchronous loop for processing protocol commands.
// If the Arduino serial follows the standard you don't have to touch anything.
// In the protocol file just write serialId::command::value


    if (!protocolInstructions.count()) { // End of the protocol
        ui->ProtocolRun->setChecked(false);
        return;
    }

    bool bcont = true;

    QStringList list = protocolInstructions[0].split(QRegExp(":+"));
  

    if (list.at(0)=="print") { // Parse print instructions
        qInfo() << qPrintable(list.at(1));
    }
     
    else if (list.at(0)=="data") { // Parse data instructions
        if (list.at(1)=="create directory") {
            // Reset run number
            nRun++;

            // Create run directory
            if (!QDir(ui->DataPath->text()).exists()) { QDir().mkdir(ui->DataPath->text()); }
            RunPath = QString(ui->DataPath->text() + "Run %1").arg(nRun, 2, 10, QLatin1Char('0'));
            if (!QDir(RunPath).exists()) { QDir().mkdir(RunPath); }

            // Save protocol file
            QFile::copy(ui->ProtocolPath->text(), RunPath + filesep + "Protocol.txt");

            // Save parameters
            QFile fparam(RunPath + filesep + "Parameters.txt");
            if (fparam.open(QIODevice::ReadWrite)) {
                QTextStream stream(&fparam);
                stream << "Strain\t" << ui->Strain->text() << endl;
                stream << "Spawning_date\t" << ui->SpawningDate->date().toString("yyyy-MM-dd") << endl;
                stream << "Age\t" << ui->Age->text() << endl;
            }
        }
    }

    else if (list.at(0)=="camera") { // Parse camera instructions
        if (list.at(1)=="start") {

        Writer->startSaving(QString(RunPath + filesep));
        }

        else if (list.at(1)=="stop") {
          Writer->stopSaving();
        }
        else if (list.at(1)=="comment") {
            comment = list.at(2);
        }
    }

    else if (list.at(0)=="wait") { // Parse wait instructions
        QTimer::singleShot(list.at(1).toInt(), this, SLOT(parsingProtocolInstructions()));
        protocolInstructions.removeFirst();
        return;
    }

    else if (serial->serialList.contains(list.at(0))) { // Parse serial instructions
      if(list.length() == 3){
        serial->sendSerialCommand(list.at(0), QString(list.at(1) + ":" + list.at(2))); // Backward compatibility 
      }
      else if (list.length() == 2){
        serial->sendSerialCommand(list.at(0), list.at(1)); // Backward compatibility 
      }
      else{
        qDebug() << "Unknown command:" << protocolInstructions[0];
      }
    }

    else if (list.at(0) == "heater") { // Parse serial instructions
      if(list.length() == 3){
        if (list.at(1) == "setTemperature") {
          heater->setTemperature(list.at(2).toDouble());
        }
      }
      else{
        qDebug() << "Unknown command:" << protocolInstructions[0];
      }
    }
    // --- Remove first command
    protocolInstructions.removeFirst();

    // --- Continue
    if (bcont) { parsingProtocolInstructions(); }
}


void MainWindow::emergencyStop() {
    heater->stop();
    serial->sendSerialCommand("river", "stopTemperature");
    QThread::msleep(1500);                                    // Arduino temperature sensor rate
    serial->sendSerialCommand("river", "stop");
    serial->sendSerialCommand("fl1", "stop");
    ui->ProtocolTime->setText("00:00:00");
    ui->ProtocolTime->setStyleSheet("QLabel { color: black;}");
    protocolInstructions.clear();
    timerProtocol->stop();
}

MainWindow::~MainWindow() {
    delete heater;
    serial->sendSerialCommand("river", "stopTemperature");
    QThread::msleep(1500);
    serial->sendSerialCommand("river", "stop");
    serial->sendSerialCommand("fl1", "stop");
    delete ui;
}
