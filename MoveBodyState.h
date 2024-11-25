#pragma once
#include "FSMState.h"

/// <summary>
/// �ۼ��� : 2023/08.17
/// �ۼ��� : ������
/// 
/// ��ΰ� ��ü�� �ű�� ���¸� �����ϴ� Ŭ����
/// </summary>

class MoveBodyState :
    public FSMState
{
public:
    MoveBodyState(StateMachine* owner);
    ~MoveBodyState();

public:
	virtual void Enter();
	virtual void Update(float deltaTime);
	virtual void Exit();
};

