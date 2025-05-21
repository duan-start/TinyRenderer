#pragma once
#include "../Renderer/Line.h"
#include "./package/model.h"

class ResourceManage
{
public:
	ResourceManage(std::shared_ptr<TGAImage> image, const std::string& modelPath) :m_Image(image) {
		m_Model = std::make_shared<Model>(modelPath.c_str());
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
	//Light m_Light;
	std::shared_ptr<Model> m_Model;
};

