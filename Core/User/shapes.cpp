#include "shapes.h"
#include <cstdlib> // for malloc and realloc
#include <utility>
#include "gfx.h" // Include the gfx header file
#include "util.h" // Include the util header file

text::text(int x, int y, std::string content, int f, unsigned char colour) {
    this->x = x, this->y = y, this->content = std::move(content), this->f = f, this->colour = colour;
}

rectangle::rectangle(int x, int y, int width, int height, int fill, unsigned char colour) {
//    this->x = x, this->y = y, this->width = width, this->height = height, this->fill = fill, this->colour = colour;
}

void rectangle::draw() {
    shapes::draw();
    if (this->fill > 0) {
        // Draw a dynamic-width outline
        for (int i = 0; i < this->width; ++i) {
            for (int j = 0; j < this->fill; ++j) {
                gfx::setPixel(this->x + i, this->y + j, this->colour); // Top border
                gfx::setPixel(this->x + i, this->y + this->height - 1 - j, this->colour); // Bottom border
            }
        }
        for (int i = 0; i < this->height; ++i) {
            for (int j = 0; j < this->fill; ++j) {
                gfx::setPixel(this->x + j, this->y + i, this->colour); // Left border
                gfx::setPixel(this->x + this->width - 1 - j, this->y + i, this->colour); // Right border
            }
        }
    } else {
        // Draw filled rectangle
        for (int i = 0; i < this->width; ++i) {
            for (int j = 0; j < this->height; ++j) {
                gfx::setPixel(this->x + i, this->y + j, this->colour);
            }
        }
    }
}

void text::draw() {
    shapes::draw();
    std::vector<uint8_t> fontData = util::getFontData(this->content);
    int charWidth = 8;
    int charHeight = 8;
    int xOffset = this->x;
    int yOffset = this->y;

    for (size_t i = 0; i < fontData.size(); i += charHeight) {
        for (int row = 0; row < charHeight; ++row) {
            for (int col = 0; col < charWidth; ++col) {
                if (fontData[i + row] & (1 << (7 - col))) {
                    gfx::setPixel(xOffset + col, yOffset + row, this->colour);
                }
            }
        }
        xOffset += charWidth;
    }
}