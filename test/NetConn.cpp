/*
 * NetConn.cpp
 *
 *  Created on: 26 Aug 2022
 *      Author: jondurrant
 */

#include "NetConn.h"

#include "stdio.h"
#include <stdint.h>
#include <string.h>

#include "iot_socket.h"


#include "Driver_WiFi.h"

#include <ctime>
#include "hardware/rtc.h"
#include "pico/stdlib.h"
#include "pico/util/datetime.h"

#define SECURITY_TYPE   ARM_WIFI_SECURITY_WPA2

extern ARM_DRIVER_WIFI Driver_WiFi1;


#define DELAY			5000

NetConn::NetConn() {
	// TODO Auto-generated constructor stub

}

NetConn::~NetConn() {
	// TODO Auto-generated destructor stub
}

/***
 * Main Run Task for agent
 */
void NetConn::run(){
	ARM_WIFI_CONFIG_t config;
	int32_t ret;
	uint8_t net_info[4];

	printf("NetConn Started\n");

	ret = Driver_WiFi1.Initialize  (NULL);
	printf("Driver_WiFix.Initialize  (NULL) = %d\r\n", ret);

	ret = Driver_WiFi1.PowerControl(ARM_POWER_FULL);
	printf("Driver_WiFix.PowerControl(ARM_POWER_FULL) = %d\r\n", ret);

	memset((void *)&config, 0, sizeof(config));

	config.ssid     = WIFI_SSID;
	config.pass     = WIFI_PASSWORD;
	config.security = SECURITY_TYPE;
	config.ch       = 0U;

	ret = Driver_WiFi1.Activate(0U, &config);
	printf("Driver_WiFix.Activate(0U, &config) = %d\r\n", ret);

	ret = Driver_WiFi1.IsConnected();
	printf("Driver_WiFix.IsConnected() = %d\r\n", ret);

	if (ret == 0U) {
		printf("WiFi network connection failed!\r\n");
	} else {
		printf("WiFi network connection succeeded!\r\n");


		uint32_t len = 4;
		Driver_WiFi1.GetOption(0, ARM_WIFI_IP, net_info, &len);
		printf("ARM_WIFI_IP = %d.%d.%d.%d\r\n", net_info[0], net_info[1], net_info[2], net_info[3]);

		Driver_WiFi1.GetOption(0, ARM_WIFI_IP_SUBNET_MASK, net_info, &len);
		printf("ARM_WIFI_IP_SUBNET_MASK = %d.%d.%d.%d\r\n", net_info[0], net_info[1], net_info[2], net_info[3]);

		Driver_WiFi1.GetOption(0, ARM_WIFI_IP_GATEWAY, net_info, &len);
		printf("ARM_WIFI_IP_GATEWAY = %d.%d.%d.%d\r\n", net_info[0], net_info[1], net_info[2], net_info[3]);

		Driver_WiFi1.GetOption(0, ARM_WIFI_IP_DNS1, net_info, &len);
		printf("ARM_WIFI_IP_DNS1 = %d.%d.%d.%d\r\n", net_info[0], net_info[1], net_info[2], net_info[3]);

		Driver_WiFi1.GetOption(0, ARM_WIFI_IP_DNS2, net_info, &len);
		printf("ARM_WIFI_IP_DNS2 = %d.%d.%d.%d\r\n", net_info[0], net_info[1], net_info[2], net_info[3]);

	}


	vTaskDelay(DELAY);
	testSocket();

	vTaskDelay(DELAY);
	testSNTP();


	char datetime_buf[256];
	datetime_t t;
	while (true) { // Loop forever


		vTaskDelay(DELAY);
		rtc_get_datetime(&t);
		datetime_to_str(datetime_buf, sizeof(datetime_buf), &t);
		printf("Time: %s\n\r", datetime_buf);
	}

}

/***
* Get the static depth required in words
* @return - words
*/
configSTACK_DEPTH_TYPE NetConn::getMaxStackSize(){
	return 5000;
}



int NetConn::testSocket(){
	printf("Test Socket\n");

	 int retval = 0;
	int32_t af;
	uint32_t ip_len = 4;
	uint8_t sock;
	uint32_t send_cnt = 0;

	uint8_t targetIP[4] ;// = {192, 168, 1, 20};
	uint16_t targetPort = 80;

	char targetHost[]="nas2.local.jondurrant.com";

	char message[]="GET / HTTP/1.1\r\n"
				"Host: nas2.local.jondurrant.com\r\n"
				"Connection: close\r\n"
				"\r\n";


	af = IOT_SOCKET_AF_INET;

	retval = iotSocketGetHostByName (targetHost,af, targetIP, &ip_len);
	if (retval != 0){
		printf("DNS Request failed \r\n");
	}
	printf("Target %s is %d.%d.%d.%d\r\n", targetHost, targetIP[0], targetIP[1], targetIP[2], targetIP[3]);



	sock = iotSocketCreate (af, IOT_SOCKET_SOCK_STREAM, IOT_SOCKET_IPPROTO_TCP);
	if (sock < 0){
		printf("iotSocketCreate failed %d\r\n", sock);
		return sock;
	}

	retval = iotSocketConnect (sock, targetIP, 4, targetPort);
	if (retval < 0){
		iotSocketClose (sock);
		printf("iotSocketConnect failed %d\r\n", retval);
		return retval;
	}


	//sprintf(g_tcp_send_buf, "Hello WizFi360-EVB-Pico-C TCP_Client_Demo send_cnt = %d\r\n", send_cnt++);
	retval = iotSocketSend(sock, message, strlen(message));
	printf("iotSocketSend retval = %d\r\n", retval);

	retval = 1;
	while (retval > 0){
		retval = iotSocketRecv(sock, pBuff, BUFSIZE);
		printf("iotSocketRecv retval = %d\r\n", retval);
		if(retval > 0){
			debugPrintBuffer("Recv:", pBuff, retval);
		}
	}

	printf("Closing\n");
	iotSocketClose(sock);
	return 0;
}

/***
 * Print the buffer in hex and plain text for debugging
 */
void NetConn::debugPrintBuffer(const char *title, const void * pBuffer, size_t bytes){
	size_t count =0;
	size_t lineEnd=0;
	const uint8_t *pBuf = (uint8_t *)pBuffer;

	printf("DEBUG: %s of size %d\n", title, bytes);

	while (count < bytes){
		lineEnd = count + DEBUG_LINE;
		if (lineEnd > bytes){
			lineEnd = bytes;
		}

		//Print HEX DUMP
		for (size_t i=count; i < lineEnd; i++){
			if (pBuf[i] <= 0x0F){
				printf("0%X ", pBuf[i]);
			} else {
				printf("%X ", pBuf[i]);
			}
		}

		//Pad for short lines
		size_t pad = (DEBUG_LINE - (lineEnd - count)) * 3;
		for (size_t i=0; i < pad; i++){
			printf(" ");
		}

		//Print Plain Text
		for (size_t i=count; i < lineEnd; i++){
			if ((pBuf[i] >= 0x20) && (pBuf[i] <= 0x7e)){
				printf("%c", pBuf[i]);
			} else {
				printf(".");
			}
		}

		printf("\n");

		count = lineEnd;

	}
}

int32_t NetConn::sntp_get_time (const char *server, uint32_t *seconds) {
  int32_t  socket;
  uint8_t  buf[48];
  uint8_t  ip[4];
  uint32_t ip_len;
  uint32_t timeout;
  int32_t  status;

  /* Resolve SNTP/NTP server IP address */
  ip_len = 4U;
  status = iotSocketGetHostByName(server, IOT_SOCKET_AF_INET, ip, &ip_len);
  if (status != 0) {
    return (-1);
  }

  /* Compose SNTP request: vers.3, mode=Client */
  memset(buf, 0, sizeof(buf));
  buf[0] = 0x1B;

  /* Create UDP socket */
  socket = iotSocketCreate(IOT_SOCKET_AF_INET, IOT_SOCKET_SOCK_DGRAM, IOT_SOCKET_IPPROTO_UDP);
  if (socket < 0) {
    return (-1);
  }

  /* Set socket receive timeout: 10 seconds */
  timeout = 10000U;
  status = iotSocketSetOpt(socket, IOT_SOCKET_SO_RCVTIMEO, &timeout, sizeof(timeout));
  if (status < 0) {
    iotSocketClose(socket);
    return (-1);
  }

  /* Send SNTP request (port 123) */
  status = iotSocketSendTo(socket, buf, sizeof(buf), ip, sizeof(ip), 123U);
  if (status < 0) {
    iotSocketClose(socket);
    return (-1);
  }

  /* Read SNTP response */
  status = iotSocketRecv(socket, buf, sizeof(buf));
  if (status < 0) {
    iotSocketClose(socket);
    return (-1);
  }

  /* Extract time */
  if (seconds != NULL) {
    *seconds = ((buf[40] << 24) | (buf[41] << 16) | (buf[42] << 8) | buf[43]) - 2208988800U;
  }

  iotSocketClose(socket);

  return 0;
}

int NetConn::testSNTP(){
	char targetHost[] = "0.uk.pool.ntp.org";
	int32_t retval;
	uint32_t raw;
	time_t time;
	struct tm * timeinfo;
	datetime_t date;

	retval = sntp_get_time(targetHost, &raw);
	printf("sntp_get_time returned %d\n\r", retval);
	if (retval == 0){
		printf("Time is %ld\n\r", raw);

		time = (time_t)raw;
		timeinfo = localtime (&time);
	    printf ("Current local time and date: %s\n\r", asctime(timeinfo));

	    printf("Daylight saving %d\n\r", timeinfo->tm_isdst);

	    date.sec = timeinfo->tm_sec;
		date.min = timeinfo->tm_min;
		date.hour = timeinfo->tm_hour;
		date.day = timeinfo->tm_mday;
		date.month = timeinfo->tm_mon;
		date.year = timeinfo->tm_year;

		rtc_init();
		rtc_set_datetime (&date);
	}

	return 0;
}
