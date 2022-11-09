#include <QtWidgets/QMainWindow>
#include <QDebug>
#include <QtSerialPort/QSerialPort>

class Sondeur : public QMainWindow
{
	Q_OBJECT

public:
	Sondeur(QWidget *parent = Q_NULLPTR);
    QObject

private:
    Ui::SondeurClass ui;

	QSerialPort *port;
	QString trameBuff;
	QString trame;

public slots:
	void serialPortRead();
};
