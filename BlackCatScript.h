#pragma once
#include "MonoBehaviour.h"
#include "ZicoMath.h"

/// <summary>
/// �ۼ��� : 2023.08.15
/// �ۼ��� : ������
/// 
/// BlackCat�� ���� ��ũ��Ʈ
/// </summary>

class StateMachine;

class BlackCatScript :
    public MonoBehaviour
{

public:
    BlackCatScript();
    ~BlackCatScript();

public:
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void FixedUpdate(float deltaTime) override;
	virtual void Finalize() override;

public:
	virtual void OnCollisionEnter(const Manifold& collision) override;
	virtual void OnCollisionStay(const Manifold& collision) override;
	virtual void OnCollisionExit(const Manifold& collision) override;

public:
	StateMachine* m_BlackCatStateMachine;

	int directionCheck;				//���� Ȯ���� ���� ����
	string detailState;				//���� ���¸� ���� ����
	string blackCatState;			//������� ����
	bool isSave;					//����̸� ������ ���
	bool action;					//����� ���ϰ� �� ���� �׼� 

	int count;						//���� �̵��� ���� ī��Ʈ
	int randomDirection;			//���� �̵��� ���� ���� �������ñ�
	float timeCount;					//�ð� ���� ī��Ʈ

	
};

