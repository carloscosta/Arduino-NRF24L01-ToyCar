// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>

// Include dependant SPI Library
#include <SPI.h>

#define DEBUG 1
#define BUZ 13

// motor one
#define ENA 10
#define IN1 3
#define IN2 4

// motor two
#define ENB 9
#define IN3 5
#define IN4 6

#define HURRY 250
#define SLUE 200

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

void turn_off()
{
#ifdef DEBUG
  Serial.println("turn off all motors");
#endif

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void left()
{
#ifdef DEBUG
  Serial.println("turn left motor direction");
#endif
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, SLUE);
}

void right()
{
#ifdef DEBUG
  Serial.println("turn motor direction to right");
#endif
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, SLUE);
}

void center()
{
#ifdef DEBUG
  Serial.println("keep direction centered");
#endif
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
}

void forward()
{
#ifdef DEBUG
  Serial.println("forward");
#endif
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, HURRY);
}

void backward()
{
#ifdef DEBUG
  Serial.println("backward");
#endif
  // turn motor direction
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // accelerate from zero to maximum speed
  for (int i = 0; i < HURRY; i++)
  {
    analogWrite(ENA, i);
    delay(10);
  }
}

void setup()
{
  pinMode(BUZ, OUTPUT); // buzzer

  // set all the motor control pins to outputs
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);


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
  uint8_t buflen = sizeof(buf);

  // Check if received packet is correct size
  if (rf_driver.recv(buf, &buflen))
  {

#ifdef DEBUG
    // Message received with valid checksum
    Serial.print("Message Received: ");
    for (int i = 0; i < 6; i++)
      Serial.print(buf[i]);
    Serial.println(" ");
#endif

    // honk if requested
    if (buf[2] > 0) bibi();

    // evaluate left/right early
    if (buf[0] > 0 && buf[1] == 0)
    {
      left();
    }
    else if (buf[0] == 0 && buf[1] > 0)
    {
      right();
    }
    else
    {
      center();
    }

    // then evaluate forward/backward actions
    if (buf[3] > 0 && buf[4] == 0)
    {
      forward();
    }
    else if (buf[3] == 0 && buf[4] > 0)
    {
      backward();
    }
    else
    {
      turn_off();
    }
  }
  delay(200);
}
