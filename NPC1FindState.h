#pragma once
#include "FSMState.h"

/// <summary>
/// �ۼ���: 2023.08.14
/// �ۼ���: ������
/// 
/// npc1�� ��ü�� �̵��ϴ� ���¸� �����ϴ� Ŭ����
/// </summary>

class StateMachine;

class NPC1FindState :
    public FSMState
{
public:
    NPC1FindState(StateMachine* owner);
    ~NPC1FindState();

public:
	virtual void Enter();
	virtual void Update(float deltaTime);
	virtual void Exit();

};

