#pragma once
#include "FSMState.h"

/// <summary>
/// �ۼ��� : 2023.08.14
/// �ۼ��� : ������
/// 
/// NPC2�� ��ü�� �̵� ���¸� �����ϴ� Ŭ����
/// </summary>

class NPC2FindState :
    public FSMState
{
public:
    NPC2FindState(StateMachine* owner);
    ~NPC2FindState();

public:
    virtual void Enter();
    virtual void Update(float deltaTime);
    virtual void Exit();
};

