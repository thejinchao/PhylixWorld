#ifndef __PHYSICS_WORLD_LIB_MASS_BODY_H__
#define __PHYSICS_WORLD_LIB_MASS_BODY_H__

namespace Phylix
{
//pre-define
class ObjectEntity;

/** 代表具有质量的点，一个MassBody可能由数个Object共有
*/
class MassBody
{
public:
	struct cpDataCache //在chipmunk空间的部分数据
	{
		// Linear components of motion (position, velocity, and force)
		cpVect p, v, f;
		// Angular components of motion (angle, angular velocity, and torque)
		// Always use cpBodySetAngle() to set the angle of the body as a and rot must agree.
		cpFloat a, w, t;
		// Cached unit length vector representing the angle of the body.
		// Used for fast vector rotation using cpvrotate().
		cpVect rot;
		// 所对应的MassBodyID
		unsigned int massBodyID;

		cpDataCache() : massBodyID(0) { }
	};

public:
	typedef std::set< ObjectEntity* > ObjectSet;

public:
	/** 获得id*/
	unsigned int getID(void) const { return m_id; }

	/** 获取变换矩阵 */
	const fMatrix3& getTrnasFormMatrix(void) const { return m_transForm; }

	/** 获得位置 */
	fVector2 getPosition(void) const { return getTrnasFormMatrix().getOffsetPart(); }

	/** 获得质量 */
	Real getMass(void) const { return m_mass; }

	/** 更新参数 */
	void updateParam(Real mass, Real moment);

	/** 获得chipmunk数据*/
	cpBody* getImpl(void) { return m_cmBody; }
	/** 从chipmunk更新数据 */
	void updateFromImpl(void);
	/** 附加形体 */
	void attachObject(ObjectEntity* obj);
	/** 获得附加的物体列表 */
	const ObjectSet& getAttachedObject(void) const { return m_setObject; }
	/** 获得chipmunk中的数据 */
	void getCpData(cpDataCache& data);

private:
	unsigned int m_id;			//!< 唯一 id
	fMatrix3 m_transForm;		//!< 世界坐标的偏移和旋转数据,这个数据和chipmunk的body中的数据通过updateFromImpl保持同步

	Real m_mass;				//!< 质量
	cpBody* m_cmBody;			//!< chipmunk的massbody指针

	ObjectSet m_setObject;		//!< 依附在该质量体上的物体

private:
	MassBody(unsigned int id, const fVector2& pos);
	~MassBody();
	friend class MassBodyManager;
};

}

#endif

