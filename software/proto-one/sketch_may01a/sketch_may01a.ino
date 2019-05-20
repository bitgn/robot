// defines pins numbers
const int stepPin = 3; 
const int dirPin = 4; 

int v0 = -1;
int v1 = -1;

void pureRead() {

  int old0 = v0;
  int old1 = v1;

  int count0 = 0;
  int value0 = 0;

  int count1 = 0;
  int value1 = 0;

  int i = 0;

  for (int i = 0; i< 8; i++) {

    if (i%2 == 0) {
      // A0
      int v = analogRead(A0);
      int d = abs(v - old0);

      if (d < 15 || old0 < 0) {
        value0 += v;
        count0 += 1;
      }  
    } else {
      // A1;
      // A0
      int v = analogRead(A1);
      int d = abs(v - old1);

      if (d < 15 || old1 < 0) {
        value1 += v;
        count1 += 1;
      } 
    }
  
  }  

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


  Serial.write(255);
  Serial.write(v0);
  Serial.write(v1);
  //Serial.write(Serial.availableForWrite());

 

 
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

    
  
  
  for(int x = 0; x < 10000; x++) {
    digitalWrite(stepPin,HIGH); 

    unsigned long start = micros();
    pureRead();
    unsigned long finish = micros();    
    delayMicroseconds(500 - (finish - start)); 

 
  

    
    digitalWrite(stepPin,LOW);
  

    start = micros();
    pureRead();
    finish = micros();    
    delayMicroseconds(500 - (finish - start)); 


    
  

    
  }

  }

  

}
