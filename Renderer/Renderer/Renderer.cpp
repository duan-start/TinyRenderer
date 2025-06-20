#include "Renderer.h"
#include "Line.h"
#include "Triangle.h"
//#include "tgaimage.h"

void Renderer::Init()
{
}

void Renderer::DrawLine(const std::vector<glm::vec2> points, std::shared_ptr<TGAImage>& image, const TGAColor& color)
{
	Line::Draw(points, image, color);
}

void Renderer::DrawLine(const std::shared_ptr<Model>& model, std::shared_ptr<TGAImage>& image, const TGAColor& color)
{
	Line::Draw(model, image, color);
}

void Renderer::DrawMesh(const std::vector<glm::vec2> position, std::shared_ptr<TGAImage>& image, const TGAColor& color)
{
	//Triangle::Draw(position, image, color);
}

void Renderer::DrawMesh(const std::shared_ptr<Model>& model, std::shared_ptr<TGAImage>& image, const TGAColor& color)
{
	Triangle::Draw(model, image, color);
}

void Renderer::DrawMesh(const std::shared_ptr<Model> model, std::shared_ptr<TGAImage>& image, const Material& material, const Light& Light)
{
	Triangle::Draw(model,image, material, Light);
}


