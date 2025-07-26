#include "Triangle.h"
#include "../System/LightSystem.h"
#include "../System/MaterialSystem.h"
#include <iostream>


glm::vec3 Triangle::barycentric(const std::array<glm::vec2,3>& pts, const glm::vec2& P){
		glm::vec3 u = glm::cross(glm::vec3(pts[2][0] - pts[0][0], pts[1][0] - pts[0][0], pts[0][0] - P[0]), glm::vec3(pts[2][1] - pts[0][1], pts[1][1] - pts[0][1], pts[0][1] - P[1]));
		/* `pts` and `P` has integer value as coordinates
		   so `abs(u[2])` < 1 means `u[2]` is 0, that means
		   triangle is degenerate, in this case return something with negative coordinates */
		   //判断三角形是否退化
		if (std::abs(u.z) < 1) return glm::vec3(-1, 1, 1);
		return glm::vec3(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
	}


void Triangle::Draw(const ScreenVertex& screenVertex, std::shared_ptr<TGAImage>& image, const std::shared_ptr<Texture>& tuxture)
{
	
	std::array<glm::vec2, 3> position;
	std::array<float, 3> depth;
	for (int i = 0; i < 3; i++) {
		position[i] = { screenVertex.ScreenPos[i].x,screenVertex.ScreenPos[i].y};
		depth[i] = screenVertex.ScreenPos[i].z;
	}
	 
	std::array<glm::vec2, 3> texcoords = screenVertex.TexCoords;

	//后续规范优化bboxmin.x y,bboxmax.x,y
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

	//双重循环进行填充,利用重心公式
	glm::vec2 p;
	for (p.x = minx;p.x<= maxx; p.x++) {
		for (p.y = miny; p.y <= maxy; p.y++) {
			glm::vec3 bc_screen = barycentric(position, p);
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
			
			glm::vec2 texc = bc_screen.x * texcoords[0] + bc_screen.y * texcoords[1] + bc_screen.z * texcoords[2];
			float zbuffer = bc_screen.x * depth[0] + bc_screen.y * depth[1] + bc_screen.z * depth[2];
			TGAColor lastColor = tuxture->GetColor(texc.x,texc.y);
			if (zbuffer > zBuffer[int(p.y * image->GetWidth()+ p.x)]) {
			image->set(p.x, p.y, lastColor);
			zBuffer[int(p.y * image->GetWidth()+p.x)] = zbuffer;
			}
			//else image->set(p.x, p.y, { 255.f,255.f,255.f,255.f });
		}
	}
}

void Triangle::Transfer(glm::vec3& worldPos,const glm::mat4& trans)
{
	glm::vec4 temp = { worldPos,1.0f };
	temp = m_ViewProjection* trans*temp;
	worldPos = { temp.x / temp.w,temp.y / temp.w,temp.z / temp.w };
}

void Triangle::Draw(const std::shared_ptr<Model>& model, std::shared_ptr<TGAImage>& image, const TGAColor& color)
{
	Draw(model, image,Material(),Light());//有点危险

}

void Triangle::Draw(const std::shared_ptr<Model>& model, std::shared_ptr<TGAImage>& image, const Material& material, const Light& light)
{
}


void Triangle::Draw(std::shared_ptr<TGAImage>& image, const MeshData& meshdata)
{
	//模型空间的变换按理还要专门写一个model函数
	zBuffer = std::vector<float>(image->GetWidth() * image->GetHeight(), -std::numeric_limits<float>::infinity());
	for (int i = 0; i < meshdata.model->SubMeshesSize(); i++) {
		std::vector<int> Vface = meshdata.model->GetSubMeshIndex(i).vertexIndices;
		std::vector<int> Tface = meshdata.model->GetSubMeshIndex(i).texCoordIndices;

		std::array<glm::vec3, 3> screen_coords;
		std::array<glm::vec3, 3> world_coords;
		std::array<glm::vec2, 3> texcoords;

		std::vector<glm::vec4> diffuse(3);

		for (int j = 0; j < 3; j++) {
			//提取对应的INDEX，注意这两个是不同的
			world_coords[j] = meshdata.model->GetVertData(Vface[j]);
			Transfer(world_coords[j]);
			texcoords[j] =meshdata.model->GetTexData(Tface[j]);
			screen_coords[j] = glm::vec3((world_coords[j].x + 1.) * image->GetWidth() / 2., (world_coords[j].y + 1.) * image->GetHeight() / 2., world_coords[j].z);
				
		}

		glm::vec3 n = glm::cross((world_coords[2] - world_coords[0]), (world_coords[1] - world_coords[0]));
		n = glm::normalize(n);
		//glm::normalize(light.Lightdir);
		float intensity = glm::dot(n, meshdata.light.Lightdir);
		//glm::vec4 lighter = { light.Color,1.0f };
		//std::vector<glm::vec4> colormix = { lighter * diffuse[0],lighter * diffuse[1],lighter * diffuse[2] };//*对应的材质特点
		
		if (intensity > 0) {
			Draw({ screen_coords, texcoords }, image, meshdata.texture);
		}

	}
}

void Triangle::SetViewProjection(const glm::mat4& viewprojection)
{
	m_ViewProjection = viewprojection;
}

