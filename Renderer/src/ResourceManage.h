#pragma once
#include "../Renderer/Renderer.h"
#include "../System/LightSystem.h"
#include "../System/MaterialSystem.h"
class ResourceManage
{
public:
	ResourceManage(std::shared_ptr<TGAImage> image, const std::string& modelPath) :m_Image(image) {
		m_Model = std::make_shared<Model>(modelPath.c_str());
		m_LightSystem.SetDir({ 0, 0, -1 });


	};
	//const Texture& GetTexture()const { return m_Texture; }
	//const m_Camera& GetCamera()const { return m_Camera; }
	//const m_Camera& GetLight()const { return m_Camera; }
	//~ResourceManage() { delete m_Image; }
	void Run();
private:
	std::shared_ptr<TGAImage> m_Image;
	//Texture m_Texture;
	//Camera m_Camera;
	LightSystem m_LightSystem;
	MaterialSystem m_MaterialSystem;
	std::shared_ptr<Model> m_Model;
};

