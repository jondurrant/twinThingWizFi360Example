/***
 * Demo program to flash 5 LED attached to GPIO PAD 0, 2,3,4, 5.
 * Uses FreeRTOS Task
 * Jon Durrant
 * 15-Aug-2022
 */


#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

#include "BlinkAgent.h"

#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

#include "MQTTAgent.h"
#include "MQTTRouterPing.h"
#include "MQTTRouterTwin.h"

#include <WifiHelper.h>

#include "StateExample.h"
#include "ExampleAgentObserver.h"

#include "Credentials.env"
#ifndef MQTTHOST
#define MQTTHOST "MQTTSERVER"
#define MQTTPORT 1883
#define MQTTUSER "MAC"
#define MQTTPASSWD "MAC"
#endif


//Standard Task priority
#define TASK_PRIORITY		( tskIDLE_PRIORITY + 1UL )

//LED PAD to use
#define LED_PAD					10
#define LED1_PAD				2
#define LED2_PAD				3
#define LED3_PAD				4
#define LED4_PAD				5



void runTimeStats(   ){
	TaskStatus_t *pxTaskStatusArray;
	volatile UBaseType_t uxArraySize, x;
	unsigned long ulTotalRunTime;


   // Get number of takss
   uxArraySize = uxTaskGetNumberOfTasks();
   printf("Number of tasks %d\n", uxArraySize);

   //Allocate a TaskStatus_t structure for each task.
   pxTaskStatusArray = (TaskStatus_t *)pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

   if( pxTaskStatusArray != NULL ){
      // Generate raw status information about each task.
      uxArraySize = uxTaskGetSystemState( pxTaskStatusArray,
                                 uxArraySize,
                                 &ulTotalRunTime );

	 // Print stats
	 for( x = 0; x < uxArraySize; x++ )
	 {
		 printf("Task: %d \t cPri:%d \t bPri:%d \t hw:%d \t%s\n",
				pxTaskStatusArray[ x ].xTaskNumber ,
				pxTaskStatusArray[ x ].uxCurrentPriority ,
				pxTaskStatusArray[ x ].uxBasePriority ,
				pxTaskStatusArray[ x ].usStackHighWaterMark ,
				pxTaskStatusArray[ x ].pcTaskName
				);
	 }


      // Free array
      vPortFree( pxTaskStatusArray );
   } else {
	   printf("Failed to allocate space for stats\n");
   }

   //Get heap allocation information
   HeapStats_t heapStats;
   vPortGetHeapStats(&heapStats);
   printf("HEAP avl: %d, blocks %d, alloc: %d, free: %d\n",
		   heapStats.xAvailableHeapSpaceInBytes,
		   heapStats.xNumberOfFreeBlocks,
		   heapStats.xNumberOfSuccessfulAllocations,
		   heapStats.xNumberOfSuccessfulFrees
		   );
}


/***
 * Main task to blink external LED
 * @param params - unused
 */
void mainTask(void *params){
	BlinkAgent blink(LED_PAD);

	vTaskDelay(2000);
	printf("Main task started\n");

	blink.start("Blink", TASK_PRIORITY);


	char mqttTarget[] = MQTTHOST;
	uint16_t mqttPort = MQTTPORT;
	char mqttUser[] = MQTTUSER;
	char mqttPwd[] = MQTTPASSWD;

	MQTTRouterTwin mqttRouter;
	StateExample state;
	ExampleAgentObserver agentObs;

	MQTTAgent mqttAgent;
	TwinTask xTwin;
	MQTTPingTask xPing;

	//Connect to WiFi
	if (WifiHelper::connectToAp(WIFI_SSID, WIFI_PASSWORD)){
		char ips[16];
		char macs[15];
		WifiHelper::getIPAddressStr(ips);
		printf("WIFI IP %s\n", ips);
		if (WifiHelper::getMACAddressStr(macs)){
			printf("MAC %s\n", macs);
		} else {
			printf("MAC NOT AVAILABLE\n");
		}
		WifiHelper::syncRTCwithSNTP();

		//Set up the credentials so we have an ID for our thing
		mqttAgent.credentials(mqttUser, mqttPwd);
		mqttRouter.init(mqttAgent.getId(), &mqttAgent);

		printf("Connecting to: %s(%d) as %s:%s:%s\n",
				mqttTarget, mqttPort, mqttAgent.getId(),
				mqttUser, mqttPwd
				);

		//Twin agent to manage the state
		xTwin.setStateObject(&state);
		xTwin.setMQTTInterface(&mqttAgent);
		xTwin.start(tskIDLE_PRIORITY+1);

		//Start up a Ping agent to mange ping requests
		xPing.setInterface(&mqttAgent);
		xPing.start(tskIDLE_PRIORITY+1);

		//Give the router the twin and ping agents
		mqttRouter.setTwin(&xTwin);
		mqttRouter.setPingTask(&xPing);

		//Setup and start the mqttAgent
		mqttAgent.setObserver(&agentObs);
		mqttAgent.setRouter(&mqttRouter);
		mqttAgent.mqttConnect(mqttTarget, mqttPort, true, false);
		mqttAgent.start(tskIDLE_PRIORITY+1);

	}



	while (true) { // Loop forever
		runTimeStats();
		vTaskDelay(3000);
	}
}




/***
 * Launch the tasks and scheduler
 */
void vLaunch( void) {

	//Start blink task
    TaskHandle_t task;
    xTaskCreate(mainTask, "MainThread", 1000, NULL, TASK_PRIORITY, &task);

    /* Start the tasks and timer running. */
    //vTaskStartScheduler();
    osKernelInitialize();                         // Initialize CMSIS-RTOS
    osKernelStart();                              // Start thread execution
    for (;;) {}
}

/***
 * Main
 * @return
 */
int main( void )
{
	//Setup serial over USB and give a few seconds to settle before we start
    stdio_init_all();
    //sleep_ms(2000);
    printf("GO\n");

    //Start tasks and scheduler
    const char *rtos_name = "FreeRTOS";
    printf("Starting %s on core 0:\n", rtos_name);
    vLaunch();


    return 0;
}
