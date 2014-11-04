#include "StdAfx.h"
#include "Phylix_FileStream.h"

namespace Phylix
{
//-------------------------------------------------------------------------------------------------------------------
FileStream::FileStream(const char* fileName, const char* flag)
{
	m_fp = fopen(fileName, flag);
	fseek(m_fp, 0, SEEK_END);
	m_size = ftell(m_fp);
	fseek(m_fp, 0, SEEK_SET);
}

//-------------------------------------------------------------------------------------------------------------------
FileStream::~FileStream()
{
	fclose(m_fp);
}

//-------------------------------------------------------------------------------------------------------------------
size_t FileStream::read(unsigned char* buf, size_t size)
{
	return fread(buf, 1, size, m_fp);
}

//-------------------------------------------------------------------------------------------------------------------
size_t FileStream::write(const unsigned char* buf, size_t size)
{
	return fwrite(buf, 1, size, m_fp);
}

//-------------------------------------------------------------------------------------------------------------------
size_t FileStream::writeParam(const char* param, ...)
{
	char szTemp[1024]={0};

	va_list ptr; va_start(ptr, param);
	_vsnprintf((char *)szTemp, 1024, param, ptr);
	va_end(ptr);

	return write((const unsigned char*)szTemp, strlen(szTemp));
}

}
