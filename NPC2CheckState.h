#pragma once
#include "FSMState.h"

/// <summary>
/// �ۼ��� : 2023.08.14
/// �ۼ��� : ������
/// 
/// NPC2�� ��ü Ȯ�� ���¸� �����ϴ� Ŭ����
/// </summary>

class NPC2CheckState :
    public FSMState
{
public:
    NPC2CheckState(StateMachine* owner);
    ~NPC2CheckState();

public:
	virtual void Enter();
	virtual void Update(float deltaTime);
	virtual void Exit();
};

