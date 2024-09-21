#include "shapes.h"
#include <cstdlib> // for malloc and realloc
#include <utility>
#include <valarray>
#include "gfx.h" // Include the gfx header file
#include "util.h" // Include the util header file

text::text(int x, int y, std::string content, int f, unsigned char colour) {
    this->x = x, this->y = y, this->content = std::move(content), this->f = f, this->colour = colour;
}

rectangle::rectangle(int x, int y, int width, int height, int fill, unsigned char colour) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->fill = fill;
    this->colour = colour;
}

void rectangle::draw() {
    shapes::draw();
    printf("Rectangle values: x=%d, y=%d, width=%d, height=%d, fill=%d, colour=%d\r\n", this->x, this->y, this->width,
           this->height, this->fill, this->colour);
    if (this->fill > 0) {
        printf("Drawing unfilled rectangle\r\n");
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
        printf("Drawing filled rectangle\r\n");
        for (int i = 0; i < this->width; ++i) {
            for (int j = 0; j < this->height; ++j) {
                gfx::setPixel(this->x + i, this->y + j, this->colour);
            }
        }
        printf("Done drawing filled rectangle\r\n");
    }
}

void text::draw() {
    shapes::draw();
    auto pair = util::getFontData(this->content, this->f);
    auto fontData = pair.first;
    int columns = pair.second; // Represents the number of columns
    int charHeight = this->f; // Height of the font in pixels
    int charWidth = columns * 8; // Width of each character in bits
    int xOffset = this->x;
    int yOffset = this->y;

    for (size_t i = 0; i < fontData.size(); i += (charHeight * columns)) { // Adjust iteration to full character height and width
        int leftmost = -1;
        int rightmost = -1;

        // Check for space character
        bool isSpace = true;
        for (int spaceCheck = 0; spaceCheck < charHeight * columns; spaceCheck++) {
            if (fontData[i + spaceCheck] != 0x00) {
                isSpace = false;
                break;
            }
        }
        if (isSpace) {
            xOffset += 3; // Move cursor for space
            continue;
        }

        // Scan through each row to determine leftmost and rightmost active pixel
        for (int row = 0; row < charHeight; ++row) {
            for (int col = 0; col < charWidth; ++col) { // Scan across all columns
                int byteIndex = i + row * columns + (col / 8); // Calculate the byte index
                if (byteIndex >= fontData.size()) break; // Prevent out-of-bounds access

                // Check the specific bit in the byte
                if (fontData[byteIndex] & (1 << (7 - (col % 8)))) { // Pixel is set
                    if (leftmost == -1 || col < leftmost) {
                        leftmost = col;
                    }
                    if (col > rightmost) {
                        rightmost = col;
                    }
                }
            }
        }

        // Calculate character width based on leftmost and rightmost pixel positions
        int actualCharWidth = (leftmost == -1 || rightmost == -1) ? 0 : (rightmost - leftmost + 1);

        // Draw the character with dynamic width
        if (actualCharWidth > 0) {
            for (int row = 0; row < charHeight; ++row) {
                for (int col = leftmost; col <= rightmost; ++col) {
                    int byteIndex = i + row * columns + (col / 8); // Calculate the byte index
                    if (byteIndex >= fontData.size()) break; // Prevent out-of-bounds access

                    if (fontData[byteIndex] & (1 << (7 - (col % 8)))) {
                        gfx::setPixel(xOffset + (col - leftmost), yOffset + row, this->colour);
                    }
                }
            }
        }

        // Move xOffset to the next character position based on the actual character width plus 1 pixel space
        xOffset += actualCharWidth + 1; // Add 1 pixel space between characters
    }
}




roundedRectangle::roundedRectangle(int x, int y, int width, int height, int fill, unsigned char colour, int radius) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->fill = fill;
    this->colour = colour;
    this->radius = radius;
}

void roundedRectangle::draw() {
    printf("Rounded Rectangle values: x=%d, y=%d, width=%d, height=%d, fill=%d, radius=%d, colour=%d\r\n",
           this->x, this->y, this->width, this->height, this->fill, this->radius, this->colour);

    // Draw the straight sides first (without rounded corners)
    for (int i = this->radius; i < this->width - this->radius; ++i) {
        for (int j = 0; j < this->fill; ++j) {
            gfx::setPixel(this->x + i, this->y + j, this->colour); // Top border
            gfx::setPixel(this->x + i, this->y + this->height - 1 - j, this->colour); // Bottom border
        }
    }

    for (int i = this->radius; i < this->height - this->radius; ++i) {
        for (int j = 0; j < this->fill; ++j) {
            gfx::setPixel(this->x + j, this->y + i, this->colour); // Left border
            gfx::setPixel(this->x + this->width - 1 - j, this->y + i, this->colour); // Right border
        }
    }

    // Draw the rounded corners using a Bresenham circle algorithm for smoother curves
    if (this->radius > 0) {
        int x = this->radius;
        int y = 0;
        int radiusError = 1 - x;

        while (x >= y) {
            // Top-left corner
            gfx::setPixel(this->x + this->radius - x, this->y + this->radius - y, this->colour);
            gfx::setPixel(this->x + this->radius - y, this->y + this->radius - x, this->colour);

            // Top-right corner
            gfx::setPixel(this->x + this->width - this->radius + x - 1, this->y + this->radius - y, this->colour);
            gfx::setPixel(this->x + this->width - this->radius + y - 1, this->y + this->radius - x, this->colour);

            // Bottom-left corner
            gfx::setPixel(this->x + this->radius - x, this->y + this->height - this->radius + y - 1, this->colour);
            gfx::setPixel(this->x + this->radius - y, this->y + this->height - this->radius + x - 1, this->colour);

            // Bottom-right corner
            gfx::setPixel(this->x + this->width - this->radius + x - 1, this->y + this->height - this->radius + y - 1, this->colour);
            gfx::setPixel(this->x + this->width - this->radius + y - 1, this->y + this->height - this->radius + x - 1, this->colour);

            y++;
            if (radiusError < 0) {
                radiusError += 2 * y + 1;
            } else {
                x--;
                radiusError += 2 * (y - x + 1);
            }
        }
    }

    // Fill the rectangle if needed
    if (this->fill == 0) {
        printf("Filling the rounded rectangle\r\n");
        for (int i = this->radius; i < this->width - this->radius; ++i) {
            for (int j = this->radius; j < this->height - this->radius; ++j) {
                gfx::setPixel(this->x + i, this->y + j, this->colour);
            }
        }
    }

    printf("Done drawing rounded rectangle\r\n");
}
