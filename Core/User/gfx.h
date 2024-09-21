#ifndef EPAPER_GFX_H
#define EPAPER_GFX_H

#include "shapes.h"
#include "util.h"

#define uchar unsigned char

#ifdef __cplusplus
class gfx {
public:
    static void init(int h, int w);
    static void clear();
    static void fill(uchar value);
    static void setPixel(int x, int y, uchar value);
    static void displayFrame(unsigned char **image);
    static uchar** readFrame();
    static void setResolution(int w, int h);

    inline static int height;
    inline static int width;
private:
    inline static uchar** buffer;

    static void allocateBuffer();


    static void freeBuffer();
};
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

void draw();
void test();
void gfx_init(int h, int w);
void gfx_fun();
void gfx_clear();
void gfx_fill(uchar value);
void displayFrame();

#ifdef __cplusplus
}
#endif

#endif // EPAPER_GFX_H