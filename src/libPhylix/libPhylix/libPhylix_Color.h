#ifndef __PHYSICS_WORLD_LIB_COLOR_H__
#define __PHYSICS_WORLD_LIB_COLOR_H__

namespace Phylix
{
/** 颜色
*/
struct fColor
{
	fColor() : r(0.f), g(0.f), b(0.f), a(0.f) {}
	fColor(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}

	float r,g,b,a;
};


}

#endif
