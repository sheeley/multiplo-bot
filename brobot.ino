#include <DCMotor.h>
#include <IRremote.h>
//#include <RobotRemote.h>
#include <Servo.h>
#include <Ping.h>

const int LED_PIN =  13;
const int SENSOR_0 = 14;
const int SENSOR_1 = 15;
const int SENSOR_2 = 16;
const int SENSOR_3 = 17;
const int SENSOR_4 = 18;
const int SENSOR_5 = 19;
const int BUZZ_PIN = 23;
const int BATT_SENSOR = 24;
const int RUN_BUTTON = 25;

const int CLAW_CLOSED = 102;
const int CLAW_OPENED = 26;
const int CLAW_MID = (CLAW_CLOSED + CLAW_OPENED) / 2;
const int ARM_HIGH = 2;
const int ARM_LOW = 105;
const int ARM_MID = (ARM_LOW - ARM_HIGH) / 2;

const int DIST_STOP = 25;
const int DIST_TURN = 45;

DCMotor motor0(M0_EN, M0_D0, M0_D1);
DCMotor motor1(M1_EN, M1_D0, M1_D1);
PingSensor ping(A0);
IRrecv irReceiver(A3);
//RobotRemote irReceiver(A3);
Servo clawServo;
Servo armServo;

const int INTERVAL = 100;

void setup(){
  motor1.setClockwise(false);

  pinMode(LED_PIN, OUTPUT);
  pinMode(SENSOR_4, OUTPUT);
  pinMode(SENSOR_5, OUTPUT);
  pinMode(SPEAKER, OUTPUT);

  armServo.attach(SENSOR_4);
  clawServo.attach(SENSOR_5);

  armServo.write(ARM_MID);
  clawServo.write(CLAW_MID);
}

int distance, oldDistance, lastValue, received,
leftTargetSpeed = 0,
rightTargetSpeed = 0,
clawDegrees = CLAW_MID,
go = 0;
long previousMillis = 0;

void loop() {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > INTERVAL) {
    // only run things with sensors every few hundred millis
    previousMillis = currentMillis;

    int pressure = analogRead(2);
    int light = analogRead(1);

    if(pressure < 110){
      clawDegrees = clawServo.read();
      
    }

//    digitalWrite(LED_PIN, HIGH);
//    digitalWrite(LED_PIN, LOW);

    if(go == 1){
      distance = ping.measureCM();
      if(distance != oldDistance){
       if(distance <= DIST_TURN){
        leftTargetSpeed = 50;
        rightTargetSpeed = -50;
      } else {
        leftTargetSpeed = rightTargetSpeed = 100;
      }
      oldDistance = distance;
    } else {
//      toneDelay(SPEAKER, 740, 100);
      leftTargetSpeed = rightTargetSpeed = 0;
    }
  }
  received = irReceiver.getIRRemoteCode();
  if (received != lastValue) {
    // Something different received from IR, show it
    if (received) {
      handleInput(received);
    }
    lastValue = received;
  }
}
smoothSpeed();
moveClaw();
}

void handleInput(int received){
  switch(received){
    case 1:
    armServo.write(ARM_HIGH);
    break;
    case 2:
    armServo.write(ARM_MID);
    break;
    case 3:
    armServo.write(ARM_LOW);
    break;
    case 4:
    clawDegrees = CLAW_OPENED;
    break;
    case 5:
    clawDegrees = CLAW_MID;
    break;
    case 6:
    clawDegrees = CLAW_CLOSED;
    break;
    case 7:

    break;
    case 8:
    go = (go == 1) ? 0 : 1;
    break;
    case 9:

    break;
    case 10:
      // ?????????
    break;

  }
}

void moveClaw(){
  if(clawDegrees <= CLAW_CLOSED && clawDegrees >= CLAW_OPENED){
    clawServo.write(clawDegrees);
 }
}

void smoothSpeed(){
  int leftSpeed = motor0.getSpeed();
  int rightSpeed = motor1.getSpeed();
  if(leftSpeed != leftTargetSpeed){
    int ls = leftTargetSpeed;
    if(abs(leftSpeed - ls) > 20){
     ls = ls / 2;
   }
   motor0.setSpeed(ls);
 }

 if(rightSpeed != rightTargetSpeed){
  int rs = rightTargetSpeed;
  if(abs(rightSpeed - rs) > 20){
   rs = rs / 2;
 }
 motor1.setSpeed(rs);
}
}




//void arm_react(){
////  if(distance != oldDistance){
////    if(distance < DIST_STOP){
////      toneDelay(SPEAKER, distance * 100, 100);
////    }
//    int height = ARM_HIGH + distance;
//    if(height > ARM_LOW){
//      height = ARM_LOW;
//    }
//    armServo.write(height);
//
//    int width = (CLAW_CLOSED) - (distance - 22);
//    if(width < CLAW_OPENED){
//      width = CLAW_OPENED;
//    } else if(width > CLAW_CLOSED){
//      width = CLAW_CLOSED;
//    }
//    clawServo.write(width);
//    delay(500);
//  }
//}
