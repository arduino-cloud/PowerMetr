//--------------------------------------------------------------------------------------------------------
// info struct
//--------------------------------------------------------------------------------------------------------
// RF Sensor
typedef struct {
  uint8_t    eis_ID ;
  double   I; 
} SensorData;
SensorData Sensor_RF;
//--------------------------------------------------------------------------------------------------------
// Interal Gateway
typedef struct {
  int     eis_ID ;  
  uint32_t Send_Data;
  uint32_t Read_Data;
  uint16_t CounterReset;  
} SendToServer;
SendToServer Gateway;
//--------------------------------------------------------------------------------------------------------
