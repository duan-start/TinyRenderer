#pragma once
#include "model.h"
#include "tgaimage.h"
#include "glm/glm.hpp"
class Line
{public:
	Line() {};
	static void Draw(const std::vector<glm::vec2>points, std::shared_ptr<TGAImage>& image,const TGAColor& color);
	//const TGAColor& GetColor() const { return m_Color; }
	//void SetColor(const TGAColor& color) { m_Color = color; }

	static void Draw(const std::shared_ptr<Model>& model, std::shared_ptr<TGAImage>& image, const TGAColor& color);
private:
};

