#pragma once
#include "FSMState.h"
/// <summary>
/// �ۼ��� : 2023.08.16
/// �ۼ��� : ������
/// 
/// Police�� ���ΰ˰� ���¸� �����ϴ� Ŭ����
/// </summary>

class PoliceCatchState :
    public FSMState
{
public:
    PoliceCatchState(StateMachine* owner);
    ~PoliceCatchState();

public:
	virtual void Enter();
	virtual void Update(float deltaTime);
	virtual void Exit();
};

