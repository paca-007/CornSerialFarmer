#pragma once
#include "FSMState.h"

/// <summary>
/// �ۼ��� : 2023.08.14
/// �ۼ��� : ������
/// 
/// NPC2�� Return(��ȯ)�� �����ϴ� Ŭ����
/// </summary>

class NPC2ReturnState :
    public FSMState
{
public:
    NPC2ReturnState(StateMachine* owner);
    ~NPC2ReturnState();

public:
	virtual void Enter();
	virtual void Update(float deltaTime);
	virtual void Exit();
};

