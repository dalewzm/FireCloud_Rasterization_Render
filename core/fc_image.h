#ifndef __CORE_FCIMAGE_H__
#define __CORE_FCIMAGE_H__
// just normal tga image, but use universal names
// tga file parser
//http://www.twinklingstar.cn/2013/471/tga-file-format/
#include <cstdint>
#include <fstream>
#include <vector>

#pragma pack(push, 1)
struct TGA_Header{
 std::uint8_t idLength{};
 std::uint8_t colorMapType;
 std::uint8_t  dataTypeCode{};
 std::uint16_t colorMapEntry{};
 std::uint16_t colorMapLength{};
 std::uint8_t  colorMapDepth{};
 std::uint16_t x_origin{};
 std::uint16_t y_origin{};
 std::uint16_t width{};
 std::uint16_t height{};
 std::uint8_t bitsPerpixel{};
 std::uint8_t imageDescriptor{};
};
#pragma pack(pop)

struct FcColor{
	std::uint8_t bgra[4] = {0,0,0,0};
    std::uint8_t bytespp = {0};

    FcColor() = default;
    FcColor(const std::uint8_t R, const std::uint8_t G, const std::uint8_t B, const std::uint8_t A=255) : bgra{B,G,R,A}, bytespp(4) { }
    FcColor(const std::uint8_t v) : bgra{v,0,0,0}, bytespp(1) { }
};

class FcImage{
protected:
    std::vector<std::uint8_t> data;
    int width;
    int height;
    int bytespp;

    bool load_rle_data(std::ifstream &in);
    bool unload_rle_data(std::ofstream &out) const;

	bool safe_write(std::ofstream &out, char* const buffer, size_t write_len) const;

public:
 	enum Format { GRAYSCALE=1, RGB=3, RGBA=4 };

    FcImage();
    FcImage(const int w, const int h, const int bpp);
    bool read_tga_file(const std::string filename);
    bool write_tga_file(const std::string filename, const bool vflip=true, const bool rle=true) const;
    void flip_horizontally();
    void flip_vertically();
    void scale(const int w, const int h);
    FcColor get(const int x, const int y) const;
    void set(const int x, const int y, const FcColor &c);
    int get_width() const;
    int get_height() const;
    int get_bytespp();
    std::uint8_t *buffer();
	std::uint8_t *buffer() const;
    void clear();

	

};


#endif