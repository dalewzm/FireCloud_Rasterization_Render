#include "core/fc_common.h"
#include "crossplatform/window.h"


const FcColor white = FcColor(255, 255, 255, 255);
const FcColor red = FcColor(255, 0, 0, 255);

void line(int x0, int y0, int x1, int y1, FcImage& image, FcColor color)
{
	bool transposed = false;
	if (std::abs(x0 - x1) < std::abs(x1 - y1))
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
			y += y0 > y1 ? -1 : 1;
			error -= 2*dx;
		}
	}
}

int main(int argc, char*argv)
{
	FcImage image(100, 100, FcImage::RGB);
	line(13, 20, 80, 40, image, white);
	line(20, 13, 40, 80, image, red);
	line(80, 40, 13, 20, image, red);
	image.flip_vertically();
	image.write_tga_file("out.tga", false, false);

	return 0;
}


