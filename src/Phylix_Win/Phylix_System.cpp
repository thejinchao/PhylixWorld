#include "StdAfx.h"
#include "Phylix_System.h"
#include "Phylix_Canvas.h"
#include "Phylix_TextureUtil.h"

namespace Phylix
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Handler* getHandler(void)
{
	static Handler s_handle;
	return &s_handle;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
System* System::s_pSingleton = 0; 

//-------------------------------------------------------------------------------------------------------------------
System::System()
	: m_pause(false)
	, m_gridMode(false)
{
	s_pSingleton = this;
}

//-------------------------------------------------------------------------------------------------------------------
System::~System()
{
	s_pSingleton = 0;

	delete m_pCanvas; m_pCanvas=0;

	delete OPERATOR_MANAGER();
	delete EVENT_MANAGER();
	delete SCENE();
	delete CAMERA();
	delete ACTION_MANAGER();
	delete SHAPE_MANAGER();
	delete OBJECT_MANAGER();
	delete MATERIAL_MANAGER();
	delete MASS_BODY_MANAGER();
}

//-------------------------------------------------------------------------------------------------------------------
void System::initSystem(int winWidth, int winHeight)
{
	// 创建画布
	m_pCanvas = new Canvas();

	//创建Phylix::Handler所属对象
	getHandler()->setHandler(this,
		new MassBodyManager, 
		new MaterialManager, 
		new ObjectManager, 
		new ShapeManager, 
		new ActionManager, 
		new Camera(_onCameraAdjust), 
		new Scene,
		new EventManager,
		new OperatorManager);

	//init opengl status
	glClearColor(0.298f, 0.4f, 1.0f, 0.0f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_2D);

	//load material template texture
	_loadMaterialTemplateTexture();

	//init physics scene
	SCENE()->resetScene();
}

//-------------------------------------------------------------------------------------------------------------------
void System::tick(void)
{
	if(m_pause || !(SCENE())) return;

	static unsigned int	s_lastLogicTick = 0;
	unsigned int timeNow = ::timeGetTime();

	float step_time = 1.f/MAX_LOGIC_FPS;
	if(s_lastLogicTick!=0) 
	{
		step_time = (timeNow-s_lastLogicTick)/1000.f;
	}

	if(step_time>=1.f/MAX_LOGIC_FPS)
	{
		SCENE()->tick(step_time);
		s_lastLogicTick = timeNow;
	}
}

//-------------------------------------------------------------------------------------------------------------------
void System::render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Project Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	fVector2 lowPos=System::getSingletonPtr()->m_lowPos;
	fVector2 upperPos=System::getSingletonPtr()->m_upperPos;
	gluOrtho2D(lowPos.x, upperPos.x, lowPos.y, upperPos.y);

	//Model Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//draw world
	SCENE()->render(System::getSingletonPtr()->m_pCanvas);

	//draw active action
	Action* action = ACTION_MANAGER()->getActiveAction();
	if(action) 
	{
		action->draw(System::getSingletonPtr()->m_pCanvas);
	}

	//draw grid
	_renderGrid();

	//draw hash cell
//	_renderHashCell(SCENE()->getImpl()->activeShapes);
//	_renderHashCell(SCENE()->getImpl()->staticShapes);

	glFlush();
}

//-------------------------------------------------------------------------------------------------------------------
void System::_renderGrid(void)
{
	if(!isGridMode()) return;

	//draw v line
	int index=0;
	for(Real x=0.0; x<=m_upperPos.x; x+=m_gridStep,index++)
	{
		Real width=0.5;
		if(index==0) width=2.5;
		else if(index%4==0) width=1.5;
		
		m_pCanvas->drawSegment(
			fVector2(x, m_lowPos.y), 
			fVector2(x, m_upperPos.y), 
			width,
			fColor(1.0, 1.0, 1.0, 1.0));
	}

	index=0;
	for(Real x=0; x>=m_lowPos.x; x-=m_gridStep,index++)
	{
		Real width=0.5;
		if(index==0) continue;
		if(index%4==0) width=1.5;
		
		m_pCanvas->drawSegment(
			fVector2(x, m_lowPos.y), 
			fVector2(x, m_upperPos.y), 
			width,
			fColor(1.0, 1.0, 1.0, 1.0));
	}

	//draw h line
	index=0;
	for(Real y=0.0; y<=m_upperPos.y; y+=m_gridStep,index++)
	{
		Real width=0.5;
		if(index==0) width=2.5;
		else if(index%4==0) width=1.5;
		
		m_pCanvas->drawSegment(
			fVector2(m_lowPos.x, y), 
			fVector2(m_upperPos.x, y), 
			width,
			fColor(1.0, 1.0, 1.0, 1.0));
	}

	index=0;
	for(Real y=0; y>=m_lowPos.y; y-=m_gridStep,index++)
	{
		Real width=0.5;
		if(index==0) continue;
		if(index%4==0) width=1.5;
		
		m_pCanvas->drawSegment(
			fVector2(m_lowPos.x, y), 
			fVector2(m_upperPos.x, y), 
			width,
			fColor(1.0, 1.0, 1.0, 1.0));
	}
}

//-------------------------------------------------------------------------------------------------------------------
cpHashValue hash_func(cpHashValue x, cpHashValue y, cpHashValue n)
{
	return (x*1640531513ul ^ y*2654435789ul) % n;
}

//-------------------------------------------------------------------------------------------------------------------
void System::_renderHashCell(cpSpaceHash* hash)
{
/*
	Real dim = hash->celldim;
	int counts = hash->numcells;

	int l = (int)floor(m_lowPos.x/dim);
	int r = (int)floor(m_upperPos.x/dim);
	int b = (int)floor(m_lowPos.y/dim);
	int t = (int)floor(m_upperPos.y/dim);

	glColorMask(GL_FALSE, GL_TRUE, GL_FALSE, GL_TRUE);

	for(int j=l; j<=r; j++)
	{
		for(int i=b; i<=t; i++)
		{
			int cell_count = 0;
			
			int index = hash_func(j, i, counts);
			for(cpSpaceHashBin *bin = hash->table[index]; bin!=0; bin = bin->next)
			{
				cell_count++;
			}
			if(cell_count==0) continue;
			
			GLfloat v = 1.0f - (GLfloat)cell_count/10.0f;
			glColor3f(v,v,v);
			glRectd(j*dim, i*dim, (j + 1)*dim, (i + 1)*dim);
		}
	}

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
*/
}

//-------------------------------------------------------------------------------------------------------------------
void System::loadDemo(void)
{
	//create enge
	{
		ObjectParams param(Object::OT_ENTITY_EDGE);
		param.setPosition1(fVector2(0.0, 0.0));
		param.setMaterial(MATERIAL_MANAGER()->createStandardMaterial(MaterialManager::_createRandomColor()));
		param.setAngle(fastAtan2(1.0, 0.0));

		OPERATOR_MANAGER()->applyOperator(new Operator_CreateObject(OT_CREATE_EDGE, param));
	}

	//create circle
	{
		ObjectParams param(Object::OT_ENTITY_CIRCLE);
		param.setPosition1(fVector2(0.0, 2.0));
		param.setAngle(0.0);
		param.setRadius(0.5);
		param.setMaterial(MATERIAL_MANAGER()->createStandardMaterial(MaterialManager::_createRandomColor()));
		OPERATOR_MANAGER()->applyOperator(new Operator_CreateObject(OT_CREATE_CIRCLE, param));
	}

/*
	//create fix joint
	{
		ObjectParams param(Object::OT_FIXED_JOINT);
		param.setPosition1(fVector2(11.0, 6.0));
		param.setMaterial(MATERIAL_MANAGER()->createStandardMaterial(MaterialManager::_createRandomColor()));
		param.setRadius(1.0/(CAMERA()->getViewZoom()*16.0));

		OPERATOR_MANAGER()->applyOperator(new Operator_CreateObject(OT_CREATE_FIXED_JOINT, param));
	}

	//create spring joint
	{
		ObjectParams param(Object::OT_SPRING_JOINT);
		param.setPosition1(fVector2(10.0, 5.0));
		param.setPosition2(fVector2(20.0, 25.0));
		param.setMaterial(MATERIAL_MANAGER()->createStandardMaterial(MaterialManager::_createRandomColor()));
		param.setWidth(1.0/(CAMERA()->getViewZoom()*16.0));

		OPERATOR_MANAGER()->applyOperator(new Operator_CreateObject(OT_CREATE_SPRING_JOINT, param));
	}
*/
}

//-------------------------------------------------------------------------------------------------------------------
void System::pauseSystem(bool pause)
{
	m_pause = pause;
//	::glutSetWindowTitle(m_pause?"Physics World(Pause)":"Physics World");
}

//-------------------------------------------------------------------------------------------------------------------
void System::setGridMode(bool grid)
{
	m_gridMode = grid;

	CAMERA()->setGridStep(m_gridMode ? getSingletonPtr()->m_gridStep : 0.0f);
}

//-------------------------------------------------------------------------------------------------------------------
void System::_onCameraAdjust(int sreenWidth, int screenHeight, const fVector2& lowPos, const fVector2& upperPos)
{
	glViewport(0, 0, sreenWidth, screenHeight);
	
	getSingletonPtr()->m_lowPos = lowPos;
	getSingletonPtr()->m_upperPos = upperPos;

	//最小grid宽度
	Real& gridStep = getSingletonPtr()->m_gridStep;
	Real viewZoom = CAMERA()->getViewZoom();
	if(viewZoom<3.0/256) gridStep = 64;
	else if(viewZoom<3.0/64) gridStep = 16.0;
	else if(viewZoom<3.0/16) gridStep = 4.0;
	else if(viewZoom<3.0/4) gridStep = 1.0;
	else if(viewZoom<3.0) gridStep = 1/4.0;
	else if(viewZoom<3.0*4) gridStep = 1/16.0;
	else gridStep = 1.0/32;

	if(getSingletonPtr()->isGridMode())
	{
		CAMERA()->setGridStep(gridStep);
	}
}

//-------------------------------------------------------------------------------------------------------------------
bool System::_loadMaterialTemplateTexture(void)
{
	int counts = MATERIAL_MANAGER()->getMaterialTemplateCount();
	MaterialTemplate* templates = MATERIAL_MANAGER()->getMaterialTemplateList();
	for(int i=0; i<counts; i++)
	{
		templates[i].proxy = 0;

		TextureInfo textureInfo;
		if(createTextureFromPngFile(templates[i].texture, textureInfo))
		{
			templates[i].proxy = new TextureInfo;

			memcpy(templates[i].proxy, &textureInfo, sizeof(TextureInfo));
		}
	}
	return true;
}

}

