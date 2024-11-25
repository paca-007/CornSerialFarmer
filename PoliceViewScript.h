#pragma once
#include "MonoBehaviour.h"

/// <summary>
/// �ۼ��� : 2023.08.16
/// �ۼ��� : ������
/// 
/// Police�� �þ߸� ����ϴ� Script
/// </summary>

class PoliceViewScript
	: public MonoBehaviour
{
public:
	PoliceViewScript();
	~PoliceViewScript();

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

