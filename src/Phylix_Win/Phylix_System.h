#pragma once

namespace Phylix
{
//pre-define
class Canvas;

/** 主系统，代表一个世界
*/
class System
{
public:
	/** 初始化系统 */
	void initSystem(int winWidth, int winHeight);
	/** 暂停系统或者继续 */
	void pauseSystem(bool pause);
	/** 系统是否处于暂停状态 */
	bool isSystemPause(void) const { return m_pause; }
	/** 打开或者关闭grid模式*/
	void setGridMode(bool grid);
	/** 是否打开grid模式*/
	bool isGridMode(void) const { return m_gridMode; }

	/** 逻辑帧*/
	void tick(void);
	/** 渲染帧*/
	void render(void);

private:
	Canvas*	m_pCanvas;			//!< 画布
	bool	m_pause;			//!< 是否处于暂停状态
	bool	m_gridMode;			//!< 是否打开grid
	Real	m_gridStep;			//!< 网格步长

	fVector2 m_lowPos;
	fVector2 m_upperPos;

private:
	/// MAX LOGIC FPS
	enum { MAX_LOGIC_FPS = 60 };
	//glut 回调函数
	static void _onCameraAdjust(int sreenWidth, int screenHeight, const fVector2& lowPos, const fVector2& upperPos);
	void _renderGrid(void);
	void _renderHashCell(cpSpaceHash* hash);
	bool _loadMaterialTemplateTexture(void);

public:
	/** 调试函数 */
	void loadDemo(void);

public:
	System();
	~System();

	static System* getSingletonPtr(void) { return s_pSingleton; }
	static System& getSingleton(void) { return *s_pSingleton; }

private:
	static System* s_pSingleton; 
};

}
