// defines pins numbers
const int stepPin = 3; 
const int dirPin = 4; 

int v0;
int v1;



const int buffer_size = 600;
int buffer_pos;

byte v1_buf[buffer_size];
byte v0_buf[buffer_size];

void reset(){
  buffer_pos = 0;
}

void mem(int v0, int v1) {
  if (buffer_pos >= buffer_size) {
    return;
  }
  
  v0_buf[buffer_pos] = v0;
  v1_buf[buffer_pos] = v1; 
  buffer_pos += 1;
}



void pureRead(int wait) {

  unsigned long start = micros();
  unsigned long current = 0;
  
  unsigned long delta;

  int old0 = v0;
  int old1 = v1;

  int count0 = 0;
  int value0 = 0;

  int count1 = 0;
  int value1 = 0;

  int i = 0;


  do {

    

    if (i%2 == 0) {
      // A0
      int v = analogRead(A0);
      int d = abs(v - old0);

      if (d < 15) {
        value0 += v;
        count0 += 1;
      }  
    } else {
      // A1;
      // A0
      int v = analogRead(A1);
      int d = abs(v - old1);

      if (d < 15) {
        value1 += v;
        count1 += 1;
      } 
    }
    i++;
     current = micros();
    delta = current - start;
  }  while (delta < (wait-50));

  if (count0 > 0) {
    v0 = value0/count0;
  } else {
    v0 = old0;
  }
  
  if (count1 > 1) {
    v1 = value1/count1;
  } else {
    v1 = old1;
  }

 

  delayMicroseconds(wait-delta); 
 
} 
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);

  
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  analogReference(INTERNAL);

   analogRead (A0);
   analogRead (A1);
  Serial.begin(115200);
  Serial.println ();

  ADCSRA &= ~(bit (ADPS0) | bit (ADPS1) | bit (ADPS2)); // clear prescaler bits
  ADCSRA |= bit (ADPS2);                               //  16 

}



/*
 * analog read runs at 9kHz. 1kHZ = 1ms 9kHZ = 1/9ms
 * two analog reads need 2/9ms or 0.222ms or 222 microseconds
 * 
 */



void loop() {



  for (int dir = 0; dir < 200; dir ++) {

    

    if (dir % 2 == 0) {
        digitalWrite(dirPin,LOW); //Changes the rotations direction
    } else {
      digitalWrite(dirPin,HIGH);
    }

    
  
   // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < buffer_size/2; x++) {
    digitalWrite(stepPin,HIGH); 

    pureRead(500);
 

    mem(v0,v1);

  

    
    digitalWrite(stepPin,LOW);
  

    pureRead(500);

    
    mem(v0,v1);
    
  

    
  }

  delay (100);

    for (int i = 0; i < buffer_pos; i++) {
      Serial.println(String("") + v0_buf[i] + " " + v1_buf[i]);
    }

    reset();
  delay (500);


    //
     // 
  
  }

  

}
