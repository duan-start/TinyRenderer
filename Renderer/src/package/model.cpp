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
            iss >> prefix; // ��ȡ "v"
            glm::vec3 v;
            for (int i = 0; i < 3; ++i) {
                if (!(iss >> v[i])) {
                    // ������󣬱������쳣��������
                    break;
                }
            }
            m_VertexData.push_back(v);
        }
        //��һ���̳̣���ľ���ȫ�ÿ�����д��
        //�������������
        else if (!line.compare(0, 2, "vt")) {
            iss >> trash >> trash;//��vt�ӵ�����Ͱ
            glm::vec2 vt;
            iss >> vt.x >> vt.y;//������iss�������������ַ��ӵ����ǵ���ʱ��������ȥ��
            //0->1��ԭʼuv
            m_TexCoordData.push_back(vt);

        }

        //�������ʹ�õĽṹ
        else if (!line.compare(0, 2, "f ")) {
            //    SubMeshIndex face;
            //    std::string token;
            //    iss >> trash; // skip 'f'
            //    while (iss >> token) {
            //        std::replace(token.begin(), token.end(), '/', ' '); // ��/�滻�ɿո񣬷������
            //        std::istringstream tokenStream(token);
            //        int vIdx = 0, vtIdx = 0, vnIdx = 0;
            //        tokenStream >> vIdx >> vtIdx >> vnIdx;

            //        face.vertexIndices.push_back(vIdx - 1);    // obj��1��ʼ��C++��0��ʼ
            //        face.texCoordIndices.push_back(vtIdx - 1);
            //        // �����֮����ҪvnIdx���������ƴ���
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

                face.vertexIndices.push_back(vIdx - 1);      // OBJ������1��ʼ�������0��ʼ
                face.texCoordIndices.push_back(vtIdx - 1);   // ���û�� vtIdx Ĭ����0��-1
                // ���Կ��Ǽ��� normalIndices ���� vnIdx
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
//���������Ӷ����µģ���ȥ˯����
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