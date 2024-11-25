#pragma once
#include "FSMState.h"

/// <summary>
/// �ۼ��� : 2023.08.17
/// �ۼ��� : ������
/// 
/// ����� �� �ִ� ���¸� �����ϴ� Ŭ����
/// </summary>

class WateringState :
    public FSMState
{
public:
    WateringState(StateMachine* owner);
    ~WateringState();

public:
    virtual void Enter();
    virtual void Update(float deltaTime);
    virtual void Exit();
};

