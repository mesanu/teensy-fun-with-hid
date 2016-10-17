#include "mbed.h"
#include "curves.h"
#include "USBMouse.h"
#include "rtos.h"
DigitalOut myled(LED1);

void goInCircles(USBMouse *mouseP){
  Circle circle(60,5);
  while(1){
    XYCoords coords=circle.deltaStep();
    ((USBMouse*)mouseP)->move(coords.x, coords.y);
    Thread::wait(10);
  }
}

void goInFig8(USBMouse *mouseP){
  Fig8 fig8(60,5);
  while(1){
    XYCoords coords=fig8.deltaStep();
    ((USBMouse*)mouseP)->move(coords.x, coords.y);
    Thread::wait(10);
  }
}

int main() {
  USBMouse mouse;
  Thread thread;
  thread.start(&mouse,goInFig8);
  Thread::wait(osWaitForever);
}
