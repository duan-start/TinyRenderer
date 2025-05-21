
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

//全局变量，定义几个常见的颜色，图像的基础信息
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
Model* model = NULL;
const int width = 800;
const int height = 800;
const int depth = 255;

Vec3f light_dir{ 0.f, 0.f, -1.f };
Vec3f camera{ 0.f,0.f,3.f };

//写一个视觉变变换的矩阵把，虽然不太规范，按理我应该是同时写至少两个矩阵出来的,用来做视口变换吧


//矩阵到世界坐标,其实这我有点没看懂，不应该是vec4->vec3吗
Vec3f m2v(Matrix m) {
    return Vec3f(m[0][0] / m[3][0], m[1][0] / m[3][0], m[2][0] / m[3][0]);

}

//标准裁剪空间-》屏幕坐标的矩阵（替代后面的函数）

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


//自己写的叉乘的函数
Vec3f cross(const Vec3f& a, const Vec3f& b) {
    return Vec3f{
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}


//根据uv坐标和转换后的texture的数据，去取对应的数据
Vec3f getTexelColor(const std::vector<unsigned char>& textureData, int width, int height, const Vec2f& uv) {
    int x = static_cast<int>(uv.x * width);
    int y = static_cast<int>(uv.y * height);

    // Replace the line causing the error with the following code，这里只是一个健壮性的优化
    //clamp只是一个限制函数，而不是一个映射函数
    x = std::clamp(x, 0, width - 1);
    x = std::max(0, std::min(x, width - 1));
    y = std::clamp(y, 0, height - 1);

    int index = (y * width + x) * 3; // 计算该像素的起始索引 ，rgb存储的一次存储顺序
    return Vec3f{
        textureData[index] / 255.0f, // Red  
        textureData[index + 1] / 255.0f, // Green  
        textureData[index + 2] / 255.0f  // Blue  
    };
}
//不需要自己写读取函数，真的很麻烦，还总是错的，感觉是他自己改了些数据



//贝尔曼的曲线的画法
//这里传参太hack 了，后面肯定得改
void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
    bool steep = false;
	//决定是画的方向，如果x的变化量比y的变化量大，那么就是沿着x轴画线，否则沿着y轴画线；
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

	//保证x0 < x1;
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

	int dx = x1 - x0;
	int dy = y1 - y0;

	//同比例缩放，这里的derror2是dy/dx的2倍；int的比较（加速结构）
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
    int y = y0;

	//误差累积，当误差累积到dx的时候，就需要移动y的位置；
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

//计算p点的重心坐标
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

//核心函数，绘制三角形
void triangle(Vec3f* pts, Vec2f* tex, float* zbuffer, TGAImage& image, TGAImage& texture) {
    //初始化包围盒
   Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
   Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());

   Vec2f clamp(image.get_width() - 1, image.get_height() - 1);

    //找到具体的包围盒
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            bboxmin[j] = std::fmin(bboxmin[j], pts[i][j]);
            bboxmax[j] = std::fmax(bboxmax[j], pts[i][j]);
           // std::cout << pts[i][j] << std::endl;
        }
    }

   Vec3f P;
    //对所有的像素进行判断，计算重心坐标
    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
            Vec3f bc_screen = barycentric(pts[0], pts[1], pts[2], P);
            //没在三角形内部，直接退出
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
            //开始插值，并进行数据收集
            P.z = 0;
            Vec2f uv(0, 0);
            //对z值和纹理的坐标进行插值，得到更加精准的坐标值
            for (int i = 0; i < 3; i++) {
                P.z += pts[i][2] * bc_screen[i];
                uv.x += tex[i].x * bc_screen[i];
                uv.y += tex[i].y * bc_screen[i];
            }
            //根据插值后的uv取颜色，而不是把顶点对应的uv坐标的纹理颜色拿过来做插值
            //根据前面插值得到的z值进行深度判断，然后在纹理文件中取出对应的颜色数据进行填色游戏
            if (zbuffer[int(P.x + P.y * width)] < P.z) {
                zbuffer[int(P.x + P.y * width)] = P.z;
                //这里要注意已经转成image 图像的坐标，y轴要进行取反
                TGAColor color = texture.get(uv.x * texture.get_width(), (1-uv.y) * texture.get_height());
                //TGAColor color=Blin_Phong()
                image.set(P.x, P.y, color);
            }
        }
    }
}



//标准的裁剪空间到屏幕空间的变化
Vec3f world2screen(Vec3f v) {
    //我发现z的值几乎偏于一
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

    //堆上创建一个z缓冲的，std::vector是堆上创建数据的，array也是（选这个应该是最好的）
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
            //收集采集按空间的顶点坐标，这里给我的文件里面是默认直接放在一个标准立方体里面的
           // world_coords[j] = model->vert(face.vertexIndices[j]);
           // std::cout << model->vert(face.vertexIndices[j]).x<< model->vert(face.vertexIndices[j]).y<< model->vert(face.vertexIndices[j]).z;
            //收集对应的屏幕坐标的信息（方便我们填色到图片的对应部分）
            pts[j] = world2screen(model->vert(face.vertexIndices[j]));

            //纹理坐标（0，1）
           // std::cout << pts[j] << std::endl;
           // world_tex[j] = model->text(face.texCoordIndices[j]);
        }


        triangle(pts,world_tex, zbuffer, image, texture);
    }

    //图像坐标反转和写入原始的数据
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");

    delete[] zbuffer;
    delete model;
    return 0;
}