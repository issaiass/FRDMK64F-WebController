/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "Board_LED.h"                  // ::Board Support:LED
#include "rl_net.h"                     // Keil.MDK-Pro::Network:CORE


extern void hardware_init (void);
extern void Init_Thread(void);

void dhcp_client_notify(uint32_t if_num, dhcpClientOption opt, const uint8_t *val, uint32_t len) {
    if (opt == dhcpClientIPaddress) {
		    LED_Off(0); //green LED
		    LED_On(0);
    }
}


/*
 * main: initialize and start the system
 */
int main (void) {
    osKernelInitialize(); // initialize CMSIS-RTOS
    hardware_init();
    Init_Thread();
    LED_Initialize();
    net_initialize();
    osKernelStart(); // start thread execution
    while(1) {
        net_main();
        osThreadYield();
    }
}
