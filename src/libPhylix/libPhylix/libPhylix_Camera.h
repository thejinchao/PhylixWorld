#ifndef __PHYSICS_WORLD_LIB_CAMERA_H__
#define __PHYSICS_WORLD_LIB_CAMERA_H__

#include "libPhylix_Handler.h"

namespace Phylix
{
/** 记录摄像机数据
*/
class Camera
{
public:
	/** 摄像机发生变化时的回调函数 */
	typedef void (* onCameraAdjust)(int sreenWidth, int screenHeight, const fVector2& lowPos, const fVector2& upperPos);

public:
	/** 设置窗口大小 */
	void setWindowSize(int width, int height);
	/** 获得窗口大小 */
	const iVector2& getWindowSize(void) const { return m_winSize; }

	/** 屏幕坐标转换为逻辑坐标 */
	fVector2 convertScreenToWorld(int x, int y, bool gridSnap, bool* closeEnough=0);

	/** 屏幕长度转换为逻辑长度 */
	Real convertScreenSizeToWorld(Real size);

	/** 设置放缩系数 */
	void setViewZoom(Real viewZoom);
	/** 获得放缩系数 */
	Real getViewZoom(void) const { return m_viewZoom; }

	/** 设置GRID宽度 */
	void setGridStep(Real gridStep);

	/** 移动摄像机 */
	void pan(const fVector2& offset);

protected:
	/** 更新 */
	void _update(void);

protected:
	enum { DEFAULT_HALF_HEIGHT = 3, };	//!< 缺省一半高度屏幕对应的现实高度

	iVector2 m_winSize;			//!< 窗口大小
	Real m_viewZoom;			//!< 放缩系数，缺省1.0				
	fVector2 m_viewCenter;		//!< 视点中心，缺省[0,0]
	onCameraAdjust m_callBack;	//!< 回调函数
    Real m_gridStep;			//!< grid宽度

public:
	Camera(onCameraAdjust callBack);
	~Camera();
};

}

#endif

