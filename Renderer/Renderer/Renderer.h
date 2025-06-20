#pragma once
#include "geometry.h"

#include "model.h"

//������ʵ����Ҳ����ɾ��

#include "../System/LightSystem.h"
#include "../System/MaterialSystem.h"


class TGAImage;
//class Model;
struct TGAColor;

class Renderer {
public:
	//����淶һ��colorӦ������material (���Ի�Ӧ���и�����ϵͳ),
	static void Init();
	static void DrawLine(const std::vector<glm::vec2>points, std::shared_ptr<TGAImage>& image, const TGAColor& color);

	static void DrawLine(const std::shared_ptr<Model>& model, std::shared_ptr<TGAImage>& image, const TGAColor& color);

	static void DrawMesh(const std::vector<glm::vec2> position, std::shared_ptr<TGAImage>& image, const TGAColor& color);

	static void DrawMesh(const std::shared_ptr<Model>& model, std::shared_ptr<TGAImage>& image, const TGAColor& color);

	static void DrawMesh(const std::shared_ptr<Model> model, std::shared_ptr<TGAImage>& image, const Material& material = Material(), const Light& Light = Light());
	
private:
	//���Է�texture��camera֮���
//	static void BeginScene(const Camera& camera, const std::pair<int, int>& aspect);
//	static void EndScene();
	
//	static void Submit(const Ref<VerTexArray>& vertexArray, const Matrix& transform = Matrix(1.0f));
//
//	inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
//private:
//	//static ������ֻ֮�����ⶨ��
//	//static RendererAPI::API m_RendererAPI;
//	struct SceneData {
//		Matrix ViewProjection;
//		float SCR_Width;
//		float SCR_Height;
//	};
//
//	static SceneData* m_SceneData;
};
