#pragma once
#include "MonoBehaviour.h"
/// <summary>
/// �ۼ��� : 2023.08.14
/// �ۼ��� : ������
/// 
/// NPC1�� �þ߸� ���� ��ũ��Ʈ
/// </summary>

class StateMachine;

class NPC1ViewScript
	:public MonoBehaviour
{
public:
	NPC1ViewScript();
	~NPC1ViewScript();

public:
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void FixedUpdate(float deltaTime) override;
	virtual void Finalize() override;

public:
	virtual void OnCollisionEnter(const Manifold& collision) override;
	virtual void OnCollisionStay(const Manifold& collision) override;
	virtual void OnCollisionExit(const Manifold& collision) override;

};

