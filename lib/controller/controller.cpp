#include <mbed.h>
#include "controller.h"
#include "USBMouseKeyboard.h"


Controller::Controller(PinName led,
                  PinName func1,
                  PinName func2,
                  PinName func3,
                  PinName func4,
                  PinName enable,
                  PinName save_to_eeprom):
                  function1(func1, PullDown),function2(func2,PullDown),
                  function3(func3,PullDown),function4(func4,PullDown),
                  en_config(enable,PullDown),save_to_eeprom(save_to_eeprom,PullDown),
                  led1(led) {

}

void Controller::led_pulse(int times){
  for(int i=0; i<times; i++){
    led1 = 1;
    Thread::wait(PULSE_INTERVAL/2);
    led1 = 0;
    Thread::wait(PULSE_INTERVAL/2);
  }
}

void Controller::main_controller(){

}
