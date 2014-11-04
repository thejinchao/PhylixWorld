#include "libPhylix_stdafx.h"
#include "libPhylix_Scene.h"
#include "libPhylix_RenderInterface.h"
#include "libPhylix_ObjectManager.h"
#include "libPhylix_Object.h"
#include "libPhylix_Shape.h"
#include "libPhylix_MassBody.h"
#include "libPhylix_MassBodyManager.h"
#include "libPhylix_Object_Joint_Fixed.h"
#include "libPhylix_SceneSnap.h"
#include "libPhylix_OperatorManager.h"
#include "libPhylix_operator.h"
#include "libPhylix_Object_Entity.h"
#include "libPhylix_FileStream.h"
#include "libPhylix_StringUtil.h"

//#include <expat.h>

static void _updateBBCache(cpShape *shape, void *unused) { cpShapeCacheBB(shape); }

namespace Phylix
{

//-------------------------------------------------------------------------------------------------------------------
Scene::Scene()
	: m_worldBounderBox(fVector2(SCENE_MIN_BORDER, SCENE_MIN_BORDER), fVector2(SCENE_MAX_BORDER, SCENE_MAX_BORDER))
	, m_cmSpace(0)
	, m_selectedObj(0)
	, m_sceneTimeStamp(0.0)
{
	//初始化chipmunk
	cpInitChipmunk();
}

//-------------------------------------------------------------------------------------------------------------------
Scene::~Scene()
{
	_releaseScene();
}

//-------------------------------------------------------------------------------------------------------------------
static void _shapeFreeWrap(cpSpace *space, cpShape *shape, void *unused){
	cpSpaceRemoveShape(space, shape);
	cpShapeFree(shape);
}

//-------------------------------------------------------------------------------------------------------------------
static void _postShapeFree(cpShape *shape, cpSpace *space){
	cpSpaceAddPostStepCallback(space, (cpPostStepFunc)_shapeFreeWrap, shape, NULL);
}

//-------------------------------------------------------------------------------------------------------------------
static void _constraintFreeWrap(cpSpace *space, cpConstraint *constraint, void *unused){
	cpSpaceRemoveConstraint(space, constraint);
	cpConstraintFree(constraint);
}

//-------------------------------------------------------------------------------------------------------------------
static void _postConstraintFree(cpConstraint *constraint, cpSpace *space){
	cpSpaceAddPostStepCallback(space, (cpPostStepFunc)_constraintFreeWrap, constraint, NULL);
}

//-------------------------------------------------------------------------------------------------------------------
static void _bodyFreeWrap(cpSpace *space, cpBody *body, void *unused){
	cpSpaceRemoveBody(space, body);
	cpBodyFree(body);
}

//-------------------------------------------------------------------------------------------------------------------
static void _postBodyFree(cpBody *body, cpSpace *space){
	cpSpaceAddPostStepCallback(space, (cpPostStepFunc)_bodyFreeWrap, body, NULL);
}

//-------------------------------------------------------------------------------------------------------------------
void Scene::_releaseScene(void)
{
	if(m_cmSpace)
	{
		// Must remove these BEFORE freeing the body or you will access dangling pointers.
		cpSpaceEachShape(m_cmSpace, (cpSpaceShapeIteratorFunc)_postShapeFree, m_cmSpace);
		cpSpaceEachConstraint(m_cmSpace, (cpSpaceConstraintIteratorFunc)_postConstraintFree, m_cmSpace);
		cpSpaceEachBody(m_cmSpace, (cpSpaceBodyIteratorFunc)_postBodyFree, m_cmSpace);

		cpSpaceFree(m_cmSpace);

		m_cmSpace = 0;
	}
}

//-------------------------------------------------------------------------------------------------------------------
void Scene::resetScene(bool gravity, int iterCounts, Real hashZoneSize, int hashZoneCounts)
{
	//释放旧场景
	_releaseScene();

	//创建chipmunk空间
	m_cmSpace = cpSpaceNew();

	//迭代次数
	cpSpaceSetIterations(m_cmSpace, 30);

	//hash空间分割
//	cpSpaceResizeStaticHash(m_cmSpace, hashZoneSize, hashZoneCounts);
//	cpSpaceResizeActiveHash(m_cmSpace, hashZoneSize, hashZoneCounts);
	//重力参数
	if(gravity) cpSpaceSetGravity(m_cmSpace, cpv(0, -SCENE_GRAVITY));
	//m_cmSpace->gravity = gravity ? cpv(0.0, -SCENE_GRAVITY) : cpv(0.0, 0.0);
	cpSpaceSetSleepTimeThreshold(m_cmSpace, 0.5f);
	//扩展排斥函数
	m_cmSpace->extraRejectFunc = _cpExtraRejectFunction;
}

//-------------------------------------------------------------------------------------------------------------------
static void _funQueryPointEntity(cpShape *shape, void* param)
{
	ZOrderSet& objectResult = *((ZOrderSet*)param);

	Object* entity = (Object*)((Shape* )(shape->data))->getOwner();

	objectResult.insert(entity);
}

//-------------------------------------------------------------------------------------------------------------------
void Scene::queryEntityObjectFromPoint(const fVector2& point, ZOrderSet& objectResult)
{
	objectResult.clear();
	cpSpacePointQuery(m_cmSpace, cpv(point.x, point.y),  CP_ALL_LAYERS, CP_NO_GROUP, _funQueryPointEntity, &objectResult);
}

//-------------------------------------------------------------------------------------------------------------------
static void _funRemeberLastObj(cpShape *shape, void* param)
{
	Object* lastObj = *((Object**)param);
	Object* obj = (Object*)((Shape* )(shape->data))->getOwner();
	if(lastObj==0 || lastObj->getZOrder()<obj->getZOrder())
	{
		*((Object**)param) = obj;
	}
}

//-------------------------------------------------------------------------------------------------------------------
Object* Scene::queryFirstObjectFromPoint(const fVector2& point)
{
	Object* lastObj = 0;
	//首先查询entity
	cpSpacePointQuery(m_cmSpace, cpv(point.x, point.y),  CP_ALL_LAYERS, CP_NO_GROUP, _funRemeberLastObj, &lastObj);

	//查询joint
	ObjectManager::ObjectMapIterator it = OBJECT_MANAGER()->getObjectIterator();
	while(it.hasMoreElements())
	{
		Object* obj = it.getNext();
		Object::Type type = obj->getType();

		if(type == Object::OT_FIXED_JOINT || type==Object::OT_HINGE_JOINT || type==Object::OT_SPRING_JOINT)
		{
			if((lastObj==0 || obj->getZOrder() > lastObj->getZOrder()) && 
				((ObjectJoint*)obj)->isInLegendArea(point))
			{
				lastObj = obj;
			}
		}
	}

	return lastObj;
}

//-------------------------------------------------------------------------------------------------------------------
void Scene::selectObject(unsigned int id)
{
	m_selectedObj = id;

	Object* obj = OBJECT_MANAGER()->findObject(id);
	if(obj) obj->notifySelected();
}

//-------------------------------------------------------------------------------------------------------------------
void Scene::_updateMassBody(cpBody* cmBody, void* data)
{
	MassBody* body = (MassBody*)(cmBody->data);
	if(body)
	{
		body->updateFromImpl();
	}
}

//-------------------------------------------------------------------------------------------------------------------
void Scene::tick(Real step_time)
{
	if(!m_cmSpace) return;
	if(floatCompare(step_time, 0.0)==0) return;

	//如果最后一个operator不是snap，并且场景中移动质量体不为空，做一个场景快照
	_tryMakeSceneSnap();

	m_sceneTimeStamp += step_time;
	cpSpaceStep(m_cmSpace, step_time);

	//刷新质量体的位置
	cpSpaceEachBody(m_cmSpace, _updateMassBody, this);
}

//-------------------------------------------------------------------------------------------------------------------
void Scene::_tryMakeSceneSnap(void)
{
	//最后一个operator已经是场景快照
	Operator* lastOp = OPERATOR_MANAGER()->getLastOperator();
	if(lastOp && lastOp->getType()==OT_SCENE_SNAP) return;

	//场景中是否包含移动质量体
	if(cpSpatialIndexCount(m_cmSpace->activeShapes)==0) return; 

	//加入场景快照
	OPERATOR_MANAGER()->applyOperator(new Operator_SceneSnap());
}

//-------------------------------------------------------------------------------------------------------------------
void Scene::restoreSceneSanp(Real timeSnap)
{
	//恢复时间
	m_sceneTimeStamp = timeSnap;

	//重新刷新上层质量体的位置
	cpSpaceEachBody(m_cmSpace, _updateMassBody, this);
}

//-------------------------------------------------------------------------------------------------------------------
void Scene::render(IRenderInterface* render)
{
	//浏览所有物体
	ZOrderSet zSortObjects;
	{
		ObjectManager::ObjectMapIterator it = OBJECT_MANAGER()->getObjectIterator();
		while(it.hasMoreElements())
		{
			zSortObjects.insert(it.getNext());
		}
	}

	//按照z顺序渲染
	ZOrderSet::iterator it, end=zSortObjects.end();
	for(it=zSortObjects.begin(); it!=end; ++it)
	{
		(*it)->draw(render);
	}

}

//-------------------------------------------------------------------------------------------------------------------
cpBool Scene::_cpExtraRejectFunction(cpShape *a, cpShape *b)
{
	Shape* a_shape = (Shape*)(a->data);
	Shape* b_shape = (Shape*)(b->data);
	ObjectEntity* a_obj = a_shape->getOwner();
	ObjectEntity* b_obj = b_shape->getOwner();
	const ObjectEntity::JointSet& a_joint = a_obj->getJointSet();
	const ObjectEntity::JointSet& b_joint = b_obj->getJointSet();
	if(a_joint.empty() || b_joint.empty()) return false;

	ObjectEntity::JointSet::const_iterator it, end=a_joint.end();
	for(it=a_joint.begin(); it!=end; ++it)
	{
		ObjectJoint* joint = *it;

		if(joint->getType() != Object::OT_HINGE_JOINT) continue;

		if(b_joint.find(joint) != b_joint.end()) return true;
	}

	return false;
}
#if 0
//-------------------------------------------------------------------------------------------------------------------
bool Scene::saveScene(IFileStream* stream)
{
	stream->writeParam("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n");
	stream->writeParam("<Scene>\n");

	ObjectManager::ObjectMapIterator obj_it = OBJECT_MANAGER()->getObjectIterator();
	while(obj_it.hasMoreElements())
	{
		Object* obj = obj_it.getNext();

		ObjectParams params;
		obj->buildCreateParam(params);

		stream->writeParam("<Object type=\"%s\">\n", toString(params.getObjectType()).c_str());

		for(int i=0; i<ObjectParams::PT_COUNTS; i++)
		{
			if(!params.getFlag((ObjectParams::PARAM_TYPE)i)) continue;

			#define CASE_WRITE_PARAM(_type, _getFunc) \
			case ObjectParams::_type: \
				{ \
					stream->writeParam("\t<Param name=\"%s\" value=\"%s\" />\n", toString((ObjectParams::PARAM_TYPE)i).c_str(), \
						toString(params._getFunc()).c_str()); \
				} \
				break;

			switch(i)
			{
				CASE_WRITE_PARAM(PT_POSITION_1, getPosition1)
				CASE_WRITE_PARAM(PT_POSITION_2, getPosition2)
				CASE_WRITE_PARAM(PT_ANGLE, getAngle)
				CASE_WRITE_PARAM(PT_SIZE, getSize)
				CASE_WRITE_PARAM(PT_RADIUS, getRadius)
				CASE_WRITE_PARAM(PT_WIDTH, getWidth)
				CASE_WRITE_PARAM(PT_POINTS, getPoints)
				CASE_WRITE_PARAM(PT_MATERIAL, getMaterial)
				CASE_WRITE_PARAM(PT_OBJECT_ID, getObjectID)
				CASE_WRITE_PARAM(PT_MASSBODY, getMassBody)
				CASE_WRITE_PARAM(PT_Z_VALUE, getZValue)
			}
		}
		stream->writeParam("</Object>\n", obj->getType());
	}
	stream->writeParam("</Scene>\n");
	return true;
}

//-------------------------------------------------------------------------------------------------------------------
void Scene::_startSceneElement(void* data, const char* element, const char** attr)
{
	Scene* pThis = (Scene*)data;
	if(_stricmp(element, "Scene")==0)
	{
		assert(pThis->m_currentLoadState==LSS_NULL);
		pThis->m_currentLoadState = LSS_SCENE;
	}
	else if(_stricmp(element, "Object")==0)
	{
		assert(pThis->m_currentLoadState==LSS_SCENE);
		pThis->m_currentLoadState = LSS_OBJECT;

		const char* name=attr[0];
		const char* value=attr[1];

		Object::Type objType;
		fromString(value, objType);
		pThis->m_currentObjectParams.reset(objType);
	}
	else if(_stricmp(element, "Param")==0)
	{
		assert(pThis->m_currentLoadState==LSS_OBJECT);

		const char* name1=attr[0];
		const char* value1=attr[1];

		const char* name2=attr[2];
		const char* value2=attr[3];

		ObjectParams::PARAM_TYPE paramType;
		fromString(value1, paramType);

		//===========
		#define CASE_READ_PARAM(_type, _data_type, _setFunc) \
		case ObjectParams::_type: \
			{ \
				_data_type value; \
				fromString(value2, value); \
				pThis->m_currentObjectParams._setFunc(value); \
			} \
			break; 
		//===========


		switch(paramType)
		{
		CASE_READ_PARAM(PT_POSITION_1, fVector2, setPosition1)
		CASE_READ_PARAM(PT_POSITION_2, fVector2, setPosition2)
		CASE_READ_PARAM(PT_ANGLE, Angle, setAngle)
		CASE_READ_PARAM(PT_SIZE, fVector2, setSize)
		CASE_READ_PARAM(PT_RADIUS, Real, setRadius)
		CASE_READ_PARAM(PT_WIDTH, Real, setWidth)
		CASE_READ_PARAM(PT_POINTS, PointVector, setPoints)
		CASE_READ_PARAM(PT_MATERIAL, Material, setMaterial)
		CASE_READ_PARAM(PT_OBJECT_ID, unsigned int, setObjectID)
		CASE_READ_PARAM(PT_MASSBODY, MassBody::cpDataCache, setMassBody)
		CASE_READ_PARAM(PT_Z_VALUE, Real, setZValue)
		}

	}
}

//-------------------------------------------------------------------------------------------------------------------
void Scene::_endSceneElement(void* data, const char* element)
{
	Scene* pThis = (Scene*)data;
	if(_stricmp(element, "Scene")==0)
	{
		assert(pThis->m_currentLoadState==LSS_SCENE);
		pThis->m_currentLoadState = LSS_NULL;
	}
	else if(_stricmp(element, "Object")==0)
	{
		assert(pThis->m_currentLoadState==LSS_OBJECT);
		pThis->m_currentLoadState = LSS_SCENE;

		OBJECT_MANAGER()->createObject(pThis->m_currentObjectParams);
	}
}

//-------------------------------------------------------------------------------------------------------------------
bool Scene::loadScene(IFileStream* stream)
{
	XML_Parser parser = XML_ParserCreate(0); // Create a parser
	
	size_t fileSize = stream->size();
	char* pData = new char[fileSize];
	stream->read((unsigned char*)pData, fileSize);

	XML_SetUserData(parser, (void*)this); // Initialise user data
	XML_SetElementHandler(parser, _startSceneElement, _endSceneElement); // Register callback for elements

	m_currentLoadState = LSS_NULL;
	XML_Parse(parser, pData, (int)fileSize, true);

	XML_ParserFree(parser);
	delete[] pData; pData=0;
	return true;
}
#endif
    
}
