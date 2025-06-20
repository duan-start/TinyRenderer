#pragma once
#include <glm/glm.hpp>

struct Light {
	//记得完善好，我先去看clip模型了
	glm::vec3 Position;
	glm::vec3 Lightdir;
	glm::vec3 Color=glm::vec3(1.0f);
};

class LightSystem
{
public:

	LightSystem(const Light& light = Light{}) : m_Light(light) {};
	//LightSystem(const Light& light) :m_Light(light) {};

	const Light& GetLight() const { return m_Light; }
	const glm::vec3& GetColor() const { return m_Light.Color; }
	const glm::vec3& GetDir() const { return m_Light.Lightdir; }
	const glm::vec3& GetPosition() const { return m_Light.Position; }

	void SetColor(const glm::vec3& color) ;
	void SetPosition(const glm::vec3& position);
	void SetDir(const glm::vec3& dir);
	
private:
	//可以拓展成为多光源
	Light m_Light;
};

