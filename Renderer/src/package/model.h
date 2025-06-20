#pragma once

#include <vector>
#include <glm/glm.hpp>

//������ϣ���ڲ�ѯ���ʱ�򣬵õ���Ӧ�Ķ���������Լ���Ӧ���������Ӱ��
//������Ҫ����һ���ṹ�壬ֱ�ӶԶ�����һ����

struct SubMeshIndex {
	std::vector<int> vertexIndices;
	std::vector<int> texCoordIndices;
};

class Model {
private:
	std::vector<glm::vec3> m_VertexData;//�洢��������
	std::vector<SubMeshIndex> m_MeshIndex;//�洢�󶨵�����Ͷ��������
	std::vector<glm::vec2> m_TexCoordData;//�洢��������
public:
	Model(const char *filename);
	~Model();
	int nverts();
	int SubMeshesSize();
	glm::vec3  GetVertData(int i);
	SubMeshIndex GetSubMeshIndex(int idx);
	glm::vec2 GetTexData(int i);
};
