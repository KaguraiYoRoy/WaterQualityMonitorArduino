double GetLM35Value(int SensorPin){
  return (double)analogRead(SensorPin)*(5/10.24);
}