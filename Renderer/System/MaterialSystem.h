#pragma once
#include <glm/glm.hpp>

struct Material {
	glm::vec4 Albedo = glm::vec4(1.0f);     // 基础色，默认白色
	//glm::vec3 Specular = glm::vec3(1.0f);   // 高光颜色
	//float Shininess = 32.0f;               // 高光系数
	//// 可扩展更多属性，如 Metallic, Roughness, AO 等

	Material() = default;
	Material(const glm::vec4& albedo) :Albedo(albedo) {};
	//Material(const glm::vec4& albedo, const glm::vec3& specular = { 1.0f, 1.0f, 1.0f }, float shininess = 32.0f)
	//	: Albedo(albedo), Specular(specular), Shininess(shininess) {}
};

class MaterialSystem
{
public:
	MaterialSystem() = default;
	MaterialSystem(const Material& material) :m_Material(material) {};
	const Material& GetMaterial()const { return m_Material; }
private:
	//同样可以拓展成多个
	Material m_Material;
};

