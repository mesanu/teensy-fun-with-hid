#include <mbed.h>
#include "rtos.h"
#include "USBSerial.h"

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

int main() {
  Controller controller(LED1,
                        FUNC1_PIN,
                        FUNC2_PIN,
                        FUNC3_PIN,
                        FUNC4_PIN,
                        EN_PIN,
                        EEPROM_SAVE_PIN);
  return 0;
}
