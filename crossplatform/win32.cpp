#include <windows.h>
#include "window.h"

//struct bitmap_meta
//{
//	int width;
//
//};


struct window {
	HWND handle;
	HDC memory_dc; // 储存着，窗口销毁需要回收的
	TgaImage *image_ptr;

	int should_close;
};

#ifdef UNICODE
static const wchar_t *WINDOW_CLASS_NAME = L"window_class_name";
#else
static const char* WINDOW_CLASS_NAME = "window_class_name";
#endif

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

static bool create_bitmap_device(HWND hwnd, int width, int height, TgaImage &image, HDC  &m_dc)
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
	/*surface = (image_t*)malloc(sizeof(image_t));
	surface->width = width;
	surface->height = height;
	surface->channels = 4;
	surface->buffer = buffer;*/
	return true;
}


LRESULT CALLBACK msg_process_func(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}



static HWND __create_window(const char* title, unsigned int width, unsigned int height)
{
	return 0;
}

window_t * create_window(const char* title, unsigned int width, unsigned int height)
{
	if (!register_class())
		return nullptr;

}

