#pragma once

#include "ResourceManage.h"
#include <memory>
#pragma once

#include "ResourceManage.h"
#include <memory>

class Application {
public:
    inline static Application& Get(int width=800,int height=800) {
        static Application instance(width,height); // 使用静态局部变量
        return instance;
    }
  /*  void SetImageHeight(int height) { m_Height = height; }
    void SetImageWidth(int width) { m_Width = width; }*/
    void Run();
private:
    Application(int width,int height):m_Width(width),m_Height(height){
        //初始化Image 资源
        m_Image =std::make_shared<TGAImage>(m_Width, m_Height, TGAImage::RGB);
    }

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

private:
    std::unique_ptr<ResourceManage> m_Res;
private:
    int m_Width;
    int m_Height;
    std::shared_ptr<TGAImage> m_Image;
    bool m_Flag = true;
};

