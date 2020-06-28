#include <Arduino.h>

// these are the encoder inputs
#define ENC_A 5
#define ENC_B 4

#define MAX_COUNTER_VALUE 2500
#define MIN_COUNTER_VALUE 0

void read_encoder(void);

volatile int counter = 500;
volatile int previousCounter = 0; // forces update on startup

void setup()
{
  // pwm output pin
  pinMode(3, OUTPUT);

  pinMode(7, INPUT_PULLUP);

  // configure the pins as input
  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);

  // all changes on both inputs trigger the interrupt
  //attachInterrupt(ENC_A, read_encoder, CHANGE);
  //attachInterrupt(ENC_B, read_encoder, CHANGE);

  Serial.begin(115200);
}

void loop()
{
  read_encoder();

  // only update when the counter has changed
  if (counter != previousCounter) {
    previousCounter = counter;
  }

  if (digitalRead(7) == LOW) {
    Serial.print("v 0 ");
    Serial.print(counter);
    Serial.print(" 0\n");
    delay(100);
  }
}

// interrupt for reading the rotary encoder
void read_encoder()
{
  static uint8_t previousState = 3;
  static boolean tick = false;

  // the teensy has fast digital inputs
  uint8_t newA = digitalRead(ENC_A);
  uint8_t newB = digitalRead(ENC_B);

  // add both inputs to get an integer for the phase
  uint8_t newState = (newA << 1) | newB;

  // only if the state has changed (might not be true for bouncing signals)
  if (newState != previousState) {
    // return to neutral state from one direction
    if (newState == 3 && previousState == 2 && tick) {
      counter = counter + 20;
      // limit
      if (counter >= MAX_COUNTER_VALUE) {
        counter = MAX_COUNTER_VALUE;
      }
      tick = false;

    // return to neutral from the other direction
    } else if (newState == 3 && previousState == 1 && tick) {
      counter = counter - 20;
      // limit
      if (counter <= MIN_COUNTER_VALUE) {
        counter = MIN_COUNTER_VALUE;
      }
      tick = false;

    // only count when the midpoint of a click has passed
    } else if (newState == 2 && previousState == 0 && !tick) {
      tick = true;
    } else if (newState == 1 && previousState == 0 && !tick) {
      tick = true;
    } else {
      tick = false;
    }

    previousState = newState;
  }
}