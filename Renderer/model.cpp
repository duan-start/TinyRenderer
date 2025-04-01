#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) : verts_(), faces_() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i=0;i<3;i++) iss >> v.raw[i];
            verts_.push_back(v);
        }
        //好一个教程，真的就是全得靠我手写了
        //查找纹理的坐标
        else if (!line.compare(0, 2, "vt")) {
            iss >> trash >> trash;//把vt扔到垃圾桶
            Vec2f vt;
            iss >> vt.u >> vt.v;//这里是iss接下来的两个字符扔到我们的临时变量里面去了
            //0->1的原始uv
            texCoords_.push_back(vt);

        }
        
        //这个索引使用的结构
        else if (!line.compare(0, 2, "f ")) {
            Face face;
            int itrash, idx,texIdx;
            iss >> trash;
            //
            while (iss >> idx >> trash >> texIdx >> trash >> itrash) {
                idx--; // in wavefront obj all indices start at 1, not zero
                texIdx--;
                face.vertexIndices.push_back(idx);
                face.texCoordIndices.push_back(texIdx);
                // 这里可以把纹理索引存储到某个数组（例如纹理坐标的索引数组）
               // 但现在只是解析索引，若有需要可以再创建相应的逻辑 
            }
            faces_.push_back(face);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() <<"   ts#  "<< texCoords_.size()<< std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}
//我现在脑子都是懵的，回去睡觉了
Face Model::face(int idx) {
    return faces_[idx];
}

Vec2f Model::text(int i)
{
    return texCoords_[i];
}

Vec3f Model::vert(int i) {
    return verts_[i];
}