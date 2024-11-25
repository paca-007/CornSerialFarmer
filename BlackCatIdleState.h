#pragma once
#include "FSMState.h"

/// <summary>
/// �ۼ��� : 2023.08.15
/// �ۼ��� : ������
/// 
/// Cat�� ���� ���¸� �����ϴ� Ŭ����
/// </summary>

class BlackCatIdleState :
    public FSMState
{
public:
    BlackCatIdleState(StateMachine* owner);
    ~BlackCatIdleState();

public:
    virtual void Enter();
    virtual void Update(float deltaTime);
    virtual void Exit();
};

