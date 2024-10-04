#ifndef EPAPER_UTIL_H
#define EPAPER_UTIL_H

#include "main.h"
#include "../Inc/stm32h7xx_hal.h"
#include "../Inc/stm32h7xx_hal_gpio.h"

#ifdef __cplusplus
#include <vector>
#include <string>


struct Font {
    int sizeName;
    int columns;
    int bytesPerCharacter;
    const uint8_t *table;
};

class util {
public:
    static std::pair<std::vector<uint8_t>, int> getFontData(const std::string &text, int fontSize);

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

    static const Font fonts[5];

    static Font getFont(int size);
};
#endif // __cplusplus

#endif // EPAPER_UTIL_H