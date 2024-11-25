#pragma once
#include <vector>
#include "MonoBehaviour.h"
#include "ZicoMath.h"

using std::wstring;
using std::vector;

/// <summary>
/// �ۼ��� : 2023.08.11
/// �ۼ��� : ������
/// 
/// NPC1�� ���� ��ũ��Ʈ
/// </summary>

class StateMachine;
class GameManagerScript;
class TextManagerScript;

class NPC1Script
	:public MonoBehaviour
{
public:
	NPC1Script();
	~NPC1Script();

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
	StateMachine* m_NPC1StateMachine;

	int directionCheck;				//���� Ȯ���� ���� ����
	//Vector2 NPC1Route;			//NPC�� ��Ʈ�� Ȯ���ϱ� ���� ����
	int conversation;				//��ȭ����
	int patrolEnd;					//������� ���ƿ�
	string detailState;				//���� ���¸� ���� ����
	string NPC1state;				//NPC�� ����

	int count;						//���� �Ÿ��� ���� ����
	float timeCount;				//�ð�����
	float deltaTime;				//��ŸŸ��
	bool playerCollisionCheck;		//�÷��̾���� �浹 üũ
	bool deadBodyCollisionCheck;	//��ü���� �浹 üũ
	bool isTalking;					//��ȭ üũ
	bool isReturn;					//������������ �̵�
	bool isFind;					//��ü�� �̵�
	bool isPlayerTalking;			//�÷��̾���� ��ȭ üũ
	bool isRevive;					//��Ȱ
	bool isDead;					//����
	string m_ScriptMapKey;			//�� npc�� Ű ���� �����ص� string
	wstring textOut;				//�����ϰ� �ؽ�Ʈ�� ������ ģ��
	
	vector<Vector2> patrolSpot1;
	vector<Vector2> patrolSpot2;	//���� ��ġ
	vector<Vector2> patrolSpot3;	//���� ��ġ
	vector<Vector2> patrolSpot4;	//���� ��ġ
	vector<Vector2> patrolSpot5;	//���� ��ġ

	int patrolNumber;				//�� ��° ��ġ�� �̵�?
	int patrolPath;					//� ��Ʈ �̿�?

	bool xCheck;
	bool yCheck;

	Vector2 bodyPos;				//��ü ��ġ
	Vector2 sponPos;				//�������
	Vector2 gotoSpot;				//������ �� ��

	GameManagerScript* m_NPC1Manager;
	TextManagerScript* m_NPC1TextManager;

public:
	void TalkingState(std::any);
	void FindBodyState(std::any);		//��ü �߰� �̺�Ʈ
};

