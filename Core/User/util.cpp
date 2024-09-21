//
// Created by joe on 16/09/24.
//

#include <vector>
#include <string>
#include "util.h"
#include "../Inc/stm32f3xx_hal_spi.h"

#define EPD_5IN65F_WIDTH 600
#define EPD_5IN65F_HEIGHT 448


#define RST_PIN     RST_GPIO_Port, RST_Pin
#define DC_PIN      DC_GPIO_Port, DC_Pin
#define PWR_PIN     PWR_GPIO_Port, PWR_Pin
#define CS_PIN      SPI_CS_GPIO_Port, SPI_CS_Pin
#define BUSY_PIN    BUSY_GPIO_Port, BUSY_Pin
#define MOSI_PIN    DIN_GPIO_Port, DIN_Pin
#define SCLK_PIN    SCK_GPIO_Port, SCK_Pin

#define digital_write(_pin, _value) HAL_GPIO_WritePin(_pin, _value == 0? GPIO_PIN_RESET:GPIO_PIN_SET)
#define digital_read(_pin) HAL_GPIO_ReadPin(_pin)

extern SPI_HandleTypeDef hspi1;
extern const uint8_t Font8_Table[];
extern const uint8_t Font12_Table[];
extern const uint8_t Font16_Table[];
extern const uint8_t Font20_Table[];
extern const uint8_t Font24_Table[];


const Font util::fonts[] = {
        {8,  1,8,  Font8_Table},
        {12, 1,12, Font12_Table},
        {16, 2,32, Font16_Table},
//        {20, 2, 40, Font20_Table},
//        {24, 3, 72, Font24_Table}
};

Font util::getFont(int size) {
    for (const Font &font: fonts) {
        if (font.sizeName == size) {
            return font;
        }
    }
    printf("Font size not found, using default font size 8\r\n");
    return fonts[0];
}


void spiWriteByte(unsigned char value) {
    HAL_SPI_Transmit(&hspi1, &value, 1, 1000);
}

void util::reset() {
    digital_write(RST_PIN, 1);
    delay_ms(200);
    digital_write(RST_PIN, 0);
    delay_ms(1);
    digital_write(RST_PIN, 1);
    delay_ms(200);
}

void util::sendData(unsigned char Data) {
    digital_write(DC_PIN, 1);
    digital_write(CS_PIN, 0);
    spiWriteByte(Data);
    digital_write(CS_PIN, 1);
}

int util::moduleInit() {
    digital_write(DC_PIN, 0);
    digital_write(CS_PIN, 0);
    digital_write(PWR_PIN, 1);
    digital_write(RST_PIN, 1);
    return 0;
}

void util::moduleExit() {
    digital_write(DC_PIN, 0);
    digital_write(CS_PIN, 0);

    //close 5V
    digital_write(PWR_PIN, 0);
    digital_write(RST_PIN, 0);
}


std::pair<std::vector<uint8_t>, int> util::getFontData(const std::string &text, int fontSize) {
    Font f = getFont(fontSize);

    std::vector<uint8_t> fontData;
    auto fontTable = f.table;
    auto bytesPerCharacter = f.bytesPerCharacter;
    int asciiOffset = 32; // Assuming the table starts with the space character
    int charCount = 0; // Counter for the number of characters processed

    for (char character: text) {
        int startIndex = (character - asciiOffset) * bytesPerCharacter;
        fontData.insert(fontData.end(), fontTable + startIndex, fontTable + startIndex + bytesPerCharacter);
        charCount++; // Increment the character count
    }

    return {fontData, f.columns}; // Return both fontData and charCount
}



void util::sendCmd(unsigned char Reg) {
    digital_write(DC_PIN, 0);
    digital_write(CS_PIN, 0);
    spiWriteByte(Reg);
    digital_write(CS_PIN, 1);
}

void util::busyHigh()// If BUSYN=0 then waiting
{
    while (!(digital_read(BUSY_PIN)));
}

void util::busyLow()// If BUSYN=1 then waiting
{
    while (digital_read(BUSY_PIN));
}


void util::sleep() {
    delay_ms(100);
    sendCmd(0x07);
    sendData(0xA5);
    delay_ms(100);
    digital_write(RST_PIN, 0); // Reset
}


void util::init() {
    util::reset();
    busyHigh();
    sendCmd(0x00);
    sendData(0xEF);
    sendData(0x08);
    sendCmd(0x01);
    sendData(0x37);
    sendData(0x00);
    sendData(0x23);
    sendData(0x23);
    sendCmd(0x03);
    sendData(0x00);
    sendCmd(0x06);
    sendData(0xC7);
    sendData(0xC7);
    sendData(0x1D);
    sendCmd(0x30);
    sendData(0x3C);
    sendCmd(0x41);
    sendData(0x00);
    sendCmd(0x50);
    sendData(0x37);
    sendCmd(0x60);
    sendData(0x22);
    sendCmd(0x61);
    sendData(0x02);
    sendData(0x58);
    sendData(0x01);
    sendData(0xC0);
    sendCmd(0xE3);
    sendData(0xAA);

    delay_ms(100);
    sendCmd(0x50);
    sendData(0x37);
}

void util::delay_ms(int i) {
    HAL_Delay(i);
}
