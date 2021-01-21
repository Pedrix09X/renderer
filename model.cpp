#include "model.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

Model::Model(const char *filename, int width, int height): vertics(), faces() {
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
            vec.x = (vec.x+1)*width/2;      // Normalisation des coordonnees
            vec.y = (vec.y+1)*height/2;
            vertics.push_back(vec);
        } else if (startLine.compare("f") == 0) {
            vector<int> face;
            for (int i = 0; i < 3; i++) {
                int vert, itmp;
                char tmp;
                //     nb      /       nb      /       nb
                iss >> vert >> tmp >> itmp >> tmp >> itmp;
                face.push_back(vert-1); // On ajoute l'index-1 car la liste commence a 1
            }
            faces.push_back(face);
        }
    }

    model.close();
}

int Model::nbVertics() {
    return vertics.size();
}

Vec3f Model::vertic(int index) {
    if (index < nbVertics()) {
        return vertics[index];
    }
}

int Model::nbFaces() {
    return faces.size();
}

vector<int> Model::face(int index) {
    if (index < nbFaces()) {
        return faces[index];
    }
}