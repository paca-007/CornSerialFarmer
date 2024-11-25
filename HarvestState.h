#pragma once
#include "FSMState.h"

/// <summary>
/// �ۼ��� : 2023.08.17
/// �ۼ��� : ������
/// 
/// Farmer�� ���۹��� ��Ȯ�ϴ� ���¸� �����ϴ� Ŭ����
/// </summary>

class HarvestState :
    public FSMState
{
public:
    HarvestState(StateMachine* owner);
    ~HarvestState();

public:
    virtual void Enter();
    virtual void Update(float deltaTime);
    virtual void Exit();
};

