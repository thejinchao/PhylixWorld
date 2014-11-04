#ifndef __PHYSICS_WORLD_LIB_RENDER_INTERFACE_H__
#define __PHYSICS_WORLD_LIB_RENDER_INTERFACE_H__

#include "libPhylix_Color.h"

namespace Phylix
{

/** 渲染接口
*/
class IRenderInterface
{
public:
	virtual void drawPolygon(const fVector2* vertices, unsigned int vertexCount, const Material& material, bool solid, bool border, bool selected) = 0;
	virtual void drawCircle(const fVector2& center, Real radius, const fColor& color) = 0;
	virtual void drawSolidCircle(const fVector2& center, Real radius, bool drawAxis, const fVector2& axis, const Material& material, bool selected) = 0;

	virtual void drawSegment(const fVector2& p1, const fVector2& p2, Real width, const fColor& color) = 0;
	virtual void drawSegmentArrary(const fVector2* vertices, unsigned int vertexCount, const fColor& color) = 0;

	virtual void drawSpring(const fVector2& p1, const fVector2& p2, const fColor& color, Real width, Real targetHeight, bool selected) = 0;

	virtual void drawCross(const fVector2& point, Real size, const fColor& color) = 0;
	virtual void drawArrow(const fVector2& start, const fVector2& end, const fColor& color) = 0;
};

}

#endif
