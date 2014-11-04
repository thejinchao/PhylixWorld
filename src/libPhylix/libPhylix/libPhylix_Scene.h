#ifndef __PHYSICS_WORLD_LIB_SCENE_H__
#define __PHYSICS_WORLD_LIB_SCENE_H__

#include "libPhylix_Handler.h"
#include "libPhylix_Object.h"
#include "libPhylix_ObjectParams.h"

struct cpSpace;
struct cpShape;
struct cpBody;

namespace Phylix
{
//pre-define
class IRenderInterface;
class ObjectManager;
class ObjectEntity;
class SceneSnap;
class IFileStream;

/** 场景数据
*/
class Scene
{
public:
	/** 场景边界数据 */
	enum { SCENE_MIN_BORDER = -1000, SCENE_MAX_BORDER = 1000 };
	/** 缺省重力值(10N/kg) */
	enum { SCENE_GRAVITY = 10 };

public:
	/** 初始化场景 */
	void resetScene(bool gravity=true, int iterCounts=10, Real hashZoneSize=5.0, int hashZoneCounts=2048);
	/** 逻辑帧 */
	void tick(Real step_time);
	/** 当前场景事件戳 */
	Real getTimeStamp(void) const { return m_sceneTimeStamp; }
	/** 从场景快照中恢复到某个时间 */
	void restoreSceneSanp(Real timeSnap);
	/** 渲染帧 */
	void render(IRenderInterface* render);
	/** 获得包围盒 */
	const AABB& getWorldBounderBox(void) const { return m_worldBounderBox; }
	/** 获得chipmunk数据*/
	cpSpace* getImpl(void) { return m_cmSpace; }
	/** 查询某个位置下的所有Entity物体, 按照z排序 */
	void queryEntityObjectFromPoint(const fVector2& point, ZOrderSet& objectResult);
	/** 查询某个位置下第一个object*/
	Object* queryFirstObjectFromPoint(const fVector2& point);
	/** 选择某个id的物体 */
	void selectObject(unsigned int id);
	/** 获得被选中的物体 */
	unsigned int getSelectedObject(void) const { return m_selectedObj; }

private:
	/** 释放场景数据 */
	void _releaseScene(void);
	/** 刷新MassBody数据*/
	static void _updateMassBody(cpBody* cmBody, void* data);
	/** 尝试做场景快照 */
	void _tryMakeSceneSnap(void);
	/** 用来排除同一个铰链连接的物体之间的排斥 */
	static cpBool _cpExtraRejectFunction(cpShape *a, cpShape *b);

private:
	AABB m_worldBounderBox;	//!< 最大包围盒
	cpSpace* m_cmSpace;		//!< chipmunk 空间
	unsigned int m_selectedObj;	//!< 被选择的物体id;
	Real m_sceneTimeStamp;	//!< 当前场景时间

#if 0
	/***********************************************
		for scene serial
	************************************************/
public:
	/** 保存场景到文件中*/
	bool saveScene(IFileStream* stream);
	/** 从场景文件中加载场景 */
	bool loadScene(IFileStream* stream);

	static void _startSceneElement(void* data, const char* element, const char** attr);
	static void _endSceneElement(void* data, const char* element);

	//for load scene
	enum LoadSceneState
	{
		LSS_NULL,
		LSS_SCENE,
		LSS_OBJECT,
	};
	LoadSceneState m_currentLoadState;
	ObjectParams m_currentObjectParams;
#endif

public:
	Scene();
	virtual ~Scene();

};


}

#endif
