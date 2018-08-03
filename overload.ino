// photoresistor in pin A0;
int pir = 2;
// mic in pin A1

// vars for the senses
int light;
int motion = 50; // start this at mid level since it takes a while to get past 0
int sound;

void setup() {
  // we need a pullup resistor to get data from the PIR sensor
  // in this code, we're using the builtin pullup from D2
  pinMode(pir, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // taking the light value
  light = analogRead(A0);

  // taking the motion value; increase gradually if there is motion
  if (digitalRead(pir) == 0 && motion < 100){
    motion++;
  }
  // vice versa
  else if (digitalRead(pir) == 1 && motion > 0{
    motion--;
  }
  
  // code for the mic
  long start = millis(); // start time
  int max = 0;
  int min = 1024;

  // collect data
  while (millis() - start < 100){
    int vol = analogRead(1);
    if (vol < 1024){
      if (vol > max){
        max = vol; // keep max vals
      }
      else if (vol < min){
        min = vol; // keep min vals
      }
    }
  }
  sound = max - min; // find the amplitude

  // map the values to a scale of 0 to 100, so we have a standard range
  light = map(light, 250, 900, 0, 100);
  sound = map(sound, 0, 1023, 0, 100);

  // print the values to serial, then delay so we don't overload the game (oh the irony)
  Serial.print(light);
  Serial.print(", ");
  Serial.print(motion);
  Serial.print(", ");
  Serial.println(sound);
  delay(100);
}
