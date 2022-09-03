/*
 * TestWifi.cpp
 *
 *  Created on: 3 Sept 2022
 *      Author: jondurrant
 */

#include "TestWifi.h"
#include "WifiHelper.h"
#include <ctime>
#include "hardware/rtc.h"
#include "pico/stdlib.h"
#include "pico/util/datetime.h"

#define DELAY 5000

TestWifi::TestWifi() {
	// TODO Auto-generated constructor stub

}

TestWifi::~TestWifi() {
	// TODO Auto-generated destructor stub
}

void TestWifi::run(){

	test();

	printf("RUN %d TESTS, SUCESSFUL %d\n", xTests, xSuccessful);

	while (true) { // Loop forever
		vTaskDelay(DELAY);
	}

}


/***
* Get the static depth required in words
* @return - words
*/
configSTACK_DEPTH_TYPE TestWifi::getMaxStackSize(){
	return 5000;
}

void TestWifi::test(){

	xTests++;
	if (!testConnect()){
		printf("CONNECTIONED FAILED\n");
	} else {
		xSuccessful++;
	}

	xTests++;
	if (!testGetIP()){
		printf("GET IP FAILED\n");
	} else {
		xSuccessful++;
	}

	xTests++;
	if (!testGetMac()){
		printf("testGetMac FAILED\n");
	} else {
		xSuccessful++;
	}

	xTests++;
	if (!testSNTP()){
		printf("testSNTP FAILED\n");
	} else {
		xSuccessful++;
	}
}


bool TestWifi::testConnect(){
	if (WifiHelper::connectToAp(WIFI_SSID, WIFI_SSID)){
		if (WifiHelper::isJoined()) {
			return true;
		} else {
			printf("is Joined is false\n\r");
		}
	} else {
		printf("Connect failed\n\r");
	}

	return false;
}


bool TestWifi::testGetIP(){
	uint8_t ip[4] = {0,0,0,0};
	if (!WifiHelper::getIPAddress(ip)) {
		return false;
	}
	for (int i=0; i < 4; i++) {
		if (ip[i] == 0){
			return false;
		}
	}

	char ipS[16];
	if (!WifiHelper::getIPAddressStr(ipS)){
		return false;
	}
	if ((strlen(ipS)< 7 ) || strlen(ipS)>16){
		return false;
	}
	return true;
}

bool TestWifi::testGetMac(){
	char mac[18];
	if(!WifiHelper::getMACAddressStr(mac)){
		printf("getMAC returns false\n\r");
		return false;
	}
	if (strlen(mac) != 12){
		printf("Mac length should be 12 but %d, %s\n\r", strlen(mac), mac);
		return false;
	}
	return true;
}

bool TestWifi::testSNTP(){
	if (!WifiHelper::syncRTCwithSNTP()){
		return false;
	}

	datetime_t t;
	rtc_get_datetime(&t);
	if ((t.year <2022) || (t.year > 2030)){
		return false;
	}
	return true;
}


