#include "libPhylix_stdafx.h"
#include "libPhylix_Action_Creater_Polygon.h"
#include "libPhylix_ActionManager.h"
#include "libPhylix_ObjectManager.h"
#include "libPhylix_RenderInterface.h"
#include "libPhylix_MaterialManager.h"
#include "libPhylix_OperatorManager.h"
#include "libPhylix_operator.h"
#include "libPhylix_Math.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
Action_Creater_Polygon::Action_Creater_Polygon() 
{
}

//-------------------------------------------------------------------------------------------------------------------
Action_Creater_Polygon::~Action_Creater_Polygon() 
{
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_Polygon::_onBegin(const fVector2& pt)
{
	m_randomColor = MaterialManager::_createRandomColor();

	m_ptArray.clear();
	m_ptArray.push_back(pt);
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_Polygon::_onEnd(const fVector2& pt, bool canceled)
{
	//优化
	optimize_line(m_ptArray, 3);
	if(m_ptArray.size()<3) return; //tool few points

	fVector2 ptCross;
	int initiative;
	int passivity;
	int cross_counts = getFirstCrossPoint(m_ptArray, ptCross, initiative, passivity);

	//构造创建参数
	ObjectParams param(Object::OT_ENTITY_POLYGON);

	if(cross_counts==0)
	{
		//没有交点
		param.setPoints(m_ptArray);
	}
	else
	{
		PointVector complexPoly;

		complexPoly.push_back(ptCross);
		complexPoly.push_back(m_ptArray[passivity+1]);

		for(int i=passivity+2; i<initiative; i++)
		{
			complexPoly.push_back(m_ptArray[i]);
		}

		complexPoly.push_back(m_ptArray[initiative]);

		param.setPoints(complexPoly);
	}

	param.setMaterial(MATERIAL_MANAGER()->createStandardMaterial(m_randomColor));
	OPERATOR_MANAGER()->applyOperator(new Operator_CreateObject(OT_CREATE_POLYGON, param));
	m_ptArray.clear();
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_Polygon::_onDrag(const fVector2& delta)
{
	if(m_ptArray.size() > 1)
	{
		const fVector2& v = m_ptArray[m_ptArray.size()-2];
		if(floatCompare(v.x, m_dragCurrent.x)==0 && floatCompare(v.y, m_dragCurrent.y)==0)
		{
			m_ptArray.pop_back();
			return;
		}
	}
	m_ptArray.push_back(m_dragCurrent);
}

//-------------------------------------------------------------------------------------------------------------------
void Action_Creater_Polygon::draw(IRenderInterface* render)
{
	if(!m_dragStarted || render==0 || m_ptArray.empty()) return;

	render->drawSegmentArrary(&(m_ptArray[0]), (unsigned int)m_ptArray.size(), m_randomColor);
}

}
