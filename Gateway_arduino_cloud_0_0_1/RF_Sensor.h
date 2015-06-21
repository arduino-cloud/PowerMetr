//--------------------------------------------------------------------------------------------------------
unsigned long Sensor_time_receive;

unsigned char bufer_rf[20];
unsigned char len_rf;
//--------------------------------------------------------------------------------------------------------
void RF_Sensor_Setup();
void RF_Sensor_loop();
//--------------------------------------------------------------------------------------------------------
void RF_Sensor_Setup(){
  vw_set_rx_pin(RX_RF_pin);
  vw_setup(RF_rate);	 
  vw_rx_start();       
}
//--------------------------------------------------------------------------------------------------------
void RF_Sensor_loop(){
  uint8_t buf[3];
  uint8_t buflen = 3;
  int num = 0;
    
  if (vw_get_message(buf, &buflen)){     
    Sensor_RF.eis_ID = buf[0];
    num = (buf[2] << 8 | buf[1]);
    Sensor_RF.I = double (num)/100;


    if (Sensor_RF.eis_ID == Gateway.eis_ID ){
      Sensor_time_receive = millis();
      ready_rf = true;
      
      if (rf_info){
        Serial.print ( "RF_Sensor ["); 
        Serial.print (Sensor_RF.eis_ID);
        Serial.print ( "] - Receive ");
        Serial.print(Sensor_RF.I); 
        Serial.print(" Irms ");  

        if (!send_info) Serial.println("");
      }    
    }
    else if (rf_info){
      Serial.print ( "RF_Sensor ["); 
      Serial.print (Sensor_RF.eis_ID);
      Serial.print ( "] - no ID "); 
      Serial.print(Sensor_RF.I); 
      Serial.print(" Irms ");  
      
      Serial.println("");
    } 
    
  } 
}
//--------------------------------------------------------------------------------------------------------

