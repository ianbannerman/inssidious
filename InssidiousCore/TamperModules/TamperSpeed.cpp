#include "TamperSpeed.h"
#include "TamperTypes.h"


TamperSpeed::TamperSpeed(void** ppTamperConfig)
{
	this->ppSpeedTime = (short**)ppTamperConfig;


	/* Start a linked list to buffer packets in before re-injecting at a later time */

	this->speedHeadNode = Packet{ 0 };
	this->speedTailNode = Packet{ 0 };
	bufferHead->next = bufferTail;
	bufferTail->prev = bufferHead;
	bufferSize = 0;


	/* Set a four second timer resolution for future timeGetTime calls */

	timeBeginPeriod(timerResolution);


}

TamperSpeed::~TamperSpeed()
{
	
}

short TamperSpeed::process(PacketList* packetList)
{

	if (!**ppSpeedTime || **ppSpeedTime == 0)
	{
		/* No speed type set, don't process these packets */

		return 0;
	}

	if (packetList->head == packetList->tail)
	{
		/* No packets */

		return 0;
	}


	/* Get the current time to log with the packets */

	DWORD currentTime = timeGetTime();
	DWORD releaseTime = timeGetTime();

	/* Loop through and acquire as many packets in the divert list as we can hold */
	
	Packet *pDivertPacket = packetList->tail->prev;	
	while (bufferSize < numMaxBufferPackets && pDivertPacket != packetList->head)
	{
		packetList->insertAfter(packetList->popNode(pDivertPacket), bufferHead)->timestamp = releaseTime;
		pDivertPacket = packetList->tail->prev;

		/* Increment the buffer size and the next packet's release time */
		++bufferSize;
		releaseTime += TamperSpeedTimes[**ppSpeedTime]; /* enum defining millisecond values for different speed */
		
	}


	/* Send any packets that are overdue to go out */

	while (!isBufEmpty()) 
	{
		if (currentTime > bufferTail->prev->timestamp)
		{
			packetList->insertAfter(packetList->popNode(bufferTail->prev), packetList->head);
			--bufferSize;
		}
	}



	//TODO: 
	// if buffer is full just flush things out
	if (bufferSize >= numMaxBufferPackets) 
	{
		while (bufferSize-- > 0) 
		{
			//TODO: Do we just reinsert packets or can I safely drop them?
			DebugBreak();
			packetList->insertAfter(packetList->popNode(bufferTail->prev), packetList->head);
		}
	}


	return bufferSize > 0;
};