
#include <iostream>  
#include <algorithm>
#include <fstream>  
#include <vector>
#include <cmath>
#include <cstdlib>
#include <limits>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

//ȫ�ֱ��������弸����������ɫ��ͼ��Ļ�����Ϣ
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
Model* model = NULL;
const int width = 800;
const int height = 800;
const int depth = 255;

Vec3f light_dir{ 0.f, 0.f, -1.f };
Vec3f camera{ 0.f,0.f,3.f };

//дһ���Ӿ���任�ľ���ѣ���Ȼ��̫�淶��������Ӧ����ͬʱд�����������������,�������ӿڱ任��


//������������,��ʵ�����е�û��������Ӧ����vec4->vec3��
Vec3f m2v(Matrix m) {
    return Vec3f(m[0][0] / m[3][0], m[1][0] / m[3][0], m[2][0] / m[3][0]);

}

//��׼�ü��ռ�-����Ļ����ľ����������ĺ�����

Matrix viewport(int x, int y, int w, int h) {
    Matrix m = Matrix::identity(4);
    m[0][3] = x + w / 2.f;
    m[1][3] = y + h / 2.f;
    m[2][3] = depth / 2.f;

    m[0][0] = w / 2.f;
    m[1][1] = h / 2.f;
    m[2][2] = depth / 2.f;
    return m;
}


//�Լ�д�Ĳ�˵ĺ���
Vec3f cross(const Vec3f& a, const Vec3f& b) {
    return Vec3f{
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}


//����uv�����ת�����texture�����ݣ�ȥȡ��Ӧ������
Vec3f getTexelColor(const std::vector<unsigned char>& textureData, int width, int height, const Vec2f& uv) {
    int x = static_cast<int>(uv.x * width);
    int y = static_cast<int>(uv.y * height);

    // Replace the line causing the error with the following code������ֻ��һ����׳�Ե��Ż�
    //clampֻ��һ�����ƺ�����������һ��ӳ�亯��
    x = std::clamp(x, 0, width - 1);
    x = std::max(0, std::min(x, width - 1));
    y = std::clamp(y, 0, height - 1);

    int index = (y * width + x) * 3; // ��������ص���ʼ���� ��rgb�洢��һ�δ洢˳��
    return Vec3f{
        textureData[index] / 255.0f, // Red  
        textureData[index + 1] / 255.0f, // Green  
        textureData[index + 2] / 255.0f  // Blue  
    };
}
//����Ҫ�Լ�д��ȡ��������ĺ��鷳�������Ǵ�ģ��о������Լ�����Щ����



//�����������ߵĻ���
//���ﴫ��̫hack �ˣ�����϶��ø�
void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
    bool steep = false;
	//�����ǻ��ķ������x�ı仯����y�ı仯������ô��������x�ử�ߣ���������y�ử�ߣ�
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

	//��֤x0 < x1;
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

	int dx = x1 - x0;
	int dy = y1 - y0;

	//ͬ�������ţ������derror2��dy/dx��2����int�ıȽϣ����ٽṹ��
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
    int y = y0;

	//����ۻ���������ۻ���dx��ʱ�򣬾���Ҫ�ƶ�y��λ�ã�
    for (int x = x0; x <= x1; x++) {
        if (steep) {
            image.set(y, x, color);
        }
        else {
            image.set(x, y, color);
        }
		error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

//����p�����������
Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P) {
    Vec3f s[2];
    for (int i = 2; i--; ) {
        s[i][0] = C[i] - A[i];
        s[i][1] = B[i] - A[i];
        s[i][2] = A[i] - P[i];
    }
    Vec3f u = cross(s[0], s[1]);
    if (std::abs(u[2]) > 1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
        return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
    return Vec3f(-1, 1, 1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
}

//���ĺ���������������
void triangle(Vec3f* pts, Vec2f* tex, float* zbuffer, TGAImage& image, TGAImage& texture) {
    //��ʼ����Χ��
   Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
   Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());

   Vec2f clamp(image.get_width() - 1, image.get_height() - 1);

    //�ҵ�����İ�Χ��
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            bboxmin[j] = std::fmin(bboxmin[j], pts[i][j]);
            bboxmax[j] = std::fmax(bboxmax[j], pts[i][j]);
           // std::cout << pts[i][j] << std::endl;
        }
    }

   Vec3f P;
    //�����е����ؽ����жϣ�������������
    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
            Vec3f bc_screen = barycentric(pts[0], pts[1], pts[2], P);
            //û���������ڲ���ֱ���˳�
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
            //��ʼ��ֵ�������������ռ�
            P.z = 0;
            Vec2f uv(0, 0);
            //��zֵ�������������в�ֵ���õ����Ӿ�׼������ֵ
            for (int i = 0; i < 3; i++) {
                P.z += pts[i][2] * bc_screen[i];
                uv.x += tex[i].x * bc_screen[i];
                uv.y += tex[i].y * bc_screen[i];
            }
            //���ݲ�ֵ���uvȡ��ɫ�������ǰѶ����Ӧ��uv�����������ɫ�ù�������ֵ
            //����ǰ���ֵ�õ���zֵ��������жϣ�Ȼ���������ļ���ȡ����Ӧ����ɫ���ݽ�����ɫ��Ϸ
            if (zbuffer[int(P.x + P.y * width)] < P.z) {
                zbuffer[int(P.x + P.y * width)] = P.z;
                //����Ҫע���Ѿ�ת��image ͼ������꣬y��Ҫ����ȡ��
                TGAColor color = texture.get(uv.x * texture.get_width(), (1-uv.y) * texture.get_height());
                //TGAColor color=Blin_Phong()
                image.set(P.x, P.y, color);
            }
        }
    }
}



//��׼�Ĳü��ռ䵽��Ļ�ռ�ı仯
Vec3f world2screen(Vec3f v) {
    //�ҷ���z��ֵ����ƫ��һ
    return Vec3f(int((v.x + 1.) * width / 2. + .5), int((v.y + 1.) * height / 2. + .5), v.z);
}


int main(int argc, char** argv) {
    if (argc == 2) {
        model = new Model(argv[1]);
    }
    else {
        model = new Model("./obj/african_head.obj");
    }

    if (!model) {
        std::cerr << "Failed to load model" << std::endl;
        return -1;
    }

    //���ϴ���һ��z����ģ�std::vector�Ƕ��ϴ������ݵģ�arrayҲ�ǣ�ѡ���Ӧ������õģ�
    float* zbuffer = new float[width * height];
    for (int i = 0; i < width * height; i++) {
        zbuffer[i] = -std::numeric_limits<float>::max();
    }

    TGAImage image(width, height, TGAImage::RGB);

    const char* filename = "./obj/diffuse.tga";
    TGAImage texture;
   // 
    if (!texture.read_tga_file(filename)) {
        std::cerr << "Failed to load texture" << std::endl;
        delete model;
        delete[] zbuffer;
        return -1;
    }

    for (int i = 0; i < model->nfaces(); i++) {
        Face face = model->face(i);
        Vec3f pts[3];
        //Vec3f world_coords[3];
        Vec2f world_tex[3];

        for (int j = 0; j < 3; j++) {
            //�ռ��ɼ����ռ�Ķ������꣬������ҵ��ļ�������Ĭ��ֱ�ӷ���һ����׼�����������
           // world_coords[j] = model->vert(face.vertexIndices[j]);
           // std::cout << model->vert(face.vertexIndices[j]).x<< model->vert(face.vertexIndices[j]).y<< model->vert(face.vertexIndices[j]).z;
            //�ռ���Ӧ����Ļ�������Ϣ������������ɫ��ͼƬ�Ķ�Ӧ���֣�
            pts[j] = world2screen(model->vert(face.vertexIndices[j]));

            //�������꣨0��1��
           // std::cout << pts[j] << std::endl;
           // world_tex[j] = model->text(face.texCoordIndices[j]);
        }


        triangle(pts,world_tex, zbuffer, image, texture);
    }

    //ͼ�����귴ת��д��ԭʼ������
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");

    delete[] zbuffer;
    delete model;
    return 0;
}