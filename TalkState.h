#pragma once
#include "FSMState.h"

/// <summary>
/// �ۼ��� : 2023.08.11
/// �ʱ� �ۼ��� : ������
/// 
/// ��ȣ�ۿ� �� Talk�� �����ϴ� Ŭ����
/// </summary>

class TalkState :
    public FSMState
{
public:
    TalkState(StateMachine* owner);
	~TalkState();

public:
	virtual void Enter();
	virtual void Update(float deltaTime);
	virtual void Exit();
};

