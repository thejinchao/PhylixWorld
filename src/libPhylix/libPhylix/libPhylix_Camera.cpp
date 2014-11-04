#include "libPhylix_stdafx.h"
#include "libPhylix_Camera.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
Camera::Camera(onCameraAdjust callBack)
	: m_callBack(callBack)
	, m_viewZoom(1.f)
	, m_viewCenter(0.f, 0.f)
	, m_gridStep(0.0)
{
}

//-------------------------------------------------------------------------------------------------------------------
Camera::~Camera()
{
}

//-------------------------------------------------------------------------------------------------------------------
void Camera::_update(void)
{
	Real ratio = (Real)m_winSize.x/(Real)m_winSize.y;

	fVector2 extents(ratio*(Real)DEFAULT_HALF_HEIGHT, (Real)DEFAULT_HALF_HEIGHT);
	extents /= m_viewZoom;

	fVector2 lower = m_viewCenter - extents;
	fVector2 upper = m_viewCenter + extents;

	if(m_callBack)
	{
		m_callBack(m_winSize.x, m_winSize.y, lower, upper);
	}
}

//-------------------------------------------------------------------------------------------------------------------
fVector2 Camera::convertScreenToWorld(int x, int y, bool cameraGrid, bool* closeEnough)
{
	Real u = x / float(m_winSize.x);
	Real v = (m_winSize.y - y) / Real(m_winSize.y);

	Real ratio = (Real)m_winSize.x/(Real)m_winSize.y;

	fVector2 extents(ratio*(Real)DEFAULT_HALF_HEIGHT, (Real)DEFAULT_HALF_HEIGHT);
	extents /= m_viewZoom;

	fVector2 lower = m_viewCenter - extents;
	fVector2 upper = m_viewCenter + extents;

	fVector2 p;

	p.x = (1.0f - u) * lower.x + u * upper.x;
	p.y = (1.0f - v) * lower.y + v * upper.y;
    
    if(closeEnough)
    {
        *closeEnough = true;
    }
    
	if(cameraGrid && m_gridStep!=0.0)
	{
		Real _x = (int)(p.x / m_gridStep + (p.x >= 0 ? 0.5 : -0.5)) * m_gridStep;
		Real _y = (int)(p.y / m_gridStep + (p.y >= 0 ? 0.5 : -0.5)) * m_gridStep;

		if(closeEnough)
		{
			Real snap_area = m_gridStep/3.0f;
			*closeEnough = (fabs(_x-p.x) <= snap_area && fabs(_y-p.y) <= snap_area);
		}
		p.x = _x;
		p.y = _y;
	}
	return p;
}

//-------------------------------------------------------------------------------------------------------------------
Real Camera::convertScreenSizeToWorld(Real size)
{
	return size*(DEFAULT_HALF_HEIGHT/m_viewZoom)/(m_winSize.y/2);
}

//-------------------------------------------------------------------------------------------------------------------
void Camera::setWindowSize(int width, int height)
{
	m_winSize.x = width;
	m_winSize.y = height;

	_update();
}

//-------------------------------------------------------------------------------------------------------------------
void Camera::setViewZoom(Real viewZoom)
{
	if(viewZoom>20.0 || viewZoom<0.01) return;

	m_viewZoom = viewZoom;

	_update();
}

//-------------------------------------------------------------------------------------------------------------------
void Camera::pan(const fVector2& offset)
{
	m_viewCenter -= offset;

	_update();
}

//-------------------------------------------------------------------------------------------------------------------
void Camera::setGridStep(Real gridStep)
{
	m_gridStep = gridStep;
}

}
