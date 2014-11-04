#include "libPhylix_stdafx.h"
#include "libPhylix_Action_Creater_Chain.h"
#include "libPhylix_MaterialManager.h"
#include "libPhylix_RenderInterface.h"
#include "libPhylix_OperatorManager.h"
#include "libPhylix_operator.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
Action_Creater_Chain::Action_Creater_Chain()
	: CHAIN_WIDTH(0.25)
{
}

//-------------------------------------------------------------------------------------------------------------------
Action_Creater_Chain::~Action_Creater_Chain() 
{
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_Chain::_onBegin(const fVector2& pt)
{
	m_randomColor = MaterialManager::_createRandomColor();

	m_ptHingeArray.clear();
	m_ptHingeArray.push_back(pt);

	m_currentHingePos = pt;
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_Chain::_onEnd(const fVector2& pt, bool canceled)
{
	//必须两个以上才能建立
	if(m_ptHingeArray.size() <= 1) return;

	//构造创建参数
	ObjectParams param(Object::OT_CHAIN);
	param.setBoolean1(false);  //not lost head yet
	param.setBoolean2(false); //not lost tail yet
	param.setWidth(CHAIN_WIDTH);	//宽度
	param.setPoints(m_ptHingeArray);	//hinge位置

	param.setMaterial(MATERIAL_MANAGER()->createStandardMaterial(m_randomColor));
	OPERATOR_MANAGER()->applyOperator(new Operator_CreateObject(OT_CREATE_CHAIN, param));
	m_ptHingeArray.clear();
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_Chain::_onDrag(const fVector2& delta)
{
	fVector2 dis = m_dragCurrent-m_currentHingePos;
	if(dis.square() < CHAIN_WIDTH*CHAIN_WIDTH) return;

	//计算长度
	Real length = dis.length();
	fVector2 normal_dis = dis;
	normal_dis /= length;
	int counts = (int)(length/CHAIN_WIDTH);

	for(int i=0; i<counts; i++)
	{
		fVector2 next_hinge = m_currentHingePos + CHAIN_WIDTH*normal_dis;
		m_ptHingeArray.push_back(next_hinge);

		m_currentHingePos = next_hinge;
	}

}

//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_Chain::draw(IRenderInterface* render)
{
	if(!m_dragStarted || render==0 ) return;

	for(int i=1; i<(int)m_ptHingeArray.size(); i++)
	{
		render->drawCircle(0.5*(m_ptHingeArray[i]+m_ptHingeArray[i-1]), CHAIN_WIDTH/(Real)2.0, m_randomColor);
	}

	render->drawSegment(m_currentHingePos, m_dragCurrent, 1.0, m_randomColor);
}

}
