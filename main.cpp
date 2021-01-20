#include <cmath>
#include <iostream>
#include "tgaimage.h"
#include "model.h"

constexpr int width     = 300;
constexpr int height    = 300;

const TGAColor white = { 255, 255, 255, 255};

void drawLine(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if (std::abs(x0-x1)<std::abs(y0-y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0>x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1-x0;
    int dy = y1-y0;
    int derror2 = std::abs(dy)*2;
    int error2 = 0;
    int y = y0;
    for (int x=x0; x<=x1; x++) {
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1>y0?1:-1);
            error2 -= dx*2;
        }
    }
}

int main() {
    Model model("../obj/african_head.obj");
    TGAImage image(width, height, TGAImage::RGB);

    for (int i = 0; i < model.nbVertics(); i++) {
        Vec3f vec = model.vertic(i);
        image.set((vec.x+1.)*width/2, (vec.y+1.)*height/2, white);
    }

    image.write_tga_file("out.tga");
    return 0;
}