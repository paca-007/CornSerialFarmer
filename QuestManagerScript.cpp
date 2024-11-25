#include "QuestManagerScript.h"


#include <random>
#include <cassert>

#include "../GraphicsEngine/GraphicsEngine.h"
#include "TextRenderer.h"
#include "ImageRenderer.h"
#include "Image.h"

#include "ResourceManager.h"

#include "GameObject.h"
#include "Scene.h"
#include "MainScene.h"
#include "Transform.h"

#include "ItemSlot.h"
#include "QuestList.h"

#include "ItemManagerScript.h"
#include "GameManagerScript.h"
#include "FarmScript.h"
#include "FarmerScript.h"
#include "WildBoarScript.h"
#include "ObjectDataScript.h"
#include "BlackCatScript.h"
#include "Animator.h"
#include "TextManagerScript.h"

#include "EventManager.h"
#include "SoundManager.h"
#include "Sound.h"

#include "ZicoMath.h"

#include "CameraScript.h"
#include "Animation.h"
#include "GameplayStatics.h"


QuestManagerScript::QuestManagerScript()
	: MonoBehaviour(typeid(this).name())
	, m_IsCornQuestStepUp(false)
	, m_QuestUiList(nullptr)
	, m_ItemManager(nullptr)
	, m_QuestUiIndex(0)
	, m_InteractionCount(0)
	, m_QuestItemList{ 0, }
	, m_questReset(false)
	, m_CornQuestCount(1)
	, m_FailWildBoar(false)
	, m_ApplyFarm(true)
	, m_CatObject(nullptr)
	, m_PondObject(nullptr)
	, m_Player(nullptr)
	, m_SaveCat(true)
	, m_WildBoar(nullptr)
	, m_Camera(nullptr)
	, m_CompleteAllActiveQuests(false)
{

}

QuestManagerScript::~QuestManagerScript()
{

}


/// ������Ʈ ID 
/// 
/// â��A : 1000
/// â��B : 2000
///	â��C : 3000
/// â��D : 4000
/// �� : 5000
/// �� : 6000 
/// ���� : 7000   
/// ���� : 8000
/// ����� ����: 9000
/// �˵��� : 10000
/// 
void QuestManagerScript::Start()
{
	// ������ ����Ʈ
	m_QuestList.insert(make_pair(10, QuestData("������ �翡 �� �Ѹ��� 1�ܰ�", { 1000 })));	//â��A
	m_QuestList.insert(make_pair(11, QuestData("������ �翡 �� �Ѹ��� 2�ܰ�", { 6000 })));	//��

	//m_QuestList.insert(make_pair(20, QuestData("������ ���� ���� �̱�", { 6000 })));	//��

	m_QuestList.insert(make_pair(30, QuestData("������ �翡 ��� �ֱ� 1�ܰ�", { 4000 })));	//â��D
	m_QuestList.insert(make_pair(31, QuestData("������ �翡 ��� �ֱ� 2�ܰ�", { 6000 })));	//��

	m_QuestList.insert(make_pair(40, QuestData("������ �翡 �� �ֱ� 1�ܰ�", { 2000 })));	//â��B
	m_QuestList.insert(make_pair(41, QuestData("������ �翡 �� �ֱ� 2�ܰ�", { 7000 })));	//����
	m_QuestList.insert(make_pair(42, QuestData("������ �翡 �� �ֱ� 3�ܰ�", { 6000 })));	//��

	m_QuestList.insert(make_pair(50, QuestData("������ ��Ȯ�ϱ� 1�ܰ�", { 3000 })));	//â��C
	m_QuestList.insert(make_pair(51, QuestData("������ ��Ȯ�ϱ� 2�ܰ�", { 6000 })));	//��

	// ���� ����Ʈ
	m_QuestList.insert(make_pair(60, QuestData("���� �����ϱ� 1�ܰ�", { 4000 })));	 // â��D
	m_QuestList.insert(make_pair(61, QuestData("���� �����ϱ� 2�ܰ�", { 8000 })));	 // ����
	m_QuestList.insert(make_pair(62, QuestData("���� �����ϱ� 3�ܰ�", { 5000 })));	 // ��

	m_QuestList.insert(make_pair(70, QuestData("â�� �����ϱ�", { 1000 })));	// â��A

	m_QuestList.insert(make_pair(80, QuestData("����� �� �ֱ� 1�ܰ�", { 5000 })));	// ��
	m_QuestList.insert(make_pair(81, QuestData("����� �� �ֱ� 2�ܰ�", { 9000 })));	// �����

	m_QuestList.insert(make_pair(90, QuestData("â�� �� ���� 1�ܰ�", { 5000 })));	// ��
	m_QuestList.insert(make_pair(91, QuestData("â�� �� ���� 2�ܰ�", { 3000 })));	// â��C

	// ��� ����Ʈ
	m_QuestList.insert(make_pair(100, QuestData("����� ���ϱ�", { 7000 })));	// ����

	// �� ����Ʈ
	m_QuestList.insert(make_pair(999, QuestData("������ ���� ���ؼ� ��ü �����", { 100000 })));
	m_QuestList.insert(make_pair(1000, QuestData("�˵��� �i�Ƴ���", { 10000 })));	//�˵���

	// �̺�Ʈ �ӽ�.
	EventManager::GetInstance()->RegisterClient("WildBoarClear", MakeCallInfo(&QuestManagerScript::WildBoarClear));
	EventManager::GetInstance()->RegisterClient("WildBoarFail", MakeCallInfo(&QuestManagerScript::WildBoarFail));
	EventManager::GetInstance()->RegisterClient("WareHouseClear", MakeCallInfo(&QuestManagerScript::WareHouseClear));
	EventManager::GetInstance()->RegisterClient("WareHouseFail", MakeCallInfo(&QuestManagerScript::WareHouseFail));
	EventManager::GetInstance()->RegisterClient("CornGameClear", MakeCallInfo(&QuestManagerScript::CornGameClear));
	EventManager::GetInstance()->RegisterClient("CornGameFail", MakeCallInfo(&QuestManagerScript::CornGameFail));
}

void QuestManagerScript::Update(float deltaTime)
{
	/*for (auto& questObject : m_QuestObjects)
	{
		for (auto& activeQuest : m_ActiveQuests)
		{
			if (dynamic_cast<ObjectDataScript*>(questObject->GetComponent<MonoBehaviour>())->id == activeQuest.second.objectId)
			{
				for (auto& questObjectChild : questObject->GetObjectChildren())
				{
					if (questObjectChild->GetObjectType() == OBJECT_TYPE::INTERACTION_ARROW)
					{
						questObjectChild->SetActivate(true);
					}
				}
			}
		}
	}*/
}

void QuestManagerScript::Finalize()
{
	for (auto& itemIter : m_QuestItemList)
	{
		if (itemIter != nullptr)
		{
			delete itemIter;
			itemIter = nullptr;
		}
	}
}


void QuestManagerScript::WildBoarClear(std::any)
{
	// �̴� ���� ���� �� 
	m_ApplyFarm = true;

	++(m_ActiveQuests.find(1000)->second.questStep);

	if (m_ActiveQuests.find(1000)->second.questStep == m_ActiveQuests.find(1000)->second.maxQuestStep)
	{
		m_ActiveQuests.find(1000)->second.isComplete = true;

		m_QuestUiList->m_CheckBoxImage[m_ActiveQuests.find(1000)->second.uiOrder]->SetImage
		(
			ResourceManager::GetInstance()->LoadResource<Image>(L"Check", L"/UI/check.png")->GetImage()
		);
	}
	else
	{
		m_ActiveQuests.find(1000)->second.objectId = m_QuestList[1000 + 1].objectId;
	}


	dynamic_cast<WildBoarScript*>(m_WildBoar->GetComponent<MonoBehaviour>())->m_IsClear = true;

	m_WildBoar->SetActivate(true);
}

void QuestManagerScript::WildBoarFail(std::any)
{
	m_WildBoar->SetActivate(true);
	m_WildBoar->GetObjectChild("wildBoarExclamationMark")->SetActivate(true);
}

void QuestManagerScript::WareHouseClear(std::any)
{
	// Ű ����
	m_ItemManager->DeleteItem(KEY_);
	KEY_ = nullptr;

	m_ItemManager->ApplyItemSlot();

	++(m_ActiveQuests.find(90)->second.questStep);

	if (m_ActiveQuests.find(90)->second.questStep == m_ActiveQuests.find(90)->second.maxQuestStep)
	{
		m_ActiveQuests.find(90)->second.isComplete = true;

		m_QuestUiList->m_CheckBoxImage[m_ActiveQuests.find(90)->second.uiOrder]->SetImage
		(
			ResourceManager::GetInstance()->LoadResource<Image>(L"Check", L"/UI/check.png")->GetImage()
		);
	}
	else
	{
		m_ActiveQuests.find(90)->second.objectId = m_QuestList[91 + 1].objectId;
	}


	for (auto& object : m_QuestObjects)
	{
		if (object->GetName() == "WarehouseC")
		{
			if (dynamic_cast<ObjectDataScript*>(object->GetComponent<MonoBehaviour>())->interactionArrowCount == 0)
			{
				object->GetObjectChild("warehouseCInteractionArrow")->SetActivate(false);
				break;
			}
			else
			{
				--(dynamic_cast<ObjectDataScript*>(object->GetComponent<MonoBehaviour>())->interactionArrowCount);
			}
		}
	}

}

void QuestManagerScript::WareHouseFail(std::any)
{

}

void QuestManagerScript::CornGameClear(std::any)
{
	++(m_ActiveQuests.find(70)->second.questStep);

	if (m_ActiveQuests.find(70)->second.questStep == m_ActiveQuests.find(70)->second.maxQuestStep)
	{
		m_ActiveQuests.find(70)->second.isComplete = true;

		m_QuestUiList->m_CheckBoxImage[m_ActiveQuests.find(70)->second.uiOrder]->SetImage
		(
			ResourceManager::GetInstance()->LoadResource<Image>(L"Check", L"/UI/check.png")->GetImage()
		);
	}
	else
	{
		m_ActiveQuests.find(70)->second.objectId = m_QuestList[70 + 1].objectId;
	}
	
	for (auto& object : m_QuestObjects)
	{
		if (object->GetName() == "WareHouseA")
		{
			if (dynamic_cast<ObjectDataScript*>(object->GetComponent<MonoBehaviour>())->interactionArrowCount == 0)
			{
				object->GetObjectChild("warehouseAInteractionArrow")->SetActivate(false);
				break;
			}
			else
			{
				--(dynamic_cast<ObjectDataScript*>(object->GetComponent<MonoBehaviour>())->interactionArrowCount);
			}
		}
	}
}

void QuestManagerScript::CornGameFail(std::any)
{

}

/// <summary>
/// ����Ʈ ���� (���� �Ŵ������� �ð��� �帧�� ���� �ҷ��´�)
/// ��� ����Ʈ �� ������Ʈ�� ���� �Ŵ������� �ð��� �帧�� ���� �����Ѵ�.
/// </summary>
void QuestManagerScript::CreateQuest(int dayCount, bool isMorning)
{
	if (isMorning)
	{
		// Ȱ��ȭ�� ����Ʈ �� ����Ʈ UI �ʱ�ȭ
		ClearQuest();

		// ������ ����Ʈ ���� 
		CreateCornQuest();

		// ���� ����Ʈ ���� ����
		CreateChoresRandomQuest();

		// ����Ʈ UI ����
		CreateQuestUi();

		// ����Ʈ ��ȣ�ۿ� ȭ��ǥ ����
		CreateQuestInteractionArrow();
	}

	else
	{
		// Ȱ��ȭ�� ����Ʈ �� ����Ʈ UI �ʱ�ȭ
		ClearQuest();

		// ����Ʈ UI ����
		CreateQuestUi();

		// �� �⺻ ����Ʈ ���� ���� (������ ���� ���ؼ� ��ü�� �����)
		CreateBasicNightQuest();

		// �� ��ü ����� ������ ���� ��ȣ�ۿ� ȭ��ǥ ����
		CreateNightInteractionArrow();
	}
}

/// <summary>
/// ������ ����Ʈ ���� �ܰ迡 ���� ����Ʈ�� ����
/// </summary>
void QuestManagerScript::CreateCornQuest()
{
	//m_CornQuestCount
	switch (m_CornQuestCount)
	{
		case 1:
			m_ActiveQuests.insert(make_pair(10, ActiveQuestData(1000, 2, L"�������翡 �� �Ѹ���"))); // ������ �� �� �Ѹ���
			m_ActiveQuests.insert(make_pair(40, ActiveQuestData(2000, 3, L"�������翡 �� �ֱ�"))); // ������ �� �� �ֱ�
			break;
		case 2:
			//m_ActiveQuests.insert(make_pair(20, ActiveQuestData(6000, 1, L"���������� ���� �̱�"))); // ������ �� ���� �̱�
			m_ActiveQuests.insert(make_pair(30, ActiveQuestData(4000, 2, L"�������翡 ��� �ֱ�"))); // ������ �� ��� �ֱ�
			m_ActiveQuests.insert(make_pair(40, ActiveQuestData(2000, 3, L"�������翡 �� �ֱ�"))); // ������ �� �� �ֱ�
			break;
		case 3:
			m_ActiveQuests.insert(make_pair(30, ActiveQuestData(4000, 2, L"�������翡 ��� �ֱ�"))); // ������ �� ��� �ֱ�
			m_ActiveQuests.insert(make_pair(40, ActiveQuestData(2000, 3, L"�������翡 �� �ֱ�"))); // ������ �� �� �ֱ�
			break;
		case 4:
			m_ActiveQuests.insert(make_pair(30, ActiveQuestData(4000, 2, L"�������翡 ��� �ֱ�"))); // ������ �� ��� �ֱ�
			m_ActiveQuests.insert(make_pair(40, ActiveQuestData(2000, 3, L"�������翡 �� �ֱ�"))); // ������ �� �� �ֱ�
			break;
		case 5:
			m_ActiveQuests.insert(make_pair(50, ActiveQuestData(3000, 2, L"������ ��Ȯ�ϱ�"))); // ������ ��Ȯ�ϱ�		
			break;

		default:
			assert(nullptr && "������ ����Ʈ �ܰ� ��ġ ����");
	}
}

/// <summary>
/// ���� ���� ���� ����Ʈ�� �̾Ƴ��� �Լ� 
/// </summary>
void QuestManagerScript::CreateChoresRandomQuest()
{
	// �õ尪�� ��� ���� random_device ����.
	std::random_device rd;

	// random_device �� ���� ���� ���� ������ �ʱ�ȭ �Ѵ�.
	std::mt19937 gen(rd());

	// 1 ���� 4 ���� �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����.
	std::uniform_int_distribution<int> dis(1, 4);
	//dis(gen)
	switch (dis(gen))
	{
		case 1:
			m_ActiveQuests.insert(make_pair(60, ActiveQuestData(4000, 3, L"���� �����ϱ�")));
			break;
		case 2:
			m_ActiveQuests.insert(make_pair(70, ActiveQuestData(1000, 1, L"â�� �����ϱ�")));
			break;
		case 3:
			m_ActiveQuests.insert(make_pair(80, ActiveQuestData(5000, 2, L"����� �� �ֱ�")));
			EventManager::GetInstance()->SendEvent("CatBowlEmpty", nullptr);
			break;
		case 4:
			m_ActiveQuests.insert(make_pair(90, ActiveQuestData(5000, 2, L"â�� �� ����")));
			break;

		default:
			assert(nullptr && "���� �� ����");
	}
}  

/// <summary>
/// ��� ����Ʈ ���� ����
/// </summary>
void QuestManagerScript::CreateEmergencyRandomQuest()
{
	//// �õ尪�� ��� ���� random_device ����.
	//std::random_device rd;

	//// random_device �� ���� ���� ���� ������ �ʱ�ȭ �Ѵ�.
	//std::mt19937 gen(rd());

	//// 1 ���� 4 ���� �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����.
	//std::uniform_int_distribution<int> dis(1);

	//int questId = 0;
	//switch (dis(gen))
	//{
	//	case 1:
	//		questId = 100;
	//		m_ActiveQuests.insert(make_pair(100, ActiveQuestData(7000, 1, L"����� ���ϱ�")));
	//		break;

	//	default:
	//		assert(nullptr && "���� �� ����");
	//}


	int uiIndex = m_ActiveQuests.size();
	m_QuestUiList->m_QuestText[uiIndex]->SetTextColor(COLOR::EMERGENCY);

	int questId = 100;
	m_ActiveQuests.insert(make_pair(100, ActiveQuestData(7000, 1, L"����� ���ϱ�", m_QuestUiIndex)));

	// ����Ʈ UI �� �����Ų��.
	m_QuestUiList->m_QuestText[m_QuestUiIndex]->SetText((m_ActiveQuests.find(questId))->second.questName);

	++m_QuestUiIndex;

	// ����̸� ����ΰ� ��Ȱ��ȭ ��Ų ����
	// ������Ʈ�� ����� ���� ��Ʈ�� �����Ű��

	// ����� ��Ȱ��ȭ
	m_SaveCat = false;
	m_CatObject->SetActivate(false);
	m_CatObject->GetTransform()->SetLocalPosition({ 10000.f,10000.f });
	m_PondObject->GetObjectChild("PondCat")->SetActivate(true);
	//m_PondObject->GetObjectChild("pondExclamationMark")->SetActivate(true);

	m_Camera->m_Target = m_PondObject;
	m_Camera->m_TargetOn = false;
	m_Camera->m_SmoothMoveToTarget = true;
}

/// <summary>
/// �� ����Ʈ ���� ����
/// </summary>
void QuestManagerScript::CreateNightRandomQuest()
{
	//// �õ尪�� ��� ���� random_device ����.
	//std::random_device rd;

	//// random_device �� ���� ���� ���� ������ �ʱ�ȭ �Ѵ�.
	//std::mt19937 gen(rd());

	//// 1 ���� 4 ���� �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����.
	//std::uniform_int_distribution<int> dis(1);

	//int questId = 0;
	//switch (dis(gen))
	//{
	//	case 1:
	//		questId = 1000;
	//		m_ActiveQuests.insert(make_pair(1000, ActiveQuestData(10000, 1, L"����� �i�Ƴ���")));
	//		break;	

	//	default:
	//		assert(nullptr && "���� �� ����");
	//}

	int uiIndex = m_ActiveQuests.size();
	m_QuestUiList->m_QuestText[uiIndex]->SetTextColor(COLOR::EMERGENCY);

	int questId = 1000;
	m_ActiveQuests.insert(make_pair(1000, ActiveQuestData(10000, 1, L"����� �i�Ƴ���", m_QuestUiIndex)));

	// ����Ʈ UI �� �����Ų��.
	m_QuestUiList->m_QuestText[m_QuestUiIndex]->SetText((m_ActiveQuests.find(questId))->second.questName);

	++m_QuestUiIndex;

	// ���� ������� ������Ų��.
	GameManagerScript* gameManagerScript = nullptr;
	const auto& gameObjects = GetGameObject()->GetScene()->GetGameObjects();
	for (auto& object : gameObjects)
	{
		if (object->GetName() == "wildBoar")
		{
			CreateWildBoar(object);
		}
	}

	m_ApplyFarm = false;

	m_Camera->m_Target = m_WildBoar;
	m_Camera->m_TargetOn = false;
	m_Camera->m_SmoothMoveToTarget = true;
}

/// <summary>
/// Ȱ��ȭ�� ����Ʈ�� �̿��Ͽ� ����Ʈ UI �� ���� 
/// </summary>
void QuestManagerScript::CreateQuestUi()
{
	// Ȱ��ȭ�� ����Ʈ���� ����Ʈ UI �� ����
	m_QuestUiIndex = 0;

	for (auto& quest : m_ActiveQuests)
	{
		m_QuestUiList->m_QuestText[m_QuestUiIndex]->SetTextColor(COLOR::QUESTLIST);

		quest.second.uiOrder = m_QuestUiIndex;
		m_QuestUiList->m_QuestText[m_QuestUiIndex]->SetText(quest.second.questName);
		++m_QuestUiIndex;
	}
}

/// <summary>
/// ����Ʈ�� �ʿ��� �������� �����.
/// </summary>
Item* QuestManagerScript::CreateQuestItem(ITEM_TYPE itemType)
{
	int index = static_cast<int>(itemType);

	switch (index)
	{
		case 0:
		{
			Item* ax = new Item(L"ax", L"����", ITEM_TYPE::AX, false);
			AX_ = ax;
			return ax;
		}
		case 1:
		{
			Item* bag = new Item(L"bag", L"�� �ڷ�", ITEM_TYPE::BAG, true, 18);
			BAG_ = bag;
			return bag;
		}
		case 2:
		{
			Item* catfood = new Item(L"catfood", L"�������", ITEM_TYPE::CATFOOD, false);
			CATFOOD_ = catfood;
			return catfood;
		}
		case 3:
		{
			Item* corn = new Item(L"corn", L"������", ITEM_TYPE::CORN, false);
			CORN_ = corn;
			return corn;
		}
		case 4:
		{
			Item* fertilizer = new Item(L"fertilizer", L"�������", ITEM_TYPE::FERTILIZER, true, 18);
			FERTILIZER_ = fertilizer;
			return fertilizer;
		}
		case 5:
		{
			Item* firewood = new Item(L"firewood", L"����", ITEM_TYPE::FIREWOOD, false);
			FIREWOOD_ = firewood;
			return firewood;
		}
		case 6:
		{
			Item* seed = new Item(L"seed", L"����", ITEM_TYPE::SEED, true, 18);
			SEED_ = seed;
			return seed;
		}
		case 7:
		{
			Item* emptyWatering = new Item(L"emptyWatering", L"���Ѹ���", ITEM_TYPE::EMPTYWATERING, false);
			EMPTYWATERING_ = emptyWatering;
			return emptyWatering;
		}
		case 8:
		{
			Item* watering = new Item(L"watering", L"���Ѹ���", ITEM_TYPE::WATERING, false);
			WATERING_ = watering;
			return watering;
		}
		case 9:
		{
			Item* key = new Item(L"key", L"����", ITEM_TYPE::KEY, false);
			KEY_ = key;
			return key;
		}

		default:
			assert(nullptr && "������ Ÿ�� üũ");
			return 0;
	}
}

void QuestManagerScript::DeleteAllItem()
{
	for (auto& item : m_QuestItemList)
	{
		if (item != nullptr)
		{
			if (item->m_ItemKeyName != L"corn")
			{
				delete item;
				item = nullptr;
			}
		}
	}

	for (auto& item : m_ItemManager->m_ItemList)
	{
		if (item != nullptr)
		{
			if (item->m_ItemKeyName != L"corn")
			{
				item = nullptr;
			}
		}
	}

	m_ItemManager->ApplyItemSlot();
}

/// <summary>
/// ��ĵ�� ObjectId ���� ����Ʈ�� ã�Ƴ��� 
/// ã�Ƴ� ����Ʈ�� ���൵�� �˾Ƴ��� 
/// ���൵�� �´� ����Ʈ�� �ҷ�����.
/// </summary>
void QuestManagerScript::CheckQuest(int scanObjectId, GameObject* scanObject)
{
	int questId = 0; // ã�Ƴ� ����Ʈ ���̵�
	int prevInteractionCount = 0; 	// ��ȿ�ۿ� Ƚ�� üũ	
	bool completeQuest = false; // �Ϸ�� ����Ʈ ���� üũ

	if (m_InteractionCount != 0)
	{
		prevInteractionCount = m_InteractionCount;
	}


	// Ȱ��ȭ ���� ����Ʈ�� �˻��Ѵ�. 
	for (auto& quest : m_ActiveQuests)
	{
		// �Ϸ�� ����Ʈ��� �ѱ��. (����Ʈ�� ���� ������� �˻��ϱ� ������ ���� ��ġ�� ���ϰų� ������ �ѹ� �� ����غ�����)
		if (quest.second.isComplete)
		{
			completeQuest = true;
			continue;
		}

		// ����Ʈ objectId �� ��ĵ�� objectId �� ���ٸ�
		if (quest.second.objectId == scanObjectId)
		{
			// �ش� ����Ʈ ���̵� + ����Ʈ �ܰ踦 ���ؼ� ����Ʈ ���൵�� �´� ����Ʈ ���̵� ���� ����.
			questId = quest.first + quest.second.questStep;

			// ����Ʈ ���̵� Ȱ���ؼ� ��ȣ�ۿ�
			bool stepUp = InteractionQuest(questId, scanObject);

			// ����Ʈ ���൵ ����
			if (stepUp)
			{
				++(quest.second.questStep);

				for (auto& scanObjectChild : scanObject->GetObjectChildren())
				{
					if (scanObjectChild->GetObjectType() == OBJECT_TYPE::INTERACTION_ARROW)
					{
						if (scanObjectChild->GetName() == "warehouseAInteractionArrow" || scanObjectChild->GetName() == "warehouseCInteractionArrow" || scanObjectChild->GetName() == "warehouseDInteractionArrow")
						{
							if (dynamic_cast<ObjectDataScript*>(scanObjectChild->GetComponent<MonoBehaviour>())->interactionArrowCount == 0)
							{
								scanObjectChild->SetActivate(false);
								break;
							}
							else
							{
								--(dynamic_cast<ObjectDataScript*>(scanObjectChild->GetComponent<MonoBehaviour>())->interactionArrowCount);

								if (dynamic_cast<ObjectDataScript*>(scanObjectChild->GetComponent<MonoBehaviour>())->interactionArrowCount == 0)
								{
									scanObjectChild->SetActivate(false);
									break;
								}
							}
						}
						else
						{
							scanObjectChild->SetActivate(false);
							break;
						}

						//scanObjectChild->SetActivate(false);
					}
				}
			}

			// �׽�Ʈ
			// ����Ʈ �Ϸ� �̹��� �׽�Ʈ �ڵ�
			//quest.second.questStep = quest.second.maxQuestStep;

			// ����Ʈ �ܰ踦 Ȯ���Ͽ� ���� �����ߴٸ� �ش��ϴ� ����Ʈ�� �Ϸ� ó���Ѵ�.
			if (quest.second.questStep == quest.second.maxQuestStep)
			{
				quest.second.isComplete = true;

				// UI ���� (����Ʈ �Ϸ� -> üũ �ڽ��� üũ�� �̹���)
				m_QuestUiList->m_CheckBoxImage[quest.second.uiOrder]->SetImage
				(
					ResourceManager::GetInstance()->LoadResource<Image>(L"Check", L"/UI/check.png")->GetImage()
				);
			}

			// �������� ����Ʈ ���൵�� ���������� �׿� ���� ��ȣ�ۿ� ���Ѿ� �� ������Ʈ Id �ֽ�ȭ
			else
			{
				if (stepUp)
				{
					quest.second.objectId = m_QuestList[questId + 1].objectId;

					for (auto& object : m_QuestObjects)
					{
						if (object->GetObjectType() == OBJECT_TYPE::FARM)
						{
							continue;
						}
						if (quest.second.objectId == dynamic_cast<ObjectDataScript*>(object->GetComponent<MonoBehaviour>())->id)
						{
							for (auto& objectChild : object->GetObjectChildren())
							{
								if (objectChild->GetObjectType() == OBJECT_TYPE::INTERACTION_ARROW)
								{
									if (objectChild->GetName() == "warehouseAInteractionArrow" || objectChild->GetName() == "warehouseCInteractionArrow" || objectChild->GetName() == "warehouseDInteractionArrow")
									{
										++(dynamic_cast<ObjectDataScript*>(objectChild->GetComponent<MonoBehaviour>())->interactionArrowCount);
										objectChild->SetActivate(true);
										break;
									}
									else
									{
										objectChild->SetActivate(true);
										break;
									}
								}
							}
						}
					}
				}
			}

			if (prevInteractionCount != m_InteractionCount)
			{
				return;
			}
		}
	}

	if (completeQuest)
	{
		return;
	}

	//assert(nullptr && "��ĵ�� ������Ʈ�� �ش��ϴ� ����Ʈ�� ����");
}

/// <summary>
/// �ش� ����Ʈ�� ã�Ƴ��� �˸´� ��ȣ�ۿ��� ��Ų��.
/// EX. â���� �� ���Ѹ����� ȹ�� -> ���� �ܰ� ����Ʈ�� �Ѿ�� (���Ѹ����� �� ä���)
/// </summary>
bool QuestManagerScript::InteractionQuest(int questId, GameObject* scanObject)
{
	switch (questId)
	{
		/// ������ ����Ʈ ------------------------------------
		case 10:	// ������ �翡 �� �Ѹ��� 1�ܰ� 
		{
			SoundManager::GetInstance()->PlaySoundSFX(
				ResourceManager::GetInstance()->LoadResource<Sound>(L"SeedSFX", L"/Sound/SFX/Interaction/Item_Pickup.mp3")->GetSound());

			// ���� ����
			Item* seed = CreateQuestItem(ITEM_TYPE::SEED);

			// ������ �߰�
			m_ItemManager->AddItem(seed);

			// ��ȣ �ۿ� Ƚ�� ����
			++m_InteractionCount;

			return true;
		}
		case 11:	// ������ �翡 �� �Ѹ��� 2�ܰ�
		{
			// ��� �ִ� �������� �ִ��� üũ
			if (m_ItemManager->GetCurrentFocusItem() != nullptr)
			{
				// ��� �ִ� �������� �������� üũ
				if (m_ItemManager->GetCurrentFocusItem()->m_ItemKeyName == L"seed")
				{
					FarmScript* farm = dynamic_cast<FarmScript*>(scanObject->GetObjectParent()->GetComponent<MonoBehaviour>());

					if (farm->detailState == "NoSeed" && farm->isInteraction == false && farm->isDestroyFarm == false)
					{
						EventManager::GetInstance()->SendEvent("FarmerInteraction", nullptr);

						SoundManager::GetInstance()->PlaySoundSFX(
							ResourceManager::GetInstance()->LoadResource<Sound>(L"SeedSFX", L"/Sound/SFX/Farm/Seed.wav")->GetSound());

						m_Player->GetObjectChild("farmerEffect")->GetComponent<Animator>()->FindAnimation(L"farmerEffectAnimation")->SetFrame(0);
						m_Player->GetObjectChild("farmerEffect")->GetComponent<Animator>()->Play(L"farmerEffectAnimation", false);
						farm->detailState = "Seed";
						farm->fakeSeed = true;

						farm->isInteraction = true;
						int& completeCornQuestCount = farm->completeQuestCount;
						++completeCornQuestCount;

						if (completeCornQuestCount == 2)
						{
							if (!m_IsCornQuestStepUp)
							{
								m_IsCornQuestStepUp = true;
							}
						}

						// ���� ������ 0 �� �ƴ϶�� 1���� ���
						if (SEED_->m_ItemCount != 0)
						{
							SEED_->m_ItemCount -= 1;

							if (SEED_->m_ItemCount == 1)
							{
								SEED_->m_IsCountable = false;
							}
						}

						// 0 �̸� ������ ����
						if (SEED_->m_ItemCount == 0)
						{
							m_ItemManager->DeleteItem(SEED_);
							SEED_ = nullptr;

							m_ItemManager->ApplyItemSlot();

							return true;
						}

						m_ItemManager->ApplyItemSlot();

						// ��ȣ �ۿ� Ƚ�� ����
						++m_InteractionCount;
					}
				}
			}

			return false;
		}
		//case 20:	// ������ ���� ���� �̱� 2�ܰ�
		//{
		//	// ������ ����Ʈ �Ϸ� ī��Ʈ 1 ����
		//	++m_DayCompleteCornQuestCount;

		//	EventManager::GetInstance()->SendEvent("FarmCompleteQuestCount", nullptr);

		//	// ��ȣ �ۿ� Ƚ�� ����
		//	++m_InteractionCount;

		//	return true;
		//}
		case 30:	// ������ �翡 ��� �ֱ� 1�ܰ�
		{
			SoundManager::GetInstance()->PlaySoundSFX(
				ResourceManager::GetInstance()->LoadResource<Sound>(L"SeedSFX", L"/Sound/SFX/Interaction/Item_Pickup.mp3")->GetSound());

			// ������� ����
			Item* fertilizer = CreateQuestItem(ITEM_TYPE::FERTILIZER);

			// ������ �߰�
			m_ItemManager->AddItem(fertilizer);

			// ��ȣ �ۿ� Ƚ�� ����
			++m_InteractionCount;

			return true;
		}
		case 31:	// ������ �翡 ��� �ֱ� 2�ܰ�
		{
			// ��� �ִ� �������� �ִ��� üũ
			if (m_ItemManager->GetCurrentFocusItem() != nullptr)
			{
				// ��� �ִ� �������� ������� üũ
				if (m_ItemManager->GetCurrentFocusItem()->m_ItemKeyName == L"fertilizer")
				{
					FarmScript* farm = dynamic_cast<FarmScript*>(scanObject->GetObjectParent()->GetComponent<MonoBehaviour>());
					string farmState = farm->detailState;
					if ((farmState == "Seed" || farmState == "First" || farmState == "Second") && !(farm->isInteraction))
					{
						// ��ȣ�ۿ� ��� �̺�Ʈ On
						EventManager::GetInstance()->SendEvent("FarmerInteraction", nullptr);

						/*SoundManager::GetInstance()->PlaySoundSFX(
							ResourceManager::GetInstance()->LoadResource<Sound>(L"FertilizerSFX", L"/Sound/SFX/Farm/Digging_Dirt_With_Hand (1).mp3")->GetSound());*/

						SoundManager::GetInstance()->PlaySoundSFX(
							ResourceManager::GetInstance()->LoadResource<Sound>(L"FertilizerSFX", L"/Sound/SFX/Farm/Digging_Dirt_With_Hand (2).mp3")->GetSound());

						// ��� �̹��� On
						int index = farm->farmNumber;
						for (auto farmChild : scanObject->GetObjectParent()->GetObjectChildren())
						{
							if (farmChild->GetName() == "SecondFarmChildFertilizer" + to_string(index) || farmChild->GetName() == "FirstFarmChildFertilizer" + to_string(index))
							{
								farmChild->SetActivate(true);
							}
						}

						// ��ȣ�ۿ� bool ���� on
						farm->isInteraction = true;

						// ������ 2���� ����Ʈ �� 1
						int& completeCornQuestCount = farm->completeQuestCount;
						++completeCornQuestCount;

						if (completeCornQuestCount == 2)
						{
							if (!m_IsCornQuestStepUp)
							{
								m_IsCornQuestStepUp = true;
							}
						}

						// ��� ������ 0 �� �ƴ϶�� 1�� ���̱�
						if (FERTILIZER_->m_ItemCount != 0)
						{
							FERTILIZER_->m_ItemCount -= 1;

							if (FERTILIZER_->m_ItemCount == 1)
							{
								FERTILIZER_->m_IsCountable = false;
							}
						}

						// 0�̶�� ������ ����
						if (FERTILIZER_->m_ItemCount == 0)
						{
							m_ItemManager->DeleteItem(FERTILIZER_);
							FERTILIZER_ = nullptr;

							m_ItemManager->ApplyItemSlot();

							return true;
						}

						// ��ȣ �ۿ� Ƚ�� ����
						++m_InteractionCount;
					}
				}
			}

			return false;
		}
		case 40:	// ������ �翡 �� �ֱ� 1�ܰ�	
		{
			SoundManager::GetInstance()->PlaySoundSFX(
				ResourceManager::GetInstance()->LoadResource<Sound>(L"SeedSFX", L"/Sound/SFX/Interaction/Item_Pickup.mp3")->GetSound());

			// �� ���Ѹ��� ����
			Item* emptyWatering = CreateQuestItem(ITEM_TYPE::EMPTYWATERING);

			// ������ �߰�	
			m_ItemManager->AddItem(emptyWatering);

			// ��ȣ �ۿ� Ƚ�� ����
			++m_InteractionCount;

			return true;
		}
		case 41:	// ������ �翡 �� �ֱ� 2�ܰ�
		{
			// ��� �ִ� �������� �ִ��� üũ
			if (m_ItemManager->GetCurrentFocusItem() != nullptr)
			{
				// ��� �ִ� �������� �� ���Ѹ��� ���� üũ
				if (m_ItemManager->GetCurrentFocusItem()->m_ItemKeyName == L"emptyWatering")
				{
					if (dynamic_cast<ObjectDataScript*>(scanObject->GetComponent<MonoBehaviour>())->inabilityGetWater == false)
					{
						EventManager::GetInstance()->SendEvent("FarmerInteraction", nullptr);

						SoundManager::GetInstance()->PlaySoundSFX(
							ResourceManager::GetInstance()->LoadResource<Sound>(L"FertilizerSFX", L"/Sound/SFX/Interaction/Water_Filled.mp3")->GetSound());

						// �� ���Ѹ��� ����
						m_ItemManager->DeleteItem(EMPTYWATERING_);
						EMPTYWATERING_ = nullptr;

						// ���Ѹ��� ���� �� �߰�
						Item* watering = CreateQuestItem(ITEM_TYPE::WATERING);

						m_ItemManager->AddItem(watering);

						// ��ȣ �ۿ� Ƚ�� ����
						++m_InteractionCount;

						return true;
					}
					/*else
					{
						string stringObjectKey = scanObject->GetName();

						if (stringObjectKey.compare("pond") != 0)
						{
							m_Player->GetObjectChild("farmerTextBubble")->SetActivate(true);

							wstring texts = dynamic_cast<FarmerScript*>(m_Player->GetComponent<MonoBehaviour>())->textManager->explainMap[stringObjectKey][1];
							m_Player->GetObjectChild("farmerTextBubble")->GetComponent<TextRenderer>()->SetTextColor(COLOR::EMERGENCY);
							m_Player->GetObjectChild("farmerTextBubble")->GetComponent<TextRenderer>()->SetText(texts);
						}
					}*/
				}
			}

			return false;
		}
		case 42:	// ������ �翡 �� �ֱ� 3�ܰ�
		{
			// �� �Ѹ����� 8�� ���� �Ѹ��� �����ؾ� ���Ѿ� �Ѵ�.
			static int waterCount = 18;
			if (m_questReset)
			{
				waterCount = 18;
				m_questReset = false;
			}

			// ��� �ִ� �������� �ִ��� üũ
			if (m_ItemManager->GetCurrentFocusItem() != nullptr)
			{
				// ��� �ִ� �������� ���Ѹ��� ���� üũ
				if (m_ItemManager->GetCurrentFocusItem()->m_ItemKeyName == L"watering")
				{
					FarmScript* farm = dynamic_cast<FarmScript*>(scanObject->GetObjectParent()->GetComponent<MonoBehaviour>());
					if (farm->farmState == "FarmNoWater" && farm->isDestroyFarm == false)
					{
						farm->farmState = "FarmWater";

						SoundManager::GetInstance()->PlaySoundSFX(
							ResourceManager::GetInstance()->LoadResource<Sound>(L"WateringSFX", L"/Sound/SFX/Farm/Watering_Plants.mp3")->GetSound());

						int& completeCornQuestCount = farm->completeQuestCount;
						++completeCornQuestCount;

						if (completeCornQuestCount == 2)
						{
							if (!m_IsCornQuestStepUp)
							{
								m_IsCornQuestStepUp = true;
							}
						}

						--waterCount;

						// �� �Ѹ��� ���
						EventManager::GetInstance()->SendEvent("FarmerWatering", nullptr);

						// ���� �� �Ѹ��� Ƚ���� 0 �̶�� 
						if (waterCount == 0)
						{
							// ���Ѹ��� ����
							m_ItemManager->DeleteItem(WATERING_);
							WATERING_ = nullptr;

							m_ItemManager->ApplyItemSlot();

							return true;
						}

						// ��ȣ �ۿ� Ƚ�� ����
						++m_InteractionCount;
					}
				}
			}

			return false;
		}
		case 50:	// ������ ��Ȯ�ϱ� 1�ܰ�
		{
			SoundManager::GetInstance()->PlaySoundSFX(
				ResourceManager::GetInstance()->LoadResource<Sound>(L"SeedSFX", L"/Sound/SFX/Interaction/Item_Pickup.mp3")->GetSound());

			// �� �ڷ� ����
			Item* bag = CreateQuestItem(ITEM_TYPE::BAG);

			// ������ �߰�	
			m_ItemManager->AddItem(bag);

			// ��ȣ �ۿ� Ƚ�� ����
			++m_InteractionCount;

			return true;
		}
		case 51:	// ������ ��Ȯ�ϱ� 2�ܰ�
		{
			// ��� �ִ� �������� �ִ��� üũ
			if (m_ItemManager->GetCurrentFocusItem() != nullptr)
			{
				// ��� �ִ� �������� ���Ѹ��� ���� üũ
				if (m_ItemManager->GetCurrentFocusItem()->m_ItemKeyName == L"bag")
				{
					FarmScript* farm = dynamic_cast<FarmScript*>(scanObject->GetObjectParent()->GetComponent<MonoBehaviour>());

					if (farm->detailState == "Third")
					{
						EventManager::GetInstance()->SendEvent("FarmerHarvest", nullptr);

						SoundManager::GetInstance()->PlaySoundSFX(
							ResourceManager::GetInstance()->LoadResource<Sound>(L"HarvestSFX", L"/Sound/SFX/Farm/Harvest_Crop.mp3")->GetSound());

						m_Player->GetObjectChild("farmerEffect")->GetComponent<Animator>()->FindAnimation(L"farmerEffectAnimation")->SetFrame(0);
						m_Player->GetObjectChild("farmerEffect")->GetComponent<Animator>()->Play(L"farmerEffectAnimation", false);

						farm->detailState = "NoSeed";

						bool createCorn = true;
						for (auto& item : m_ItemManager->m_ItemList)
						{
							if (item != nullptr)
							{
								if (item->m_ItemKeyName == L"corn")
								{
									if (item->m_IsCountable == false)
									{
										item->m_IsCountable = true;
										++(item->m_ItemCount);
										createCorn = false;
									}
									else
									{
										++(item->m_ItemCount);
										createCorn = false;
									}
								}
							}
						}

						if (createCorn)
						{
							Item* corn = CreateQuestItem(ITEM_TYPE::CORN);
							m_ItemManager->AddItem(corn);
						}

						if (BAG_->m_ItemCount != 0)
						{
							BAG_->m_ItemCount -= 1;

							if (BAG_->m_ItemCount == 1)
							{
								BAG_->m_IsCountable = false;
							}
						}

						if (BAG_->m_ItemCount == 0)
						{
							m_ItemManager->DeleteItem(BAG_);
							BAG_ = nullptr;

							m_ItemManager->ApplyItemSlot();

							return true;
						}

						m_ItemManager->ApplyItemSlot();

						// ��ȣ �ۿ� Ƚ�� ����
						++m_InteractionCount;

						return false;
					}
				}
			}

			return false;
		}

		/// ���� ����Ʈ ------------------------------------
		case 60:	// ���� �����ϱ� 1�ܰ�
		{
			SoundManager::GetInstance()->PlaySoundSFX(
				ResourceManager::GetInstance()->LoadResource<Sound>(L"SeedSFX", L"/Sound/SFX/Interaction/Item_Pickup.mp3")->GetSound());

			// ���� ����
			Item* ax = CreateQuestItem(ITEM_TYPE::AX);

			// ������ �߰�	
			m_ItemManager->AddItem(ax);

			// ��ȣ �ۿ� Ƚ�� ����
			++m_InteractionCount;

			return true;
		}
		case 61:	// ���� �����ϱ� 2�ܰ�
		{
			// ��� �ִ� �������� �ִ��� üũ
			if (m_ItemManager->GetCurrentFocusItem() != nullptr)
			{
				// ������ ��� �ִ���
				if (m_ItemManager->GetCurrentFocusItem()->m_ItemKeyName == L"ax")
				{
					EventManager::GetInstance()->SendEvent("FarmerInteraction", nullptr);

					SoundManager::GetInstance()->PlaySoundSFX(
						ResourceManager::GetInstance()->LoadResource<Sound>(L"FertilizerSFX", L"/Sound/SFX/Interaction/Cut_Down_Tree_With_Axe.mp3")->GetSound());

					// ���� ����
					m_ItemManager->DeleteItem(AX_);
					AX_ = nullptr;

					// ���� ����
					Item* firewood = CreateQuestItem(ITEM_TYPE::FIREWOOD);
					m_ItemManager->AddItem(firewood);

					// ��ȣ �ۿ� Ƚ�� ����
					++m_InteractionCount;

					return true;
				}
			}

			return false;
		}
		case 62:	// ���� �����ϱ� 3�ܰ�
		{
			// ��� �ִ� �������� �ִ��� üũ
			if (m_ItemManager->GetCurrentFocusItem() != nullptr)
			{
				// ������ ��� �ִ���
				if (m_ItemManager->GetCurrentFocusItem()->m_ItemKeyName == L"firewood")
				{
					SoundManager::GetInstance()->PlaySoundSFX(
						ResourceManager::GetInstance()->LoadResource<Sound>(L"SeedSFX", L"/Sound/SFX/Interaction/Item_Pickup.mp3")->GetSound());

					// ���� ����
					m_ItemManager->DeleteItem(FIREWOOD_);
					FIREWOOD_ = nullptr;

					m_ItemManager->ApplyItemSlot();

					// ��ȣ �ۿ� Ƚ�� ����
					++m_InteractionCount;

					return true;
				}
			}

			return false;
		}
		case 70:	// â�� �����ϱ�
			/// �̴� ���� �ߵ��ϴ� �ڵ� �־��ֱ� 

			EventManager::GetInstance()->SendEvent("CornGame", nullptr);

			SoundManager::GetInstance()->PlaySoundSFX(
				ResourceManager::GetInstance()->LoadResource<Sound>(L"SeedSFX", L"/Sound/SFX/Interaction/Item_Pickup.mp3")->GetSound());

			// ��ȣ �ۿ� Ƚ�� ����
			++m_InteractionCount;

			return false;
		case 80:	// ����� �� �ֱ� 1�ܰ�
		{
			SoundManager::GetInstance()->PlaySoundSFX(
				ResourceManager::GetInstance()->LoadResource<Sound>(L"SeedSFX", L"/Sound/SFX/Interaction/Item_Pickup.mp3")->GetSound());

			// ������� ����
			Item* catfood = CreateQuestItem(ITEM_TYPE::CATFOOD);

			// ������ �߰�	
			m_ItemManager->AddItem(catfood);

			// ��ȣ �ۿ� Ƚ�� ����
			++m_InteractionCount;

			return true;
		}
		case 81:	// ����� �� �ֱ� 2�ܰ�
		{
			// ��� �ִ� �������� �ִ��� üũ
			if (m_ItemManager->GetCurrentFocusItem() != nullptr)
			{
				// ������븦 ��� �ִ���
				if (m_ItemManager->GetCurrentFocusItem()->m_ItemKeyName == L"catfood")
				{
					EventManager::GetInstance()->SendEvent("FarmerInteraction", nullptr);

					SoundManager::GetInstance()->PlaySoundSFX(
						ResourceManager::GetInstance()->LoadResource<Sound>(L"FertilizerSFX", L"/Sound/SFX/Interaction/catFood_Pouring.mp3")->GetSound());

					// ������� ����
					m_ItemManager->DeleteItem(CATFOOD_);

					CATFOOD_ = nullptr;

					m_ItemManager->ApplyItemSlot();

					EventManager::GetInstance()->SendEvent("CatBowlFull", nullptr);

					// ��ȣ �ۿ� Ƚ�� ����
					++m_InteractionCount;

					return true;
				}
			}

			return false;
		}
		case 90:	// â�� �� ���� 1�ܰ�
		{
			SoundManager::GetInstance()->PlaySoundSFX(
				ResourceManager::GetInstance()->LoadResource<Sound>(L"SeedSFX", L"/Sound/SFX/Interaction/Item_Pickup.mp3")->GetSound());

			// ���� ����
			Item* key = CreateQuestItem(ITEM_TYPE::KEY);

			// ������ �߰�	
			m_ItemManager->AddItem(key);

			// ��ȣ �ۿ� Ƚ�� ����
			++m_InteractionCount;

			return true;
		}
		case 91:	// â�� �� ���� 2�ܰ�
		{
			// ��� �ִ� �������� �ִ��� üũ
			if (m_ItemManager->GetCurrentFocusItem() != nullptr)
			{
				// ���踦 ��� �ִ���
				if (m_ItemManager->GetCurrentFocusItem()->m_ItemKeyName == L"key")
				{
					/// �̴� ����
					EventManager::GetInstance()->SendEvent("WareHouseGame", nullptr);

					// ��ȣ �ۿ� Ƚ�� ����
					++m_InteractionCount;
				}
			}

			return false;
		}

		/// ��� ����Ʈ  ----------------------------------
		case 100:	// ����� ���ϱ�
		{
			EventManager::GetInstance()->SendEvent("FarmerInteraction", nullptr);

			dynamic_cast<BlackCatScript*>(m_CatObject->GetComponent<MonoBehaviour>())->isSave = true;
			dynamic_cast<BlackCatScript*>(m_CatObject->GetComponent<MonoBehaviour>())->action = true;
			/*m_CatObject->GetObjectChild("blackCatChild")->SetActivate(true);

			SoundManager::GetInstance()->StopSoundSFX();

			SoundManager::GetInstance()->PlaySoundSFX(
				ResourceManager::GetInstance()->LoadResource<Sound>(L"CatMeowSFX", L"/Sound/SFX/Interaction/Cat_Meow.mp3")->GetSound());*/

			Vector2 playerPosition = m_Player->GetTransform()->GetLocalPosition();
			Vector2 pondPosition = m_PondObject->GetTransform()->GetLocalPosition();


			if (playerPosition.x > pondPosition.x)
			{
				m_CatObject->GetTransform()->SetLocalPosition(playerPosition + Vector2{ 70.f, 0.f });
			}
			else
			{
				m_CatObject->GetTransform()->SetLocalPosition(playerPosition + Vector2{ -70.f, 0.f });
			}

			m_CatObject->SetActivate(true);
			m_PondObject->GetObjectChild("PondCat")->SetActivate(false);
			//m_PondObject->GetObjectChild("pondExclamationMark")->SetActivate(false);

			m_SaveCat = true;

			// ��ȣ �ۿ� Ƚ�� ����
			++m_InteractionCount;

			return true;
		}

		/// �� ����Ʈ  ------------------------------------
		case 1000:	// �˵��� �i�Ƴ���	
		{
			if (dynamic_cast<WildBoarScript*>(scanObject->GetObjectParent()->GetComponent<MonoBehaviour>())->m_IsCollision)
			{
				/// �̴� ����
				EventManager::GetInstance()->SendEvent("WildBoarGame", nullptr);

				m_WildBoar->SetActivate(false);
				m_WildBoar->GetObjectChild("wildBoarExclamationMark")->SetActivate(false);

				// ��ȣ �ۿ� Ƚ�� ����
				++m_InteractionCount;
			}

			return false;
		}
	}

	assert(nullptr && "�ش��ϴ� ����Ʈ�� ����");

	return false;
}

/// <summary>
/// Ȱ��ȭ ����Ʈ �� UI �ʱ�ȭ
/// </summary>
void QuestManagerScript::ClearQuest()
{
	// ����Ʈ �ʱ�ȭ true (InteractionQuest ���� ��� ����)
	m_questReset = true;

	// Ȱ��ȭ�� ����Ʈ �ʱ�ȭ
	m_ActiveQuests.clear();

	// ����Ʈ UI �ʱ�ȭ
	for (int index = 0; index < QUEST_UI_COUNT; index++)
	{
		m_QuestUiList->m_QuestText[index]->SetText(L"");
		m_QuestUiList->m_CheckBoxImage[index]->SetImage
		(
			ResourceManager::GetInstance()->LoadResource<Image>(L"noneCheck", L"/UI/nonecheck.png")->GetImage()
		);
	}

	for (auto& object : m_QuestObjects)
	{
		if (object->GetObjectType() == OBJECT_TYPE::FARM)
		{
			continue;
		}

		if (object->GetName() == "CatBowl" || object->GetName() == "Truck" || object->GetName() == "wildBoarChild" || object->GetName() == "Tree")
		{
			continue;
		}

		for (auto& objectChild : object->GetObjectChildren())
		{
			if (objectChild->GetObjectType() == OBJECT_TYPE::INTERACTION_ARROW)
			{
				if (objectChild->GetName() == "warehouseAInteractionArrow" || objectChild->GetName() == "warehouseCInteractionArrow" || objectChild->GetName() == "warehouseDInteractionArrow")
				{
					if (dynamic_cast<ObjectDataScript*>(objectChild->GetComponent<MonoBehaviour>())->interactionArrowCount == 0)
					{
						objectChild->SetActivate(false);
						break;
					}
					else
					{
						--(dynamic_cast<ObjectDataScript*>(objectChild->GetComponent<MonoBehaviour>())->interactionArrowCount);

						if (dynamic_cast<ObjectDataScript*>(objectChild->GetComponent<MonoBehaviour>())->interactionArrowCount == 0)
						{
							objectChild->SetActivate(false);
							break;
						}
					}
				}
				else
				{
					objectChild->SetActivate(false);
					break;
				}


				/*objectChild->SetActivate(false);
				break;*/
			}
		}
	}
}

/// <summary>
/// ���� Ȱ��ȭ�� ��� ����Ʈ�� �Ϸ� �ߴ��� üũ 
/// ���� �� �Ͻ������� ���� ���� ���� 
/// PoliceScript �̺�Ʈ ȣ�� 
/// 
/// ���� Ư�� ����Ʈ ���� �� �г�Ƽ �ο� (�� ����Ʈ ����� ���)
/// </summary>
void QuestManagerScript::CheckCompleteAllActiveQuests()
{
	// �� �ϳ��� ������ ����Ʈ�� �ִٸ� ���� ���� ����.
	for (auto& questIter : m_ActiveQuests)
	{
		if (!(questIter.second.isComplete))
		{
			m_CompleteAllActiveQuests = false;
			break;
		}
		else
		{
			m_CompleteAllActiveQuests = true;
		}
	}

	// ���� ���� ������ ���� PoliceScript �̺�Ʈ ȣ��
	if (m_CompleteAllActiveQuests == false)
	{
		EventManager::GetInstance()->SendEvent("PatrolLevelUp", nullptr);
		EventManager::GetInstance()->SendEvent("PatrolLevelTemporary", nullptr);
	}
}

// �� ����Ʈ : ����� �i�Ƴ���
// ����Ʈ �߻��� ���� ������� �߰���Ų��.
void QuestManagerScript::CreateWildBoar(GameObject* wildBoarObject)
{
	wildBoarObject->GetTransform()->SetLocalPosition(Vector2{ -2000.f, -150.f } + Vector2{ 960.f, 540.f });
	dynamic_cast<WildBoarScript*>(wildBoarObject->GetComponent<MonoBehaviour>())->m_PauseUpdate = false;
	dynamic_cast<WildBoarScript*>(wildBoarObject->GetComponent<MonoBehaviour>())->m_GameManager = dynamic_cast<FarmerScript*>(m_Player->GetComponent<MonoBehaviour>())->manager;
	wildBoarObject->GetComponent<Animator>()->Play(L"WildBoarRightRun", true);
	dynamic_cast<WildBoarScript*>(wildBoarObject->GetComponent<MonoBehaviour>())->m_IsCollision = false;
	wildBoarObject->SetActivate(true);
}

/// <summary>
/// �� ��ȣ�ۿ� ������ ����Ʈ �ǹ� ȭ��ǥ �̹��� �߰� 
/// </summary>
void QuestManagerScript::CreateQuestInteractionArrow()
{
	for (auto& quest : m_ActiveQuests)
	{
		for (auto& object : m_QuestObjects)
		{
			if (object->GetObjectType() == OBJECT_TYPE::FARM)
			{
				continue;
			}

			if (quest.second.objectId == dynamic_cast<ObjectDataScript*>(object->GetComponent<MonoBehaviour>())->id)
			{
				for (auto& objectChild : object->GetObjectChildren())
				{
					if (objectChild->GetObjectType() == OBJECT_TYPE::INTERACTION_ARROW)
					{
						if (objectChild->GetName() == "warehouseAInteractionArrow" || objectChild->GetName() == "warehouseCInteractionArrow" || objectChild->GetName() == "warehouseDInteractionArrow")
						{
							++(dynamic_cast<ObjectDataScript*>(objectChild->GetComponent<MonoBehaviour>())->interactionArrowCount);
							int test = dynamic_cast<ObjectDataScript*>(objectChild->GetComponent<MonoBehaviour>())->interactionArrowCount;
							objectChild->SetActivate(true);
							break;
						}
						else
						{
							objectChild->SetActivate(true);
							break;
						}
					}
				}
			}
		}
	}
}

/// <summary>
/// �� ���� �� �⺻������ �־���ϴ� ����Ʈ ���� ����
/// </summary>
void QuestManagerScript::CreateBasicNightQuest()
{
	m_QuestUiList->m_QuestText[0]->SetTextColor(COLOR::EMERGENCY);
	m_ActiveQuests.insert(make_pair(999, ActiveQuestData(100000, 1, L"������ ���� ���ؼ� ��ü �����", m_QuestUiIndex)));
	m_QuestUiList->m_QuestText[0]->SetText((m_ActiveQuests.find(999))->second.questName);
	++m_QuestUiIndex;
}

/// <summary>
/// �� ���� �� ��ü ����� ������ ���� ȭ��ǥ �̹��� ����
/// </summary>
void QuestManagerScript::CreateNightInteractionArrow()
{
	for (auto& object : m_QuestObjects)
	{
		if (object->GetObjectType() == OBJECT_TYPE::FARM)
		{
			continue;
		}

		if (object->GetName() == "CatBowl" || object->GetName() == "Truck" || object->GetName() == "wildBoarChild" || object->GetName() == "Tree" || object->GetName() == "WarehouseB")
		{
			continue;
		}
		
		for (auto& objectChild : object->GetObjectChildren())
		{
			if (objectChild->GetObjectType() == OBJECT_TYPE::INTERACTION_ARROW)
			{
				objectChild->SetActivate(true);
				break;
			}
		}
	}
}
