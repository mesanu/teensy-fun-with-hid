#include <mbed.h>
#include <stdlib.h>

#include "controller.h"
extern "C" {
#include "eeprom.h"
}

#define EN_PIN          D0
#define EEPROM_SAVE_PIN D1
#define FUNC1_PIN       D2
#define FUNC2_PIN       D3
#define FUNC3_PIN       D4
#define FUNC4_PIN       D5
#define ADC_NOISE       A0

AnalogIn noise(ADC_NOISE);

DigitalOut led1(LED1);

int main() {
  eeprom_initialize();
  Controller controller(LED1,
                        FUNC1_PIN,
                        FUNC2_PIN,
                        FUNC3_PIN,
                        FUNC4_PIN,
                        EN_PIN,
                        EEPROM_SAVE_PIN);
  controller.load_from_eeprom();
  controller.start();
  while(1);
}
