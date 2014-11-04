#ifndef __PHYSICS_WORLD_LIB_TYPEDEFINE_H__
#define __PHYSICS_WORLD_LIB_TYPEDEFINE_H__

namespace Phylix
{

//Real type
typedef float Real;

//std function
#ifdef _MSC_VER
	#define snprintf	_snprintf
	#define strcasecmp	_stricmp
	#define MIN			min
	#define MAX			max
#else
    #ifndef MIN
        #define MIN			std::min
    #endif
    #ifndef MAX
        #define MAX			std::max
    #endif
#endif

}


#endif
