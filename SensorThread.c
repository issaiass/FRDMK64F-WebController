
#include <cmsis_os.h>                                           // CMSIS RTOS header file
#include "Board_LED.h"                  // ::Board Support:LED
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
void Thread (void const *argument);                             // thread function
osThreadId tid_Thread;                                          // thread id
osThreadDef (Thread, osPriorityNormal, 1, 0);                   // thread object

int Init_Thread (void) {

  tid_Thread = osThreadCreate (osThread(Thread), NULL);
  if(!tid_Thread) return(-1);
  
  return(0);
}

void Thread (void const *argument) {

  while (1) {
    osDelay(500);
    LED_On(2); //red LED
    osDelay(500);
    LED_Off(2);
    osThreadYield();                                            // suspend thread
  }
}

