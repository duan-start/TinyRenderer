#include "ResourceManage.h"


void ResourceManage::InitTexture(const std::string& texturePath)
{
	m_Texture = std::make_shared<Texture>(texturePath);
}

void ResourceManage::InitModel(const std::string& modelPath)
{
	m_Model = std::make_shared<Model>(modelPath.c_str());
}

void ResourceManage::Run()
{
	//test
	auto wordpos = glm::mat4(1.0f);
	auto& trans = wordpos[3];
	trans.z += 3;

	Renderer::BeginScene(m_CameraContral.GetCamera(), wordpos);
	Renderer::DrawMesh(m_Image, { m_Model,m_Texture,m_MaterialSystem.GetMaterial(),m_LightSystem.GetLight() });

}
