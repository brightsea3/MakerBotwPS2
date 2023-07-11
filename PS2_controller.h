#include <PS2X_lib.h>

PS2X ps2x; // create PS2 Controller Class object

#define PS2_DAT 12 // MISO  19
#define PS2_CMD 13 // MOSI  23
#define PS2_SEL 15 // SS     5
#define PS2_CLK 14 // SLK   18

#define SERVO 3

#define TOP_SPEED 4095
#define NORM_SPEED 2048
#define TURNING_FACTOR 0.25

int speed = NORM_SPEED;
bool ThuBong;

void setupPS2controller()
{
  int err = -1;
  while (err != 0)
  {
    err = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, true, true);
  }
  ThuBong = true;
}

bool PS2control()
{
  // ban
  // if (ps2x.Button(PSB_L2)){
  //   pwm.setPWM(2, 0, 400);
  //   pwm.setPWM(14, 0, 4095);
  //   ThuBong = false;
  // } else {
  //   pwm.setPWM(2, 0, 0);
  //   pwm.setPWM(14, 0, 0);
  // }
  if (ps2x.ButtonPressed(PSB_PAD_DOWN)){
    Serial.println("thay doi");
    if (ThuBong){
      ThuBong = false;
      pwm.setPWM(12, 0, 0);
    } else{
      ThuBong = true;
      pwm.setPWM(12, 0, 2048);
    }
  }
  if (!ThuBong) {
    if (ps2x.Button(PSB_L1)) {
      Serial.println("ban bong");
      pwm.setPWM(14, 0, 4095);
      delay(500);
      while (ps2x.Button(PSB_L1)) {
        pwm.setPWM(SERVO, 0, 400);
        delay(500);
        pwm.setPWM(SERVO, 0, 210);
        ps2x.read_gamepad(0, 0);
        delay(250);
      }
      pwm.setPWM(14, 0, 0);
      pwm.setPWM(SERVO, 0, 400); 
    }
  } else {
  //thu bong

  // if (ps2x.Button(PSB_R2))
  //   {
  //   speed = NORM_SPEED;
  //   } else {
  //     speed = TOP_SPEED;
  //   } 

  int nJoyX = 128 - ps2x.Analog(PSS_RY); // read x-joystick
  int nJoyY = 128 - ps2x.Analog(PSS_LX); // read y-joystick
  int nMotMixL;                          // Motor (left) mixed output
  int nMotMixR;                          // Motor (right) mixed output
  nJoyX = pow(nJoyX,3)/16384;
  nJoyY = pow(nJoyY,3)/16384;
  Serial.println(nJoyX);
  Serial.println(nJoyY);

  bool temp = (nJoyY * nJoyX > 0);
  if (nJoyY > 1 or nJoyY < -1) // Turning
  {
    nMotMixL = -nJoyY * !temp;
    nMotMixR = -nJoyY * !temp;
    nMotMixL *= TURNING_FACTOR;
    nMotMixR *= TURNING_FACTOR;
  }
  else // Forward or Reverse
  {
    nMotMixL = -nJoyX;
    nMotMixR = nJoyX;
  }

  int c1 = 0, c2 = 0, c3 = 0, c4 = 0;

  if (nMotMixR > 0)
  {
    c3 = nMotMixR;
    c3 = map(c3, 0, 128, 0, speed);
  }

  else if (nMotMixR < 0)
  {
    c4 = abs(nMotMixR);
    c4 = map(c4, 0, 128, 0, speed);
  }

  if (nMotMixL > 0)
  {
    c1 = nMotMixL;
    c1 = map(c1, 0, 128, 0, speed);
  }
  else if (nMotMixL < 0)
  {
    c2 = abs(nMotMixL);
    c2 = map(c2, 0, 128, 0, speed);
  }
  setPWMMotors(c1, c2, c3, c4);
  return 1;
  }
}

