#pragma once
#include "FSMState.h"

/// <summary>
/// �ۼ��� : 2023.08.11
/// �ۼ��� : ������
/// 
/// �̺�Ʈ �߻� �� ������ҷ� ���ư��� ����
/// </summary>

class StateMachine;

class NPC1ReturnState :
    public FSMState
{
public:
    NPC1ReturnState(StateMachine* owner);
    ~NPC1ReturnState();

public:
	virtual void Enter();
	virtual void Update(float deltaTime);
	virtual void Exit();

};

