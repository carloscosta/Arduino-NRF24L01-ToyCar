
// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>

// Include dependant SPI Library
#include <SPI.h>

#define DEBUG 1
#define BUZ 13

// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

/* 0: left;
   1: right;
   2: honk;
   3: front;
   4: back;
   5: panic */
uint8_t buf[6] = {0, 0, 0, 0, 0, 0};

const int in1 = 3;
const int in2 = 4;
const int in3 = 5;
const int in4 = 6;


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

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Initialize ASK Object
  rf_driver.init();

#ifdef DEBUG
  Serial.begin(9600);
  Serial.println("----- Start Serial Monitor -----");
#endif

  startup_song();
}

void loop()
{
  uint8_t buflen = sizeof(buf);

  // Check if received packet is correct size
  if (rf_driver.recv(buf, &buflen))
  {

#ifdef DEBUG
    // Message received with valid checksum
    Serial.print("Message Received: ");
    for (int i = 0; i < 6; i++) {
      Serial.print(buf[i]);
    }
    Serial.println(" ");
#endif

    // honk if requested
    if (buf[2] > 0) {
      bibi();
    }

    /* 0: left;
       1: right;
       2: honk;
       3: front;
       4: back;
       5: panic */

    if (buf[0] > 0)
    {
#ifdef DEBUG
      Serial.println("left");
#endif
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    }

    if (buf[1] > 0)
    {
#ifdef DEBUG
      Serial.println("right");
#endif
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    }

    if (buf[3] > 0)
    {
#ifdef DEBUG
      Serial.println("forward");
#endif
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
    }

    if (buf[4] > 0)
    {
#ifdef DEBUG
      Serial.println("backward");
#endif
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
    }

    if ((buf[0] == 0 && buf[1] == 0) && (buf[3] == 0 && buf[4] == 0))
    {
#ifdef DEBUG
      Serial.println("all motor off");
#endif
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
    }
  }
}
