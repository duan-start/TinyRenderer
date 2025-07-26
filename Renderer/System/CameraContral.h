#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "Camera.h"

struct CameraBounds {
	float Left, Right;
	float Bottom, Top;
	float GetWidth() { return Right - Left; }
	float GetHeight() { return Top - Bottom; }
};

class CameraContral
{
public:
	//ǿ�ҵ�Ĭ�����ԣ��󲿷�����²���Ҫ�������
	//CameraContral(float aspectRatio, bool rotation = true) {};

	CameraContral() = default;
	Camera& GetCamera() {
		return m_Camera;
	}
	const Camera& GetCamera()const {
		return m_Camera;
	}
	const float GetZoomLevel()const { return m_ZoomLevel; }
	void SetZoomLevel(float zoom) { m_ZoomLevel = zoom; }

	const CameraBounds& GetBounds() const { return m_Bounds; }

private:
	//��������Ҫʵʱ�����ģ����Ӹ���Ա�б��ʼ����
	float m_AspectRatio;
	//��������ڹ̶��Ĳ����Ļ�������ʹ�ã���ͨ�����Ը���Ĭ��ֵ�����������
	float m_ZoomLevel = 1.0f;
	//���˳�����ң�����ĳ�ʼ����Ҫǰ��Ĳ���

	//������ֱ�Ӳ��յ�Hack��������ֱ������д�����Ǹ�����ģ�ͨ��ָ��ķ�ʽ����������չ3d��Ⱦ����ʱ������ع�
	//�����г�ʼ����˳��Ҫһ��
	CameraBounds m_Bounds;
	Camera m_Camera;
	//����ʹ�������Ҫ�õĻ�
	bool m_rotation;
	float m_Rotation{ 0.f };

	float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 45.0f;
	glm::vec3 m_CameraPos{ 0.f,0.f,0.f };


};

