#pragma once
#include "FSMState.h"

/// <summary>
/// �ۼ��� : 2023.08.16
/// �ۼ��� : ������
/// 
/// ������ ���� ���¸� �����ϴ� Ŭ����
/// </summary>

class PolicePatrolState :
    public FSMState
{
public:
    PolicePatrolState(StateMachine* owner);
    ~PolicePatrolState();

public:
	virtual void Enter();
	virtual void Update(float deltaTime);
	virtual void Exit();
};

