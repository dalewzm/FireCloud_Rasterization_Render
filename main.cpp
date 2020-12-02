#include "core/fc_common.h"
#include "crossplatform/window.h"
#include "core/model.h"
#include <iostream>

const FcColor white = FcColor(255, 255, 255, 255);
const FcColor red = FcColor(255, 0, 0, 255);
const int width = 800;
const int height = 800;

void line(int x0, int y0, int x1, int y1, FcImage& image, FcColor color)
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

int main(int argc, char*argv)
{
	//Model *model = new Model("res/african_head.obj");
	Model *model = new Model("res/test.obj");
	FcImage image(width, height, FcImage::RGB);
	
	for (int i = 0; i < model->num_faces(); ++i)
	{
		auto face = model->face(i);
		//if (face.size() != 3)
		
		for (int j = 0; j < 3; ++j)
		{
			auto v = model->vert(face[j]);
			auto v1 = model->vert(face[(j + 1) % 3]);
			//turn ndc to viewport coordinate
			int x0 = (v.x + 1.0)*width / 2;
			int y0 = (v.y + 1.0)*height / 2;
			int x1 = (v1.x + 1.0)* width / 2;
			int y1 = (v1.y + 1.0)* height / 2;
			//fou << x0 <<" "<<x1<<" " <<y0<< " "<<y1<< std::endl;
			line(x0, y0, x1, y1, image, white);
		}

	}
	
	image.flip_vertically();
	image.write_tga_file("out.tga", false, false);
	
	return 0;
}


