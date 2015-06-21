//--------------------------------------------------------------------------------------------------------
// www.Arduino-Cloud.com
// Pover Sensor v 0.0.1
// 19-21.06.2015 for Hack4Energy - Dnipropetrovsk
// Gordeev Anrdey gordeev@openpyro.com
//--------------------------------------------------------------------------------------------------------
#include <VirtualWire.h>
#include <LowPower.h>

// 16 MHz
//#define m_1_samples_t 0.176 //300 msec
//#define m_1_samples_t 0.177 //200 msec
//#define m_1_samples_t 0.178 //100 msec

// 8MHz
#define m_1_samples_t    0.385 //100 msec

#define time_to_sleep    5000
#define time_megement    200
#define time_transmit    59
#define time_wekeap      150
#define m_samples        (time_megement / m_1_samples_t)
#define time_all_time    (time_transmit + time_megement + time_wekeap)

#define _id           1 
#define RF_rate       2500
#define Tx_pin        5

#define Sleep_count      3+_id

#define PHASE_1_IN       0

#define SUPPLY_VOLTAGE   3300

#include "Struct.h"
#include "P_Sensor.h"
#include "RF_Send.h"

unsigned long time_now  = 0;

uint8_t Count = 0;
//--------------------------------------------------------------------------------------------------------
void time_start(){
  time_now = millis();
}
//--------------------------------------------------------------------------------------------------------
void time_show(char* t){
  Serial.print ( millis() - time_now);
  Serial.println (t); 
  Serial.flush(); 
}
//--------------------------------------------------------------------------------------------------------
void setup(){
  Serial.begin(115200);	  // Debugging only
  Serial.println (" Setup "); 
  Serial.print (m_samples );
  Serial.println (" - samples"); 
  Serial.flush(); 

  Sensor_RF.eis_ID = _id;

//  for (int i =0;i<19;i++){
//    if(i!=14 ) {
//      pinMode(i,OUTPUT);
//      digitalWrite(i,LOW);
//    }
//  } 

  P_Sensor_Setup();
  RF_Setup();
}
//--------------------------------------------------------------------------------------------------------
void loop(){
  if (Count == Sleep_count) {
    Count = 0;
    time_start();
    Sensor_RF.I = GetCurent(0);
    time_show(" Meger");  
  
    time_start();
    RF_Send();
    time_show(" Send");
  
    Serial.print   (Sensor_RF.I );
    Serial.println ("A - Irms"); 
    Serial.flush();     
  }  

  LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
  Count ++;
}
//--------------------------------------------------------------------------------------------------------
