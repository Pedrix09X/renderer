#ifndef RENDERER_MODEL_H
#define RENDERER_MODEL_H

#include <vector>
#include "geometry.h"

class Model {
private:
    std::vector<Vec3f> vertics;
    std::vector<std::vector<int>> faces;

public:
    Model(const char *filename);
    int nbVertics();
    Vec3f vertic(int index);
    int nbFaces();
    std::vector<int> face(int index);
};

#endif //RENDERER_MODEL_H
