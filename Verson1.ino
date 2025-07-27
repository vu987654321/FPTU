#include"Servo_Motor.h"
#include"Controller.h"

void setup()
{
  Serial.begin(115200); 
  SetupController();
  Setup_Servo_Motor();
}

void loop()
{
  ps2x.read_gamepad(false, Rumble_Motor);
  Control();
}