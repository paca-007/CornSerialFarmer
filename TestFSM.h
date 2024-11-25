   #pragma once
#include "StateMachine.h"

#include "IdleState.h"
#include "WalkState.h"
#include "TalkState.h"
#include "WateringState.h"
#include "HarvestState.h"
#include "MoveBodyState.h"
#include "InteractionState.h"

/// <summary>
/// ���� �ۼ��� : 2023.08.09
/// �ۼ��� : ������ 
///
/// �׽�Ʈ �� FSM ����
/// ������ �̰��� �׽�Ʈ�� �ƴ� ����� FSM�� �Ǿ���.
/// </summary>


class TestFSM :
    public StateMachine
{
public:
    TestFSM(GameObject* owner);
    ~TestFSM();

public:
    //���µ�
    IdleState           m_IdleState;
    WalkState           m_WalkState;
    TalkState           m_TalkState;
    WateringState       m_WateringState;
    HarvestState        m_HarvestState;
    MoveBodyState       m_MoveBodyState;
    InteractionState    m_InteractionState;

    virtual void Update(float deltaTime) override;

};

 