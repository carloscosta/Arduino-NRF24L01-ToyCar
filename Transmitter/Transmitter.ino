// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>

// Include dependant SPI Library
#include <SPI.h>

#define DEBUG 0
#define BUZ 9

#define LVRx A0
#define LVRy A1
#define Lsw  6

#define RVRx A2
#define RVRy A3
#define Rsw  7

int val_lvrx = 0;
int val_rvrx = 0;
int val_rvry = 0;

// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

/* 0: left;
   1: right;
   2: honk;
   3: front;
   4: back;
   5: panic */
uint8_t buf[6] = {0, 0, 0, 0, 0, 0};

void startup_song()
{
  tone(BUZ, 262, 200); //DO
  delay(200);
  tone(BUZ, 294, 300); //RE
  delay(200);
  tone(BUZ, 330, 300); //MI
  delay(200);
  tone(BUZ, 349, 300); //FA
  delay(300);
  tone(BUZ, 349, 300); //FA
  delay(300);
  tone(BUZ, 349, 300); //FA
  delay(300);
}

void bibi()
{
  tone (BUZ, 330, 300); // MI
  delay(500); // tempo
  tone (BUZ, 330, 300); // MI

  delay(2000); // tempo
}

void setup()
{
  pinMode(BUZ, OUTPUT); // buzzer

#ifdef DEBUG
  Serial.begin(9600);
  Serial.println("----- Start Serial Monitor -----");
#endif

  // Initialize ASK Object
  rf_driver.init();

  startup_song();
}

void loop()
{
  val_lvrx = analogRead(LVRx);
  val_lvrx = map(val_lvrx, 0, 1023, 0, 255);
  val_lvrx = constrain(val_lvrx, 0, 255);

#ifdef DEBUG
  Serial.print("LVRx = ");
  Serial.println(val_lvrx);
#endif

  val_rvry = analogRead(RVRy);
  val_rvry = map(val_rvry, 0, 1023, 0, 255);
  val_rvry = constrain(val_rvry, 0, 255);

#ifdef DEBUG
  Serial.print("RVRy = ");
  Serial.println(val_rvry);
#endif

  /* right joystick */
  if (val_rvry == 255)
  {
    buf[0] = 1;
    buf[1] = 0;
  }
  else if (val_rvry == 0)
  {
    buf[0] = 0;
    buf[1] = 1;
  }
  else if (val_rvry >= 115 && val_rvry <= 135)
  {
    buf[0] = 0;
    buf[1] = 0;
  }

  // panic
  buf[5] = 0;

  /* left joystick */
  if (val_lvrx == 0)
  {
    buf[3] = 1;
    buf[4] = 0;
  }
  else if (val_lvrx == 255)
  {
    buf[3] = 0;
    buf[4] = 1;
  }
  else if (val_rvry >= 115 && val_rvry <= 135)
  {
    buf[3] = 0;
    buf[4] = 0;
  }

  buf[5] = 0;

#ifdef DEBUG
  Serial.print("buf[left;right;honk;front;back;panic] = ");
  for (int i = 0; i < 6; i++)
  {
    Serial.print(buf[i]);
    Serial.print(";");
  }
  Serial.println(" ");
#endif

  rf_driver.send((uint8_t *)buf, 6);
  rf_driver.waitPacketSent();
}
