#pragma once
#include <vector>
#include "MonoBehaviour.h"
#include "ZicoMath.h"

using std::vector;
using std::wstring;

/// <summary>
/// �ۼ��� : 2023.08.14
/// �ۼ��� : ������
/// 
/// NPC2�� ���� ��ũ��Ʈ
/// </summary>

class StateMachine;
class GameManagerScript;
class TextManagerScript;

class NPC2Script :
    public MonoBehaviour
{
public:
    NPC2Script();
    ~NPC2Script();

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
	StateMachine* m_NPC2StateMachine;

	int directionCheck;				//���� Ȯ���� ���� ����
	//Vector2 NPC1Route;			//NPC�� ��Ʈ�� Ȯ���ϱ� ���� ����
	string detailState;				//���� ���¸� ���� ����
	string NPC2State;				//NPC�� ����

	int count;						//���� �Ÿ��� ���� ����
	int backCount;					//������ ���� ����
	int randomBackNumber;			//���� ������ ���� ����
	float timeCount;				//��ü �߰ߺ��� �� ���ı��� �ð��Լ�
	float deltaTime;				//��ŸŸ��
	bool playerCollisionCheck;		//�÷��̾���� �浹 üũ
	bool deadBodyCollisionCheck;	//��ü���� �浹 üũ
	bool isBuildingCollisionCheck;	//�ǹ��� �浹 üũ
	bool isTalking;					//��ȭ üũ
	bool isReturn;					//������������ �̵�
	bool isFind;					//��ü�� �̵�
	bool isPlayerTalking;			//�÷��̾���� ��ȭ üũ
	bool isPositionStart;			//���� ��ġ�� �̵��ߴ°�?
	bool isRevive;					//��Ȱ
	bool isDead;					//����?
	int conversation;				//��ȭ����
	string m_ScriptMapKey;			//�� npc�� Ű ���� �����ص� string
	wstring textOut;				//�����ϰ� �ؽ�Ʈ�� ������ ģ��

	bool xCheck;
	bool yCheck;

	Vector2 bodyPos;				//��ü��ġ
	Vector2 sponPos;				//������ġ
		
	vector<Vector2> startPoint1;	//���� ���� �̵� 
	vector<Vector2> startPoint2;	//���� ���� �̵�2
	int startPoint;					//���� �̵� ��ȣ
	Vector2 gotoSpot;				//�������� ���ÿ�
	int startPointCheck;			//� ���� �̵� ���?

	GameManagerScript* m_NPC2Manager;
	TextManagerScript* m_NPC2TextManager;
public:
	void TalkingState(std::any);
	void FindBodyState(std::any);
};

