#pragma once

#include "libPhylix_Action.h"
#include "libPhylix_Color.h"

namespace Phylix
{
//pre-define
class Material;

/** 边界体创建
*/
class Action_Creater_Edge : public Action
{
public:
	/** 获得类型 */
	virtual ACTION_TYPE getType(void) { return AT_CREATER_EDGE; }
	virtual void draw(IRenderInterface* render);

protected:
	virtual void _onBegin(const fVector2& pt);
	virtual void _onDrag(const fVector2& delta);
	virtual void _onEnd(const fVector2& pt, bool canceled);

private:
	fColor m_randomColor;	//!< 临时材质颜色
	fVector2 m_normal;		//!< 法线方向
	Polygon m_polygon;		//!< 用于绘制的多边形

	//for assist circle
	enum { ASSIST_CIRCLE_RADIUS = 100 };
	
public:
	Action_Creater_Edge();
	virtual ~Action_Creater_Edge();
};


}
