//--------------------------------------------------------------------------------------------------------
// Local Network Settings
static byte mac[6]     = { 0xD0, 0x28, 0xB2, 0xFF, 0xA0, EIS_ID }; 

#define  tray_send        3
// Variable Setup
unsigned char   resetCounter = 0;
extern EthernetClient Emon_client ;
//--------------------------------------------------------------------------------------------------------
void connection_failed();
void resetEthernetShield();
void StartEthernet();
//--------------------------------------------------------------------------------------------------------
void connection_failed(){ 
  resetCounter++;  
  Serial.print(" -- resetCounter " );
  Serial.print(resetCounter);
  Serial.println(" -- "); 

  if (resetCounter > tray_send) resetEthernetShield();
}
//--------------------------------------------------------------------------------------------------------
void resetEthernetShield(){
  Gateway.CounterReset++;
  resetCounter = 0;
  
  Serial.println(" -- Resetting Ethernet Shield -- ");   
  Serial.println();

  Emon_client.flush();
  Emon_client.stop(); 
  StartEthernet();
}
//--------------------------------------------------------------------------------------------------------
void StartEthernet(){
  while( !Ethernet.begin(mac) ){
    Serial.println("");
    Serial.println(" -- No DHCP -- ");
  }
}
//--------------------------------------------------------------------------------------------------------

