#include <cmath>
#include <iostream>
#include "tgaimage.h"
#include "model.h"

constexpr int width     = 1000;
constexpr int height    = 1000;

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

void drawTriangle(Vec3f v1, Vec3f v2, Vec3f v3, TGAImage &image, TGAColor color) {
    if (v1.y==v2.y && v1.y==v3.y) return;
    if (v1.y>v2.y) std::swap(v1, v2);
    if (v1.y>v3.y) std::swap(v1, v3);
    if (v2.y>v3.y) std::swap(v2, v3);

    int total_height = v3.y-v1.y;
    for (int i=0; i<total_height; i++) {
        bool second_half = i>v2.y-v1.y || v2.y==v1.y;
        int segment_height = second_half ? v3.y-v2.y : v2.y-v1.y;
        float alpha = (float)i/total_height;
        float beta  = (float)(i-(second_half ? v2.y-v1.y : 0))/segment_height; // be careful: with above conditions no division by zero here
        Vec3f A =               v1 + (v3-v1)*alpha;
        Vec3f B = second_half ? v2 + (v3-v2)*beta : v1 + (v2-v1)*beta;
        if (A.x>B.x) std::swap(A, B);
        for (int j=A.x; j<=B.x; j++) {
            image.set(j, v1.y+i, color); // attention, due to int casts v1.y+i != A.y
        }
    }

    /*
    drawLine(v1.x, v1.y, v2.x, v2.y, image, color);
    drawLine(v2.x, v2.y, v3.x, v3.y, image, color);
    drawLine(v3.x, v3.y, v1.x, v1.y, image, color);
     */
}

Vec3f translateCoords(Vec3f vec) {
    vec.x = (vec.x+1)*width/2;
    vec.y = (vec.y+1)*height/2;
    return vec;
}

int main() {
    Model model("../obj/african_head.obj", width, height);
    TGAImage image(width, height, TGAImage::RGB);

    Vec3f light(0, 0, -1 );
    for (int i = 0; i < model.nbFaces(); i++) {
        std::vector<int> face = model.face(i);

        Vec3f v1 = model.vertic(face[0]);
        Vec3f v2 = model.vertic(face[1]);
        Vec3f v3 = model.vertic(face[2]);

        Vec3f n = (v3-v1)^(v2-v1);
        n.normalize();

        float intensity = n*light;
        if (intensity > 0) {
            TGAColor color = {intensity * 255, intensity * 255, intensity * 255, 255};
            //TGAColor color = { static_cast<uint8_t>(rand()%255), static_cast<uint8_t>(rand()%255), static_cast<uint8_t>(rand()%255) };
            v1 = translateCoords(v1);
            v2 = translateCoords(v2);
            v3 = translateCoords(v3);
            drawTriangle(v1, v2, v3, image, color);
        }
    }

    image.write_tga_file("out.tga");
    return 0;
}