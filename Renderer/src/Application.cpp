#include "Application.h"

void Application::Run()
{
    m_Res = std::make_unique<ResourceManage>(m_Image,"assets/obj/head.obj"); // 初始化资源管理器

	m_Res->Run();
    //图像坐标反转和写入原始的数据
    m_Image->flip_vertically(); // i want to have the origin at the left bottom corner of the image
    m_Image->write_tga_file("output.tga");

}

