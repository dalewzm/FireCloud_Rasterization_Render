#include <fstream>
#include "fc_image.h"


bool FcImage::read_tga_file(const std::string filename)
{
	fstream fio(filename);
	if(!fio.is_open())
	{
		std::cerr << "can't open file, please check is a valid tga file " << filename << std::endl;
        in.close();
        return false;
	}
	TGA_Header header;
	fio.read(reinterpret_cast<char*> (&header), sizeof(header));
	if(!fio.good())
	{
		std::cerr << "read tga file fail"<<"\n";
		fio.close();
		return false;
	}
	int w = header.width;
	int h = header.height;
	// byte = bits/8
	int bpp = header.bitsPerpixel>> 3;
	if(w<=0 || h<=0 || (bpp!=GRAYSCALE && bpp!=RGB && bpp!=RGBA))
	{
		std::cerr<<"wrong tga file size or tga color style";
		fio.close();
		return false;
	}
	size_t sum = w*h*bpp;
	auto data = std::vector<std::uint8_t>(sum, 0);
}
