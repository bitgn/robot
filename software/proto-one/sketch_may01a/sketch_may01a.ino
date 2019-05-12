/*     Simple Stepper Motor Control Exaple Code
 *      
 *  by Dejan Nedelkovski, www.HowToMechatronics.com
 *  
 */
// defines pins numbers
const int stepPin = 3; 
const int dirPin = 4; 

int v0;
int v1;



const int buffer_size = 5;
int v1_buf[buffer_size];
int v0_buf[buffer_size];

byte buffer_pos;


void mem(int v0, int v1) {
  buffer_pos += 1;
  if (buffer_pos >= buffer_size) {
    buffer_pos = 0;
  }
  
  v0_buf[buffer_pos] = v0;
  v1_buf[buffer_pos] = v1; 
}


int sum_v0() {
  int i;
  int total = 0;
  for (i = 0; i < buffer_size; i++) {    
      total += v0_buf[i];    
  }
  return total;
}
int sum_v1() {
  int i;
  int total = 0;
  for (i = 0; i < buffer_size; i++) {    
      total += v1_buf[i];    
  }
  return total;
}





bool last_v0;
bool last_v1;

int pos;
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);

  analogReference(INTERNAL);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  Serial.begin(115200);

  pos = 0;
}



/*
 * analog read runs at 9kHz. 1kHZ = 1ms 9kHZ = 1/9ms
 * two analog reads need 2/9ms or 0.222ms or 222 microseconds
 * 
 */


void loop() {


  unsigned long time;
  unsigned long delta;
  
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 20000; x++) {
    digitalWrite(stepPin,HIGH); 

    time = micros();     

    v0 = analogRead(A0); // green
    v1 = analogRead(A1); // red  

    mem(v0, v1);

    int v1_tot = sum_v1() / buffer_size;
    int v0_tot = sum_v0() / buffer_size;

    bool v1_high = v1 > 20;
    bool v0_high = v0 > 20;

  
     int old_pos;
  old_pos = pos;
  // V0 low to high transition
  if (last_v0 == false && v0_high == true) {
    if (v1_high) {
      pos += 1;
    } else {
      pos -= 1;
    }
    //
  }
  // V0 high to low transition
  if (last_v0 == true && v0_high == false) {
    if (v1_high) {
      pos -= 1;
    } else {
      pos += 1;
    }
  }


  
  if (last_v1 == false && v1_high == true) {
    if (v0_high) {
      pos -= 1;
    } else {
      pos += 1;
    }
  }


  
  if (last_v1 == true && v1_high == false) {
    if (v0_high) {
      pos += 1;
    } else {
      pos -= 1;
    }
  }
  
  
  


    delta = micros() - time;
    
    

    delayMicroseconds(500-delta); // 0.5 ms

    
    digitalWrite(stepPin,LOW);
    time = micros();     

    
    Serial.println(String("") + v0_tot + " " + v1_tot + " " + pos);
    delta = micros() - time; 
    delayMicroseconds(500-delta); // 0.5 ms 
  
  
  last_v0 = v0_high;
  last_v1 = v1_high;

    
  }
  delay(1000); // One second delay
  
  digitalWrite(dirPin,LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < 400; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(500);
  }
  delay(1000);
}
