#include <cmath>
#include <iostream>
#include "tgaimage.h"
#include "model.h"

constexpr int width     = 500;
constexpr int height    = 500;

const TGAColor white    = { 255, 255, 255, 255 };
const TGAColor red      = { 255, 0, 0, 255 };

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

    for (int i = 0; i < model.nbFaces(); i++) {
        std::vector<int> face = model.face(i);
        for (int j = 0; j < 3; j++) {
            Vec3f v1 = model.vertic(face[j]);
            Vec3f v2 = model.vertic(face[(j+1)%3]);
            drawLine((v1.x+1)*width/2, (v1.y+1)*height/2, (v2.x+1)*width/2, (v2.y+1)*height/2, image, white);
        }
    }

    image.write_tga_file("out.tga");
    return 0;
}