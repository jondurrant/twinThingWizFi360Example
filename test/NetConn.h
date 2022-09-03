/*
 * NetConn.h
 *
 *  Created on: 26 Aug 2022
 *      Author: jondurrant
 */

#ifndef NETTEST_SRC_NETCONN_H_
#define NETTEST_SRC_NETCONN_H_

#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

#include "Agent.h"

#define BUFSIZE 1024
#define DEBUG_LINE 25

class NetConn : public Agent{
public:
	NetConn();
	virtual ~NetConn();




	static void debugPrintBuffer(const char *title, const void * pBuffer, size_t bytes);
protected:

	/***
	 * Run loop for the agent.
	 */
	virtual void run();


	/***
	 * Get the static depth required in words
	 * @return - words
	 */
	virtual configSTACK_DEPTH_TYPE getMaxStackSize();


private:

	int testSocket();

	int testSNTP();

	char pBuff[BUFSIZE];

	int32_t sntp_get_time (const char *server, uint32_t *seconds);

};

#endif /* NETTEST_SRC_NETCONN_H_ */
