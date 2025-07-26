#pragma once
#include "model.h"
#include "tgaimage.h"
#include "glm/glm.hpp"
#include "Texture.h"
#include <array>
//减少头文件的污染，必要的扔到cpp文件里面

#include "../System/MaterialSystem.h"
#include "../System/LightSystem.h"


struct ScreenVertex {
	std::array<glm::vec3,3> ScreenPos;
	std::array<glm::vec2,3> TexCoords;
};

struct MeshData {
	std::shared_ptr<Model> model = nullptr;
	std::shared_ptr<Texture>texture = nullptr;
	Material material;
	Light light=Light() ;
//	glm::mat4 viewprojectMat;
};

class Triangle
{
public:
	Triangle() {};

	//const TGAColor& GetColor() const { return m_Color; }
	//void SetColor(const TGAColor& color) { m_Color = color; }

	static void Draw(const std::shared_ptr<Model>& model, std::shared_ptr<TGAImage>& image, const TGAColor& color);

	//test 
	static void Draw(const std::shared_ptr<Model>& model, std::shared_ptr<TGAImage>& image, const Material& material, const Light& light);

	static void Draw(std::shared_ptr<TGAImage>& image, const MeshData& meshdata);

	static void SetViewProjection(const glm::mat4& viewprojection);
private:

	static glm::vec3 barycentric(const std::array<glm::vec2,3> & pts, const glm::vec2& P);

	static void Draw(const ScreenVertex& screenvertex, std::shared_ptr<TGAImage>& image,const std::shared_ptr<Texture>& tuxture);

	static void Transfer(glm::vec3& worldPos,const glm::mat4& trans=glm::mat4(1.0f));
private:
	
	inline static std::vector<float> zBuffer;
	inline static glm::mat4 m_ViewProjection;
};
