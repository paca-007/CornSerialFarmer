#pragma once

#include <MonoBehaviour.h>

#include <Map>
using std::map;
#include <Vector>
using std::vector;
using std::make_pair;
#include <string>
using std::wstring;

class QuestList;
class ItemManagerScript;
class Item;
class CameraScript;

enum class ITEM_TYPE;

#define AX_ m_QuestItemList[static_cast<int>(ITEM_TYPE::AX)]
#define BAG_ m_QuestItemList[static_cast<int>(ITEM_TYPE::BAG)]
#define CATFOOD_ m_QuestItemList[static_cast<int>(ITEM_TYPE::CATFOOD)]
#define CORN_ m_QuestItemList[static_cast<int>(ITEM_TYPE::CORN)]
#define FERTILIZER_ m_QuestItemList[static_cast<int>(ITEM_TYPE::FERTILIZER)]
#define FIREWOOD_ m_QuestItemList[static_cast<int>(ITEM_TYPE::FIREWOOD)]
#define SEED_ m_QuestItemList[static_cast<int>(ITEM_TYPE::SEED)]
#define EMPTYWATERING_ m_QuestItemList[static_cast<int>(ITEM_TYPE::EMPTYWATERING)]
#define WATERING_ m_QuestItemList[static_cast<int>(ITEM_TYPE::WATERING)]
#define KEY_ m_QuestItemList[static_cast<int>(ITEM_TYPE::KEY)]

/// <summary>
/// Ȱ��ȭ ���� ����Ʈ ������
/// </summary>
struct ActiveQuestData
{
	ActiveQuestData(int objectId, int maxQuestStep, wstring questName, int uiOrder = 0, int questStep = 0, bool isComplete = false)
	{
		this->objectId = objectId;
		this->maxQuestStep = maxQuestStep;
		this->questName = questName;
		this->uiOrder = uiOrder;
		this->questStep = questStep;
		this->isComplete = isComplete;
	}

	int objectId;
	int maxQuestStep;
	wstring questName;
	int uiOrder;
	int questStep;
	bool isComplete;
};

/// <summary>
/// ����Ʈ ������
/// </summary>
struct QuestData
{
	QuestData() {}
	QuestData(string questName, int objectId)
	{
		this->questName = questName;
		this->objectId = objectId;
	}

	string questName = "";
	int objectId = 0;
};


/// <summary>
/// �ۼ��� : 2023.08.17
/// �ۼ��� : �赿��
/// 
/// ����Ʈ�� �����ϴ� �Ŵ���
/// </summary>
class QuestManagerScript : public MonoBehaviour
{
public:
	QuestManagerScript();
	~QuestManagerScript();

public:
	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void Finalize();

public:
	// �̺�Ʈ
	void WildBoarClear(std::any);
	void WildBoarFail(std::any);
	void WareHouseClear(std::any);
	void WareHouseFail(std::any);
	void CornGameClear(std::any);
	void CornGameFail(std::any);

public:
	void CreateQuest(int dayCount, bool isMorning); // ����Ʈ ����
	void CreateCornQuest(); // ������ ����Ʈ ����
	void CreateChoresRandomQuest(); // ���� ����Ʈ ���� ����
	void CreateEmergencyRandomQuest(); // ��� ����Ʈ ���� ����
	void CreateNightRandomQuest(); // �� ����Ʈ ���� ����
	void CreateQuestUi(); // ����Ʈ UI ����
	Item* CreateQuestItem(ITEM_TYPE itemType); // Ȱ��ȭ�� ����Ʈ ���� ������ ����
	void CreateWildBoar(GameObject* wildBoarObject);

	// ����Ʈ ��ȣ�ۿ� ȭ��ǥ ����
	void CreateQuestInteractionArrow();

	// �� �⺻ ����Ʈ ���� ����
	void CreateBasicNightQuest();

	// �� ��ü ����� ������ ���� ��ȣ�ۿ� ȭ��ǥ ����
	void CreateNightInteractionArrow();

	void DeleteAllItem();

public:
	void CheckQuest(int scanObjectId, GameObject* scanObject); // ��ȣ�ۿ� ���� ������Ʈ ���̵�� ����Ʈ üũ
	bool InteractionQuest(int questId, GameObject* scanObject); // ����Ʈ ��Ʈ�� (����Ʈ �����Ŵ)
	void ClearQuest(); // ����Ʈ �ʱ�ȭ
	void CheckCompleteAllActiveQuests(); // Ȱ��ȭ�� ��� ����Ʈ�� �Ϸ� �ߴ��� üũ
	
public:
	// CheckQuest ���� ����ϰ� �� ����
	// ����Ʈ�� ���� ������� üũ�ϱ� ������ ��� �׸��� ����� �˻��ϱ� ���� ����
	// ������ ����Ʈ��� �������� ���ϰ� �� ������ Ȱ���Ͽ� ����Ʈ üũ�� ������� ���� �����Ѵ�.
	int m_InteractionCount;

	static const int QUEST_UI_COUNT = 4; // ����Ʈ UI �ʱ�ȭ�� ���� static ����
	int m_CornQuestCount; // �����ؾ� �� ������ ����Ʈ �ܰ踦 �˷��ش�.
	bool m_IsCornQuestStepUp; 
	bool m_questReset; 	// ����Ʈ ������ �ߴ°� (�� �Ѹ��� Ƚ�� �ʱ�ȭ�� ����)
	int m_QuestUiIndex; // 30~60�� ���̿� �߻��ϴ� ����Ʈ (UI �ֽ�ȭ �۾�)
	bool m_FailWildBoar; // ����� ����Ʈ ���� 
	bool m_ApplyFarm;
	bool m_SaveCat;
	bool m_CompleteAllActiveQuests;

	// �ϴ� ����    �� 
	GameObject* m_CatObject;
	GameObject* m_PondObject;
	GameObject* m_Player;
	GameObject* m_WildBoar;
	CameraScript* m_Camera;

	map<int, ActiveQuestData> m_ActiveQuests; 	// ���� Ȱ��ȭ ���� ��� ����Ʈ�� (����Ʈ ���̵�, �ܰ� ������ ������ ����)
	vector<GameObject*> m_QuestObjects;  	// ��ü ����Ʈ ���� ������Ʈ��
	map<int, QuestData> m_QuestList;  // ��ü ����Ʈ ����Ʈ
	QuestList* m_QuestUiList; // ����Ʈ ����Ʈ UI
	
	Item* m_QuestItemList[10]; 	// ����Ʈ ������ �迭 (����Ʈ ���� �� ������ �׿� ���õ� �����۵� ����,����)

	ItemManagerScript* m_ItemManager; 	// ������ �Ŵ���
};

