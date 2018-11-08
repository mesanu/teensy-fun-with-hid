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
                  func1_interrupt(func1,PullDown),
                  func2_interrupt(func2,PullDown),
                  func3_interrupt(func3,PullDown),
                  func4_interrupt(func4,PullDown),
                  en_interrupt(enable,PullDown),
                  save_interrupt(save_to_eeprom,PullDown),
                  led1(led) {
  func1_interrupt.rise(this,&Controller::func1_event);
  func2_interrupt.rise(this,&Controller::func2_event);
  func3_interrupt.rise(this,&Controller::func3_event);
  func4_interrupt.rise(this,&Controller::func4_event);
  en_interrupt.rise(this,&Controller::en_event);
  save_interrupt.rise(this,&Controller::save_event);
}

void Controller::main_controller(){

}

void Controller::led_pulse(int times){
  for(int i=0; i<times; i++){
    led1 = 1;
    Thread::wait(PULSE_INTERVAL/2);
    led1 = 0;
    Thread::wait(PULSE_INTERVAL/2);
  }
}

void Controller::func1_event() {
  this->events.set(FUNC1_EVENT_FLAG);
}

void Controller::func2_event() {
  events.set(FUNC2_EVENT_FLAG);
}

void Controller::func3_event() {
  events.set(FUNC3_EVENT_FLAG);
}

void Controller::func4_event() {
  events.set(FUNC4_EVENT_FLAG);
}

void Controller::en_event() {
  events.set(EN_EVENT_FLAG);
}

void Controller::save_event() {
  events.set(SAVE_EVENT_FLAG);
}
