#pragma once

#include <string>
#include <tgaimage.h>
#include <algorithm>

class Texture {
	//���������ֻ�Ǽ򵥵�дһ�£���ʱֻ֧��tga�ĸ�ʽ�ͺ���
	//todu:����һ��image,������Ӧ��Ļ�����ÿһ����ֵ��ȡ�����ͺ�
public:
    Texture(const std::string& filename) {
        m_ImageTex = std::make_shared<TGAImage>();
        if (!m_ImageTex->read_tga_file(filename.c_str())) {
            printf("Failed to load texture: %s\n", filename.c_str());
        }
        else {
            printf("Loaded texture: %s (%d x %d)\n", filename.c_str(),
                m_ImageTex->GetWidth(), m_ImageTex->GetHeight());
        }
        m_ImageTex->flip_vertically();
    }

   const TGAColor& GetColor(float u, float v) const {
        if (!m_ImageTex) return glm::vec4(0.0f);

        // Clamp u,v to [0,1]
        u = std::clamp(u, 0.0f, 1.0f);
        v = std::clamp(v, 0.0f, 1.0f);

        int x = static_cast<int>(u * (m_ImageTex->GetWidth() - 1));
        int y = static_cast<int>(v * (m_ImageTex->GetHeight() - 1));

        return m_ImageTex->get(x, y);
        // ע�⣺��ɫͨ���� 0-255�����һ��
     //   return glm::vec4(n.r, n.g, n.b, n.a);
    }

private:
    std::shared_ptr<TGAImage> m_ImageTex;
};

