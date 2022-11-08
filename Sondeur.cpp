#include "Sondeur.h"

Sondeur::Sondeur(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);

	//Initialisation du port s�rie
	port = new QSerialPort(this);
	port->setPortName("COM1");
	port->open(QIODevice::ReadWrite);
	port->setBaudRate(QSerialPort::Baud4800);
	port->setDataBits(QSerialPort::DataBits::Data8);
	port->setParity(QSerialPort::Parity::NoParity);
	port->setStopBits(QSerialPort::StopBits::OneStop);
	port->setFlowControl(QSerialPort::NoFlowControl);
	ui.labelStatus->setText("Port connecte");
}

void Sondeur::onSerialPortReadyRead() {

	//R�cup�ration les trames et s'incr�mente entre elles jusqu'a ce que la trame soit compl�te
	QByteArray trame = port->readAll();
	data = data + trame.toStdString().c_str();
	ui.listWidgetHistorique_2->addItem("Trame Generale:");
	ui.listWidgetHistorique_2->addItem(data+"\n");

	//V�rification des expressions r�guli�res pour la trame GPS ($GPGGA) et la trame Temp�rature ($SDMTW)
	QRegExp regExpGPS("\\$GPGGA([^\*]\*)");
	QRegExp regExpTemp("\\$SDMTW([^\*]\*)");
	if (regExpGPS.indexIn(data) > -1 && regExpTemp.indexIn(data) > -1) {

		/* TRAME GPS */
		//D�coupage de la trame GPS
		QStringList resDataListGPS = regExpGPS.capturedTexts();
		QString resDataGPS = resDataListGPS[0];
		QString gps = resDataGPS.left(resDataGPS.size());
		dataListGPS = gps.split(QLatin1Char(','), Qt::SkipEmptyParts);

		//R�cup�ration des donn�es GPS dans la trame
		QString Longitude = dataListGPS[2];
		QString Latitude = dataListGPS[4];
		QString Profondeur = dataListGPS[9];

		//Affichage des donn�es de la trame GPS
		ui.listWidgetHistorique->addItem("Trame GPS ($GPGGA):");
		ui.listWidgetHistorique->addItem(gps);
		ui.labelLongitude->setText(Longitude); 
		ui.labelLatitude->setText(Latitude);
		ui.labelProfondeur->setText(Profondeur);

		/* TRAME TEMPERATURE */
		//D�coupage de la trame GPS
		QStringList resDataListTemp = regExpTemp.capturedTexts();
		QString resDataTemp = resDataListTemp[0];
		QString temp = resDataTemp.left(resDataTemp.size());
		dataListTemp = temp.split(QLatin1Char(','), Qt::SkipEmptyParts);

		ui.listWidgetHistorique->addItem("Trame Temperature ($SDMTW):");
		ui.listWidgetHistorique->addItem(temp);

		//V�rification de l'existance d'une donn�e de temp�rature dans la trame
		if (dataListTemp[1] == "C") {
			ui.labelTemperature->setText("NULL");
		}
		else {
			//R�cup�ration de la temp�rature dans la trame
			QString Temperature = dataListTemp[1];

			//Affichage de la temp�rature de la trame Temp�rature
			ui.labelTemperature->setText(Temperature);
		}

		ui.listWidgetHistorique->addItem("\n");
	}
}


void Sondeur::connectPort() {
	//Initialisation du timer
	QTimer *timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(disconnectPort()));
	
	if (ui.temps->text().size() > 0) {
		QString time = ui.temps->text();
		timer->start(time.toInt());
		ui.start->setEnabled(false);
		ui.listWidgetHistorique->clear();
		ui.temps->clear();

		//Connexion du port s�rie
		QObject::connect(port, SIGNAL(readyRead()), this, SLOT(onSerialPortReadyRead()));
	}
}

void Sondeur::disconnectPort() {
	ui.start->setEnabled(true);

	//D�connexion du port s�rie
	QObject::disconnect(port, nullptr, nullptr, nullptr);
}