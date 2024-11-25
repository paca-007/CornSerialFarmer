#pragma once
#include "FSMState.h"


/// <summary>
/// �ۼ��� : 2023.08.16
/// �ۼ��� : ������
/// 
/// ������ ��ü �߰� ���¸� �����ϴ� Ŭ����
/// </summary>

class PoliceFindState :
    public FSMState
{
public:
    PoliceFindState(StateMachine* owner);
    ~PoliceFindState();

public:
	virtual void Enter();
	virtual void Update(float deltaTime);
	virtual void Exit();
};

