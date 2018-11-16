#include <mbed.h>
#include <stdlib.h>

#include "controller.h"
#include "USBMouseKeyboard.h"

extern "C" {
#include "eeprom.h"
}

#ifdef FWHID_DEBUG
#include "USBSerial.h"
#include <stdarg.h>
USBSerial serial;
#endif

#ifdef FWHID_STALL_ON_CONSTRUCTOR
#define TRIGGER 99
#endif

#define RAND_CHAR_ASCII_OFFSET 65
#define RAND_CHAR_ASCII_MAX 57

Controller::Controller(PinName led,
                  PinName func1,
                  PinName func2,
                  PinName func3,
                  PinName func4,
                  PinName enable,
                  PinName save_to_eeprom,
                  PinName mac_or_pc):
                  func1_interrupt(func1,PullUp),
                  func2_interrupt(func2,PullUp),
                  func3_interrupt(func3,PullUp),
                  func4_interrupt(func4,PullUp),
                  en_interrupt(enable,PullUp),
                  save_interrupt(save_to_eeprom,PullUp),
                  mac_or_pc_interrupt(mac_or_pc,PullUp),
                  led1(led),
                  circle(F4_CURVE_FREQ,F4_CURVE_DELAY),
                  fig8(F4_CURVE_FREQ,F4_CURVE_DELAY)
                  #ifdef FWHID_DEBUG
                  , print_settings_interrupt(PRINT_SETTINGS_PIN, PullUp)
                  #endif
                  {

                  }

void Controller::start(){

  led_pulse(3);


  #ifdef FWHID_STALL_ON_CONSTRUCTOR
  while(serial._getc() != TRIGGER);
  #endif
  func1_interrupt.fall(this,&Controller::func1_event);
  func2_interrupt.fall(this,&Controller::func2_event);
  func3_interrupt.fall(this,&Controller::func3_event);
  func4_interrupt.fall(this,&Controller::func4_event);
  en_interrupt.fall(this,&Controller::en_event);
  save_interrupt.fall(this,&Controller::save_event);
  mac_or_pc_interrupt.fall(this,&Controller::mac_or_pc_event);
  #ifdef FWHID_DEBUG
  print_settings_interrupt.fall(this,&Controller::print_event);
  serial_dbg("Wired print event interrupt\n");
  #endif


  serial_dbg("Wired interrupts\n");

  func1_thread.start(this,&Controller::func1);
  func2_thread.start(this,&Controller::func2);
  func3_thread.start(this,&Controller::func3);
  func4_thread.start(this,&Controller::func4);

  serial_dbg("Started function threads\n");

  main_controller_thread.start(this,&Controller::main_controller);

  serial_dbg("Started main controller thread\n");

}

void Controller::main_controller(){
  uint32_t events_list;
  while(1){
    events.wait_any(EN_EVENT_FLAG
                  | FUNC1_EVENT_FLAG
                  | FUNC2_EVENT_FLAG
                  | FUNC3_EVENT_FLAG
                  | FUNC4_EVENT_FLAG
                  | SAVE_EVENT_FLAG
                  | MAC_OR_PC_EVENT_FLAG
                  #ifdef FWHID_DEBUG
                  | PRINT_SETTINGS_EVENT_FLAG
                  #endif
                  ,osWaitForever,false);
    wait(0.5);
    events_list = events.get();
    if(events_list & EN_EVENT_FLAG){
      enabled_all = !enabled_all;
      serial_dbg("EN_Interrupt_triggered: %x, %d\n",events.get(),enabled_all);
      if(enabled_all){
        func1_thread.signal_set(MAIN_EN_SIGNAL);
        func2_thread.signal_set(MAIN_EN_SIGNAL);
        func3_thread.signal_set(MAIN_EN_SIGNAL);
        func4_thread.signal_set(MAIN_EN_SIGNAL);
      }
      else{
        func1_thread.signal_clr(MAIN_EN_SIGNAL);
        func2_thread.signal_clr(MAIN_EN_SIGNAL);
        func3_thread.signal_clr(MAIN_EN_SIGNAL);
        func4_thread.signal_clr(MAIN_EN_SIGNAL);
      }
      events.clear(EN_EVENT_FLAG);
      led_pulse((int)enabled_all + 1);
    }
    else if(events_list & FUNC1_EVENT_FLAG){
      func1_aggro = (func1_aggro + 1) % (FUNC_AGGRO_MAX + 1);
      serial_dbg("FUNC1_Interrupt_triggered: %x, %d\n",events.get(),func1_aggro);
      events.clear(FUNC1_EVENT_FLAG);
      led_pulse(func1_aggro + 1);
    }
    else if(events_list & FUNC2_EVENT_FLAG){
      func2_aggro = (func2_aggro + 1) % (FUNC_AGGRO_MAX + 1);
      serial_dbg("FUNC2_Interrupt_triggered: %x, %d\n",events.get(),func2_aggro);
      events.clear(FUNC2_EVENT_FLAG);
      led_pulse(func2_aggro + 1);
    }
    else if(events_list & FUNC3_EVENT_FLAG){
      func3_aggro = (func3_aggro + 1) % (FUNC_AGGRO_MAX + 1);
      serial_dbg("FUNC3_Interrupt_triggered: %x, %d\n",events.get(),func3_aggro);
      events.clear(FUNC3_EVENT_FLAG);
      led_pulse(func3_aggro + 1);
    }
    else if(events_list & FUNC4_EVENT_FLAG){
      func4_aggro = (func4_aggro + 1) % (FUNC_AGGRO_MAX + 1);
      serial_dbg("FUNC4_Interrupt_triggered: %x, %d\n",events.get(),func4_aggro);
      events.clear(FUNC4_EVENT_FLAG);
      led_pulse(func4_aggro + 1);
    }
    else if(events_list & SAVE_EVENT_FLAG){
      serial_dbg("SAVE_Interrupt_triggered: %x\n",events.get());
      save_to_eeprom();
      events.clear(SAVE_EVENT_FLAG);
      led_pulse(1);
    }
    else if(events_list & MAC_OR_PC_EVENT_FLAG)
    {
      mac_or_pc = !mac_or_pc;
      serial_dbg("mac_or_pc_Interrupt_triggered: %x, %d\n",events.get(),mac_or_pc);
      events.clear(MAC_OR_PC_EVENT_FLAG);
      led_pulse((int)mac_or_pc + 1);
    }
    #ifdef FWHID_DEBUG
    else if(events_list & PRINT_SETTINGS_EVENT_FLAG){
      serial_dbg("print_Interrupt_triggered: %x\n",events.get());
      print_settings();
      events.clear(PRINT_SETTINGS_EVENT_FLAG);
      led_pulse(1);
    }
    #endif
  }
}

void Controller::led_pulse(int times){
  for(int i=0; i<times; i++){
    led1.write(1);
    wait(PULSE_INTERVAL/2);
    led1.write(0);
    wait(PULSE_INTERVAL/2);
  }
}

void Controller::func1(){
  while(1){
    Thread::wait(THREAD_DELAY);
    if(roll(FUNC_AGGRO_MAX) < func1_aggro){
      #ifdef FWHID_DEBUG
      serial_dbg("Func1 activated\n");
      #else
      if(mac_or_pc){
        device.keyCode(KEY_RLOGO,KEY_TAB);
      else{
        device.keyCode(KEY_ALT,KEY_TAB);
      }
      #endif
    }
  }
}

void Controller::func2(){
  while(1){
    Thread::wait(THREAD_DELAY);
    if(roll(FUNC_AGGRO_MAX) < func2_aggro){
      #ifdef FWHID_DEBUG
      serial_dbg("Func2 activated\n");
      #else
      device.keyCode(KEY_CAPS_LOCK);
      #endif
    }
  }
}

void Controller::func3(){
  char r_char;
  while(1){
    Thread::wait(THREAD_DELAY);
    serial_dbg("Func3 activated\n");
    if(roll(FUNC_AGGRO_MAX) < func3_aggro){
      while(1){
        r_char = roll(RAND_CHAR_ASCII_MAX) + RAND_CHAR_ASCII_OFFSET;
        #ifdef FWHID_DEBUG
        serial_dbg("Print char: %c\n",r_char);
        #else
        device.putc(rand_char);
        #endif
        if(roll(F3_ROLL_MAX) < F3_ROLL_I){
          break;
        }
     }
    }
  }
}

void Controller::func4(){
  while(1){
    uint32_t num_steps;
    Thread::wait(THREAD_DELAY);
    if(roll(FUNC_AGGRO_MAX) < func4_aggro){
      serial_dbg("Func4 activated\n");
      num_steps = roll(F4_CURVE_FREQ);
      if(roll(2)){
        serial_dbg("picked circle\n");
        for(int i=0;i<num_steps;i++){
         #ifndef FWHID_DEBUG
         circle.move_mouse(&device)
         #endif
         Thread::wait(F4_CURVE_DELAY);
        }
      }
      else{
        serial_dbg("picked fig8\n");
        for(int i=0;i<num_steps;i++){
         #ifndef FWHID_DEBUG
         fig8.move_mouse(&device);
         #endif
         Thread::wait(F4_CURVE_DELAY);
        }
      }
    }
  }
}

void Controller::func1_event() {
  events.set(FUNC1_EVENT_FLAG);
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

void Controller::mac_or_pc_event(){
  events.set(MAC_OR_PC_EVENT_FLAG);
}

#ifdef FWHID_DEBUG
void Controller::print_event() {
  events.set(PRINT_SETTINGS_EVENT_FLAG);
}
#endif

void Controller::save_to_eeprom() {
  uint16_t enc_settings;
  enc_settings = (enabled_all << EEPROM_ENABLED) |
                 (mac_or_pc << EEPROM_MAC_OR_PC) |
                 (func1_aggro << EEPROM_FUNC1_AGGRO) |
                 (func2_aggro << EEPROM_FUNC2_AGGRO) |
                 (func3_aggro << EEPROM_FUNC3_AGGRO) |
                 (func4_aggro << EEPROM_FUNC4_AGGRO);
  eeprom_mutex.lock();
  eeprom_busy_wait();
  eeprom_write_word(EEPROM_SAVE_ADDR,enc_settings);
  #ifdef FWHID_DEBUG
  eeprom_busy_wait();
  serial_dbg("Saved to eeprom: 0x%x\n",eeprom_read_word(EEPROM_SAVE_ADDR));
  #endif
  eeprom_mutex.unlock();
}

void Controller::load_from_eeprom(){
  uint16_t enc_settings;
  eeprom_mutex.lock();
  eeprom_busy_wait();
  enc_settings = eeprom_read_word(EEPROM_SAVE_ADDR);
  eeprom_mutex.unlock();
  enabled_all = (bool)((enc_settings >> EEPROM_ENABLED) & 0b1);
  mac_or_pc = (bool)((enc_settings >> EEPROM_MAC_OR_PC) & 0b1);
  func1_aggro = (char)((enc_settings >> EEPROM_FUNC1_AGGRO) & 0b11);
  func2_aggro = (char)((enc_settings >> EEPROM_FUNC2_AGGRO) & 0b11);
  func3_aggro = (char)((enc_settings >> EEPROM_FUNC3_AGGRO) & 0b11);
  func4_aggro = (char)((enc_settings >> EEPROM_FUNC4_AGGRO) & 0b11);
}

void Controller::serial_dbg(const char* format, ...){
  #ifdef FWHID_DEBUG
  va_list args;
  va_start(args, format);
  serial.vprintf(format,args);
  va_end(args);
  #endif
}

int Controller::roll(unsigned int max){
  uint32_t divisor;
  uint32_t rand_num;
  int ret;
  divisor = RAND_MAX / max;
  do{
    rand_mutex.lock();
    rand_num = rand();
    rand_mutex.unlock();
    ret = rand_num/divisor;
  } while( ret == max);
  return ret;
}

void Controller::print_settings(void){
  serial_dbg("---Controller Settings---\n");
  serial_dbg("Enabled :%d\n",(int)enabled_all);
  serial_dbg("Mac or PC :%d\n",(int)mac_or_pc);
  serial_dbg("Function 1 Aggro :%d\n",func1_aggro);
  serial_dbg("Function 2 Aggro :%d\n",func2_aggro);
  serial_dbg("Function 3 Aggro :%d\n",func3_aggro);
  serial_dbg("Function 4 Aggro :%d\n",func4_aggro);
}
