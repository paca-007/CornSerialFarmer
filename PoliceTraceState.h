#pragma once
#include "FSMState.h"
/// <summary>
/// �ۼ��� : 2023.08.16
/// �ۼ��� : ������
/// 
/// ������ ���� ���¸� �����ϴ� Ŭ����
/// </summary>

class PoliceTraceState 
	: public FSMState
{
public:
	PoliceTraceState(StateMachine* onwer);
	~PoliceTraceState();

public:
	virtual void Enter();
	virtual void Update(float deltaTime);
	virtual void Exit();
};

