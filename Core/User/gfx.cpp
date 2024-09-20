#include "gfx.h"

#define BLACK   (unsigned char)0x0  // 000
#define WHITE   (unsigned char)0x1  // 001
#define GREEN   (unsigned char)0x2  // 010
#define BLUE    (unsigned char)0x3  // 011
#define RED     (unsigned char)0x4  // 100
#define YELLOW  (unsigned char)0x5  // 101
#define ORANGE  (unsigned char)0x6  // 110
#define CLEAN   (unsigned char)0x7  // 111   unavailable  Afterimage

#define RST_PIN     RST_GPIO_Port, RST_Pin
#define DC_PIN      DC_GPIO_Port, DC_Pin
#define PWR_PIN     PWR_GPIO_Port, PWR_Pin
#define CS_PIN      SPI_CS_GPIO_Port, SPI_CS_Pin
#define BUSY_PIN    BUSY_GPIO_Port, BUSY_Pin
#define MOSI_PIN    DIN_GPIO_Port, DIN_Pin
#define SCLK_PIN    SCK_GPIO_Port, SCK_Pin

#define digital_write(_pin, _value) HAL_GPIO_WritePin(_pin, _value == 0? GPIO_PIN_RESET:GPIO_PIN_SET)
#define digital_read(_pin) HAL_GPIO_ReadPin(_pin)

void gfx::allocateBuffer() {
    buffer = (uchar **) malloc(height * sizeof(uchar *));
    for (int i = 0; i < height; ++i) {
        buffer[i] = (uchar *) malloc(width * sizeof(uchar));
    }
}

void gfx::freeBuffer() {
    for (int i = 0; i < height; ++i) {
        free(buffer[i]);
    }
    free(buffer);
}

// Define the gfx class methods
void gfx::init(int h, int w) {
    height = h;
    width = w;
    allocateBuffer();
}

void gfx::clear() {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            buffer[i][j] = 0x11;
        }
    }
}

void gfx::fill(uchar value) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            buffer[i][j] = value;
        }
    }
}

void gfx::setPixel(int x, int y, uchar value) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        buffer[y][x] = value;
    }
}

void gfx::setResolution(int w, int h) {
    util::sendCmd(0x61); // Set Resolution setting
    util::sendData((w >> 8) & 0xFF); // Width high byte
    util::sendData(w & 0xFF);        // Width low byte
    util::sendData((h >> 8) & 0xFF); // Height high byte
    util::sendData(h & 0xFF);        // Height low byte
}

void gfx::displayFrame(unsigned char **image) {
    if (image == nullptr) {
        printf("Image is null\n");
        return;
    }
    setResolution(width, height);
    util::sendCmd(0x10);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width/2; j++) {
            util::sendData(image[i][j]);
        }
    }
    util::sendCmd(0x04);
    util::busyHigh();
    util::sendCmd(0x12);
    util::busyHigh();
    util::sendCmd(0x02);
    util::busyLow();
    util::delay_ms(500);
}


uchar **gfx::readFrame() {
    auto **condensedBuffer = (uchar **) malloc(height * sizeof(uchar *));
    for (int i = 0; i < height; ++i) {
        condensedBuffer[i] = (uchar *) malloc((width / 2) * sizeof(uchar));
    }

    // Fill the condensed buffer with the condensed pixel data
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; j += 2) {
            uchar leftPixel = buffer[i][j] & 0x0F;
            uchar rightPixel = buffer[i][j + 1] & 0x0F;
            condensedBuffer[i][j / 2] = (leftPixel << 4) | rightPixel;
        }
    }

    return condensedBuffer;
}



// Define the C-compatible functions
extern "C" {
void gfx_init(int h, int w) {
    util::init();
//    gfx::init(h, w);
}

void gfx_clear() {
    digital_write(RST_PIN, 1);
    util::delay_ms(200);
    digital_write(RST_PIN, 0);
    util::delay_ms(1);
    digital_write(RST_PIN, 1);
    util::delay_ms(200);
    util::busyHigh();
    util::sendCmd(0x00);
    util::sendData(0xEF);
    util::sendData(0x08);
    util::sendCmd(0x01);
    util::sendData(0x37);
    util::sendData(0x00);
    util::sendData(0x23);
    util::sendData(0x23);
    util::sendCmd(0x03);
    util::sendData(0x00);
    util::sendCmd(0x06);
    util::sendData(0xC7);
    util::sendData(0xC7);
    util::sendData(0x1D);
    util::sendCmd(0x30);
    util::sendData(0x3C);
    util::sendCmd(0x41);
    util::sendData(0x00);
    util::sendCmd(0x50);
    util::sendData(0x37);
    util::sendCmd(0x60);
    util::sendData(0x22);
    util::sendCmd(0x61);
    util::sendData(0x02);
    util::sendData(0x58);
    util::sendData(0x01);
    util::sendData(0xC0);
    util::sendCmd(0xE3);
    util::sendData(0xAA);
    util::delay_ms(100);
    util::sendCmd(0x50);
    util::sendData(0x37);
    gfx::setResolution(600, 448);
    util::sendCmd(0x10);
    for (int i = 0; i < 600 / 2; i++) {
        for (int j = 0; j < 448; j++)
            util::sendData(0x11);
    }
    util::sendCmd(0x04);//0x04
    util::busyHigh();
    util::sendCmd(0x12);//0x12
    util::busyHigh();
    util::sendCmd(0x02);  //0x02
    util::busyLow();
    util::delay_ms(1000);
}

void gfx_fill(uchar value) {
    gfx::fill(value);
}

void displayFrame() {
    uchar **frame = gfx::readFrame();
    gfx::displayFrame(frame);

    // Free the allocated memory for the condensed buffer
    for (int i = 0; i < gfx::height; ++i) {
        free(frame[i]);
    }
    free(frame);
}

void test() {
//    gfx_fill(0x77);
    rectangle rect = rectangle(5, 5, 20, 20, 3, 0x33);
//    text txt = text(5, 5, "hi", 8, 0x22);
}

}