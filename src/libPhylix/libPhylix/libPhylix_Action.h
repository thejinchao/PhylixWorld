#ifndef __PHYSICS_WORLD_LIB_ACTION_H__
#define __PHYSICS_WORLD_LIB_ACTION_H__


namespace Phylix
{
//pre-define
class IRenderInterface;
class ActionManager;

/** 操作类型*/
enum ACTION_TYPE
{
	AT_NULL=0,

	AT_SELECT,				//!< 选择物体

	AT_CAMERA_PAN,			//!< 摄像机平移
	AT_CAMERA_ZOOM,			//!< 摄像机放缩

	AT_CREATER_EDGE,		//!< 边界创建
	AT_CREATER_BOX,			//!< 方盒物体创建
	AT_CREATER_CIRCLE,		//!< 圆形物体创建
	AT_CREATER_POLYGON,		//!< 多边形物体创建

	AT_CREATE_FIX_JOINT,		//!< 创建固定铰链
	AT_CREATE_HINGE_JOINT,		//!< 创建旋转铰链
	AT_CREATE_SPRING_JOINT,		//!< 创建弹簧

	AT_CREATE_CHAIN,		//!< 创建链子

	AT_DRAG_OBJECT,			//!< 拖动物体

};


/** 操作基类
*/
class Action
{
public:
	/** 获得类型 */
	virtual ACTION_TYPE getType(void) = 0;

	void onBegin(int x, int y);

	void onMotion(int x, int y);

	void onEnd(int x, int y, bool canceled = false);

	void onUpdate(const iVector2& pt, float seconds) { onUpdate(pt.x, pt.y, seconds); }
	void onUpdate(int x, int y, float seconds);

	virtual void draw(IRenderInterface* render) {}

protected:
	bool m_dragStarted;			//!< 是否开始“拖动”操作
	fVector2 m_dragOrigin;		//!< 开始拖动的起点
	fVector2 m_dragCurrent;		//!< 当前拖动点
	fVector2 m_dragDelta;		//!< 拖动偏移点

protected:
	virtual void _onBegin(const fVector2& pt) {}
	virtual void _onDrag(const fVector2& delta) {}
	virtual void _onClick(const fVector2& pt) {}
	virtual void _onEnd(const fVector2& pt, bool canceled) {}
	virtual void _onUpdate(const fVector2& pt, float seconds) {}
	virtual bool _needCameraGrid(void) const { return true; }

protected:
	Action();
	virtual ~Action();
	friend class ActionManager;
};

}

#endif
