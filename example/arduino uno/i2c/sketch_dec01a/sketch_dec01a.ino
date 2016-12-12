#include <Wire.h>
#include "stdLCD.h"

volatile int32_t i;
stdLCD_t lcd0;
void setup() {
  Wire.begin();
  stdLCD_init(&lcd0, 0, stdLCD_display_16x2);
  
  stdLCD_puts(&lcd0, "hello world!!\n\r");
  stdLCD_puts(&lcd0, "hello world!!");
  for (i = 0; i < 200000; i++);
  stdLCD_set_backLight_off(&lcd0);
  for (i = 0; i < 200000; i++);
  stdLCD_set_backLight_on(&lcd0);
  for (i = 0; i < 200000; i++);
  stdLCD_set_backLight_off(&lcd0);
  for (i = 0; i < 200000; i++);
  stdLCD_set_backLight_on(&lcd0);
  for (i = 0; i < 200000; i++);
}

void loop() {
  stdLCD_display_shift(&lcd0, -1);
  for (i = 0; i < 200000; i++);
  stdLCD_display_shift(&lcd0, -1);
  for (i = 0; i < 200000; i++);
  stdLCD_display_shift(&lcd0, -1);
  for (i = 0; i < 200000; i++);
  stdLCD_display_shift(&lcd0, -1);
  for (i = 0; i < 200000; i++);
  stdLCD_display_shift(&lcd0, 1);
  for (i = 0; i < 200000; i++);
  stdLCD_display_shift(&lcd0, 1);
  for (i = 0; i < 200000; i++);
  stdLCD_display_shift(&lcd0, 1);
  for (i = 0; i < 200000; i++);
  stdLCD_display_shift(&lcd0, 1);
  for (i = 0; i < 200000; i++);

}
