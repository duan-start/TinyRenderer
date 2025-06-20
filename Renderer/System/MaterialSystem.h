#pragma once
#include <glm/glm.hpp>

struct Material {
	glm::vec4 Albedo = glm::vec4(1.0f);     // ����ɫ��Ĭ�ϰ�ɫ
	//glm::vec3 Specular = glm::vec3(1.0f);   // �߹���ɫ
	//float Shininess = 32.0f;               // �߹�ϵ��
	//// ����չ�������ԣ��� Metallic, Roughness, AO ��

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
	//ͬ��������չ�ɶ��
	Material m_Material;
};

