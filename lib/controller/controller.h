#include <mbed.h>
#include "USBMouseKeyboard.h"

#define PULSE_INTERVAL 0.5

#define EN_EVENT_FLAG 1 << 0
#define FUNC1_EVENT_FLAG 1 << 1
#define FUNC2_EVENT_FLAG 1 << 2
#define FUNC3_EVENT_FLAG 1 << 3
#define FUNC4_EVENT_FLAG 1 << 4
#define SAVE_EVENT_FLAG 1 << 5

class Controller{
  public:
    Controller(PinName led,
              PinName func1,
              PinName func2,
              PinName func3,
              PinName func4,
              PinName enable,
              PinName save_to_eeprom);
    void start(void);
  private:
    USBMouseKeyboard device;
    InterruptIn func1_interrupt;
    InterruptIn func2_interrupt;
    InterruptIn func3_interrupt;
    InterruptIn func4_interrupt;
    InterruptIn en_interrupt;
    InterruptIn save_interrupt;

    EventFlags events;

    DigitalOut led1;

    Thread func1_thread;
    Thread func2_thread;
    Thread func3_thread;
    Thread func4_thread;
    Thread main_controller_thread;

    void func1(void);
    void func2(void);
    void func3(void);
    void func4(void);
    void main_controller(void);

    void func1_event(void);
    void func2_event(void);
    void func3_event(void);
    void func4_event(void);
    void en_event(void);
    void save_event(void);

    void led_pulse(int times);
};
