
#define ARRY_LENGTH 10

float GetPHvalue(int SensorPin, int TempValue){
  float PH_VALUE = 0;
  int pv[ARRY_LENGTH];
  for(int i = 0; i < ARRY_LENGTH; i++ ){
    pv[i] = analogRead(SensorPin);
    delay(1);
  }
  for(int i = 0; i < ARRY_LENGTH; i++){
    for(int k = i; k < ARRY_LENGTH; k++ ){
      if( pv[i] < pv[k] ){
        int tmp = pv[i];
        pv[i] = pv[k];
        pv[k] = tmp;
      }
    }
  }

  //温度补偿开始
  if( TempValue > 42 )  pv[ARRY_LENGTH/2] += 5;
  else if(TempValue > 28){
    pv[ARRY_LENGTH/2] += 5*(TempValue - 28)/14;
  }
  //温度补偿结束
  
  PH_VALUE = pv[ARRY_LENGTH/2];
  PH_VALUE = -5.887*(PH_VALUE*5/1024) + 21.677;  
  if(PH_VALUE > 14.1){
    PH_VALUE = 14.1;
  }
  if( PH_VALUE < 0 ){
    PH_VALUE = 0;
  }
  
  return PH_VALUE;
//    return pv[ARRY_LENGTH/2];
}
