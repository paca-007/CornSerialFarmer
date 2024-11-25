#pragma once
#include "MonoBehaviour.h"
#include "ZicoMath.h"

using std::wstring;

class StateMachine;
class GameManagerScript;
class TextManagerScript;

/// <summary>
/// �ۼ��� : 2023.08.16
/// �ۼ��� : ������
/// 
/// Police�� ���� script
/// </summary>

class PoliceScript :
    public MonoBehaviour
{
public:
    PoliceScript();
    ~PoliceScript();

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
	StateMachine* m_PoliceStateMachine;

public:
	string detailState;
	string police1State;
	Vector2 sponPos;

	Vector2 patrolSpot1;
	Vector2 patrolSpot2;
	Vector2 patrolSpot3;
	Vector2 patrolSpot4;
	Vector2 patrolSpot;

	float timeCount;
	int randomPatrolNumber;
	string m_ScriptMapKey;			//�� npc�� Ű ���� �����ص� string
	wstring textOut;				//�����ϰ� �ؽ�Ʈ�� ������ ģ��

	bool isCatch;
	bool isRevive;
	bool isBuildingCollision;		//�ǹ��� �ε���?
	bool isDead;					//����?
	int backCount;					//����ī��Ʈ
	int randomBackNumber;			//���� ����

	int speed;						//������ ���ǵ�


	bool xCheck;
	bool yCheck;

	GameManagerScript* m_Policemanager;
	TextManagerScript* m_PoliceTextManager;
};

