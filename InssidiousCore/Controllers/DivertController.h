#ifndef DIVERTCONTROLLER_H
#define DIVERTCONTROLLER_H


#include <QThread>						//Base of DivertController
#include <InssidiousCore/TamperModules/PacketList.h>


class TamperModule;

class DivertController : public QThread
{
	Q_OBJECT


public:
	DivertController(QString, void*[], volatile bool[]);	


signals:
	void divertStopped(QString MACAddress);

public slots:
	void onDivertStop(QString MACAddress);
	void onDivertUpdateIPAddress(QString MACAddress, QString IPAddress);

private:

	/* QThread class calls run as the first thing executed as the new thread */
	/* Do initialization work like identifying IP address here and opening WinDivert handle */

	void run() override;


	/* Copies of the values sent in the constructor */

	QString MACAddress;
	void** ppTamperModulesConfig;
	volatile bool* pTamperModulesActive;


	TamperModule** ppTamperModules;


	/* PacketList to hold all diverted packets, mutex to sychnronize read & tamper threads */

	PacketList* packetList;
	HANDLE mutex;
	volatile short divertActive = 0;

	/* Handles to the WinDivert instance, the divert threads, and the associated filter strings */

	HANDLE divertHandleLayerNetwork, divertHandleLayerNetworkForward = 0;
	HANDLE readLoop1, readLoop2, readLoop3, readLoop4, readLoop5, readLoop6, clockThread = 0;
	QString filterString;


	/* Functions to create threads to process packets captured by WinDivert */
	/* Static functions to pass to CreateThread that fire the instance-specific function */

	static DWORD DivertReadLoop1(void* pDivertControllerInstance);
	static DWORD DivertReadLoop2(void* pDivertControllerInstance);
	static DWORD DivertClockLoop(void* pDivertControllerInstance);
	DWORD divertReadLoop(HANDLE divertHandle);
	DWORD divertClockLoop();


	// step function to let module process and consume all packets on the list
	void divertConsumeStep();


	// sends all packets on the list
	void sendAllListPackets();

};

#endif // DIVERTCONTROLLER_H
