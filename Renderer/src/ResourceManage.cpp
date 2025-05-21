#include "ResourceManage.h"



void ResourceManage::Run()
{
	std::vector<glm::vec2> v = {{2,3},{200,300}};
	//Line::Draw(v, m_Image,{255,255,255,255});
	Line::Draw(m_Model, m_Image);
	//Renderer::Init();
	//Renderer::BeginScene();

	//Renderer::End();
}
