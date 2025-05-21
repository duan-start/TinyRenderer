#include "Line.h"
#include <iostream>

//������ֱ�߻���
void Line::Draw(const std::vector<glm::vec2>points, std::shared_ptr<TGAImage>&image, const TGAColor& color)
{
    int x0 = points[0].x;
    int y0 = points[0].y;
    int x1 = points[1].y;
    int y1 = points[1].y;
	 bool steep = false;
	//�����ǻ��ķ������x�ı仯����y�ı仯������ô��������x�ử�ߣ���������y�ử�ߣ�
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

	//��֤x0 < x1;
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

	int dx = x1 - x0;
	int dy = y1 - y0;

	//ͬ�������ţ������derror2��dy/dx��2����int�ıȽϣ����ٽṹ��
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
    int y = y0;

	//����ۻ���������ۻ���dx��ʱ�򣬾���Ҫ�ƶ�y��λ�ã�
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
