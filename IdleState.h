#pragma once
#include "FSMState.h"

/// <summary>
/// �ۼ��� : 2023.08.09
/// �ʱ� �ۼ��� : ������
/// 
/// �⺻���� Idle�� �����ϴ� Ŭ����
/// </summary>

class StateMachine;
class IdleState 
	: public FSMState
{

public:
	IdleState(StateMachine* owner);
	~IdleState();

public:
	virtual void Enter();
	virtual void Update(float deltaTime);
	virtual void Exit();

};

