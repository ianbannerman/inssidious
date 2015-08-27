#ifndef DEVICECONTROLLER_H
#define DEVICECONTROLLER_H

#include <QObject>
#include <QTimer>
#include "DivertController.h"


class DeviceController : public QObject
{
	Q_OBJECT

public:
	DeviceController();
	~DeviceController();

signals:
	void coreTamperStarted(QString MACAddress, TamperType tamperType);
	void coreTamperStopped(QString MACAddress, TamperType tamperType);

	void dcUpdateDevice(QString MACAddress, QString ipAddress);
	void dcDivertStop();

public slots:
	void onCoreAddDevice(QString MACAddress);
	void onCoreDropDevice(QString MACAddress);
	void onCoreUpdateDevice(QString MACAddress, QString ipAddress);

	void onCoreTamperStart(QString MACAddress, TamperType tamperType);
	void onCoreTamperStop(QString MACAddress, TamperType tamperType);

private slots:
	void onDivertStopped(QString MACAddress);

private:

	QList<device*>deviceList;

	struct LostPair
	{
		QString MACAddress;
		QString ipAddress;
	};

	QList <LostPair*> lostPairs;
};

#endif // DEVICECONTROLLER_H
