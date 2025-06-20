#pragma once

#include <vector>
#include <glm/glm.hpp>

//由于我希望在查询面的时候，得到对应的顶点的索引以及对应的纹理的缩影。
//所以我要定义一个结构体，直接对二者做一个绑定

struct SubMeshIndex {
	std::vector<int> vertexIndices;
	std::vector<int> texCoordIndices;
};

class Model {
private:
	std::vector<glm::vec3> m_VertexData;//存储顶点数据
	std::vector<SubMeshIndex> m_MeshIndex;//存储绑定的纹理和顶点的索引
	std::vector<glm::vec2> m_TexCoordData;//存储纹理数据
public:
	Model(const char *filename);
	~Model();
	int nverts();
	int SubMeshesSize();
	glm::vec3  GetVertData(int i);
	SubMeshIndex GetSubMeshIndex(int idx);
	glm::vec2 GetTexData(int i);
};
