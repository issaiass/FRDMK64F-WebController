/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "Board_LED.h"                  // ::Board Support:LED


extern void hardware_init (void);
extern void Init_Thread(void);

/*
 * main: initialize and start the system
 */
int main (void) {
    osKernelInitialize(); // initialize CMSIS-RTOS
    hardware_init();
    Init_Thread();
    LED_Initialize();
    osKernelStart(); // start thread execution
    while(1) {
        osDelay(500);
        LED_On(1); //red LED
        osDelay(500);
        LED_Off(1);
    }
}
