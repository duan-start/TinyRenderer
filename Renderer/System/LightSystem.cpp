#include "LightSystem.h"

void LightSystem::SetColor(const glm::vec3& color)
{
	m_Light.Color = color;
}

void LightSystem::SetPosition(const glm::vec3& position)
{
	m_Light.Position = position;
}

void LightSystem::SetDir(const glm::vec3& dir)
{
	m_Light.Lightdir = dir;
}
