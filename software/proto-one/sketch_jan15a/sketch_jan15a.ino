int v0;
int v1;
int pos;
const int pResistor = A0; // Photoresistor at Arduino analog pin A0
#include <AFMotor.h>
  AF_DCMotor motor(1);


AF_Stepper(200, 1);
 
void setup() {
  // we don't expect high voltage on the transistor, 
  // so lower the reference for more precision
  analogReference(INTERNAL);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  Serial.begin(115200);
  motor.setSpeed(255);
  pos = 0;
}
bool last_v0;
bool last_v1;
void loop() {

  
  v0 = analogRead(A0); // green
  v1 = analogRead(A1); // red
  bool v0_high;
  bool v1_high;

  
  v0_high = v1 < 270;
  v1_high = v0 < 220;


  
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
  
  
  if (pos != old_pos) {
 //   Serial.print("0 -10 10 ");
   // Serial.println(pos);
    
  }
  /*
  if (abs(pos) < 20) {
    motor.setSpeed(150);
  } else {
    motor.setSpeed(255);
  }

  
  if (pos > 0) {
    motor.run(FORWARD);
  } else if (pos < 0) {
    motor.run(BACKWARD);
  } else {
    motor.run(RELEASE);
  }
  
*/
if (pos ==0) {
    motor.run(RELEASE);
} else {


  
if (pos < 0) {
    motor.run(FORWARD);
  } else if (pos > 0) {
    motor.run(BACKWARD);
  }

  // run for a second
  delay(1000);
  // stop
  motor.run(RELEASE);
  delay(2000);
  pos = 0;

}



  
  
  last_v0 = v0_high;
  last_v1 = v1_high;

 // return;
 
  Serial.print(pos);
  Serial.print(" ");
  Serial.print(v0);
  Serial.print(" ");
  Serial.print(v1);
  Serial.println(" 100 400");
}
