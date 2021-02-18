#include "model.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

Model::Model(const string filename): vertices(), faces(), tex_vertices(), diffuse(){
    ifstream model;
    model.open(filename, std::ifstream::in);

    if (!model) {
        return;
    }

    string line;
    while (std::getline(model, line)) {
        istringstream iss(line); // Decoupe line par les espaces
        string startLine;
        iss >> startLine;
        if (startLine.compare("v") == 0) {
            Vec3f vec;
            iss >> vec.x;
            iss >> vec.y;
            iss >> vec.z;
            vertices.push_back(vec);
        } else if (startLine.compare("f") == 0) {
            std::vector<Vec2i> face;
            for (int i = 0; i < 3; i++) {
                Vec2i vec;
                int vert, tex, itmp;
                char tmp;
//                      nb      /      nb     /      nb
                iss >> vert >> tmp >> tex >> tmp >> itmp;
                vec.x = vert -1;    // Pour les points
                vec.y = tex -1;     // Pour les textures
                face.push_back(vec);
            }
            faces.push_back(face);
        } else if (startLine.compare("vt") == 0) {
            Vec2f vec;
            iss >> vec.x;
            iss >> vec.y;
            tex_vertices.push_back(vec);
        }
    }

    diffuse.read_tga_file("../obj/african_head_diffuse.tga");
    model.close();
}

int Model::nbVertices() {
    return vertices.size();
}

Vec3f Model::vertex(int index) {
    if (index < nbVertices() && index >= 0) {
        return vertices[index];
    }
}

int Model::nbFaces() {
    return faces.size();
}

std::vector<Vec2i> Model::face(int index) {
    if (index < nbFaces() && index >= 0) {
        return faces[index];
    }
}

int Model::nbTextures() {
    return tex_vertices.size();
}

Vec2f Model::texture(int index) {
    if (index < nbTextures() && index >= 0) {
        return tex_vertices[index];
    }
}

TGAColor Model::getTexturePoint(Vec2f vec) {
    return diffuse.get(vec.x * diffuse.get_width(), vec.y * diffuse.get_height());
}