#pragma once
#include "MonoBehaviour.h"

class StateMachine;

/// <summary>
/// �ۼ��� : 2023.08.15
/// �ۼ��� : ������
/// 
/// ���� script
/// </summary>

class FarmScript :
	public MonoBehaviour
{
public:
	FarmScript();
	~FarmScript();

public:
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void FixedUpdate(float deltaTime) override {};
	virtual void Finalize() override;

public:
	StateMachine* m_FarmStateMachine;

	string farmState;
	string detailState;
	bool fakeSeed;
	
	int completeQuestCount;
	bool isInteraction;

	bool isDestroyFarm;

	int farmNumber;
};

