#pragma once

namespace Phylix
{
//pre-define
class Canvas;

/** ��ϵͳ������һ������
*/
class System
{
public:
	/** ��ʼ��ϵͳ */
	void initSystem(int winWidth, int winHeight);
	/** ��ͣϵͳ���߼��� */
	void pauseSystem(bool pause);
	/** ϵͳ�Ƿ�����ͣ״̬ */
	bool isSystemPause(void) const { return m_pause; }
	/** �򿪻��߹ر�gridģʽ*/
	void setGridMode(bool grid);
	/** �Ƿ��gridģʽ*/
	bool isGridMode(void) const { return m_gridMode; }

	/** �߼�֡*/
	void tick(void);
	/** ��Ⱦ֡*/
	void render(void);

private:
	Canvas*	m_pCanvas;			//!< ����
	bool	m_pause;			//!< �Ƿ�����ͣ״̬
	bool	m_gridMode;			//!< �Ƿ��grid
	Real	m_gridStep;			//!< ���񲽳�

	fVector2 m_lowPos;
	fVector2 m_upperPos;

private:
	/// MAX LOGIC FPS
	enum { MAX_LOGIC_FPS = 60 };
	//glut �ص�����
	static void _onCameraAdjust(int sreenWidth, int screenHeight, const fVector2& lowPos, const fVector2& upperPos);
	void _renderGrid(void);
	void _renderHashCell(cpSpaceHash* hash);
	bool _loadMaterialTemplateTexture(void);

public:
	/** ���Ժ��� */
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
