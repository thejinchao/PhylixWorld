#pragma once

#include "libPhylix_Action.h"
#include "libPhylix_Color.h"

namespace Phylix
{
//pre-define
class Material;

/** 边界体创建
*/
class Action_Creater_Polygon : public Action
{
public:
	/** 获得类型 */
	virtual ACTION_TYPE getType(void) { return AT_CREATER_POLYGON; }
	virtual void draw(IRenderInterface* render);

protected:
	virtual void _onBegin(const fVector2& pt);
	virtual void _onDrag(const fVector2& delta);
	virtual void _onEnd(const fVector2& pt, bool canceled);

private:
	fColor m_randomColor;		//!< 临时材质颜色
	PointVector m_ptArray;		//!< 笔迹

	//!< 相交点
	struct CrossPoint
	{
		int firstIndex;		//!< 主动相交线段序列号
		int secondIndex;	//!< 被动相交点序列号
		fVector2 ptCross;	//!< 相交点坐标
		fVector2 ptFristSegStart;  //!< 主动线段的起始点
	};

	//!< 相交点排序工具类
	struct CrossPointSortHelper
	{
		bool operator()(const struct CrossPoint& pt1, const struct CrossPoint& pt2);
	};

public:
	Action_Creater_Polygon();
	virtual ~Action_Creater_Polygon();
};


}
