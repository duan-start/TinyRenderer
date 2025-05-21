#include "Line.h"
#include <iostream>

//贝尔曼直线画法
void Line::Draw(const std::vector<glm::vec2>points, std::shared_ptr<TGAImage>&image, const TGAColor& color)
{
    int x0 = points[0].x;
    int y0 = points[0].y;
    int x1 = points[1].y;
    int y1 = points[1].y;
	 bool steep = false;
	//决定是画的方向，如果x的变化量比y的变化量大，那么就是沿着x轴画线，否则沿着y轴画线；
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

	//保证x0 < x1;
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

	int dx = x1 - x0;
	int dy = y1 - y0;

	//同比例缩放，这里的derror2是dy/dx的2倍；int的比较（加速结构）
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
    int y = y0;

	//误差累积，当误差累积到dx的时候，就需要移动y的位置；
    for (int x = x0; x <= x1; x++) {
        if (steep) {
            image->set(y, x, color);
        }
        else {
            image->set(x, y, color);
        }
		error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }

}

void Line::Draw(const std::shared_ptr<Model>& model, std::shared_ptr<TGAImage>& image, const TGAColor& color)
{
    for (int i = 0; i < model->SubMeshesSize(); i++) {
        int width = image->GetWidth();
        int height = image->GetHeight();
        std::vector<int> subMeshIndex = model->GetSubMeshIndex(i).vertexIndices;
        //std::cout << subMeshIndex[0]<< subMeshIndex[1] << subMeshIndex[2]<<"\n";
        for (int j = 0; j < 3; j++) {
            glm::vec3 v0 = model->GetVertData(subMeshIndex[j]);
            glm::vec3 v1 = model->GetVertData(subMeshIndex[(j + 1) % 3]);
            int x0 = (v0.x + 1.) * width / 2.;
            int y0 = (v0.y + 1.) * height / 2.;
            int x1 = (v1.x + 1.) * width / 2.;
            int y1 = (v1.y + 1.) * height / 2.;
            Line::Draw({{x0,y0},{x1,y1}}, image, color);
        }
    }

}
