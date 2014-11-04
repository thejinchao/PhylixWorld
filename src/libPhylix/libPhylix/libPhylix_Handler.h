#ifndef __PHYSICS_WORLD_LIB_HANDLER_H__
#define __PHYSICS_WORLD_LIB_HANDLER_H__

namespace Phylix
{

//pre-define
class MassBodyManager;
class MaterialManager;
class ObjectManager;
class ShapeManager;
class ActionManager;
class Camera;
class Scene;
class EventManager;
class OperatorManager;

/** 用于保存常用的管理器指针
*/
class Handler
{
public:
	void setHandler(void* system,
        MassBodyManager* massManager, 
		MaterialManager* materialManager, 
		ObjectManager* objManager, 
		ShapeManager* shapeManager, 
		ActionManager* actionManager,
		Camera* camera,
		Scene* scene,
		EventManager* eventManager,
		OperatorManager* operatorManager)
	{
        m_system = system;
		m_massManager = massManager;
		m_materialManager = materialManager;
		m_objectManager = objManager;
		m_shapeManager = shapeManager;
		m_actionManager = actionManager;
		m_cameraPtr = camera;
		m_scenePtr = scene;
		m_eventManager = eventManager;
		m_operatorManager = operatorManager;
	}

    void* getSystem(void) { return m_system; }
	MassBodyManager* getMassBodyManager(void) { return m_massManager; }
	MaterialManager* getMaterialManager(void) { return m_materialManager; }
	ObjectManager* getObjectManager(void) { return m_objectManager; }
	ShapeManager* getShapeManager(void) { return m_shapeManager; }
	ActionManager* getActionManager(void) { return m_actionManager; }
	Camera* getCamera(void) { return m_cameraPtr; }
	Scene* getScene(void) { return m_scenePtr; }
	EventManager* getEventManager(void) { return m_eventManager; }
	OperatorManager* getOperatorManager(void) { return m_operatorManager; }

private:
    void* m_system;
	MassBodyManager* m_massManager;
	MaterialManager* m_materialManager;
	ObjectManager* m_objectManager;
	ShapeManager* m_shapeManager;
	ActionManager* m_actionManager;
	Camera* m_cameraPtr;
	Scene* m_scenePtr;
	EventManager* m_eventManager;
	OperatorManager* m_operatorManager;

public:
	Handler() 
		: m_system(0)
        , m_massManager(0)
		, m_materialManager(0)
		, m_objectManager(0)
		, m_shapeManager(0)
		, m_actionManager(0)
		, m_cameraPtr(0)
		, m_scenePtr(0)
		, m_eventManager(0)
		, m_operatorManager(0)
	{
	}
	~Handler() {}
};

/** API函数，用于返回唯一句柄，上层提供
*/
Handler* getHandler(void);

/** 快捷定义
*/
inline MassBodyManager* MASS_BODY_MANAGER(void) { return getHandler()->getMassBodyManager(); }
inline MaterialManager* MATERIAL_MANAGER(void) { return getHandler()->getMaterialManager(); }
inline ObjectManager* OBJECT_MANAGER(void) { return getHandler()->getObjectManager(); }
inline ShapeManager* SHAPE_MANAGER(void) { return getHandler()->getShapeManager(); }
inline ActionManager* ACTION_MANAGER(void) { return getHandler()->getActionManager(); }
inline Camera* CAMERA(void) { return getHandler()->getCamera(); }
inline Scene* SCENE(void) { return getHandler()->getScene(); }
inline EventManager* EVENT_MANAGER(void) { return getHandler()->getEventManager(); }
inline OperatorManager* OPERATOR_MANAGER(void) { return getHandler()->getOperatorManager(); }

}


#endif
