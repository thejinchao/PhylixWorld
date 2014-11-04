#pragma once

namespace Phylix
{

/** ÎÄ¼þÁ÷
*/
class FileStream : public IFileStream
{
public:
	virtual size_t size(void) const { return m_size; }
	virtual size_t read(unsigned char* buf, size_t size);
	virtual size_t write(const unsigned char* buf, size_t size);
	virtual size_t writeParam(const char* param, ...);

private:
	FILE* m_fp;
	size_t m_size;

public:
	FileStream(const char* fileName, const char* flag);
	virtual ~FileStream();
};

}
