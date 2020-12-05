#include "core/fc_common.h"
#include "crossplatform/window.h"
#include "core/model.h"
#include <iostream>

const FcColor white = FcColor(255, 255, 255, 255);
const FcColor red = FcColor(255, 0, 0, 255);
const int width = 300;
const int height = 300;

void line(int x0, int y0, int x1, int y1, TgaImage& image, FcColor color)
{
	bool transposed = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		transposed = true;
	}
	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror = std::abs(dy) * 2;
	int error = 0;
	int y = y0;
	for (int x = x0; x <= x1; ++x)
	{
		/*float t = (x - x0) / (float)(x1 - x0);
		int y = t*y1 + (1 - t)*y0;*/
		if(transposed)
			image.set(y, x, color);
		else
			image.set(x, y, color);
		error += derror;
		if (error > dx)
		{
			y += y0 >= y1 ? -1 : 1;
			error -= 2*dx;
		}
	}
}

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TgaImage &image, FcColor color) {
	//sort 3 vert by y value
	if (t0.y > t1.y)
		std::swap(t0, t1);
	if (t0.y > t2.y)
		std::swap(t0, t2);
	if (t1.y > t2.y)
		std::swap(t1, t2);
	
	//lower triangle
	int y0(t0.y), y1(t1.y), y2(t2.y);
	int x0(t0.x), x1(t1.x), x2(t2.x);
	for (int y = t0.y; y <= y1; ++y)
	{
		//left line
		auto lx = x0 + (y - y0)*(x2 - x0) / (float)(y2 - y0);
		auto rx = x0 + (y - y0)*(x1 - x0) / (float)(y1 - y0);
		line(lx, y, rx, y, image, color);
	}
	//upper triangle
	for (int y = y1; y <= y2; ++y)
	{
		auto lx = x2 + (y - y2)*(x2 - x0) / (float)(y2 - y0);
		auto rx = x2 + (y - y2)*(x2 - x1) / (float)(y2 - y1);
		line(lx, y, rx, y, image, color);
	}
}

void test_wireframe_mode(TgaImage& image)
{
	//image need 800x800
	Model *model = new Model("res/test.obj");
	for (int i = 0; i < model->num_faces(); ++i)
	{
		auto face = model->face(i);
		for (int j = 0; j < 3; ++j)
		{
			auto v = model->vert(face[j]);
			auto v1 = model->vert(face[(j + 1) % 3]);
			//turn ndc to viewport coordinate
			int x0 = (v.x + 1.0)*width / 2;
			int y0 = (v.y + 1.0)*height / 2;
			int x1 = (v1.x + 1.0)* width / 2;
			int y1 = (v1.y + 1.0)* height / 2;
			line(x0, y0, x1, y1, image, white);
		}

	}
	delete model;
}


class App
{
public:
	App():main_window(nullptr),buffer(width, height, TgaImage::RGB){}
	bool setup_render_context()
	{
		main_window = create_window("test", 300, 300);
		if (main_window) {
			Vec2i t0[3] = { Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80) };
			Vec2i t1[3] = { Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180) };
			Vec2i t2[3] = { Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180) };
			triangle(t0[0], t0[1], t0[2], buffer, red);
			triangle(t1[0], t1[1], t1[2], buffer, white);
			buffer.flip_vertically();
			//buffer.write_tga_file("out.tga", false, false);
			return true;
		}
		return false;
	}
	void main_loop()
	{
		if (setup_render_context())
		{
			while (!window_should_close(main_window))
			{
				window_draw_image(main_window, buffer);
			}
		}
	}
private:
	window_t* main_window;
	TgaImage buffer;
};



int main(int argc, char*argv)
{
	//Model *model = new Model("res/african_head.obj");

	//TgaImage image(width, height, TgaImage::RGB);
	
	App app;
	app.main_loop();
	
	//image.flip_vertically();
	//image.write_tga_file("out.tga", false, false);
	
	return 0;
}


