#pragma once
#include "MonoBehaviour.h"

using std::wstring;

/// <summary>
/// �ۼ��� : 2023.08.09
/// �ۼ��� : ������
/// 
/// FSM ���ư��°� Ȯ���� ���� �׽�Ʈ ��ũ��Ʈ
/// </summary>

class StateMachine;
class Camera;
class GameManagerScript;
class TextManagerScript;

class FarmerScript :
    public MonoBehaviour
{
public:
    FarmerScript();
    ~FarmerScript();

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
	// �̺�Ʈ
	void MiniGame(std::any); // �̴ϰ���
	void FarmerWatering(std::any); // �� �ֱ� ���
	void FarmerInteraction(std::any); // ��ȣ�ۿ� ���
	void FarmerHarvest(std::any);	// ��Ȯ ���
	
public:
	StateMachine* m_FarmerStateMachine;

	float speed;						//�� ���¿� ���� ���ǵ�
	float diagonalSpeed;				//�밢�� �̵��� ���� ���ǵ�
	int directionCheck;				//���� Ȯ���� ���� ����
	float timeCount;				//�ð��� Ȯ���ϴ� ����
	string detailState;				//�����ִ� ���¸� ���� ����
	string Farmerstate;				//���¸� ���� ����
	bool collisionCheck;			//�浹üũ
	 
	bool deadBodyCollisionCheck;	//��ü���� �浹 üũ
	bool farmCollisionCheck;		//����� �浹 üũ
	bool buildingCollisionCheck;	//������ �浹 üũ		
	bool isNPCTalking;				//��ȭ ����
	bool isPlayerTalkingState;		//�÷��̾ ����ϴ� ��Ȳ
	bool isWatering;				//�� �ִ� ����
	bool isInterection;				//��ȣ�ۿ� ����
	bool isHarvest;					//��Ȯ�ϴ� ����
	bool haveDeadBody;				//��ü ��� �ִ� ����
	bool hideDeadBody;				//��ü ����� ����
	bool isGameOver;				//���� ���� ����
	bool isMorning;
	bool isNight;					//�ð��� ���� �ʱ�ȭ�� ���� ����
	
	
	string m_ScriptMapKey;			//�� npc�� Ű ���� �����ص� string
	wstring textOut;				//�����ϰ� �ؽ�Ʈ�� ������ ģ��
	Vector2 sponPos;

	GameManagerScript* manager;
	TextManagerScript* textManager;
};

