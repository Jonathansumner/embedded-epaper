#ifndef EPAPER_SHAPES_H
#define EPAPER_SHAPES_H

#define PI 3.14159265358979323846

#ifdef __cplusplus
#include <string>
#endif

#ifdef __cplusplus
extern "C" {
#endif

// C-compatible code here

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
class gfx;

class shapes {
    inline static int id = 0;
    inline static shapes* shapeList[100];

public:
    shapes() {
        _id = id++;
        addShape(this);
    }

    static void addShape(shapes* shape) {
        if (id < 100) {
            shapeList[id++] = shape;
        }
    }

    static shapes** getShapeList() {
        return shapeList;
    }

    virtual void draw() {};

    int getId() {
        return _id;
    }

protected:
    int _id;
};

class rectangle : public shapes {
public:
    rectangle(int x, int y, int width, int height, int fill, unsigned char colour);
    void draw() override;

private:
    int x, y, width, height, fill;
    unsigned char colour;
};

class roundedRectangle : public shapes {
public:
    roundedRectangle(int x, int y, int width, int height, int fill, unsigned char colour, int radius);
    void draw() override;

private:
    int x, y, width, height, fill, radius;
    unsigned char colour;
};

class text : public shapes {
public:
    text(int x, int y, std::string content, int f, unsigned char colour);
    void draw() override;

private:
    int x, y;
    std::string content;
    unsigned char colour;
    int f;
};
#endif // __cplusplus

#endif // EPAPER_SHAPES_H