int prox; // the value of the proximity sensor
int sound; // the value of the mic
int vol; // how loud the mic is

int caughtCount = 0;
int fearCount = 0;

int state = 0; // default (0), near (1), fear (2), and caught (3)
String stateName[] = {"default", "near", "fear", "caught"};

void setup() {
  // start the serial monitor, set output and input pins
  Serial.begin(9600);
  pinMode(A0, INPUT); // proximity sensor
  pinMode(A1, INPUT); // mic

  pinMode(11, OUTPUT); // red
  pinMode(12, OUTPUT); // green
  pinMode(13, OUTPUT); // blue

  pinMode(3, OUTPUT); // left h-bridge leg 1
  pinMode(4, OUTPUT); // left h-bridge leg 2
  pinMode(5, OUTPUT); // left h-bridge enable
  
  pinMode(6, OUTPUT); // right h-bridge leg 1
  pinMode(8, OUTPUT); // right h-bridge leg 2
  pinMode(9, OUTPUT); // right h-bridge enable

  digitalWrite(5, HIGH);
  digitalWrite(9, HIGH);
}

void loop() {  
  // read the proximity sensor
  prox = analogRead(A0);

  // code for the mic
  long start = millis(); // start time
  int max = 0;
  int min = 1024;

  // collect data
  while (millis() - start < 100){
    sound = analogRead(1);
    if (sound < 1024){
      if (sound > max){
        max = sound; // keep max vals
      }
      else if (sound < min){
        min = sound; // keep min vals
      }
    }
  }
  vol = max - min; // find the amplitude

  // state change code
  if (caughtCount > 0){
    state = 3;
  }
  else if (prox < 950){
    state = 1; // set to near state
  }
  else if (fearCount > 0){
    state = 2;
  }
  else if (vol > 400 && caughtCount == 0){
    state = 3;
    caughtCount = 40; // * 100 = 4000 ms because of the mic while loop
  }
  else if (vol > 200 && fearCount == 0){
    state = 2; // set to fear state
    fearCount = 20; // * 100 = 2000 ms because of the mic while loop
  }
  else{
    state = 0; // return to default state
  }

  
  if (caughtCount > 0){
    caughtCount -= 1;
  }
  if (fearCount > 0){
    fearCount -= 1;
  }
  
  //Serial.println(stateName[state]);
  Serial.println(prox);

  // write to LED and motors. for some reason I couldn't get the LED
  // to work with the anode connected to ground - maybe this LED was
  // a shared cathode instead? I got it from a mixed bin. At any rate,
  // the pin that isn't RGB is connected to 5V, not ground.
  if (state == 0){ // default
    // white
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);

    // left motor forward
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    // right motor forward
    digitalWrite(6, LOW);
    digitalWrite(8, HIGH);
  }
  else if (state == 1){ // near
    digitalWrite(11, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(13, HIGH);

    // left motor forward
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    // right motor backward
    digitalWrite(6, HIGH);
    digitalWrite(8, LOW);
  }
  else if (state == 2){ // fear
    digitalWrite(11, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);

    // left motor backward
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    // right motor forward
    digitalWrite(6, LOW);
    digitalWrite(8, HIGH);
  }
  else if (state == 3){ // caught
    digitalWrite(11, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(13, HIGH);

    // left motor off
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    // right motor off
    digitalWrite(6, LOW);
    digitalWrite(8, LOW);
  }
}
