#include <PS2X_lib.h>

#define PS2_DAT 12 
#define PS2_CMD 13 
#define PS2_SEL 15 
#define PS2_CLK 14 
#define pressures false
#define rumble true

PS2X ps2x;

unsigned long Last_Rumble_Time = 0;
const int Rumble_Time = 500;
int Rumble_Motor = 200;
bool Rumble = true;

unsigned long Last_Update_Time = 0;
const int Update_Interval = 15;
int Speed_Step = 0;
int R_X = 0, L_Y = 0;
int Target_Motor1 = 0, Target_Motor4 = 0;
int Current_Motor1 = 0, Current_Motor4 = 0;

int Speed_State = 1;
int Run_Speed[3] = {0, 1000, 3000};
int Run_Speed_Step[3] = {0, 30, 90};

int Rotate_State = 1;
int Rotate_Speed[3] = {0, 1000, 1500};
int Rotate_Speed_Step[3] = {0, 500, 1250};

int Servo_Balls_State = 0;
int Servo_Current_Balls[3] = {0, 180, 100};

int Servo_Fruits_State = 0;
int Servo_Current_Fruits[3] = {0, 130, 100};

const int Bottom_Fruits_Linear = 2;
const int Top_Fruits_Linear = 32; 
const int Bottom_Balls_Linear = 39; 
const int Top_Balls_Linear = 36;

int Motor2_Direct = 0;

void SetupController()
{
  Serial.print("Ket noi voi tay cam PS2:");
  int error = -1;
  for (int i = 1; i <= 5; i++){
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
    Serial.print(".");
    delay(1000);
  }
  switch (error){
  case 0:
    Serial.println(" Ket noi tay cam PS2 thanh cong");
    break;
  case 1:
    Serial.println(" LOI: Khong tim thay tay cam, hay kiem tra day ket noi vơi tay cam ");
    break;
  case 2:
    Serial.println(" LOI: khong gui duoc lenh");
    break;
  case 3:
    Serial.println(" LOI: Khong vao duoc Pressures mode ");
    break;
  }
  pinMode(Bottom_Fruits_Linear, INPUT_PULLUP);
  pinMode(Top_Fruits_Linear, INPUT_PULLUP);
  pinMode(Bottom_Balls_Linear, INPUT);
  pinMode(Top_Balls_Linear, INPUT);
  Last_Rumble_Time = millis();
}

void Control()
{
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

  if(Rumble == true && millis() - Last_Rumble_Time >= Rumble_Time){
    Last_Rumble_Time = millis();
    Rumble_Motor = 0;
    Rumble = false;
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

  if (ps2x.Button(PSB_L1) && digitalRead(Top_Fruits_Linear) != LOW){
    SetServo(SERVO_CHANNEL_1, SERVO_RIGHT);
  }
  else if (ps2x.Button(PSB_L2) && digitalRead(Bottom_Fruits_Linear) != LOW){
    SetServo(SERVO_CHANNEL_1, SERVO_LEFT);
  }
  else{
    SetServo(SERVO_CHANNEL_1, SERVO_STOP);
  }

  if (ps2x.Button(PSB_L1) && digitalRead(Top_Fruits_Linear) == LOW){
    if(Rumble == false){
      Rumble = true;
      Rumble_Motor = 255;
      Last_Rumble_Time = millis();
    }
  }

  if (ps2x.Button(PSB_L2) && digitalRead(Bottom_Fruits_Linear) == LOW){
    if(Rumble == false){
      Rumble = true;
      Rumble_Motor = 255;
      Last_Rumble_Time = millis();
    }
  }

  if (ps2x.ButtonPressed(PSB_PAD_UP)){
    if(Servo_Fruits_State == 2){
      Servo_Fruits_State=0;
    }
    Servo_Fruits_State++;
    SetServo(SERVO_CHANNEL_2, doToPWM(Servo_Current_Fruits[Servo_Fruits_State]));
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

  if (ps2x.Button(PSB_PINK)){
    SetServo(SERVO_CHANNEL_4, SERVO_RIGHT);
  }
  else if (ps2x.Button(PSB_RED)){
    SetServo(SERVO_CHANNEL_4, SERVO_LEFT);
  }
  else{
    SetServo(SERVO_CHANNEL_4, SERVO_STOP);
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (ps2x.Button(PSB_R1) && digitalRead(Top_Balls_Linear) != LOW){
    delay(10);
    SetMotor3(3300);
    SetMotor2(4000);
    Motor2_Direct = 1;
  }
  else if (ps2x.Button(PSB_R2) && digitalRead(Bottom_Balls_Linear) != LOW){
    delay(10);
    SetMotor3(-800);
    SetMotor2(-4000);
    Motor2_Direct = 2;
  }
  else if (digitalRead(Bottom_Balls_Linear) != LOW){
    SetMotor3(200);
    SetMotor2(0);
    if(Motor2_Direct == 1){
      SetMotor2(-500);
      delay(100);
    }
    else if(Motor2_Direct == 2){
      SetMotor2(500);
      delay(100);
    }
    SetMotor2(0);
    Motor2_Direct = 0;
  }

  if(ps2x.ButtonPressed(PSB_PAD_DOWN)){
    SetMotor2(-3000);
    SetMotor3(-500);
    delay(2000);
    SetMotor3(0);
    SetMotor2(-1000);
    while(true){
      ps2x.read_gamepad(false, Rumble_Motor);
      if(ps2x.ButtonPressed(PSB_START)){
        break;
      }
    }
  }
  
  if(digitalRead(Bottom_Balls_Linear) == LOW && ps2x.Button(PSB_R1)==false){
    SetMotor3(0);
    SetMotor2(0);
  }

  if (ps2x.Button(PSB_R1) && digitalRead(Top_Balls_Linear) == LOW){
    if(Rumble == false){
      Rumble = true;
      Rumble_Motor = 255;
      Last_Rumble_Time = millis();
    }
  }

  if (ps2x.Button(PSB_R2) && digitalRead(Bottom_Balls_Linear) == LOW){
    if(Rumble == false){
      Rumble = true;
      Rumble_Motor = 255;
      Last_Rumble_Time = millis();
    }
  }

  if (ps2x.ButtonPressed(PSB_GREEN)){
    if(Servo_Balls_State == 2){
      Servo_Balls_State=0;
    }
    Servo_Balls_State++;
    SetServo(SERVO_CHANNEL_3, doToPWM(Servo_Current_Balls[Servo_Balls_State]));
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

  if (ps2x.ButtonPressed(PSB_L3)){
    if(Speed_State == 2){
      Speed_State=0;
    }
    Speed_State++;
    if(Rumble == false){
      Rumble = true;
      if (Speed_State == 1){
        Rumble_Motor = 100;
      }
      else if (Speed_State == 2){
        Rumble_Motor = 255;
      }
      Last_Rumble_Time = millis();
    }
  }

  if (ps2x.ButtonPressed(PSB_R3)){
    if(Rotate_State == 2){
      Rotate_State=0;
    }
    Rotate_State++;
    if(Rumble == false){
      Rumble = true;
      if (Rotate_State == 1){
        Rumble_Motor = 100;
      }
      else if (Rotate_State == 2){
        Rumble_Motor = 255;
      }
      Last_Rumble_Time = millis();
    }
  }
  
  L_Y = 128-ps2x.Analog(PSS_LY);
  R_X = ps2x.Analog(PSS_RX)-127;
  Target_Motor1 = map(L_Y, -127, 128, -Run_Speed[Speed_State], Run_Speed[Speed_State]);
  Target_Motor4 = Target_Motor1;
  Speed_Step = Run_Speed_Step[Speed_State];

  if(R_X < 0){
    Target_Motor1 += Rotate_Speed[Rotate_State];
    Target_Motor4 -= Rotate_Speed[Rotate_State];
    Speed_Step = Rotate_Speed_Step[Rotate_State];
  }
  else if(R_X > 0){
    Target_Motor1 -= Rotate_Speed[Rotate_State];
    Target_Motor4 += Rotate_Speed[Rotate_State];
    Speed_Step = Rotate_Speed_Step[Rotate_State];
  }

  if (millis() - Last_Update_Time >= Update_Interval){
    Last_Update_Time = millis();
    Current_Motor1 = approachSpeed(Current_Motor1, Target_Motor1, Speed_Step);
    Current_Motor4 = approachSpeed(Current_Motor4, Target_Motor4, Speed_Step);
    SetMotor1(Current_Motor1);
    SetMotor4(Current_Motor4);
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
}