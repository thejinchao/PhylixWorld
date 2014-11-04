#pragma once

namespace Phylix
{
/** ª≠≤º¿‡
*/
class Canvas : public IRenderInterface
{
public:
	virtual void drawPolygon(const fVector2* vertices, unsigned int vertexCount, const Material& material, bool solid, bool border, bool selected);
	virtual void drawCircle(const fVector2& center, Real radius, const fColor& color);
	virtual void drawSolidCircle(const fVector2& center, Real radius, bool drawAxis, const fVector2& axis, const Material& material, bool selected);

	virtual void drawSegment(const fVector2& p1, const fVector2& p2, Real width, const fColor& color);
	virtual void drawSegmentArrary(const fVector2* vertices, unsigned int vertexCount, const fColor& color);

	virtual void drawSpring(const fVector2& p1, const fVector2& p2, const fColor& color, Real width, Real targetHeight, bool selected);

	virtual void drawCross(const fVector2& point, Real size, const fColor& color);
	virtual void drawArrow(const fVector2& start, const fVector2& end, const fColor& color);
public:
	Canvas();
	~Canvas();

	static Canvas& getSingleton(void) { return *s_pSingleton; }
	static Canvas* getSingletonPtr(void) { return s_pSingleton; }

private:
	static Canvas* s_pSingleton; 
};

}

