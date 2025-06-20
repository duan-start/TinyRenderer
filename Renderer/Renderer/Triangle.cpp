#include "Triangle.h"
#include "../System/LightSystem.h"
#include "../System/MaterialSystem.h"
//�ڴ�������position��Ӧ������Ļ�ռ�����
//���position��texc�ϲ���vertex
void Triangle::Draw(const std::vector<glm::vec2> position, std::shared_ptr<TGAImage>& image, const std::vector<glm::vec2>& texcoords)
{
	if (position.size() < 3) { printf("Trangle points not right"); return; }
	//������ͨ����������
	//ʵ���������ȵ�����ʱ�ڷ�������ܲ���ջ�Ϸ���

	//�����淶�Ż�bboxmin.x y,bboxmax.x,y
	int minx= image->GetWidth()-1, maxx=0, miny = image->GetHeight()-1, maxy=0;
	for (int i = 0; i < 3; i++) {
		minx = fmin(position[i].x, minx);
		maxx = fmax(position[i].x, maxx);
		miny = fmin(position[i].y, miny);
		maxy = fmax(position[i].y, maxy);
	}

	// Clamp to image bounds
	minx = std::max(0, minx);
	maxx = std::min(image->GetWidth() - 1, maxx);
	miny = std::max(0, miny);
	maxy = std::min(image->GetHeight() - 1, maxy);

	//˫��ѭ���������,�������Ĺ�ʽ
	glm::vec2 p;
	for (p.x = minx;p.x<= maxx; p.x++) {
		for (p.y = miny; p.y <= maxy; p.y++) {
			glm::vec3 bc_screen = barycentric(position, p);
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
			//���һ����������⣬���뱣֤texc�Ͷ�Ӧ��vexһ��
			glm::vec2 texc = bc_screen.x * texcoords[0] + bc_screen.y * texcoords[1] + bc_screen.z * texcoords[2];
			TGAColor lastColor =m_Texture.GetColor(texc.x,texc.y);
			image->set(p.x, p.y, lastColor);
		}
	}
}

void Triangle::Draw(const std::shared_ptr<Model>& model, std::shared_ptr<TGAImage>& image, const TGAColor& color)
{
	Draw(model, image,Material(),Light());//�е�Σ��
	//for (int i = 0; i < model->SubMeshesSize(); i++) {
	//	std::vector<int> face = model->GetSubMeshIndex(i).vertexIndices;
	//	std::vector<glm::vec2> screen_coords(3);
	//	for (int j = 0; j < 3; j++) {
	//		glm::vec3 world_coords = model->GetVertData(face[j]);
	//		screen_coords[j] = glm::vec2((world_coords.x + 1.) * image->GetWidth() / 2., (world_coords.y + 1.) * image->GetHeight() / 2.);
	//	}
	//	Draw(screen_coords, image, TGAColor(rand() % 255, rand() % 255, rand() % 255, 255));
	//}
}

void Triangle::Draw(const std::shared_ptr<Model> model, std::shared_ptr<TGAImage>& image, const Material& material, const Light& light)
{
	for (int i = 0; i < model->SubMeshesSize(); i++) {
		std::vector<int> face = model->GetSubMeshIndex(i).vertexIndices;

		std::vector<glm::vec2> screen_coords(3);
		std::vector<glm::vec3> world_coords(3);
		std::vector<glm::vec2> texcoords(3);

		std::vector<glm::vec4> diffuse(3);
		for (int j = 0; j < 3; j++) {
			world_coords[j] = model->GetVertData(face[j]);
			 texcoords[j] = model->GetTexData(face[j]);

			//diffuse[j] = m_Texture.GetColor(texcoord.x, texcoord.y);
			//printf(diffuse[j].x,);
			screen_coords[j] = glm::vec2((world_coords[j].x + 1.) * image->GetWidth() / 2., (world_coords[j].y + 1.) * image->GetHeight() / 2.);
		}

		glm::vec3 n =glm::cross( (world_coords[2] - world_coords[0]) , (world_coords[1] - world_coords[0]));
		n=glm::normalize(n);
		//glm::normalize(light.Lightdir);
		float intensity = glm::dot(n , light.Lightdir);
		glm::vec4 lighter = { light.Color,1.0f };
		std::vector<glm::vec4> colormix = { lighter * diffuse[0],lighter *diffuse[1],lighter *diffuse[2]};//*��Ӧ�Ĳ����ص�
		if (intensity > 0) {
			Draw(screen_coords, image, texcoords);
		}
	
	}
}
