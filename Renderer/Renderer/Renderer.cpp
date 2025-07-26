#include "Renderer.h"

#include "Line.h"
#include "Triangle.h"
#include "Texture.h"
//#include "tgaimage.h"


void Renderer::BeginScene(const Camera& camera,const glm::mat4& transform)
{
	m_Projection = camera.GetProjection()* glm::inverse(transform);
	Triangle::SetViewProjection(m_Projection);
}

void Renderer::DrawLine(const std::vector<glm::vec2> points, std::shared_ptr<TGAImage>& image, const TGAColor& color)
{
	Line::Draw(points, image, color);
}

void Renderer::DrawLine(const std::shared_ptr<Model>& model, std::shared_ptr<TGAImage>& image, const TGAColor& color)
{
	Line::Draw(model, image, color);
}

void Renderer::DrawMesh(const std::shared_ptr<Model>& model, std::shared_ptr<TGAImage>& image, const TGAColor& color)
{
	Triangle::Draw(model, image, color);
}

void Renderer::DrawMesh(std::shared_ptr<TGAImage>& image, const MeshData& meshdata)
{
	Triangle::Draw(image, meshdata);
}


