#pragma once
#include "StateMachine.h"

#include "FarmNoWaterState.h"
#include "FarmWaterState.h"

/// <summary>
/// �ۼ��� : 2023.08.15
/// �ۼ��� : ������
/// 
/// Farm�� FSM
/// </summary>


class FarmFSM :
    public StateMachine
{

public:
    FarmFSM(GameObject* owner);
    ~FarmFSM();

public:
    //���µ�
    FarmNoWaterState m_FarmNoWaterState;
    FarmWaterState m_FarmWaterState;

    virtual void Update(float deltaTime) override;
};

