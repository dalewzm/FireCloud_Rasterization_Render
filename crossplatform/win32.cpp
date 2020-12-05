#include <windows.h>
#include "window.h"



struct window {
	HWND handle;
	HDC memory_dc; // 储存着，窗口销毁需要回收的
	//TgaImage image;
	int width;
	int height;
	int bpp;
	int should_close;
	std::uint8_t* buf;
	window(int w, int h) :width(w),height(h),bpp(4),buf(nullptr),should_close(0){}
};

#ifdef UNICODE
static const wchar_t *WINDOW_CLASS_NAME = L"window_class_name";
#else
static const char* WINDOW_CLASS_NAME = "window_class_name";
#endif

static const char* WINDOW_ENTRY_NAME = "window_entry";

LRESULT CALLBACK msg_process_func(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

static int register_class() 
{
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = msg_process_func;  
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = GetModuleHandle(NULL);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = WINDOW_CLASS_NAME;
	return RegisterClass(&wndclass);
}

//CreateDibSection to create a DIBSection.
//Copy your data to the memory block returned by CreateDibSection.
//create a DC compatible with the target DC.
//Select the DIBSection into your newly created DC.
//BitBlt(or StretchBlt) from your DC to the target DC.
static bool create_bitmap_device(HWND hwnd, int width, int height, std::uint8_t* &buf_ptr, HDC  &m_dc)
{
	HDC hdc, memory_dc;
	BITMAPINFOHEADER bi_header;
	HBITMAP new_bitmap, old_bitmap;
	hdc = GetDC(hwnd);
	if (!hdc)
		return false;
	unsigned char* buffer;
	// Create a memory device context compatible with the device. 
	memory_dc = CreateCompatibleDC(hdc);
	ReleaseDC(hwnd, memory_dc); //release dc 与 getdc 成对使用

	memset(&bi_header, 0, sizeof(BITMAPINFOHEADER));
	bi_header.biSize = sizeof(BITMAPINFOHEADER);
	bi_header.biWidth = width;
	bi_header.biHeight = -height;  /* top-down */
	bi_header.biPlanes = 1;
	bi_header.biBitCount = 32;
	bi_header.biCompression = BI_RGB;
	
	
	new_bitmap = CreateDIBSection(memory_dc, (BITMAPINFO*)&bi_header,
		DIB_RGB_COLORS, (void**)&buffer, NULL, 0);
	
	old_bitmap = (HBITMAP)SelectObject(memory_dc, new_bitmap);
	DeleteObject(old_bitmap);

	m_dc = memory_dc;
	buf_ptr = buffer;
	return true;
}


LRESULT CALLBACK msg_process_func(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	window_t* window = (window_t*)GetProp(hwnd, WINDOW_ENTRY_NAME);
	if (window == NULL) {
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	else if (message == WM_CLOSE) {
		window->should_close = 1;
		return 0;
	}
	
	else {
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}



static HWND __create_window(const char* title_, unsigned int width, unsigned int height)
{
	DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	RECT rect;
	HWND handle{};

#ifdef UNICODE
	wchar_t title[LINE_SIZE];
	mbstowcs(title, title_, LINE_SIZE);
#else
	const char* title = title_;
#endif

	rect.left = 0;
	rect.top = 0;
	rect.right = width;
	rect.bottom = height;
	AdjustWindowRect(&rect, style, 0);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	handle = CreateWindow(WINDOW_CLASS_NAME, title, style,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		NULL, NULL, GetModuleHandle(NULL), NULL);
	
	return handle;
}

window_t * create_window(const char* title, unsigned int width, unsigned int height)
{
	
	if (!register_class())
		return nullptr;
	window_t* window_ptr = new window_t(width, height);
	window_ptr->handle = __create_window(title, width, height);
	//window_ptr->image.
	if (!create_bitmap_device(window_ptr->handle, width, height, (window_ptr->buf), window_ptr->memory_dc))
	{
		destroy_window(window_ptr);
		return nullptr;
	}
	SetProp(window_ptr->handle, WINDOW_ENTRY_NAME, window_ptr);
	ShowWindow(window_ptr->handle, SW_SHOW);
	return window_ptr;
}

int window_should_close(window_t* window) 
{
	return window->should_close;
}

static void present_surface(window_t* window) {
	HDC window_dc = GetDC(window->handle);
	HDC memory_dc = window->memory_dc;
	//image_t* surface = window->surface;
	int width = window->width;
	int height = window->height;
	BitBlt(window_dc, 0, 0, width, height, memory_dc, 0, 0, SRCCOPY);
	ReleaseDC(window->handle, window_dc);
}

void window_draw_image(window_t* window_ptr, TgaImage& image)
{
	auto width = window_ptr->width;
	auto height = window_ptr->height;
	unsigned char* dst_value(0);
	FcColor color;
	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			//int flipped_r = src->height - 1 - r;
			//vec4_t src_value = get_buffer_value(src, flipped_r, c);
			//unsigned char* dst_pixel = private_get_pixel(dst, r, c);
			color = image.get(c, r);
			dst_value = &(window_ptr->buf[ (r * width + c) * 4]);
			dst_value[0] = color.bgra[2];  /* red */
			dst_value[1] = color.bgra[1];  /* green */
			dst_value[2] = color.bgra[0];  /* blue */
		}
	}
	
	present_surface(window_ptr);
}

void destroy_window(window_t* window_ptr)
{
	delete window_ptr;
}



