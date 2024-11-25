#pragma once
#include "FSMState.h"
#include "ZicoMath.h"

/// <summary>
/// �ۼ��� : 2023.08.09
/// �ʱ� �ۼ��� : ������
/// 
/// �����̴� ���� walk�� �����ϴ� Ŭ����
/// </summary>


class WalkState :
    public FSMState
{
public:
    WalkState(StateMachine* owner);
    ~WalkState();

public:
	virtual void Enter();
	virtual void Update(float deltaTime);
	virtual void Exit();

};

 