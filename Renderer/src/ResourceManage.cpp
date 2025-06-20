#include "ResourceManage.h"



void ResourceManage::Run()
{
	std::vector<glm::vec2> v = {{2,3},{200,300}};
	std::vector<glm::vec2> t = { {2,3},{200,300},{400,100} };
	//Line::Draw(v, m_Image,{255,255,255,255});
	//Line::Draw(m_Model, m_Image);
	//Triangle::Draw(t,m_Image);
	Renderer::DrawMesh(m_Model, m_Image,m_MaterialSystem.GetMaterial(),m_LightSystem.GetLight());
	//Renderer::Init();
	//Renderer::BeginScene();

	//Renderer::End();
}
