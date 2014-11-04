#ifndef __PHYSICS_WORLD_LIB_OPERATOR_MANAGER_H__
#define __PHYSICS_WORLD_LIB_OPERATOR_MANAGER_H__


namespace Phylix
{
//pre-define
class Operator;

/** 动作管理器
*/
class OperatorManager
{
public:
	/** 添加一个动作 */
	void applyOperator(Operator* op);
	/** 执行undo操作 */
	void undo(void);
	/** 执行redo操作*/
	void redo(void);
	/** 能否执行undo操作 */
	bool canUndo(void) const;
	/** 能否执行redo操作 */
	bool canRedo(void) const;
	/** 清空redo列表 */
	void cleanRedoList(void);
	/** 获得最后一个Operator */
	Operator* getLastOperator(void);

	void _debug(void);

private:
	typedef std::list< Operator* > OperatorList;

	OperatorList m_undoList;		//!< 可以执行undo操作的列表
	OperatorList m_redoList;		//!< 可以执行redo操作的列表

private:
	/** 清空某个列表 */
	void _clearOperatorList(OperatorList& opList);

public:
	OperatorManager();
	~OperatorManager();
};

}

#endif
