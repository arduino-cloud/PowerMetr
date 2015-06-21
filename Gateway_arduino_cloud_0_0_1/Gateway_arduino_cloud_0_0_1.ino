//--------------------------------------------------------------------------------------------------------
// www.Arduino-Cloud.com
// Gateway v 0.0.1
// 19-21.06.2015
// For Hack4Energy - Dnipropetrovsk
// Gordeev Anrdey gordeev@openpyro.com

// visit http://app.arduino-cloud.com/ , to get api_key

#define API_KEY "yoer_api_key"

//--------------------------------------------------------------------------------------------------------
#include <SPI.h>
#include <Ethernet.h> 
#include <VirtualWire.h>
#include <EEPROMex.h>
#include <avr/wdt.h>
//--------------------------------------------------------------------------------------------------------
unsigned long time_now    =0;
unsigned long time_send_t =0;
unsigned long time_RF     =0;
unsigned long time_send   =0; 

#define RF_LED 8
 
#define EIS_ID                 1

#define Power_measurement_set  1   // измерение данных каждую секунду
#define RFM_send_set           1   // отправка данных каждую секунду
#define RF_rate                2500
#define RX_RF_pin              2
//#define Interrup_IN            0 // Arduino Uno, Mega
#define Interrup_IN            1 // Arduino Leonardo
#define SUPPLY_VOLTAGE         3300

#define RF_S_online_timeout  10000
#define RFM_S_online_timeout 10000

#define time_send_t_delay    15000

unsigned char ready_rfm     = 0;
unsigned char ready_rf      = 0;

boolean Emon_read_server =  true;
boolean Emon_print_server=  false;
boolean Emon_show_time   =  true;

boolean send_info        = true;
boolean rf_info          = true;
//--------------------------------------------------------------------------------------------------------
#include "Struct.h"
#include "RF_Sensor.h"
#include "Ether.h"
#include "Emon.h"
//--------------------------------------------------------------------------------------------------------
void PrintSendTime(char * t) {
    if (send_info)Serial.print(t);  
    time_send  = millis();   
    if (send_info)Serial.println(time_send - time_RF);
    time_RF = time_send; 
}
//--------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
//  while (!Serial) {
//       ; // wait for serial port to connect. Needed for Leonardo only
//  }  
  Serial.println("Start setup"); 
   
  Gateway.eis_ID  = EIS_ID ;
  
  Serial.print("RF_Sensor_Setup"); 
  RF_Sensor_Setup();
  Serial.println(" - OK");  
   
  Serial.print("StartEthernet"); 
  StartEthernet(); 
  Serial.println(" - OK");
  
  Serial.println(Ethernet.localIP());  
   
  Serial.println("End setup");
  Serial.println("--- ");  

//  Serial.print("WDT Enable 8 sec"); 
//  wdt_enable(WDTO_8S);
//  Serial.println(" - OK");  
}
//--------------------------------------------------------------------------------------------------------
void loop() {  
  Emon_loop();
//  wdt_reset();
  
  time_now  = millis(); 
  RF_Sensor_loop();

  if (ready_rf == 1){     
    PrintSendTime("RF_Send Sensor - Time "); 
    Emon_GET(); 

    ready_rf = 0;
    time_send_t = time_now + time_send_t_delay; 
  } 

  if (time_send_t <= time_now ){ 
   time_send_t = time_now + time_send_t_delay;  
   Emon_GET();   
  }  
}
//--------------------------------------------------------------------------------------------------------
