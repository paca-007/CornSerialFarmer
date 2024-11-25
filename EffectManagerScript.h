/// [2023/8/18] �ڿ���
/// --------------------------
/// ������ ����Ʈ�� �����ϴ� �Ŵ���

#pragma once

#include "MonoBehaviour.h"

class EffectManagerScript : public MonoBehaviour
{
public:
	EffectManagerScript();
	~EffectManagerScript();

public:
	virtual void Start();
	virtual void Update(float deltaTime);

public:
	void ApplyTimeFlowEffect(
		float temperature,
		float colorTint,
		float brightness);
};

