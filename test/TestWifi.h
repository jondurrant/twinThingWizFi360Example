/*
 * TestWifi.h
 *
 *  Created on: 3 Sept 2022
 *      Author: jondurrant
 */

#ifndef NETTEST_SRC_TESTWIFI_H_
#define NETTEST_SRC_TESTWIFI_H_

#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

#include "Agent.h"

class TestWifi : public Agent{
public:
	TestWifi();
	virtual ~TestWifi();

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

	void test();

private:

	bool testConnect();
	bool testGetIP();
	bool testGetMac();
	bool testSNTP();

	int xTests = 0;
	int xSuccessful = 0;
};

#endif /* NETTEST_SRC_TESTWIFI_H_ */
