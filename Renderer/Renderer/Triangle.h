#pragma once
#include "model.h"
#include "tgaimage.h"
#include "glm/glm.hpp"
#include "Texture.h"
//减少头文件的污染，必要的扔到cpp文件里面
struct Material;
struct Light;
//class Model;
//class TGAImage;

class Triangle
{
public:
	Triangle() {};
	static void Draw(const std::vector<glm::vec2>position, std::shared_ptr<TGAImage>& image, const std::vector<glm::vec2>& texcoordsx);
	//const TGAColor& GetColor() const { return m_Color; }
	//void SetColor(const TGAColor& color) { m_Color = color; }

	static void Draw(const std::shared_ptr<Model>& model, std::shared_ptr<TGAImage>& image, const TGAColor& color);


	//test 
	static void Draw(const std::shared_ptr<Model> model, std::shared_ptr<TGAImage>& image, const Material& material, const Light& light);
private:

	static glm::vec3 barycentric(const std::vector<glm::vec2>& pts, const glm::vec2& P) {
		glm::vec3 u =glm::cross( glm::vec3(pts[2][0] - pts[0][0], pts[1][0] - pts[0][0], pts[0][0] - P[0]) , glm::vec3(pts[2][1] - pts[0][1], pts[1][1] - pts[0][1], pts[0][1] - P[1]));
		/* `pts` and `P` has integer value as coordinates
		   so `abs(u[2])` < 1 means `u[2]` is 0, that means
		   triangle is degenerate, in this case return something with negative coordinates */
		//判断三角形是否退化
		if (std::abs(u.z) < 1) return glm::vec3(-1, 1, 1);
		return glm::vec3(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
	}

	//这里要解一下耦合
	inline static Texture m_Texture = Texture("D:/Learn/renderer/Renderer/TinyRenderer/Renderer/assets/obj/diffuse.tga");
};
