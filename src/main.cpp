#include "mbed.h"
#include "curves.h"
#include "USBMouse.h"
#include "rtos.h"
DigitalOut myled(LED1);
DigitalIn circlesEn(D8);
DigitalIn fig8En(D9);

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
  myled.write(1);

  circlesEn.mode(PullDown);
  fig8En.mode(PullDown);
  while(1){
     if(circlesEn.read() == 1)
     {
          thread.start(&mouse,goInCircles);
          Thread::wait(osWaitForever);
     }
     else if(fig8En.read() == 1)
     {
          thread.start(&mouse,goInFig8);
          Thread::wait(osWaitForever);
     }
     Thread::wait(1);
     }
}
