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

void gfx::allocateBuffer()
{
    buffer = (uchar**)malloc(height * sizeof(uchar*));
    for (int i = 0; i < height; ++i)
    {
        buffer[i] = (uchar*)malloc(width * sizeof(uchar));
    }

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            buffer[i][j] = 0x11;
        }
    }
}

void gfx::freeBuffer()
{
    for (int i = 0; i < height; ++i)
    {
        free(buffer[i]);
    }
    free(buffer);
}

// Define the gfx class methods
void gfx::init(int h, int w)
{
    height = h;
    width = w;
    allocateBuffer();
}

void gfx::clear()
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            buffer[i][j] = 0x11;
        }
    }
}

void gfx::fill(uchar value)
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            buffer[i][j] = value;
        }
    }
}

void gfx::setPixel(int x, int y, uchar value)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        buffer[y][x] = value;
    }
}

void gfx::setResolution(int w, int h)
{
    util::sendCmd(0x61); // Set Resolution setting
    util::sendData((w >> 8) & 0xFF); // Width high byte
    util::sendData(w & 0xFF); // Width low byte
    util::sendData((h >> 8) & 0xFF); // Height high byte
    util::sendData(h & 0xFF); // Height low byte
}

void gfx::displayFrame(unsigned char** image)
{
    if (image == nullptr)
    {
        printf("Image is null\r\n");
        return;
    }
    setResolution(width, height);
    util::sendCmd(0x10);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
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


uchar** gfx::readFrame()
{
    auto** condensedBuffer = (uchar**)malloc(height * sizeof(uchar*));
    for (int i = 0; i < height; ++i)
    {
        condensedBuffer[i] = (uchar*)malloc((width / 2) * sizeof(uchar));
    }
    printf("Height: %i, Width: %i\r\n", height, width);

    // Fill the condensed buffer with the condensed pixel data
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; j += 2)
        {
            uchar leftPixel = buffer[i][j] & 0x0F;
            uchar rightPixel = buffer[i][j + 1] & 0x0F;
            condensedBuffer[i][j / 2] = (leftPixel << 4) | rightPixel;
        }
    }

    //print the buffer
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width / 2; ++j)
        {
            printf("%x ", condensedBuffer[i][j]);
        }
        printf("\r\n");
    }

    return condensedBuffer;
}


// Define the C-compatible functions
extern "C" {
void gfx_init(int h, int w)
{
    util::init();
    gfx::init(h, w);
}

void gfx_clear()
{
    auto w = 600, h = 448;
    gfx::setResolution(w, h);
    util::sendCmd(0x10);
    for (int i = 0; i < w / 2; i++)
    {
        for (int j = 0; j < h; j++)
            util::sendData(0x11);
    }
    util::sendCmd(0x04); //0x04
    util::busyHigh();
    util::sendCmd(0x12); //0x12
    util::busyHigh();
    util::sendCmd(0x02); //0x02
    util::busyLow();
    util::delay_ms(1000);
}

void gfx_fill(uchar value)
{
    gfx::fill(value);
}

void displayFrame()
{
    printf("reading frame\r\n");
    uchar** frame = gfx::readFrame();
    printf("displaying frame\r\n");
    gfx::displayFrame(frame);

    printf("Freeing memory\r\n");
    // Free the allocated memory for the condensed buffer
    for (int i = 0; i < gfx::height; ++i)
    {
        free(frame[i]);
    }
    free(frame);
}

void test()
{
    //    gfx_fill(0x77);
    roundedRectangle rect = roundedRectangle(0, 0, 600, 448, 6, 0x66, 16);
    rect.draw();
    text txt0 = text(0, 2, "J", 8, 0x33);
    txt0.draw();
    text txt1 = text(10, 2, "J", 12, 0x33);
    txt1.draw();
    text txt2 = text(22, 2, "J", 16, 0x33);
    txt2.draw();
}

void gfx_fun()
{
    auto w = 600, h = 448;
    gfx::setResolution(w, h);
    util::sendCmd(0x10);

    const int sections = 8;
    int sectionHeight = h / sections;
    uchar values[sections] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};

    for (int section = 0; section < sections; ++section)
    {
        for (int i = 0; i < w / 2; ++i)
        {
            for (int j = section * sectionHeight; j < (section + 1) * sectionHeight; ++j)
            {
                util::sendData(values[section]);
            }
        }
    }

    util::sendCmd(0x04);
    util::busyHigh();
    util::sendCmd(0x12);
    util::busyHigh();
    util::sendCmd(0x02);
    util::busyLow();
    util::delay_ms(1000);
}

//void draw() {
//    auto shapes = shapes::getShapeList();
//    for (int i = 0; i < 100; ++i) {
//        if (shapes[i] != nullptr) {
//            printf("Drawing shape %i\r\n", i);
//            shapes[i]->draw();
//        }
//    }
//}
}
