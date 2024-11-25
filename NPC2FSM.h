#pragma once
#include "StateMachine.h"

#include "NPC2TraceState.h"
#include "NPC2ReturnState.h"
#include "NPC2TalkState.h"
#include "NPC2CheckState.h"
#include "NPC2FindState.h"
#include "NPC2IdleState.h"

/// <summary>
/// �ۼ��� : 2023.08.14
/// �ۼ��� : ������
/// 
/// NPC2�� FSM (�����ϴ� NPC)
/// </summary>

class NPC2FSM :
    public StateMachine
{
public:
    NPC2FSM(GameObject* owner);
    ~NPC2FSM();

public:
    //���µ�
    NPC2TraceState  m_NPC2TraceState;
    NPC2ReturnState m_NPC2ReturnState;
    NPC2TalkState   m_NPC2TalkState;
    NPC2CheckState  m_NPC2CheckState;
    NPC2FindState   m_NPC2FindState;
    NPC2IdleState   m_NPC2IdleState;

    virtual void Update(float deltaTime) override;
};

