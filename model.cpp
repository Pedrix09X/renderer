#include "model.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

Model::Model(const char *filename): vertics() {
    ifstream model;
    model.open(filename, std::ifstream::in);

    if (!model) {
        return;
    }

    string line;
    while (std::getline(model, line)) {
        istringstream iss(line);
        string startLine;
        iss >> startLine;
        if (startLine.compare("v") == 0) {
            Vec3f vec;
            iss >> vec.x;
            iss >> vec.y;
            iss >> vec.z;

            vertics.push_back(vec);
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