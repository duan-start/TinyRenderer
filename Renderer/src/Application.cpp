#include "Application.h"

void Application::Run()
{
    m_Res = std::make_unique<ResourceManage>(m_Image,"assets/obj/head.obj"); // ��ʼ����Դ������

	m_Res->Run();
    //ͼ�����귴ת��д��ԭʼ������
    m_Image->flip_vertically(); // i want to have the origin at the left bottom corner of the image
    m_Image->write_tga_file("output.tga");

}

