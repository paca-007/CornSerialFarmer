#pragma once
#include "StateMachine.h"

#include "NPC1PatrolState.h"
#include "NPC1ReturnState.h"
#include "NPC1TalkState.h"
#include "NPC1CheckState.h"
#include "NPC1FindState.h"
#include "NPC1IdleState.h"


/// <summary>
/// ���� �ۼ��� : 2023.08.11
/// �ۼ��� : ������
/// 
/// NPC1�� FSM
/// </summary>

class NPC1FSM :
    public StateMachine
{
public:
    NPC1FSM(GameObject* owner);
    ~NPC1FSM();

public:
    //���µ�
    NPC1PatrolState     m_NPC1PatrolState;
    NPC1ReturnState     m_NPC1ReturnState;
    NPC1TalkState       m_NPC1TalkState;
    NPC1CheckState      m_NPC1CheckState;
    NPC1FindState       m_NPC1FindState;
    NPC1IdleState       m_NPC1IdleState;

	/*
	NPC1FindBodyState m_NPC1FindBodyState;
	NPC1TalkState m_NPC1TalkState;*/

    virtual void Update(float deltaTime) override;
};

