#ifndef __PHYSICS_WORLD_LIB_SCENE_SNAP_H__
#define __PHYSICS_WORLD_LIB_SCENE_SNAP_H__

#include "libPhylix_MassBody.h"

namespace Phylix
{

/** 场景快照
*/
class SceneSnap
{
public:
	typedef std::vector< MassBody::cpDataCache > MassBodyVector;

public:
	/** 清空数据*/
	void clean(void);
	/** 获得当前快照*/
	void makeCurrentSanp(void);
	/** 将当前快照恢复到场景中 */
	void restoreToScene(void);
	/** 获得快照的时间戳 */
	Real getTimeStamp(void) const { return m_timeStamp; }
	/** 获得质量体数据*/
	const MassBodyVector& getMassBodyCache(void) const { return m_massBodyData; }

private:
	MassBodyVector m_massBodyData;	//!< 质量体数据
	Real m_timeStamp;				//!< 时间

public:
	SceneSnap();
	~SceneSnap();
};

}


#endif
