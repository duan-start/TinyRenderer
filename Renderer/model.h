#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"

//由于我希望在查询面的时候，得到对应的顶点的索引以及对应的纹理的缩影。
//所以我要定义一个结构体，直接对二者做一个绑定

struct Face {
	std::vector<int> vertexIndices;
	std::vector<int> texCoordIndices;
};

class Model {
private:
	std::vector<Vec3f> verts_;//存储顶点数据
	std::vector<Face> faces_;//存储绑定的纹理和顶点的缩影
	std::vector<Vec2f> texCoords_;//存储纹理数据
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