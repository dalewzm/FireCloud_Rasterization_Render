#include "core/fc_common.h"
#include "crossplatform/window.h"


const FcColor white = FcColor(255, 255, 255, 255);
const FcColor red = FcColor(255, 0, 0, 255);

int main(int argc, char*argv)
{
	FcImage image(100, 100, FcImage::RGB);
	image.set(30, 60, red);
	image.write_tga_file("out.tga", false, false);
	return 0;
}


