#pragma once
#include "FSMState.h"
/// <summary>
/// �ۼ��� : 2023.08.18
/// �ۼ��� : ������
/// 
/// ������ ��Ÿ���� �ʴ� ����(��ħ)�� �����Ѵ�.
/// </summary>

class PoliceDieState :
    public FSMState
{
public:
    PoliceDieState(StateMachine* owner);
    ~PoliceDieState();

public:
    virtual void Enter();
    virtual void Update(float deltaTime);
    virtual void Exit();
};

