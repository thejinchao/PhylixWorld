#include "libPhylix_stdafx.h"
#include "libPhylix_ObjectManager.h"
#include "libPhylix_Object_Entity.h"
#include "libPhylix_Object_Joint_Fixed.h"
#include "libPhylix_Object_Joint_Hinge.h"
#include "libPhylix_Object_Joint_Spring.h"
#include "libPhylix_Object_Chain.h"
#include "libPhylix_Shape.h"
#include "libPhylix_ShapeManager.h"
#include "libPhylix_Material.h"
#include "libPhylix_MaterialManager.h"
#include "libPhylix_MassBodyManager.h"
#include "libPhylix_Scene.h"
#include "libPhylix_MassBody.h"
#include "libPhylix_Group.h"
#include "libPhylix_Camera.h"

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
ObjectManager::ObjectManager()
	: m_nextID(0)
	, m_nextZValue(0.0)
	, Z_VALUE_STEP(0.0001f)
{
}

//-------------------------------------------------------------------------------------------------------------------
ObjectManager::~ObjectManager()
{
}

//-------------------------------------------------------------------------------------------------------------------
unsigned int ObjectManager::_generateID(int counts)
{
	unsigned int ret = m_nextID+1;
	m_nextID+=counts;
	return ret;
}

//-------------------------------------------------------------------------------------------------------------------
Real ObjectManager::_generateZValue(int counts)
{
	Real ret = m_nextZValue+Z_VALUE_STEP;
	m_nextZValue += Z_VALUE_STEP*counts;
	return ret;
}

//-------------------------------------------------------------------------------------------------------------------
Object* ObjectManager::findObject(unsigned int id)
{
	ObjectMap::iterator it = m_allObject.find(id);
	if(it==m_allObject.end()) return 0;

	return it->second;
}

//-------------------------------------------------------------------------------------------------------------------
Object* ObjectManager::createObject(const ObjectParams& param)
{
	switch(param.getObjectType())
	{
	case Object::OT_ENTITY_BOX:		return _createStandardBox(param); break;
	case Object::OT_ENTITY_EDGE:	return _createStandardEdge(param); break;
	case Object::OT_ENTITY_CIRCLE:	return _createStandardCircle(param); break;
	case Object::OT_ENTITY_POLYGON:	return _createPolygon(param); break;
	case Object::OT_FIXED_JOINT:	return _createFixedJoint(param); break;
	case Object::OT_HINGE_JOINT:	return _createHingeJoint(param); break;
	case Object::OT_SPRING_JOINT:	return _createSpringJoint(param); break;
	case Object::OT_CHAIN:			return _createChain(param); break;
	default: break;
	}
	return 0;
}
//-------------------------------------------------------------------------------------------------------------------
ObjectEntity* ObjectManager::_createStandardBox(const ObjectParams& param)
{
	//生成一个物体
	ObjectEntity* newObj = new ObjectEntity(param.getFlag(ObjectParams::PT_OBJECT_ID) ? param.getObjectID() : _generateID(), Object::OT_ENTITY_BOX);
	//确定材质
	newObj->m_material = param.getMaterial();
	//生成形状
	newObj->m_shape = SHAPE_MANAGER()->createBoxShape(newObj, param.getSize(), &(newObj->m_material));
	//生成质量体
	Real mass = newObj->m_shape->getArea() * newObj->m_material.density;
	Real moment = mass * newObj->m_shape->getMoment();
	newObj->m_massBody = MASS_BODY_MANAGER()->createMassBody(param.getPosition1(), mass, moment, 
		param.getFlag(ObjectParams::PT_MASSBODY) ? &(param.getMassBody()) : 0);
	newObj->m_massBody->attachObject(newObj);

	//确定z值
	newObj->m_zOrder = param.getFlag(ObjectParams::PT_Z_VALUE) ? param.getZValue() : _generateZValue();
	//加入索引表
	m_allObject.insert(std::make_pair(newObj->getID(), newObj));

	//加入chipmunk场景
	cpSpace* space = SCENE()->getImpl();
	cpBody* body = newObj->m_massBody->getImpl();
	int cpShapeCounts = newObj->m_shape->getImplCounts();
	if(space && body && cpShapeCounts>0)
	{
		cpSpaceAddBody(space, body);  //加入质量体
		//加入Shape
		for(int i=0; i<cpShapeCounts; i++)
		{
			cpShape* shape = newObj->m_shape->getImpl(i);
			shape->body = body;
			cpShapeCacheBB(shape);
			cpSpaceAddShape(space, shape);
		}
	}

	return newObj;
}

//-------------------------------------------------------------------------------------------------------------------
CircleObject* ObjectManager::_createStandardCircle(const ObjectParams& param)
{
	//生成一个物体
	CircleObject* newObj = new CircleObject(param.getFlag(ObjectParams::PT_OBJECT_ID) ? param.getObjectID() : _generateID(), Object::OT_ENTITY_CIRCLE);
	//确定材质
	newObj->m_material = param.getMaterial();
	//生成形状
	newObj->m_shape = SHAPE_MANAGER()->createCircleShape(newObj, param.getRadius(), 
		param.getFlag(ObjectParams::PT_ANGLE), 
		param.getFlag(ObjectParams::PT_ANGLE) ? param.getAngle() : fVector2::ZERO, 
		&(newObj->m_material));
	//生成质量体
	Real mass = newObj->m_shape->getArea() * newObj->m_material.density;
	Real moment = mass * newObj->m_shape->getMoment();
	newObj->m_massBody = MASS_BODY_MANAGER()->createMassBody(param.getPosition1(), mass, moment, 
		param.getFlag(ObjectParams::PT_MASSBODY) ? &(param.getMassBody()) : 0);
	newObj->m_massBody->attachObject(newObj);
	//确定z值
	newObj->m_zOrder = param.getFlag(ObjectParams::PT_Z_VALUE) ? param.getZValue() : _generateZValue();
	//加入索引表
	m_allObject.insert(std::make_pair(newObj->getID(), newObj));

	//加入chipmunk场景
	cpSpace* space = SCENE()->getImpl();
	cpBody* body = newObj->m_massBody->getImpl();
	int cpShapeCounts = newObj->m_shape->getImplCounts();
	if(space && body && cpShapeCounts>0)
	{
		cpSpaceAddBody(space, body);  //加入质量体
		//加入Shape
		for(int i=0; i<cpShapeCounts; i++)
		{
			cpShape* shape = newObj->m_shape->getImpl(i);
			shape->body = body;
			cpShapeCacheBB(shape);
			cpSpaceAddShape(space, shape);
		}
	}
	return newObj;
}

//-------------------------------------------------------------------------------------------------------------------
ObjectEntity* ObjectManager::_createStandardEdge(const ObjectParams& param)
{
	//生成一个物体
	ObjectEntity* newObj = new ObjectEntity(param.getFlag(ObjectParams::PT_OBJECT_ID) ? param.getObjectID() : _generateID(), Object::OT_ENTITY_EDGE);
	//确定材质
	newObj->m_material = param.getMaterial();
	//生成形状
	newObj->m_shape = SHAPE_MANAGER()->createEdgeShape(newObj, param.getPosition1(), param.getAngle(), &(newObj->m_material));
	//边界永远是固定物体
	newObj->m_massBody = 0;
	//确定z值
	newObj->m_zOrder = param.getFlag(ObjectParams::PT_Z_VALUE) ? param.getZValue() : _generateZValue();
	//加入索引表
	m_allObject.insert(std::make_pair(newObj->getID(), newObj));

	//加入chipmunk场景
	newObj->m_shape->getImpl(0)->body = SCENE()->getImpl()->staticBody;
	cpShapeCacheBB(newObj->m_shape->getImpl(0));
	cpSpaceAddStaticShape(SCENE()->getImpl(), newObj->m_shape->getImpl(0));

	return newObj;
}

//-------------------------------------------------------------------------------------------------------------------
ObjectEntity* ObjectManager::_createPolygon(const ObjectParams& param)
{
	//生成一个物体
	ObjectEntity* newObj = new ObjectEntity(param.getFlag(ObjectParams::PT_OBJECT_ID) ? param.getObjectID() : _generateID(), Object::OT_ENTITY_POLYGON);
	//确定材质
	newObj->m_material = param.getMaterial();
	//生成形状
	fVector2 ptCenter;
	newObj->m_shape = SHAPE_MANAGER()->createPolygon(newObj, param.getPoints(), &(newObj->m_material), ptCenter);
	//生成质量体
	Real mass = newObj->m_shape->getArea() * newObj->m_material.density;
	Real moment = mass * newObj->m_shape->getMoment();
	newObj->m_massBody = MASS_BODY_MANAGER()->createMassBody(ptCenter, mass, moment, 
		param.getFlag(ObjectParams::PT_MASSBODY) ? &(param.getMassBody()) : 0);
	newObj->m_massBody->attachObject(newObj);
	//确定z值
	newObj->m_zOrder = param.getFlag(ObjectParams::PT_Z_VALUE) ? param.getZValue() : _generateZValue();
	//加入索引表
	m_allObject.insert(std::make_pair(newObj->getID(), newObj));

	//加入chipmunk场景
	cpSpace* space = SCENE()->getImpl();
	cpBody* body = newObj->m_massBody->getImpl();
	int cpShapeCounts = newObj->m_shape->getImplCounts();
	if(space && body && cpShapeCounts>0)
	{
		cpSpaceAddBody(space, body);  //加入质量体
		//加入Shape
		for(int i=0; i<cpShapeCounts; i++)
		{
			cpShape* shape = newObj->m_shape->getImpl(i);
			shape->body = body;
			cpShapeCacheBB(shape);
			cpSpaceAddShape(space, shape);
		}
	}
	return newObj;
}

//-------------------------------------------------------------------------------------------------------------------
ObjectFixedJoint* ObjectManager::_createFixedJoint(const ObjectParams& param)
{
	const fVector2& ptPosition = param.getPosition1();

	//查询鼠标下面的entity
	ZOrderSet objList;
	SCENE()->queryEntityObjectFromPoint(ptPosition, objList);
	if(objList.empty()) return 0;

	ObjectEntity* firstObject=0, *secondObject=0;
	if(objList.size()==1)
	{
		//如果是单个物体，将物体设置为静态物体
		firstObject = (ObjectEntity*)(*(objList.begin()));
		secondObject = 0;

	}
	else if(objList.size()>=2)
	{
		//如果是多个物体，连接最上面的两个物体
		ZOrderSet::reverse_iterator itFirst = objList.rbegin();
		ZOrderSet::reverse_iterator itSecond = itFirst; itSecond++;

		firstObject = (ObjectEntity*)(*itFirst);
		secondObject = (ObjectEntity*)(*itSecond);
	}

	//生成一个物体
	ObjectFixedJoint* joint = new ObjectFixedJoint(param.getFlag(ObjectParams::PT_OBJECT_ID) ? param.getObjectID() : _generateID());
	//确定材质
	joint->m_material = param.getMaterial();
	//确定z值
	joint->m_zOrder = param.getFlag(ObjectParams::PT_Z_VALUE) ? param.getZValue() : _generateZValue();
	//半径
	joint->m_radius = param.getRadius();
	//加入索引表
	m_allObject.insert(std::make_pair(joint->getID(), joint));

	//设置物体所连接的关节
	firstObject->m_joints.insert((ObjectJoint*)joint);
	if(secondObject) secondObject->m_joints.insert((ObjectJoint*)joint);

	//计算相对位置（第一个物体）
	MassBody* body1 = firstObject->getMassBody();
	joint->m_firstObj = firstObject;
	joint->m_posFirst = firstObject->getTrnasFormMatrix().solve(ptPosition);

	//计算相对位置（第二个物体）
	MassBody* body2 = secondObject ? secondObject->getMassBody() : 0;
	joint->m_secondObj = secondObject;
	joint->m_posSecond = secondObject ? secondObject->getTrnasFormMatrix().solve(ptPosition) : ptPosition;

	//属于同一个质量体中的物体
	if(body1==body2) return joint;

	//两个质量体原先所依附的所有物体
	MassBody::ObjectSet objSet;
	if(body1) objSet.insert(body1->getAttachedObject().begin(), body1->getAttachedObject().end());
	if(body2) objSet.insert(body2->getAttachedObject().begin(), body2->getAttachedObject().end());

	//计算新的质量体
	MassBody* newMassBody = 0;

	//合并质量体
	if(body1 !=0 && body2!=0 )
	{
		Real mass1 = body1->getMass();
		Real mass2 = body2->getMass();
		Real newMass = mass1 + mass2;

		//计算新的质心位置
		fVector2 newPos;
		newPos.x = (body1->getPosition().x * mass1 + body2->getPosition().x * mass2)/newMass;
		newPos.y = (body1->getPosition().y * mass1 + body2->getPosition().y * mass2)/newMass;

		//计算新的转动惯量
		Real newMoment = 0.0;
		for(MassBody::ObjectSet::iterator it=objSet.begin(); it!=objSet.end(); ++it)
		{
			ObjectEntity* o = *it;
			fVector2 off = o->getTrnasFormMatrix().getOffsetPart()-newPos;
			newMoment += (o->getShape()->getMoment() + off.square()) * o->getShape()->getArea() * o->getMaterial().density;
		}

		//生成质量体
		/*
		TODO: 质量体合并时，会忽略原来质量体的线形速度和旋转速度，也就是说，合并后的质量体处于完全静止状态，
			  而正常的物理计算应该是依据动能守恒计算新的质量体的线形速度和旋转速度
		*/
		newMassBody = MASS_BODY_MANAGER()->createMassBody(newPos, newMass, newMoment, 
			param.getFlag(ObjectParams::PT_MASSBODY) ? &(param.getMassBody()) : 0);
	}

	//chipmunk空间加入质量体
	if(newMassBody)
	{
		cpSpaceAddBody(SCENE()->getImpl(), newMassBody->getImpl());  
	}

	//重新设置原有物体的质量体
	for(MassBody::ObjectSet::iterator it=objSet.begin(); it!=objSet.end(); ++it)
	{
		ObjectEntity* entity = *it;
		if(newMassBody==0 && entity->getMassBody() == 0) continue;

		entity->getShape()->resetMassOwner(newMassBody);
		entity->m_massBody = newMassBody;

		if(newMassBody)
			newMassBody->attachObject(*it);
	}

	//调整所涉及的铰链
	Group group(firstObject, secondObject, false);
	Group::JointObjectSet& joints = group.getJoints();
	Group::JointObjectSet::iterator itJoints, itEnd = joints.end();
	for(itJoints=joints.begin(); itJoints!=itEnd; ++itJoints)
	{
		ObjectJoint* joint = *itJoints;

		//重新检查质量体
		joint->reCheckMassBody();
	}

	//销毁原有质量体
	if(body1) body1->getCpData(joint->m_firstDeleteMassBody);
	if(body2) body2->getCpData(joint->m_secondDeleteMassBody);
	MASS_BODY_MANAGER()->destroyMassBody(body1);
	MASS_BODY_MANAGER()->destroyMassBody(body2);

	return joint;
}

//-------------------------------------------------------------------------------------------------------------------
ObjectHingeJoint* ObjectManager::_createHingeJoint(const ObjectParams& param)
{
	const fVector2& ptPosition = param.getPosition1();

	ObjectEntity* firstObject=0, *secondObject=0;
	if(param.getFlag(ObjectParams::PT_TARGET_ID1) && param.getFlag(ObjectParams::PT_TARGET_ID2))
	{
		firstObject = (ObjectEntity*)findObject(param.getTargetID1());
		secondObject = (ObjectEntity*)findObject(param.getTargetID2());
	}
	else
	{
		//查询鼠标下面的entity
		ZOrderSet objList;
		SCENE()->queryEntityObjectFromPoint(ptPosition, objList);
		if(objList.empty()) return 0;

		if(objList.size()==1)
		{
			//如果是单个物体，将物体设置为静态物体
			firstObject = (ObjectEntity*)(*(objList.begin()));
			secondObject = 0;

		}
		else if(objList.size()>=2)
		{
			//如果是多个物体，连接最上面的两个物体
			ZOrderSet::reverse_iterator itFirst = objList.rbegin();
			ZOrderSet::reverse_iterator itSecond = itFirst; itSecond++;

			firstObject = (ObjectEntity*)(*itFirst);
			secondObject = (ObjectEntity*)(*itSecond);
		}
	}

	//生成铰链
	ObjectHingeJoint* joint = new ObjectHingeJoint(param.getFlag(ObjectParams::PT_OBJECT_ID) ? param.getObjectID() : _generateID());

	//确定材质
	joint->m_material = param.getMaterial();
	//确定z值
	joint->m_zOrder = param.getFlag(ObjectParams::PT_Z_VALUE) ? param.getZValue() : _generateZValue();
	//半径
	joint->m_radius = param.getRadius();
	//加入索引表
	m_allObject.insert(std::make_pair(joint->getID(), joint));

	//设置物体所连接的关节
	firstObject->m_joints.insert((ObjectJoint*)joint);
	if(secondObject) secondObject->m_joints.insert((ObjectJoint*)joint);

	//计算相对位置（第一个物体）
	MassBody* body1 = firstObject->getMassBody();
	joint->m_firstObj = firstObject;
	joint->m_posFirst = firstObject->getTrnasFormMatrix().solve(ptPosition);
	joint->m_nFirstMassBody = (body1==0 ? 0 : body1->getID());

	//计算相对位置（第二个物体）
	MassBody* body2 = secondObject ? secondObject->getMassBody() : 0;
	joint->m_secondObj = secondObject;
	joint->m_posSecond = secondObject ? secondObject->getTrnasFormMatrix().solve(ptPosition) : ptPosition;
	joint->m_nSecondMassBody = (body2==0 ? 0 : body2->getID());

	joint->m_headHinge = false;
	joint->m_tailHinge = false;

	//如果属于链子的第一个hinge
	if(param.getBoolean1())
	{
		joint->m_headHinge = true;
		joint->m_chainID = ((CircleObject*)firstObject)->m_chainID;

		ObjectChain* chain = (ObjectChain*)findObject(joint->m_chainID);
		chain->resumeHead(joint);
	}
	if(param.getBoolean2())
	{
		joint->m_tailHinge = true;
		joint->m_chainID = ((CircleObject*)firstObject)->m_chainID;

		ObjectChain* chain = (ObjectChain*)findObject(joint->m_chainID);
		chain->resumeTail(joint);
	}

	//生成chipmunk数据
	if((body1==0 || body1->getImpl()==0) && (body2==0 || body2->getImpl()==0))
	{
		//两端都是静态物体
		joint->m_cpConstraint = 0;
	}
	else
	{
		fVector2 p1 = body1->getTrnasFormMatrix().solve(ptPosition);
		fVector2 p2 = body2 ? body2->getTrnasFormMatrix().solve(ptPosition) : ptPosition;

		joint->m_cpConstraint = cpPivotJointNew2(
			body1->getImpl(), 
			body2 ? body2->getImpl() : SCENE()->getImpl()->staticBody, 
			cpv(p1.x, p1.y), cpv(p2.x, p2.y)); 

		cpSpaceAddConstraint(SCENE()->getImpl(), joint->m_cpConstraint);
	}

	return joint;
}

//-------------------------------------------------------------------------------------------------------------------
ObjectSpringJoint* ObjectManager::_createSpringJoint(const ObjectParams& param)
{
	const fVector2& ptPosition1 = param.getPosition1();
	const fVector2& ptPosition2 = param.getPosition2();

	ObjectEntity *firstObject=0, *secondObject=0;

	//查询鼠标下面的entity
	ZOrderSet objList;
	SCENE()->queryEntityObjectFromPoint(ptPosition1, objList);
	if(!objList.empty())
	{
		//第一个物体
		firstObject = (ObjectEntity*)(*(objList.begin()));
	}
	else
	{
		firstObject = 0;
	}

	//查询鼠标下面的entity
	SCENE()->queryEntityObjectFromPoint(ptPosition2, objList);
	if(!objList.empty())
	{
		//第二个物体
		secondObject = (ObjectEntity*)(*(objList.begin()));
	}
	else
	{
		secondObject = 0;
	}

	if((firstObject==0 && secondObject==0) || firstObject==secondObject) return 0;

	//生成弹簧
	ObjectSpringJoint* joint = new ObjectSpringJoint(param.getFlag(ObjectParams::PT_OBJECT_ID) ? param.getObjectID() : _generateID());

	//确定材质
	joint->m_material = param.getMaterial();
	//确定z值
	joint->m_zOrder = param.getFlag(ObjectParams::PT_Z_VALUE) ? param.getZValue() : _generateZValue();
	//宽度
	joint->m_width = param.getWidth();;
	//加入索引表
	m_allObject.insert(std::make_pair(joint->getID(), joint));

	//设置物体所连接的关节
	if(firstObject) firstObject->m_joints.insert((ObjectJoint*)joint);
	if(secondObject) secondObject->m_joints.insert((ObjectJoint*)joint);

	//计算相对位置（第一个物体）
	MassBody* body1 = firstObject ? firstObject->getMassBody() : 0;
	joint->m_firstObj = firstObject;
	joint->m_posFirst = firstObject ? firstObject->getTrnasFormMatrix().solve(ptPosition1) : ptPosition1;
	joint->m_nFirstMassBody = (body1==0 ? 0 : body1->getID());

	//计算相对位置（第二个物体）
	MassBody* body2 = secondObject ? secondObject->getMassBody() : 0;
	joint->m_secondObj = secondObject;
	joint->m_posSecond = secondObject ? secondObject->getTrnasFormMatrix().solve(ptPosition2) : ptPosition2;
	joint->m_nSecondMassBody = (body2==0 ? 0 : body2->getID());

	//弹簧系数
	joint->m_springParam.targetLength = (ptPosition1-ptPosition2).length();
	joint->m_springParam.stiffness = (((body1 ? body1->getMass() : 0) + (body2 ? body2->getMass() : 0)) * Scene::SCENE_GRAVITY)*10.0f;
	joint->m_springParam.damp = ObjectSpringJoint::DAMP_DEFAULT;

	//生成chipmunk数据
	if((body1==0 || body1->getImpl()==0) && (body2==0 || body2->getImpl()==0))
	{
		//两端都是静态物体
		joint->m_cpConstraint = 0;
	}
	else
	{
		fVector2 p1 = body1 ? body1->getTrnasFormMatrix().solve(ptPosition1) : ptPosition1;
		fVector2 p2 = body2 ? body2->getTrnasFormMatrix().solve(ptPosition2) : ptPosition2;

		joint->m_cpConstraint = cpDampedSpringNew(
			body1 ? body1->getImpl() : SCENE()->getImpl()->staticBody, 
			body2 ? body2->getImpl() : SCENE()->getImpl()->staticBody, 
			cpv(p1.x, p1.y), 
			cpv(p2.x, p2.y), 
			joint->m_springParam.targetLength, 
			joint->m_springParam.stiffness, 
			joint->m_springParam.damp);
		cpSpaceAddConstraint(SCENE()->getImpl(), joint->m_cpConstraint);
	}

	return joint;
}

//-------------------------------------------------------------------------------------------------------------------
ObjectChain* ObjectManager::_createChain(const ObjectParams& param)
{
	const PointVector& ptArray = param.getPoints();
	int nodeCounts = (int)ptArray.size()-1;

	ObjectEntity* firstObj=0;
	ObjectEntity* secondObj=0;

	ZOrderSet objList;
	//前端
	SCENE()->queryEntityObjectFromPoint(ptArray[0], objList);
	if(!objList.empty())
	{
		//链子前端挂在某物体上
		firstObj = (ObjectEntity*)(*(objList.begin()));
	}

	//末端物体
	fVector2 lastPoint = ptArray[ptArray.size()-1];
	SCENE()->queryEntityObjectFromPoint(lastPoint, objList);
	if(!objList.empty())
	{
		//链子末端挂在某物体上
		secondObj = (ObjectEntity*)(*(objList.begin()));
	}

	int allChildCounts = nodeCounts*2+1;

	//创建链子物体
	ObjectChain* chainObj = new ObjectChain(param.getFlag(ObjectParams::PT_OBJECT_ID) ? param.getObjectID() : _generateID(allChildCounts+1));
	chainObj->m_width = param.getWidth();
	chainObj->m_zOrder = param.getFlag(ObjectParams::PT_Z_VALUE) ? param.getZValue() : _generateZValue(allChildCounts);

	//加入索引表
	m_allObject.insert(std::make_pair(chainObj->getID(), chainObj));

	//预分配z值
	Real current_z = chainObj->m_zOrder;

	//生成circle
	unsigned int objID = chainObj->getID();
	for(int i=1; i<=nodeCounts; i++)
	{
		const fVector2& pt = 0.5*(ptArray[i]+ptArray[i-1]);

		ObjectParams circleParam;
		circleParam.setObjectID(++objID);
		circleParam.setPosition1(pt);
		circleParam.setRadius(param.getWidth()/2.0f);
		circleParam.setMaterial(param.getMaterial());
		circleParam.setZValue(current_z);
		current_z += Z_VALUE_STEP;
		if(param.getFlag(ObjectParams::PT_CHILD_MASS))
		{
			const ObjectParams::MassCache& mass = param.getChildMass()[i-1];
			if(mass.massBodyID!=0)
			{
				circleParam.setMassBody(mass);
			}
		}

		CircleObject* circleObj = _createStandardCircle(circleParam);
		chainObj->m_circleList.push_back(circleObj);
		circleObj->m_chainID = chainObj->getID();
	}

	//生成hinge
	for(int i=0; i<=nodeCounts; i++)
	{
		fVector2 pt;
		ObjectParams hingeParam;

		if(i==0) 
		{
			hingeParam.setBoolean1(true); //head hinge
			hingeParam.setBoolean2(false);
			hingeParam.setTargetID1(chainObj->m_circleList[0]->getID());
			hingeParam.setTargetID2(firstObj ? firstObj->getID() : 0);
		}
		else if(i==nodeCounts)
		{
			hingeParam.setBoolean1(false);
			hingeParam.setBoolean2(true); //tail hinge
			hingeParam.setTargetID1(chainObj->m_circleList[i-1]->getID());
			hingeParam.setTargetID2(secondObj ? secondObj->getID() : 0);
		}
		else 
		{
			hingeParam.setBoolean1(false);
			hingeParam.setBoolean2(false);
			hingeParam.setTargetID1(chainObj->m_circleList[i]->getID());
			hingeParam.setTargetID2(chainObj->m_circleList[i-1]->getID());
		}

		hingeParam.setPosition1(ptArray[i]);
		hingeParam.setObjectID(++objID);
		hingeParam.setMaterial(param.getMaterial());
		hingeParam.setRadius(param.getWidth()/8.0f);
		hingeParam.setZValue(current_z);
		current_z += Z_VALUE_STEP;

		//lost first hinge 
		if(i==0 && param.getBoolean1()) 
		{
			chainObj->m_lostFirst = true;
			chainObj->m_hingeList.push_back(0);
			continue;
		}
		//lost second hinge
		if(i==nodeCounts && (param.getBoolean2() || secondObj==0)) 
		{
			chainObj->m_hingeList.push_back(0);
			chainObj->m_lostTail = true;
			continue;
		}

		ObjectHingeJoint* hingeObj = _createHingeJoint(hingeParam);
		chainObj->m_hingeList.push_back(hingeObj);
		hingeObj->m_chainID = chainObj->getID();
	}

	return chainObj;
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectManager::destroyObject(unsigned int id, std::vector< ObjectParams >& params)
{
	Object* obj = findObject(id);
	if(!obj) return;

	//通知,即将被删除
	obj->notifyDelete();

	switch(obj->getType())
	{
	case Object::OT_ENTITY_EDGE:
	case Object::OT_ENTITY_BOX:
	case Object::OT_ENTITY_CIRCLE:
	case Object::OT_ENTITY_POLYGON:
		//删除实体
		_destroyEntityObject((ObjectEntity*)obj, params);
		break;

	case Object::OT_FIXED_JOINT:
	case Object::OT_HINGE_JOINT:
	case Object::OT_SPRING_JOINT:
		//删除连接体
		_destroyJointObject((ObjectJoint*)obj, params);
		break;

	case Object::OT_CHAIN:
		//删除链子
		_destroyChainObject((ObjectChain*)obj, params);
		break;

	default: break;
	}

	//重置选择对象
	if(SCENE()->getSelectedObject()==id) SCENE()->selectObject(0);
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectManager::_destroyEntityObject(ObjectEntity* entityObj, std::vector< ObjectParams >& params)
{
	assert(entityObj);
	if(!entityObj) return;

	//删除所连接的链接体
	ObjectEntity::JointSet joints = entityObj->getJointSet();
	ObjectEntity::JointSet::iterator it, end=joints.end();
	for(it=joints.begin(); it!=end; ++it)
	{
		destroyObject((*it)->getID(), params);
	}

	//保存创建参数,undo使用
	ObjectParams param;
	entityObj->buildCreateParam(param);
	params.push_back(param);

	//删除本体（形状）
	SHAPE_MANAGER()->destroyShape(entityObj->m_shape);
	entityObj->m_shape = 0;

	//删除本体（质量体）
	MASS_BODY_MANAGER()->destroyMassBody(entityObj->m_massBody);
	entityObj->m_massBody=0;
	
	//删除自身
	m_allObject.erase(entityObj->getID());
	delete entityObj;
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectManager::_destroyJointObject(ObjectJoint* jointObj, std::vector< ObjectParams >& params)
{
	assert(jointObj);
	if(!jointObj) return;

	//焊接点特殊对待
	if(jointObj->getType()==Object::OT_FIXED_JOINT)
	{
		_destroyFixedJointObject((ObjectFixedJoint*)jointObj, params);
		return;
	}

	//保存创建参数,undo使用
	ObjectParams param;
	jointObj->buildCreateParam(param);
	params.push_back(param);

	//删除所连接物体的引用
	if(jointObj->m_firstObj) jointObj->m_firstObj->m_joints.erase(jointObj);
	if(jointObj->m_secondObj) jointObj->m_secondObj->m_joints.erase(jointObj);

	//删除chipmunk数据
	cpSpace* space = SCENE()->getImpl();

	cpConstraint* cpJoint = ( (jointObj->getType()==Object::OT_HINGE_JOINT) ? 
		((ObjectHingeJoint*)jointObj)->m_cpConstraint :
 		((ObjectSpringJoint*)jointObj)->m_cpConstraint);
	if(cpJoint)
		cpSpaceRemoveConstraint(space, cpJoint);

	//删除数据
	m_allObject.erase(jointObj->getID());
	delete jointObj;
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectManager::_destroyFixedJointObject(ObjectFixedJoint* jointObj, std::vector< ObjectParams >& params)
{
	//保存创建参数,undo使用
	{
		ObjectParams param;
		params.push_back(param);
	}
	ObjectParams& param = params.back();
	jointObj->buildCreateParam(param);


	//用于将钉在场景中的物体解除绑定
	bool unThumbSceneMode = (jointObj->m_secondObj==0);

	//所涉及的Group
	Group groupStatic(jointObj->m_firstObj, 0, true);
	
	if(unThumbSceneMode) //试图解除group的静态属性
	{
		bool stillThumbScene = false;
		Group::JointObjectSet& js = groupStatic.getJoints();
		Group::JointObjectSet::iterator js_it, js_end = js.end();
		for(js_it=js.begin(); js_it!=js_end; ++js_it)
		{
			if(*js_it!=jointObj && (*js_it)->m_secondObj==0)
			{
				stillThumbScene=true;
				break;
			}
		}

		//仍旧钉在场景中
		if(stillThumbScene)
		{
			//去除引用
			m_allObject.erase(jointObj->getID());
			jointObj->m_firstObj->m_joints.erase(jointObj);

			//删除物体
			delete jointObj;
			return;
		}
		else
		{
			//重新生成质量体
			_rebuildMassBody(&groupStatic, &(jointObj->getFirstDeleteMassBody()));

			//调整所涉及的铰链
			Group group(jointObj->m_firstObj, 0, false);
			js = group.getJoints();
			js_end= js.end();
			for(js_it=js.begin(); js_it!=js_end; ++js_it)
			{
				ObjectJoint* joint = *js_it;

				//重新检查质量体
				joint->reCheckMassBody();
			}

			//去除引用
			m_allObject.erase(jointObj->getID());
			jointObj->m_firstObj->m_joints.erase(jointObj);
			//删除物体
			delete jointObj;
			return;
		}
	}
	else //试图分割成两个质量体
	{
		//旧的质量体
		MassBody* oldMassBody = jointObj->m_firstObj->getMassBody();

		//去除引用
		jointObj->m_firstObj->m_joints.erase(jointObj);
		jointObj->m_secondObj->m_joints.erase(jointObj);

		//第一个Group
		Group firstStaticGroup(jointObj->m_firstObj, 0, true);
		//是否仍然有静态链接存在
		if(firstStaticGroup.getObjects().find(jointObj->m_secondObj) != firstStaticGroup.getObjects().end())
		{
			//仅仅删除该链接
			m_allObject.erase(jointObj->getID());
			delete jointObj;
			return;
		}
		else
		{
			//第一个Group
			Group firstStaticGroup(jointObj->m_firstObj, 0, true);
			//第二个Group
			Group secondStaticGroup(jointObj->m_secondObj, 0, true);
			
			//重新生成质量体
			_rebuildMassBody(&firstStaticGroup, &(jointObj->getFirstDeleteMassBody()));
			_rebuildMassBody(&secondStaticGroup, &(jointObj->getSecondDeleteMassBody()));

			//调整所涉及的铰链
			{
				//第一个Group
				Group firstGroup(jointObj->m_firstObj, 0, false);

				Group::JointObjectSet& js = firstGroup.getJoints();
				Group::JointObjectSet::iterator js_it, js_end = js.end();
				for(js_it=js.begin(); js_it!=js_end; ++js_it)
				{
					ObjectJoint* joint = *js_it;

					//重新检查质量体
					joint->reCheckMassBody();
				}
			}

			{
				//第一个Group
				Group secondGroup(jointObj->m_secondObj, 0, false);

				Group::JointObjectSet& js = secondGroup.getJoints();
				Group::JointObjectSet::iterator js_it, js_end = js.end();
				for(js_it=js.begin(); js_it!=js_end; ++js_it)
				{
					ObjectJoint* joint = *js_it;

					//重新检查质量体
					joint->reCheckMassBody();
				}
			}

			//销毁原有质量体
			MassBody::cpDataCache cacheData;
			if(oldMassBody)oldMassBody->getCpData(cacheData);
			param.setMassBody(cacheData);

			MASS_BODY_MANAGER()->destroyMassBody(oldMassBody);

			//删除物体
			m_allObject.erase(jointObj->getID());
			delete jointObj;
			return;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectManager::_destroyChainObject(ObjectChain* chainObj, std::vector< ObjectParams >& params)
{
	assert(chainObj);
	if(!chainObj) return;

	//保存创建参数,undo使用
	ObjectParams chainParam;
	chainObj->buildCreateParam(chainParam);
	params.push_back(chainParam);

	cpSpace* space = SCENE()->getImpl();

	//删除hinge joint
	for(int i=0; i<(int)(chainObj->m_hingeList.size()); i++)
	{
		ObjectHingeJoint* jointObj = chainObj->m_hingeList[i];
		if(!jointObj) continue;

		jointObj->notifyDelete();

		//删除所连接物体的引用
		if(jointObj->m_firstObj) jointObj->m_firstObj->m_joints.erase(jointObj);
		if(jointObj->m_secondObj) jointObj->m_secondObj->m_joints.erase(jointObj);

		//删除chipmunk数据
		cpConstraint* cpJoint = ((ObjectHingeJoint*)jointObj)->m_cpConstraint;
		if(cpJoint)
			cpSpaceRemoveConstraint(space, cpJoint);

		//删除数据
		m_allObject.erase(jointObj->getID());
		delete jointObj;
	}

	//删除circle
	for(int i=0; i<(int)(chainObj->m_circleList.size()); i++)
	{
		ObjectEntity* circleObj = chainObj->m_circleList[i];

		//删除所连接的链接体
		ObjectEntity::JointSet joints = circleObj->getJointSet();
		ObjectEntity::JointSet::iterator it, end=joints.end();
		for(it=joints.begin(); it!=end; ++it)
		{
			destroyObject((*it)->getID(), params);
		}

		//删除形状
		SHAPE_MANAGER()->destroyShape(circleObj->m_shape);
		circleObj->m_shape = 0;

		//删除质量体
		MASS_BODY_MANAGER()->destroyMassBody(circleObj->m_massBody);
		circleObj->m_massBody=0;
		
		//删除
		m_allObject.erase(circleObj->getID());
		delete circleObj;
	}

	//删除自身
	m_allObject.erase(chainObj->getID());
	delete chainObj;
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectManager::_rebuildMassBody(Group* group, const MassBody::cpDataCache* massData)
{
	assert(group);

	fVector2 mass_pos(0, 0);
	Real mass_total=0.0;

	Group::EntityObjectSet& es = group->getObjects();
	Group::EntityObjectSet::iterator es_it, es_end=es.end();
	for(es_it=es.begin(); es_it!=es_end; ++es_it)
	{
		ObjectEntity* entity = *es_it;

		Real _mass = entity->getShape()->getArea()*entity->getMaterial().density;
		fVector2 _pos = entity->getTrnasFormMatrix().getOffsetPart();

		mass_pos.x = _pos.x * _mass;
		mass_pos.y = _pos.y * _mass;
		mass_total += _mass;
	}
	mass_pos.x /= mass_total;
	mass_pos.y /= mass_total;

	//转动惯量
	Real moment=0.0;
	for(es_it=es.begin(); es_it!=es_end; ++es_it)
	{
		ObjectEntity* entity = *es_it;

		Real _mass = entity->getShape()->getArea()*entity->getMaterial().density;
		fVector2 _off = entity->getTrnasFormMatrix().getOffsetPart()-mass_pos;
		moment += (entity->getShape()->getMoment() + _off.square()) * _mass;
	}

	//生成质量体
	MassBody* mass_body = MASS_BODY_MANAGER()->createMassBody(mass_pos, mass_total, moment, massData);

	//chipmunk空间加入质量体
	cpSpaceAddBody(SCENE()->getImpl(), mass_body->getImpl());  

	//重新设置原有物体的质量体
	for(es_it=es.begin(); es_it!=es_end; ++es_it)
	{
		ObjectEntity* entity = *es_it;

		entity->getShape()->resetMassOwner(mass_body);
		entity->m_massBody = mass_body;

		mass_body->attachObject(entity);
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FILE* fp =0;
static void _debugMassBody(cpBody* cmBody, void* data)
{
	fprintf(fp, "Mass:%f,  Pos:<%f,%f>, Speed<%f,%f>\n", cmBody->m, cmBody->p.x, cmBody->p.y, cmBody->v.x, cmBody->v.y);
}

static void _debugCircleShape(cpBody *body, cpCircleShape *circle, cpSpace *space)
{
	fprintf(fp, "CircleShape: <%f,%f><%f,%f> -> %f\n", 
            circle->c.x, circle->c.y, circle->tc.x, circle->tc.y, circle->r);
}

static void _debugSegmentShape(cpBody *body, cpSegmentShape *seg, cpSpace *space)
{
}

static void _debugPolyShape(cpBody *body, cpPolyShape *poly, cpSpace *space)
{
	int count = poly->numVerts;
	fprintf(fp, "PolygonShape: counts=%d\n", count);
	for(int i=0; i<count; i++)
	{
		fprintf(fp, "<%f, %f><%f, %f>\n", poly->verts[i].x, poly->verts[i].y,  poly->tVerts[i].x, poly->tVerts[i].y);
	}
}

static void _debugShape(cpShape *shape, cpSpace *space)
{
	cpBody *body = shape->body;
	switch(shape->CP_PRIVATE(klass)->type)
	{
	case CP_CIRCLE_SHAPE:
		_debugCircleShape(body, (cpCircleShape *)shape, space);
		break;
	case CP_SEGMENT_SHAPE:
		_debugSegmentShape(body, (cpSegmentShape *)shape, space);
		break;
	case CP_POLY_SHAPE:
		_debugPolyShape(body, (cpPolyShape *)shape, space);
		break;
        default: break;
	}
}

static void _debugConstraint(cpConstraint *ptr)
{
	if(ptr->CP_PRIVATE(klass) == cpDampedSpringGetClass())
	{
		cpDampedSpring* spring = (cpDampedSpring*)ptr;
		fprintf(fp, "Spring: <%f,%f>-<%f,%f>, restLen=%f, stiff=%f, damping=%f\n", 
			spring->anchr1.x, spring->anchr1.y,
			spring->anchr2.x, spring->anchr2.y,
			spring->restLength, spring->stiffness, spring->damping);
	}
}

//-------------------------------------------------------------------------------------------------------------------
void ObjectManager::debug(const char* fileName)
{
	cpSpace* space = SCENE()->getImpl();

	fp = fopen(fileName, "w");
	fprintf(fp, "-------MassBody---------\n");
	cpSpaceEachBody(space, _debugMassBody, fp);
	
	fprintf(fp, "-------ActiveShape------------\n");
	cpSpatialIndexEach(space->CP_PRIVATE(activeShapes), (cpSpatialIndexIteratorFunc)_debugShape, space);

	fprintf(fp, "-------StaticShape------------\n");
	cpSpatialIndexEach(space->CP_PRIVATE(staticShapes), (cpSpatialIndexIteratorFunc)_debugShape, space);

	fprintf(fp, "-------Constraint------------\n");
	for(int i=0; i<space->CP_PRIVATE(constraints)->num; i++) 
	{
		//freeFunc(arr->arr[i]);
	}
	//cpArrayFreeEach(space->CP_PRIVATE(constraints), &_debugConstraint, 0);

	fclose(fp);
}

}
