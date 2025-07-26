#pragma once
#include "../Renderer/Renderer.h"
#include "../System/LightSystem.h"
#include "../System/MaterialSystem.h"
#include "../System/CameraContral.h"
class ResourceManage
{
public:
	ResourceManage(std::shared_ptr<TGAImage> image) :m_Image(image) {
		//Only to update
		m_LightSystem.SetDir({ 0, 0, -1 });
		//m_CameraContral.GetCamera().SetOrthographicSize(5.0f);
		m_CameraContral.GetCamera().SetProjectionType(Camera::ProjectionType::Perspective);
		InitModel("assets/obj/african_head.obj");
		InitTexture("assets/obj/diffuse.tga");
	};
	void Run();

private:
	void InitTexture(const std::string& Texturepath);
	void InitModel(const std::string& ModelPath);

private:
	std::shared_ptr<TGAImage> m_Image=nullptr;
	//后面改成材质系统会更好
	std::shared_ptr<Texture> m_Texture = nullptr;
	std::shared_ptr<Model> m_Model;

	CameraContral m_CameraContral;
	LightSystem m_LightSystem;
	MaterialSystem m_MaterialSystem;
};

