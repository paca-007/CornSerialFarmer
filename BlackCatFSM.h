#pragma once
#include "StateMachine.h"

#include "BlackCatIdleState.h"
#include "BlackCatMoveState.h"

/// <summary>
/// �ۼ��� : 2023.08.15
/// �ۼ��� : ������
/// 
/// BlackCat�� FSM
/// </summary>

class BlackCatFSM :
    public StateMachine
{

public:
    BlackCatFSM(GameObject* owner);
    ~BlackCatFSM();

public:
    //���µ�
    BlackCatIdleState m_BlackCatIdleState;
    BlackCatMoveState m_BlackCatMoveState;

    virtual void Update(float deltaTime) override;

};

