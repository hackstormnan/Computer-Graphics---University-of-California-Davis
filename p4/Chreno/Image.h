#pragma once
class Image
{
public:
	Image(const char* p_path);
	~Image();

	int GetWidth();
	int GetHeight();
	int GetBytesPerPixel();
	unsigned char* GetBuffer();
private:
	int m_width;
	int m_height;
	int m_bytesPerPixel; 
	unsigned char * m_localBuffer;
};