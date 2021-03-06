/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2014 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HTTP_Server_CGI.c
 * Purpose: HTTP Server CGI Module
 * Rev.:    V6.00
 *----------------------------------------------------------------------------*/
//! [code_HTTP_Server_CGI]
#include <stdio.h>
#include <string.h>
#include "rl_net.h"
#include "Board_Accelerometer.h"        // ::Board Support:Accelerometer
#include "Board_Magnetometer.h"         // ::Board Support:Magnetometer
#include "Board_LED.h"                  // ::Board Support:LED
#include "jansson.h"                    // Keil::Data Exchange:JSON:Jansson
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "fsl_gpio_hal.h"               // Keil::Device:KSDK HAL:GPIO
#include "fsl_gpio_driver.h"            // Keil::Device:KSDK Driver:GPIO

extern MAGNETOMETER_STATE g_magneto;
extern ACCELEROMETER_STATE g_accel;  
extern osTimerId id1;                                     // timer id

// Process query string received by GET request.
void cgi_process_query (const char *qstr) {
  char var[40];

  do {
    // Loop through all the parameters
    qstr = http_get_env_var (qstr, var, sizeof (var));
    // Check return string, 'qstr' now points to the next parameter
    if (var[0] != 0) {
      // First character is non-null, string exists

      /* Example of a parameter "ip=192.168.0.100"
      if (strncmp (var, "ip=", 3) == 0) {
        uint8_t ip_addr[IP4_ADDR_LEN];
        // Read parameter IP address submitted by the browser
        ip4_aton (&var[3], ip_addr);
         ...
      }
      */
    }
  } while (qstr);
}

static char* json_rpc_cmd = 0;
static char post_dat_buf[256];

// Process data received by POST request.
// Type code: - 0 = www-url-encoded form data.
//            - 1 = filename for file upload (null-terminated string).
//            - 2 = file upload raw data.
//            - 3 = end of file upload (file close requested).
//            - 4 = any XML encoded POST data (single or last stream).
//            - 5 = the same as 4, but with more XML data to follow.
void cgi_process_data (uint8_t code, const char *data, uint32_t len) {
//  char var[40];

  switch (code) {
    case 0:
      // Url encoded form data received
      do {
        // Parse all parameters
        data = http_get_env_var (data, post_dat_buf, sizeof (post_dat_buf));
        if (post_dat_buf[0] != 0) {
          // First character is non-null, string exists
          json_rpc_cmd = 0;
          if (strncmp (post_dat_buf, "{\"jsonrpc", 9) == 0) {
            json_rpc_cmd = post_dat_buf;
          } 
        }
      } while (data);
      break;

    case 1:
      // Filename for file upload received
      /* Example
      if (data[0] != 0) {
        // Open a file for writing
        file = fopen (var, "w");
      }
      */
      break;

    case 2:
      // File content data received
      /* Example
      if (file != NULL) {
        // Write data to a file
        fwrite (data, 1, len, file);
      }
      */
      break;

    case 3:
      // File upload finished
      /* Example
      if (file != NULL) {
        // Close a file
        fclose (file);
      }
      */
      break;

    case 4:
      // XML encoded content type, last packet
      /* Example
      if (strcmp (http_get_content_type(), "text/xml; charset=utf-8" == 0) {
        // Content type xml, utf-8 encoding
         ...
      }
      */
      break;

    case 5:
      // XML encoded content type, more to follow
      // ... Process xml data
      break;

    default:
      // Ignore all other codes
      break;
  }
}
 
// Generate dynamic web data from a script line.
uint32_t cgi_script (const char *env, char *buf, uint32_t buflen, uint32_t *pcgi) {
  //uint32_t len = 0;
 
  // Analyze a 'c' script line starting position 2
  /* Example script
  // c a i <td><input type=text name=ip value="%s" size=18 maxlength=18></td>

  switch (env[0]) {
    case 'a' :
      switch (env[2]) {
        case 'i':
          // Write the local IP address
          len = sprintf (buf, &env[4], ip4_ntoa (IpAddr));
          break;
          ...
      }
      break;
  }
  */
//  return (len);
    uint32_t len = 0;
    json_t* jdata;
	  int32_t error_flag = 0;
    unsigned char i;	
//	LED_On(2);
    switch (env[0]) {
        case 'l':
			      sprintf(buf, "{\"x\":\"%d\", \"y\":\"%d\", \"z\":\"%d\", \"ax\":\"%d\", \"ay\":\"%d\", \"az\":\"%d\"}" \
		        ,g_magneto.x, g_magneto.y, g_magneto.z, g_accel.x, g_accel.y, g_accel.z);	
            len = strlen(buf);
            break;
				case 'm':
            i = GPIO_HAL_ReadPinInput(PTE_BASE, 26);
            sprintf(buf, "{\"rly1\":\"%d\"}", i);	
            len = strlen(buf);
				    break;
	  // RPC Call
        case 'r': {
		        char* met;
			      char* par;
				    json_error_t jerror;
			      json_t* jmethod, *jparams;
            uint16_t tmr;			  
			
			      if (json_rpc_cmd == NULL) 
				        break; 
		        jdata = json_loads(json_rpc_cmd, 0, &jerror);
			      if(jdata) {
					/* Parse Parameters from the JSON string */
					      jmethod  = json_object_get(jdata, "method");
					      jparams  = json_object_get(jdata, "params");
                met = (char*)json_string_value(jmethod);
					      par = (char *)json_string_value(jparams);
					  switch (met[0]) {
						    case 's':
							      if(par[0] == '1')
								        LED_On(0);
							      break;
						    case 'c':
							      if(par[0] == '0')
									      LED_Off(0);
								    break;
						    case 't':
							      tmr = atoi(par);
							      if(tmr > 0) {
									      LED_On(0);
									      osTimerStart (id1, tmr); 
								    }
							  // osSignalWait(0x0001, osWaitForever); // debe ir en la funcion de thread
                // osSignalSet(tid_ToggleLED, 0x0001); //ver
								    break;
						    default:							
				            break;
					    }
					/* Acknowledge the JSON RPC call*/
				    strcpy(buf, "{\"jsonrpc\": \"2.0\", \"result\": 1, \"id\":\"jrpc\"}");
					  len = 44;
				}
		    json_decref( jdata );
			  json_decref( jmethod );
				json_decref( jparams );
        return (len| (1U<<30));
        }
    }
//   LED_Off(2);
    return (len);
}
 
// Override default Content-Type for CGX script files.
const char *cgx_content_type (void) {
  /* Example
  return ("text/xml; charset=utf-8");
  */
    return ("application/json"); 
//  return (NULL);
}
 
// Override default character encoding in html documents.
const char *http_encoding (void) {
  /* Example
  return ("utf-8");
  */
  return (NULL);
}
//! [code_HTTP_Server_CGI]
