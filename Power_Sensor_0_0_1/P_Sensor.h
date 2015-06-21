//--------------------------------------------------------------------------------------------------------
// Pover Sensor
//--------------------------------------------------------------------------------------------------------
#define ADC_BITS    10
#define ADC_COUNTS  (1<<ADC_BITS)
//--------------------------------------------------------------------------------------------------------
//sample_ holds the raw analog read value, lastSample_ holds the last sample
int lastSampleI;
int sampleI;                   
//Filtered_ is the raw analog value minus the DC offset
double lastFilteredI;
double filteredI;                  
//sq = squared, sum = Sum, inst = instantaneous
double sqI;
double sumI;
double ICAL;
double Irms;
//--------------------------------------------------------------------------------------------------------
void P_Sensor_Setup();
uint16_t GetCurent(int inPinI);  
double calcIrms(int inPinI,int NUMBER_OF_SAMPLES);
//--------------------------------------------------------------------------------------------------------
uint16_t GetCurent(int inPinI){
  return uint16_t(calcIrms(inPinI,m_samples)*100);
}
//--------------------------------------------------------------------------------------------------------
void P_Sensor_Setup(){
    ICAL = 30.2 ; 
    calcIrms(0,3000); 
}
//--------------------------------------------------------------------------------------------------------
double calcIrms(int inPinI,int NUMBER_OF_SAMPLES)
{
  int SUPPLYVOLTAGE = SUPPLY_VOLTAGE;  
  double   Ir = 0;
  
  for (int n = 0; n < NUMBER_OF_SAMPLES; n++)
  {
    lastSampleI = sampleI;
    sampleI = analogRead(inPinI);
    lastFilteredI = filteredI;
    filteredI = 0.996*(lastFilteredI+sampleI-lastSampleI);
    // Root-mean-square method current
    // 1) square current values
    sqI = filteredI * filteredI;
    // 2) sum 
    sumI += sqI;
  }
  double I_RATIO = ICAL *((SUPPLYVOLTAGE/1000.0) / (ADC_COUNTS));
  Ir = I_RATIO * sqrt(sumI / NUMBER_OF_SAMPLES); 
  //Reset accumulators
  sumI = 0;
  return Ir;
} 
//--------------------------------------------------------------------------------------------------------
