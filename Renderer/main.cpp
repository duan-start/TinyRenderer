
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



//你给我的是什么阉割版，向量里面什么都没有
Vec3f cross(const Vec3f& a, const Vec3f& b) {
    return Vec3f{
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

//三角形绘画，我直接从显卡的那种绘画方式，cpu的绘画方式有点麻
//重心坐标和boundingbox的方式
Vec3f barycentric(Vec3f* pts, Vec3f P) {
    Vec3f u = Vec3f(pts[2].x - pts[0].x, pts[1].x - pts[0].x, pts[0].x - P.x) ^
        Vec3f(pts[2].y - pts[0].y, pts[1].y - pts[0].y, pts[0].y - P.y);
    if (std::abs(u.z) < 1e-6) return Vec3f(-1, 1, 1); // 防止三角形退化
    return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}




Vec3f getTexelColor(const std::vector<unsigned char>& textureData, int width, int height, const Vec2f& uv) {
    int x = static_cast<int>(uv.u * width);
    int y = static_cast<int>(uv.v * height);
    x = std::clamp(x, 0, width - 1);

    // Replace the line causing the error with the following code
    x = std::max(0, std::min(x, width - 1));
    y = std::clamp(y, 0, height - 1);

    int index = (y * width + x) * 3; // 计算该像素的起始索引  
    return Vec3f{
        textureData[index] / 255.0f, // Red  
        textureData[index + 1] / 255.0f, // Green  
        textureData[index + 2] / 255.0f  // Blue  
    };
}
//不需要自己写读取函数，真的很麻烦，还总是错的，感觉是他自己改了些数据

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
Model* model = NULL;
const int width = 800;
const int height = 800;


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


//void triangle(Vec3f* pts, float* zbuffer, TGAImage& image, const std::vector<TGAColor>& color) {
//   //初始化包围盒,小的初始化为最大，大的初始化为最小
//    Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
//    Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
//    Vec2f clamp(image.get_width() - 1, image.get_height() - 1);
//    
//    //获得最大值,最小值
//    for (int i = 0; i < 3; i++) {
//        for (int j = 0; j < 2; j++) {
//            bboxmin[j] = std::fmax(0.f, std::fmin(bboxmin[j], pts[i][j]));
//            bboxmax[j] = std::fmin(clamp[j], std::fmax(bboxmax[j], pts[i][j]));
//        }
//    }
//    //根据深度值进行是否更新
//    Vec3f P;
//    Vec3f C (0.,0.,0.);
//    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
//        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
//            //获得p点的重心表示
//            Vec3f bc_screen = barycentric(pts[0],pts[1],pts[2], P);
//            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
//            P.z = 0;
//            //通过重心坐标获取实际的深度值
//            for (int i = 0; i < 3; i++) {
//            P.z += pts[i][2] * bc_screen[i]; 
//            //深度值是负的
//            C.x += color[i].r * bc_screen[i];
//            C.y += color[i].g * bc_screen[i];
//            C.z += color[i].b * bc_screen[i];
//            }
//            //判断和更新
//            if (abs(zbuffer[int(P.x + P.y * width)]) >abs( P.z)) {
//                zbuffer[int(P.x + P.y * width)] = P.z;
//                image.set(P.x, P.y, TGAColor(C,255.f));
//            }
//
//        }
//    }
//}



void triangle(Vec3f* pts, Vec2f* tex, float* zbuffer, TGAImage& image, TGAImage& texture) {
    Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    Vec2f clamp(image.get_width() - 1, image.get_height() - 1);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            bboxmin[j] = std::fmax(0.f, std::fmin(bboxmin[j], pts[i][j]));
            bboxmax[j] = std::fmin(clamp[j], std::fmax(bboxmax[j], pts[i][j]));
        }
    }

    Vec3f P;
    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
            Vec3f bc_screen = barycentric(pts[0], pts[1], pts[2], P);
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
            P.z = 0;
            Vec2f uv(0, 0);
            for (int i = 0; i < 3; i++) {
                P.z += pts[i][2] * bc_screen[i];
                uv.x += tex[i].x * bc_screen[i];
                uv.y += tex[i].y * bc_screen[i];
            }
            //根据插值去找uv坐标，然后取取颜色，而不是把顶点对应的uv坐标的纹理颜色拿过来做插值
            if (zbuffer[int(P.x + P.y * width)] < P.z) {
                zbuffer[int(P.x + P.y * width)] = P.z;
                TGAColor color = texture.get(uv.x * texture.get_width(), (1-uv.y) * texture.get_height());
                image.set(P.x, P.y, color);
            }
        }
    }
}


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

    float* zbuffer = new float[width * height];
    for (int i = 0; i < width * height; i++) {
        zbuffer[i] = -std::numeric_limits<float>::max();
    }

    TGAImage image(width, height, TGAImage::RGB);

    const char* filename = "./obj/diffuse.tga";
    TGAImage texture;
   // texture.flip_vertically();
    if (!texture.read_tga_file(filename)) {
        std::cerr << "Failed to load texture" << std::endl;
        delete model;
        delete[] zbuffer;
        return -1;
    }

    for (int i = 0; i < model->nfaces(); i++) {
        Face face = model->face(i);
        Vec3f pts[3];
        Vec3f world_coords[3];
        Vec2f world_tex[3];

        for (int j = 0; j < 3; j++) {
            world_coords[j] = model->vert(face.vertexIndices[j]);
            pts[j] = world2screen(model->vert(face.vertexIndices[j]));
            world_tex[j] = model->text(face.texCoordIndices[j]);
        }


        triangle(pts,world_tex, zbuffer, image, texture);
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");

    delete[] zbuffer;
    delete model;
    return 0;
}