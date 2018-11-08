#include "mbed.h"
#include "USBMouseKeyboard.h"

#define PULSE_INTERVAL 0.5

#define EN_EVENT_FLAG 1 << 0
#define FUNC1_EVENT_FLAG 1 << 1
#define FUNC2_EVENT_FLAG 1 << 2
#define FUNC3_EVENT_FLAG 1 << 3
#define FUNC4_EVENT_FLAG 1 << 4

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
    InterruptIn function1;
    InterruptIn function2;
    InterruptIn function3;
    InterruptIn function4;
    InterruptIn en_config;
    InterruptIn save_to_eeprom;

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

    void led_pulse(int times);
};
