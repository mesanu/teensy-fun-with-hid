#include <mbed.h>
#include <stdint.h>
#include "USBMouseKeyboard.h"
#include "curves.h"

#define PULSE_INTERVAL 0.25
#define FUNC_AGGRO_MAX 3

#define EEPROM_SAVE_ADDR 0

#define EEPROM_ENABLED 0
#define EEPROM_MAC_OR_PC 1
#define EEPROM_FUNC1_AGGRO 2
#define EEPROM_FUNC2_AGGRO 4
#define EEPROM_FUNC3_AGGRO 6
#define EEPROM_FUNC4_AGGRO 8

#define EN_EVENT_FLAG 1 << 0
#define FUNC1_EVENT_FLAG 1 << 1
#define FUNC2_EVENT_FLAG 1 << 2
#define FUNC3_EVENT_FLAG 1 << 3
#define FUNC4_EVENT_FLAG 1 << 4
#define SAVE_EVENT_FLAG 1 << 5
#define MAC_OR_PC_EVENT_FLAG 1 << 6

#ifdef FWHID_DEBUG
#define PRINT_SETTINGS_EVENT_FLAG 1 << 7
#define PRINT_SETTINGS_PIN D12
#endif

#define MAIN_EN_SIGNAL 1
#define FUNC_EN_SIGNAL 2

#define THREAD_DELAY 15000

#define F3_ROLL_I 3
#define F3_ROLL_MAX 4

#define F4_CURVE_FREQ 100
#define F4_CURVE_DELAY 5
#define F4_RADIUS 50


class Controller{
  public:
    Controller(PinName led,
              PinName func1,
              PinName func2,
              PinName func3,
              PinName func4,
              PinName enable,
              PinName save_to_eeprom,
              PinName mac_or_pc);
    void start(void);
    void load_from_eeprom();
  private:
    Mutex rand_mutex;
    Mutex eeprom_mutex;

    #ifndef FWHID_DEBUG
    USBMouseKeyboard device;
    #else
    int device;
    #endif
    InterruptIn func1_interrupt;
    InterruptIn func2_interrupt;
    InterruptIn func3_interrupt;
    InterruptIn func4_interrupt;
    InterruptIn en_interrupt;
    InterruptIn save_interrupt;
    InterruptIn mac_or_pc_interrupt;
    #ifdef FWHID_DEBUG
    InterruptIn print_settings_interrupt;
    #endif

    EventFlags events;

    DigitalOut led1;

    Thread func1_thread;
    Thread func2_thread;
    Thread func3_thread;
    Thread func4_thread;
    Thread main_controller_thread;

    Circle circle;
    Fig8 fig8;

    void func1(void);
    void func2(void);
    void func3(void);
    void func4(void);
    void save_to_eeprom(void);
    void main_controller(void);
    int roll(unsigned int max);

    void func1_event(void);
    void func2_event(void);
    void func3_event(void);
    void func4_event(void);
    void en_event(void);
    void save_event(void);
    void mac_or_pc_event(void);
    #ifdef FWHID_DEBUG
    void print_event(void);
    #endif

    void led_pulse(int times);

    void serial_dbg(const char* format, ...);
    void print_settings(void);

    bool enabled_all = false;
    bool mac_or_pc = false;
    char func1_aggro = 0;
    char func2_aggro = 0;
    char func3_aggro = 0;
    char func4_aggro = 0;

};
