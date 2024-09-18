#ifndef EPAPER_UTIL_H
#define EPAPER_UTIL_H

#include "main.h"
#include "../Inc/stm32f3xx_hal.h"
#include "../Inc/stm32f3xx_hal_gpio.h"

#ifdef __cplusplus
#include <vector>
#include <string>

class util {
public:
    static std::vector<uint8_t> getFontData(const std::string &text);

    static void init();

    static void reset();

    static void sleep();

    static void sendData(unsigned char Data);

    static int moduleInit();

    static void moduleExit();

    static void sendCmd(unsigned char Reg);

    static void busyHigh();

    static void busyLow();

    static void delay_ms(int i);
};
#endif // __cplusplus

#endif // EPAPER_UTIL_H