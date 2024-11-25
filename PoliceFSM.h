#pragma once
#include "StateMachine.h"

#include "PoliceCatchState.h"
#include "PoliceFindState.h"
#include "PolicePatrolState.h"
#include "PoliceTraceState.h"
#include "PoliceIdleState.h"
#include "PoliceDieState.h"

/// <summary>
/// �ۼ��� : 2023.08.16
/// �ۼ��� : ������
/// 
/// Police�� FSM
/// </summary>

class PoliceFSM :
    public StateMachine
{
public:
    PoliceFSM(GameObject* owner);
    ~PoliceFSM();

public:
    //���µ�
    PoliceCatchState m_PoliceCatchState;
    PoliceFindState m_PoliceFindState;
    PolicePatrolState m_PolicePatroState;
    PoliceTraceState m_PoliceTraceState;
    PoliceIdleState m_PoliceIdleState;
    PoliceDieState m_PoliceDieState;

    virtual void Update(float deltaTime) override;
};

