#pragma once

#include "atlopengl.h"

namespace Phylix
{

class CMainView : public CWindowImpl<CMainView>, COpenGL<CMainView>, public CIdleHandler
{
public:
	DECLARE_WND_CLASS_EX(NULL, 0, -1)

	virtual BOOL OnIdle(void);
	void OnGLInit(void);
	void OnGLRender(void);
	void OnGLResize(int cx, int cy);

	BEGIN_MSG_MAP(CMainView)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_LBUTTONDOWN(OnMouseLButtonDown)
		MSG_WM_LBUTTONUP(OnMouseLButtonUp)
		MSG_WM_MBUTTONDOWN(OnMouseMButtonDown)
		MSG_WM_MBUTTONUP(OnMouseMButtonUp)
		MSG_WM_MOUSEWHEEL(OnMouseWheel)

		MSG_WM_KEYDOWN(OnKeyDown);
		MSG_WM_CHAR(OnCharDown);

		CHAIN_MSG_MAP(COpenGL<CMainView>)
	END_MSG_MAP()

	/******************************************************
		Message Handle
	*******************************************************/
	LRESULT OnMouseMove(UINT key, const CPoint& point);
	LRESULT OnMouseLButtonDown(UINT key, const CPoint& point);
	LRESULT OnMouseLButtonUp(UINT key, const CPoint& point);
	LRESULT OnMouseMButtonDown(UINT key, const CPoint& point);
	LRESULT OnMouseMButtonUp(UINT key, const CPoint& point);
	LRESULT OnMouseWheel(UINT key, short zDelta, const CPoint& point);
	void	OnKeyDown(TCHAR tchar, UINT repeatCount, UINT flags);
	void	OnCharDown(TCHAR tchar, UINT repeatCount, UINT flags);

private:
	ACTION_TYPE m_actionBeforePan;

public:
	CMainView();
	virtual ~CMainView();
};

}
