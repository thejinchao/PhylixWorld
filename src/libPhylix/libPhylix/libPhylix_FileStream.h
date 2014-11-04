#ifndef __PHYSICS_WORLD_LIB_FILESTREAM_INTERFACE_H__
#define __PHYSICS_WORLD_LIB_FILESTREAM_INTERFACE_H__

#include "libPhylix_Color.h"

namespace Phylix
{

/** 文件操作接口
*/
class IFileStream
{
public:
	virtual size_t size(void) const = 0;
	virtual size_t read(unsigned char* buf, size_t size) = 0;
	virtual size_t write(const unsigned char* buf, size_t size) = 0;
	virtual size_t writeParam(const char* param, ...) = 0;
};

}

#endif
