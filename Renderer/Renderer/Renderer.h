#pragma once
#include "geometry.h"

#include "model.h"

//这两个实际上也可以删除

#include "../System/LightSystem.h"
#include "../System/MaterialSystem.h"


class TGAImage;
//class Model;
struct TGAColor;

class Renderer {
public:
	//这里规范一点color应该属于material (所以还应该有个材质系统),
	static void Init();
	static void DrawLine(const std::vector<glm::vec2>points, std::shared_ptr<TGAImage>& image, const TGAColor& color);

	static void DrawLine(const std::shared_ptr<Model>& model, std::shared_ptr<TGAImage>& image, const TGAColor& color);

	static void DrawMesh(const std::vector<glm::vec2> position, std::shared_ptr<TGAImage>& image, const TGAColor& color);

	static void DrawMesh(const std::shared_ptr<Model>& model, std::shared_ptr<TGAImage>& image, const TGAColor& color);

	static void DrawMesh(const std::shared_ptr<Model> model, std::shared_ptr<TGAImage>& image, const Material& material = Material(), const Light& Light = Light());
	
private:
	//可以放texture和camera之类的
//	static void BeginScene(const Camera& camera, const std::pair<int, int>& aspect);
//	static void EndScene();
	
//	static void Submit(const Ref<VerTexArray>& vertexArray, const Matrix& transform = Matrix(1.0f));
//
//	inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
//private:
//	//static 的数据之只能类外定义
//	//static RendererAPI::API m_RendererAPI;
//	struct SceneData {
//		Matrix ViewProjection;
//		float SCR_Width;
//		float SCR_Height;
//	};
//
//	static SceneData* m_SceneData;
};
