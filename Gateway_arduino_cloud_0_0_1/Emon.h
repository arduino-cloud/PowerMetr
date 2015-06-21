//--------------------------------------------------------------------------------------------------------
#define Emon_receive_timeout 16000
#define Emon_connect_timeout 16000

const char Emon_server[] = "app.arduino-cloud.com" ;
const IPAddress Emon_IP(178,62,140,198); 
const char apikey[] = API_KEY;

EthernetClient Emon_client ;

boolean    Emon_begin_connect = false;
boolean    Emon_begin_recive  = false;
boolean    Emon_lastConnected = false;

unsigned long Emon_time_connect ;
unsigned long Emon_time_send    ;
unsigned long Emon_time_end     ;
//--------------------------------------------------------------------------------------------------------
void Emon_GET();
void Emon_loop();
//--------------------------------------------------------------------------------------------------------
void Emon_loop(){
  Emon_lastConnected = Emon_client.connected();  
  // Print Update Response to Serial Monitor
  if (Emon_client.available())
  {
    char c_ = Emon_client.read();
    if(Emon_print_server) Serial.print(c_);
  }
  // Disconnect
  if (!Emon_client.connected() && Emon_lastConnected)
  {
    Emon_begin_recive= 0;
    Emon_client.stop();
    if(Emon_read_server) digitalWrite(RF_LED, HIGH); 
    Emon_time_end = millis();
    resetCounter = 0;
    if(Emon_show_time){
      Gateway.Read_Data = Emon_time_end-Emon_time_send;
      if (Emon_print_server) Serial.println("");
      Serial.print(Gateway.Read_Data);
      Serial.println(" msec - Recive rfom server time");
      Serial.println("--- ");
    }   
  }
  
  if (Emon_begin_connect &&( (millis()-Emon_time_send )> Emon_receive_timeout)){
    Emon_begin_connect = 0;
    Serial.println();
    Serial.println(" -- receive_timeout -- ");
    Serial.println(); 
    connection_failed();
  } 
  
  if (Emon_begin_recive &&( (millis()-Emon_time_connect )> Emon_connect_timeout)){
    Emon_begin_recive = 0 ;
    Serial.println();
    Serial.println(" -- connect_timeout -- ");
    Serial.println(); 
    connection_failed();
  }   
} 
//--------------------------------------------------------------------------------------------------------
void Emon_GET()
{ 
  if (Emon_client.connected() ){
    Serial.println(); 
    Serial.println(" -- Ethernet buzy -- ");
    Serial.println(); 
    return;
  }
  
  digitalWrite(RF_LED, LOW);
  Emon_begin_connect= 1;  
  Emon_time_connect    = millis();
  
  if (Emon_client.connect(Emon_IP, 80)){
    Emon_client.print("GET /input/post.json?");
//    Emon_client.print(Emon_path);
//    Emon_client.print("?"); 

    Emon_client.print("apikey=");
    Emon_client.print(apikey);
    Emon_client.print("&node=");
    Emon_client.print(Gateway.eis_ID);
    Emon_client.print("&json={");

    Emon_client.print("G_Reset:");
    Emon_client.print(Gateway.CounterReset);
   
    if(  Gateway.Send_Data != NAN && Gateway.Read_Data != NAN){   
      Emon_client.print(",G_Send:");
      Emon_client.print(Gateway.Send_Data);
      Emon_client.print(",G_Read:");
      Emon_client.print(Gateway.Read_Data); 
    }    

    if (ready_rf && Sensor_RF.eis_ID == Gateway.eis_ID ){ 
      Emon_client.print(",S_Irms:");
      Emon_client.print(Sensor_RF.I);
    }
 

    Emon_client.print("}");
    
    Emon_client.print(" HTTP/1.1 \r\n"); 
    Emon_client.print("Host: ");
    Emon_client.print(Emon_server);
    Emon_client.print(" \r\n");
    Emon_client.print("Connection: close\r\n");
    Emon_client.print("\r\n\r\n");     
    
    if(!Emon_read_server){
      resetCounter = 0;
      Emon_client.stop();
    }  
  }
  else{ 
    Serial.println();
    Serial.println(" -- Emon_client.connect fault -- ");
    Serial.println();
    
    Emon_begin_connect = 0;
    Emon_begin_recive  = 0;
    digitalWrite(RF_LED, HIGH);
    resetEthernetShield();
    return;       
  }

  Emon_begin_connect= 0;
  Emon_begin_recive= 1;
  Emon_time_send = millis();
  Gateway.Send_Data = Emon_time_send - Emon_time_connect;

  if(!Emon_read_server) digitalWrite(RF_LED, HIGH);   
  
  if (Emon_show_time){
    Serial.print(Gateway.Send_Data);
    Serial.println(" msec - Send to server time");
  }  
}
//--------------------------------------------------------------------------------------------------------

