#pragma once
#include "FSMState.h"
/// <summary>
/// �ۼ��� : 2023.08.14
/// �ۼ��� : ������
/// 
/// NPC2�� ��ȭ���¸� �����ϴ� Ŭ����
/// </summary>

class NPC2TalkState :
    public FSMState
{
public:
    NPC2TalkState(StateMachine* owner);
    ~NPC2TalkState();

public:
    virtual void Enter();
    virtual void Update(float deltaTime);
    virtual void Exit();

};

