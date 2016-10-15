#include "mbed.h"
#include "curves.h"
#include "USBMouse.h"
#include "rtos.h"
DigitalOut myled(LED1);

void goInCircles(USBMouse *mouseP){
  Circle circle(60,5);
  myled=1;
  while(1){
    XYCoords coords=circle.deltaStep();
    ((USBMouse*)mouseP)->move(coords.x, coords.y);
    Thread::wait(10);
  }
}

int main() {
  USBMouse mouse;
  Thread thread;
  thread.start(&mouse,goInCircles);
  Thread::wait(osWaitForever);
}
