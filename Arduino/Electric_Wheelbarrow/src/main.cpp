#include <Arduino.h>

#define HAL A1

#define MAX_COUNTER_VALUE 3000
#define MIN_COUNTER_VALUE 300

int min_trottle = 0;

void setup()
{
  pinMode(HAL, INPUT);

  delay(500);
  min_trottle = analogRead(HAL) + 50;

  Serial.begin(115200);
}

void loop()
{
  int val = analogRead(HAL);

  if (val < min_trottle) {
    val = 0;
  } else {
    val = int(map(val, min_trottle, 1023, MIN_COUNTER_VALUE, MAX_COUNTER_VALUE));
  }

  Serial.print("v 0 ");
  Serial.print(val);
  Serial.print(" 0\n");
}
