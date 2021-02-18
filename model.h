#ifndef RENDERER_MODEL_H
#define RENDERER_MODEL_H

#include <vector>
#include "geometry.h"
#include "tgaimage.h"

class Model {
private:
    std::vector<Vec3f> vertices;
    std::vector<std::vector<Vec2i>> faces;
    std::vector<Vec2f> tex_vertices;
    TGAImage diffuse;

public:
    Model(const std::string& filename);

    int nbVertices();
    Vec3f vertex(int index);
    int nbFaces();
    std::vector<Vec2i> face(int index);

    int nbTextures();
    Vec2f texture(int index);
    void loadDiffuse(const std::string textureName);
    TGAColor getTexturePoint(Vec2f vec);
};

#endif //RENDERER_MODEL_H
