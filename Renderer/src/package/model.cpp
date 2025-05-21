#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) : m_VertexData(), m_MeshIndex() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix; // 读取 "v"
            glm::vec3 v;
            for (int i = 0; i < 3; ++i) {
                if (!(iss >> v[i])) {
                    // 处理错误，比如抛异常或者跳过
                    break;
                }
            }
            m_VertexData.push_back(v);
        }
        //好一个教程，真的就是全得靠我手写了
        //查找纹理的坐标
        else if (!line.compare(0, 2, "vt")) {
            iss >> trash >> trash;//把vt扔到垃圾桶
            glm::vec2 vt;
            iss >> vt.x >> vt.y;//这里是iss接下来的两个字符扔到我们的临时变量里面去了
            //0->1的原始uv
            m_TexCoordData.push_back(vt);

        }

        //这个索引使用的结构
        else if (!line.compare(0, 2, "f ")) {
            //    SubMeshIndex face;
            //    std::string token;
            //    iss >> trash; // skip 'f'
            //    while (iss >> token) {
            //        std::replace(token.begin(), token.end(), '/', ' '); // 把/替换成空格，方便解析
            //        std::istringstream tokenStream(token);
            //        int vIdx = 0, vtIdx = 0, vnIdx = 0;
            //        tokenStream >> vIdx >> vtIdx >> vnIdx;

            //        face.vertexIndices.push_back(vIdx - 1);    // obj从1开始，C++从0开始
            //        face.texCoordIndices.push_back(vtIdx - 1);
            //        // 如果你之后需要vnIdx，可以类似处理
            //    }
            //    m_MeshIndex.push_back(face);
            //}

            SubMeshIndex face;
            iss >> trash; // skip 'f'
            std::string token;

            while (iss >> token) {
                int vIdx = 0, vtIdx = 0, vnIdx = 0;

                size_t firstSlash = token.find('/');
                size_t secondSlash = token.find('/', firstSlash + 1);

                if (firstSlash == std::string::npos) {
                    vIdx = std::stoi(token);
                }
                else {
                    vIdx = std::stoi(token.substr(0, firstSlash));
                    if (secondSlash > firstSlash + 1) {
                        vtIdx = std::stoi(token.substr(firstSlash + 1, secondSlash - firstSlash - 1));
                    }
                    if (secondSlash != std::string::npos && secondSlash + 1 < token.size()) {
                        vnIdx = std::stoi(token.substr(secondSlash + 1));
                    }
                }

                face.vertexIndices.push_back(vIdx - 1);      // OBJ索引从1开始，数组从0开始
                face.texCoordIndices.push_back(vtIdx - 1);   // 如果没有 vtIdx 默认是0或-1
                // 可以考虑加入 normalIndices 处理 vnIdx
            }

            m_MeshIndex.push_back(face);
        }
    }
    std::cerr << "# v# " << m_VertexData.size() << " f# "  << m_MeshIndex.size() <<"   ts#  "<< m_TexCoordData.size()<< std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return (int)m_VertexData.size();
}

int Model::SubMeshesSize() {
    return (int)m_MeshIndex.size();
}
//我现在脑子都是懵的，回去睡觉了
SubMeshIndex Model::GetSubMeshIndex(int idx) {
    return m_MeshIndex[idx];
}

glm::vec2 Model::GetTexData(int i)
{
    return m_TexCoordData[i];
}

glm::vec3 Model::GetVertData(int i) {
    return m_VertexData[i];
}