#pragma once

#include "geometry.h"
#include "model.h"
//这两个实际上也可以删除
#include "Triangle.h"
#include "../System/LightSystem.h"
#include "../System/MaterialSystem.h"
#include "../System/Camera.h"

class TGAImage;
class Texture;
struct TGAColor;


class Renderer {
public:
	static void BeginScene(const Camera& camera,const glm::mat4& transform=glm::mat4(1.0f));
	static void EndScene() {};

	static void DrawLine(const std::vector<glm::vec2>points, std::shared_ptr<TGAImage>& image, const TGAColor& color);

	static void DrawLine(const std::shared_ptr<Model>& model, std::shared_ptr<TGAImage>& image, const TGAColor& color);

	//static void DrawMesh(const std::vector<glm::vec2> position, std::shared_ptr<TGAImage>& image, const TGAColor& color);
	static void DrawMesh(const std::shared_ptr<Model>& model, std::shared_ptr<TGAImage>& image, const TGAColor& color);

	//参数越写越多决定写一个通用的结构体
	static void DrawMesh(std::shared_ptr<TGAImage>& image, const MeshData& meshdata);

private:
	inline static glm::mat4 m_Projection;

};
