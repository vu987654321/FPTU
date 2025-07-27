#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>

#define SERVO_CHANNEL_1 2
#define SERVO_CHANNEL_2 3
#define SERVO_CHANNEL_3 4
#define SERVO_CHANNEL_4 5
#define SERVO_LEFT 110
#define SERVO_STOP 310
#define SERVO_RIGHT 510

#define MOTOR_1_CHANNEL_A 8
#define MOTOR_1_CHANNEL_B 9
#define MOTOR_2_CHANNEL_A 10
#define MOTOR_2_CHANNEL_B 11
#define MOTOR_3_CHANNEL_A 12
#define MOTOR_3_CHANNEL_B 13
#define MOTOR_4_CHANNEL_A 14
#define MOTOR_4_CHANNEL_B 15

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void Setup_Servo_Motor()
{
  pwm.begin();
  pwm.setPWMFreq(50);
}

int doToPWM(int angle, int minAngle = 0, int maxAngle = 180, int minPWM = SERVO_LEFT, int maxPWM = SERVO_RIGHT)
{
  if (angle < minAngle) angle = minAngle;
  if (angle > maxAngle) angle = maxAngle;
  int pwm = minPWM + (angle - minAngle) * (maxPWM - minPWM) / (maxAngle - minAngle);
  return pwm;
}

void SetServo(uint8_t channel, uint16_t pulse)
{
  pwm.setPWM(channel, 0, pulse);
}

int approachSpeed(int current, int target, int maxStep)
{
  int diff = target - current;
  int step;
  if ((current > 0 && target < 0) || (current < 0 && target > 0)){
    step = maxStep * 2;
  }
  else{
    step = constrain(abs(diff) / 5, 20, maxStep);
  }
  if (diff > 0){
    current += step;
    if (current > target) current = target;
  }
  else if (diff < 0){
    current -= step;
    if (current < target) current = target;
  }
  return current;
}

void SetMotor1(int speedR1)
{
  if (speedR1 >= 0){
    pwm.setPin(MOTOR_1_CHANNEL_A, speedR1);
    pwm.setPin(MOTOR_1_CHANNEL_B, 0);
  } 
  else{
    pwm.setPin(MOTOR_1_CHANNEL_A, 0);
    pwm.setPin(MOTOR_1_CHANNEL_B, abs(speedR1));
  }
}

void SetMotor2(int speedR2)
{
  if (speedR2 >= 0){
    pwm.setPin(MOTOR_2_CHANNEL_A, speedR2);
    pwm.setPin(MOTOR_2_CHANNEL_B, 0);
  } 
  else{
    pwm.setPin(MOTOR_2_CHANNEL_A, 0);
    pwm.setPin(MOTOR_2_CHANNEL_B, abs(speedR2));
  }
}

void SetMotor3(int speedR3)
{
  if (speedR3 >= 0){
    pwm.setPin(MOTOR_3_CHANNEL_A, speedR3);
    pwm.setPin(MOTOR_3_CHANNEL_B, 0);
  } 
  else{
    pwm.setPin(MOTOR_3_CHANNEL_A, 0);
    pwm.setPin(MOTOR_3_CHANNEL_B, abs(speedR3));
  }
}

void SetMotor4(int speedR4)
{
  if (speedR4 >= 0){
    pwm.setPin(MOTOR_4_CHANNEL_A, speedR4);
    pwm.setPin(MOTOR_4_CHANNEL_B, 0);
  } 
  else{
    pwm.setPin(MOTOR_4_CHANNEL_A, 0);
    pwm.setPin(MOTOR_4_CHANNEL_B, abs(speedR4));
  }
}