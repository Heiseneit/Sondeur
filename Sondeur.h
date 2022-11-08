#pragma once

#include <QObject>
#include <QSerialPort>
#include <QDebug>
#include <QRegExp>
#include <QtWidgets/QMainWindow>
#include "ui_Sondeur.h"
#include <QThread>
#include <QTimer>

class Sondeur : public QMainWindow
{
    Q_OBJECT

public:
    Sondeur(QWidget *parent = Q_NULLPTR);

public slots:
	void onSerialPortReadyRead();
	void connectPort();
	void disconnectPort();

private:
    Ui::SondeurClass ui;
	QSerialPort * port;
	QString data;
	QStringList dataListGPS;
	QStringList dataListTemp;
	int i = 0;
};