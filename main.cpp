#include <cmath>
#include <iostream>
#include <limits>
#include "tgaimage.h"
#include "model.h"

constexpr int width     = 1000;
constexpr int height    = 1000;

Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P) {
    Vec3f s[2];
    for (int i=2; i--; ) {
        s[i][0] = C[i]-A[i];
        s[i][1] = B[i]-A[i];
        s[i][2] = A[i]-P[i];
    }
    Vec3f u = cross(s[0], s[1]);
    if (std::abs(u[2])>1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
        return Vec3f(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
    return Vec3f(-1,1,1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
}

void drawTriangle(Vec3f *pts, float *zbuffer, TGAImage &image, TGAColor color) {
    Vec2f bboxmin( std::numeric_limits<float>::max(),  std::numeric_limits<float>::max());
    Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    Vec2f clamp(image.get_width()-1, image.get_height()-1);
    for (int i=0; i<3; i++) {
        for (int j=0; j<2; j++) {
            bboxmin[j] = std::max(0.f,      std::min(bboxmin[j], pts[i][j]));
            bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
        }
    }
    Vec3f P;
    for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) {
        for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) {
            Vec3f bc_screen = barycentric(pts[0], pts[1], pts[2], P);
            if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue;
            P.z = 0;
            for (int i=0; i<3; i++) P.z += pts[i][2]*bc_screen[i];
            if (zbuffer[int(P.x+P.y*width)]<P.z) {
                zbuffer[int(P.x+P.y*width)] = P.z;
                image.set(P.x, P.y, color);
            }
        }
    }
}

Vec3f translateCoords(Vec3f vec) {
    Vec3f v;
    v.x = int((vec.x+1)*width/2);
    v.y = int((vec.y+1)*height/2);
    v.z = int(vec.z);
    return v;
}

int main() {
    Model model("../obj/african_head.obj");
    TGAImage image(width, height, TGAImage::RGB);

    float zbuffer[width*height];
    for (int i = 0; i < width*height; i++) {
        zbuffer[i] = std::numeric_limits<float>::min();
    }

    Vec3f light(0, 0, -1 );
    for (int i = 0; i < model.nbFaces(); i++) {
        std::vector<Vec2i> face = model.face(i);

        Vec3f v[3];
        Vec2f tex[3];
        for (int i = 0; i < 3; i++) {
            v[i] = model.vertex(face[i].x);
            tex[i] = model.texture(face[i].y);
        }

        Vec3f n = cross((v[2]-v[0]), (v[1]-v[0]));
        n.normalize();

        float intensity = n*light;
        if (intensity > 0) {
            TGAColor color = {intensity*255, intensity*255, intensity*255};
            for (int i = 0; i < 3; i++) {
                v[i] = translateCoords(v[i]);
                //color[i] *= intensity;
            }
            drawTriangle(v, zbuffer, image, color);
        }
    }

    image.write_tga_file("out.tga");
    return 0;
}