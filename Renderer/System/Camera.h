#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
	enum class ProjectionType { Orthographic = 0, Perspective = 1 };
public:
	Camera() = default;

	Camera(const glm::mat4& projeciton);

	virtual ~Camera() = default;

	//ר����������ͬһ�Ŀ�߱�
	void SetViewPortSize(uint32_t width, uint32_t height);

	//����������Ŀ�߱�Ҳ��һ����Ҫ�Ĳ���
	void SetOrthoGraphic(float size, float nearClip, float farClip);
	void SetPerspetive(float verticalFov, float clipNear, float clipFar);

	//Ortho
	float GetOrthographicNearClip() const { return m_OrthographicNear; }
	void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }
	float GetOrthographicFarClip() const { return m_OrthographicFar; }
	void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }

	//��νsize��ʵ����height��Ҳ�ǷŴ���С�ĸ���ԭ��
	float GetOrthographicSize() const { return m_OrthographicSize; }
	void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }

	//Perspective 
	float GetPerspectiveNearClip()const { return m_PerspectiveNear; }
	float GetPerspectiveFarClip()const { return  m_PerspectiveFar; }
	float GetPerspectiveVerticalFOV()const { return  m_PerspectiveFOV; }

	void SetPerspectiveNearClip(float nearClip) {
		m_PerspectiveNear = nearClip; RecalculateProjection();
	}
	void SetPerspectiveFarClip(float farClip) {
		m_PerspectiveFar = farClip; RecalculateProjection();
	}
	void SetPerspectiveVerticalFOV(float fov) {
		m_PerspectiveFOV = fov; RecalculateProjection();
	}



	void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }
	ProjectionType GetProjectionType()const { return m_ProjectionType; }

	const glm::mat4& GetProjection() const { return m_Projection; }

private:
	void RecalculateProjection();
private:
	ProjectionType m_ProjectionType = ProjectionType::Orthographic;

	float m_PerspectiveFOV = glm::radians(45.0f);
	float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

	float m_OrthographicSize = 10.f;
	float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

	float m_AspectRatio = 1.f;
	glm::mat4 m_Projection{0.f};
};

