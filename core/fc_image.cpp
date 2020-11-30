#include <iostream>
#include <fstream>
#include "fc_image.h"


FcImage::FcImage(const int w, const int h, const int bpp):width(w),height(h),bytespp(bpp)
{
	data.resize(w*h*bpp, 0);
}

bool FcImage::read_tga_file(const std::string filename)
{
	std::fstream fio(filename);
	if(!fio.is_open())
	{
		std::cerr << "can't open file, please check is a valid tga file " << filename.c_str() << std::endl;
        fio.close();
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
	int bpp = header.bitsPerpixel >> 3;
	if(w<=0 || h<=0 || (bpp!=GRAYSCALE && bpp!=RGB && bpp!=RGBA))
	{
		std::cerr<<"wrong tga file size or tga color style"<<std::endl;
		fio.close();
		return false;
	}
	size_t sum = w*h*bpp;
	auto data = std::vector<std::uint8_t>(sum, 0);
	// 2,3 uncompress, color gray image, 10,11 rle image
	if(2 == header.dataTypeCode || 3 == header.dataTypeCode)
	{
		fio.read((char*)(data.data()), sum);
		if(!fio.good())
		{
			fio.close();
			return false;
		}
	}
	else if(10 == header.dataTypeCode || 11 == header.dataTypeCode)
	{
		
	}
	else{
		fio.close();
		std::cerr<<"unknown file format"<<std::endl;
		return false;
	}
	//bit 4 表示从左到右，bit 5表示从上到下
	if(!(header.imageDescriptor & 0x20))
		flip_vertically();
	if(header.imageDescriptor & 0x10)
		flip_horizontally();
	
	fio.close();
	return true;
}


bool FcImage::safe_write(std::ofstream &out,  char*const buffer, size_t write_len) const
{
	out.write(buffer, write_len);
	if(!out.good())
	{
		std::cerr<<"write tga file fail"<<std::endl;
		return false;
	}
	return true;
}

bool FcImage::write_tga_file(const std::string filename, const bool vfliped, const bool rle) const
{
	std::uint8_t developer_area_ref[4] = {0, 0, 0, 0};
	std::uint8_t extension_area_ref[4] = {0, 0, 0, 0};
	std::uint8_t footer[18] = {'T','R','U','E','V','I','S','I','O','N','-','X','F','I','L','E','.','\0'};
	std::ofstream fou;
	fou.open(filename, std::ios::binary);
	if(!fou.is_open()){
		std::cerr<<"open file fail"<<filename.c_str()<<std::endl;
	}
	TGA_Header header;
	header.width = width;
	header.height = height;
	header.bitsPerpixel = bytespp << 3;
	header.imageDescriptor = vfliped ? 0x00 : 0x20;

	if(rle)
	{

	}
	else{
		if(bytespp == GRAYSCALE)
			header.dataTypeCode = 3;
		else
			header.dataTypeCode = 2;
		fou.write(reinterpret_cast<char*>(&header), sizeof(header));
		size_t num = width * height * bytespp;
		fou.write(reinterpret_cast<const char *>(buffer()), num);
	}
	auto ret = safe_write(fou, reinterpret_cast<char*>(developer_area_ref), sizeof(developer_area_ref));
	if (!ret)
		return false;
	ret = safe_write(fou, reinterpret_cast<char*>(extension_area_ref), sizeof(extension_area_ref));
	if (!ret)
		return false;
	ret = safe_write(fou, reinterpret_cast<char*>(footer), sizeof(footer));
	fou.close();
	return ret;
}


void FcImage::flip_horizontally()
{

}


void FcImage::flip_vertically()
{

}

void FcImage::set(const int x, const int y, const FcColor &c)
{
	if (data.size() <= 0 || width < 0 || height < 0)
		return;
	memcpy(data.data() + (x + y*width)*bytespp, c.bgra, bytespp);
}

std::uint8_t * FcImage::buffer()
{
	return data.data();
}

std::uint8_t * FcImage::buffer() const
{
	return (std::uint8_t* const)(data.data());
}
