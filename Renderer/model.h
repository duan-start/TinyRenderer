#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"

//������ϣ���ڲ�ѯ���ʱ�򣬵õ���Ӧ�Ķ���������Լ���Ӧ���������Ӱ��
//������Ҫ����һ���ṹ�壬ֱ�ӶԶ�����һ����

struct Face {
	std::vector<int> vertexIndices;
	std::vector<int> texCoordIndices;
};

class Model {
private:
	std::vector<Vec3f> verts_;//�洢��������
	std::vector<Face> faces_;//�洢�󶨵�����Ͷ������Ӱ
	std::vector<Vec2f> texCoords_;//�洢��������
public:
	Model(const char *filename);
	~Model();
	int nverts();
	int nfaces();
	Vec3f vert(int i);
	Face face(int idx);
	Vec2f text(int i);
};

#endif //__MODEL_H__