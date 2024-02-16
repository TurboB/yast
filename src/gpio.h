
#ifndef _HARDWARE_H_
        #define _HARDWARE_H_

void yaSleep(double sleepsecs);
int yaDigitalWrite(int gpio_num, int level);
int DisplaylgMode(int);
int yaGpioGetMode(int, int);

#endif
