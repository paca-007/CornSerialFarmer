#include "GameManagerScript.h"

#include "ResourceManager.h"
#include "EventManager.h"

#include "Image.h"

#include "GameObject.h"
#include "Scene.h"

#include "ImageRenderer.h"
#include "TextRenderer.h"

#include "TimeBar.h"
#include "ItemSlot.h"
#include "PauseButton.h"
#include "GameOverWindow.h"

#include "QuestManagerScript.h"
#include "ObjectDataScript.h"
#include "ItemManagerScript.h"
#include "EffectManagerScript.h"
#include "FarmScript.h"
#include "WildBoarScript.h"
#include "Transform.h"

#include "GameplayStatics.h"
#include "FarmerScript.h"
#include "NPC1Script.h"
#include "NPC2Script.h"
#include "PoliceScript.h"
#include "Police2Script.h"
#include "KillWindow.h"
#include "QuestList.h"
#include "BlackScreenScript.h"
#include "BlackScreen.h"

#include "Animator.h"

// Sound
#include "SoundManager.h"
#include "Sound.h"

#include <random>

using namespace std;

GameManagerScript::GameManagerScript()
	: MonoBehaviour(typeid(this).name())
	, m_DayCount(1), m_IsDay(true), m_TimePassed(0.f), m_TimeBar(nullptr)
	, m_QuestManager(nullptr), m_ItemManager(nullptr), m_EffectManager(nullptr)
	, m_IsCreateQuest(true), m_IsCreateEmergencyQuest(false), m_EmergencyQuestTime(0)
	, m_IsCreateNightQuest(false), m_NightQuestTime(0)
	, m_PatrolLevelTemporary(false), m_PatrolLevel(1)
	, m_CatBowlAnimator(nullptr)
	, m_CurrentTimeZone(DAY_NIGHT::MORNING_EARLY), m_DayPassed(0.f), m_EffectTimePassed(0.f)
	, m_ColorTint(0.f), m_Temperature(0.f), m_Brightness(0.f)
	, m_ColorTintValue(0.f), m_TemperatureValue(0.f), m_BrightnessValue(0.f)
	, m_DayBGMVolume(0.f), m_NightBGMVolume(0.f)
	, m_IsGameOver(false), m_ShowGameOverWindow(false)
	, isNight(false), timeCount(0), isMorning(false)
	, m_NPC1Script(nullptr)
	, m_NPC2Script(nullptr)
	, m_NPC3Script(nullptr)
	, m_NPC4Script(nullptr)
	, m_NPC5Script(nullptr)
	, m_NPC6Script(nullptr)
	, m_Police1Script(nullptr)
	, m_Police2Script(nullptr)
	, m_Police3Script(nullptr)
	, m_Police4Script(nullptr)
	, m_BlackScreenScript(nullptr)
{
	DAY_TOTAL_TIME = DAYTIME_LENGTH + NIGHTTIME_LENGTH;

	// ����Ʈ �� ����
	m_TemperatureValue = 0.8f / (DAYTIME_LENGTH / 2);
	m_ColorTintValue = 0.5f / (DAYTIME_LENGTH / 2);
	m_BrightnessValue = 0.06f / (DAYTIME_LENGTH / 2);

	// ���� ����
	m_DayBGMVolume = 0.4f;
	m_NightBGMVolume = 0.3f;
}

GameManagerScript::~GameManagerScript()
{

}

void GameManagerScript::Start()
{
	// BGM
	SoundManager::GetInstance()->StopSoundBGM();
	SoundManager::GetInstance()->PlaySoundBGM(
		ResourceManager::GetInstance()->LoadResource<Sound>(L"DayBGM", L"/Sound/BGM/DAY/Joy Ride.mp3")->GetSound());
	SoundManager::GetInstance()->SetBGMVolume(m_DayBGMVolume);

	//�ʿ��� �̺�Ʈ �޴��� ����

	// ����
	EventManager::GetInstance()->RegisterClient("PatrolLevelUp", MakeCallInfo(&GameManagerScript::PatrolLevelUp));
	EventManager::GetInstance()->RegisterClient("PatrolLevelDown", MakeCallInfo(&GameManagerScript::PatrolLevelDown));
	EventManager::GetInstance()->RegisterClient("PatrolLevelTemporary", MakeCallInfo(&GameManagerScript::PatrolLevelTemporary));

	// �����
	EventManager::GetInstance()->RegisterClient("CatBowlEmpty", MakeCallInfo(&GameManagerScript::CatBowlEmpty));
	EventManager::GetInstance()->RegisterClient("CatBowlFull", MakeCallInfo(&GameManagerScript::CatBowlFull));

	//�˶�����
	EventManager::GetInstance()->RegisterClient("AlarmAlert", MakeCallInfo(&GameManagerScript::alarmAlert));

	//��ü ��ġ �ʱ�ȭ
	m_DeadBodySpot = Vector2(-150, 1110);
	killNPCList.push_back(1);
	killNPCList.push_back(2);
	killNPCList.push_back(5);
	killNPCList.push_back(6);

	EventManager::GetInstance()->RegisterClient("CompleteKill", MakeCallInfo(&GameManagerScript::GetKillNpcIndex));

}
void GameManagerScript::Update(float deltaTime)
{
	// ���ӿ��� üũ
	CheckGameOver();

	CheckDayProgress(deltaTime);
	TimeEffectManagement(deltaTime);

	if (m_QuestManager->m_FailWildBoar)
	{
		m_QuestManager->m_FailWildBoar = false;
		DestoryFarm();
	}
}

void GameManagerScript::Finalize()
{

}

/// <summary>
/// ���� ���� ����
/// �Ͻ������� �ƴ����� ����Ʈ�Ŵ����ʿ��� ���ӸŴ����� �����ش�.
/// �� ����Ʈ ���� : �� ���� ���� ���� ����
/// �� ����Ʈ ���� : ���������� ���� ���� ����
/// max ���� ������ 6
/// </summary>
void GameManagerScript::PatrolLevelUp(std::any)
{
	if (GetGameObject()->GetScene()->GetName() == "MainScene")
	{
		if (m_PatrolLevel < 6)
		{
			m_PatrolLevel++;
		}

		else
		{
			assert(nullptr && "���� ���� ��ġ ����");
		}
	}

}

/// <summary>
/// ���� ���� ����
/// </summary>
void GameManagerScript::PatrolLevelDown(std::any)
{
	if (GetGameObject()->GetScene()->GetName() == "MainScene")
	{
		if (m_PatrolLevel > 1)
		{
			--m_PatrolLevel;
		}

		else
		{
			assert(nullptr && "���� ���� ��ġ ����");
		}
	}
}

/// <summary>
/// ���� ���� ������ �Ͻ����̸� ���� �Ŵ����� �����ϰ� 
/// ���� �Ŵ��������� �ð��� �帧�� ���� ���� ������ �ٽ� ���ҽ����ش�.
/// </summary>
/// <param name=""></param>
void GameManagerScript::PatrolLevelTemporary(std::any)
{
	if (GetGameObject()->GetScene()->GetName() == "MainScene")
	{
		m_PatrolLevelTemporary = true;
	}
}

/// <summary>
/// ���� ������ ���� ���� ���ǵ� �Ҵ� �Լ�
/// </summary>
void GameManagerScript::PolicePatrolLevelCheck()
{
	OutputDebugString(TEXT("Patrol Level")); OutputDebugString(to_wstring(m_PatrolLevel).c_str()); OutputDebugString(TEXT("\n"));

	// ���� ������ üũ ��
	switch (m_PatrolLevel)
	{
		case 1:
			m_Police1Script->speed = 200;
			m_Police2Script->speed = 200;
			m_Police3Script->speed = 100;
			m_Police4Script->speed = 100;
			break;
		case 2:
			m_Police1Script->speed = 300;
			m_Police2Script->speed = 300;
			m_Police3Script->speed = 200;
			m_Police4Script->speed = 200;
			break;
		case 3:
			m_Police1Script->speed = 400;
			m_Police2Script->speed = 400;
			m_Police3Script->speed = 300;
			m_Police4Script->speed = 300;
			break;
		case 4:
			m_Police1Script->speed = 450;
			m_Police2Script->speed = 450;
			m_Police3Script->speed = 400;
			m_Police4Script->speed = 400;
			break;
		case 5:
			m_Police1Script->speed = 550;
			m_Police2Script->speed = 550;
			m_Police3Script->speed = 500;
			m_Police4Script->speed = 500;
			break;
		case 6:
			m_Police1Script->speed = 1000;
			m_Police2Script->speed = 1000;
			m_Police3Script->speed = 1000;
			m_Police4Script->speed = 1000;
			break;
	}
	// ���� ������ ���� speed�� �Ҵ�����ش�
}

/// <summary>
/// �˶��� �ʿ��� ���, �ʿ��� �˶��� �ҷ��ش�.
/// </summary>
void GameManagerScript::alarmAlert(std::any)
{

}

/// <summary>
/// ����� �� �ִϸ��̼� ���� ���� (�� ����)
/// </summary>
void GameManagerScript::CatBowlEmpty(std::any)
{
	m_CatBowlAnimator->Play(L"catBowlEmpty", true);
}

/// <summary>
/// ����� �� �ִϸ��̼� ���� ���� (�� ����)
/// </summary>
void GameManagerScript::CatBowlFull(std::any)
{
	m_CatBowlAnimator->Play(L"catBowlFull", true);
}

void GameManagerScript::CheckCornQuest()
{
	int& cornQuestCount = m_QuestManager->m_CornQuestCount;
	if (m_QuestManager->m_IsCornQuestStepUp)
	{
		++(cornQuestCount);

		if (cornQuestCount > 5)
		{
			cornQuestCount = 1;
		}
	}

	if (cornQuestCount < 1 && cornQuestCount > 5)
	{
		assert(nullptr && "������ �ܰ� ��ġ ����");
	}
}

void GameManagerScript::CheckDayProgress(float deltaTime)
{
	if (m_DayCount > DAY_MAX)
	{
		CheckEndingQuarter();
	}

	// TimeBar ���� ��������
	Vector2 timeBarSize = m_TimeBar->GetTimeBarImage()->GetSize();
	const float TIMEBAR_LENGTH = timeBarSize.x;

	// TimeBar ���� ����
	float timePassedRatio = static_cast<float>(m_TimePassed / (m_IsDay ? DAYTIME_LENGTH : NIGHTTIME_LENGTH));
	float timeBarLength = timePassedRatio * TIMEBAR_LENGTH;
	m_TimeBar->GetTimeBarImage()->SetSliceSize(Vector2(timeBarLength, timeBarSize.y));
	D2D1_RECT_F additionalOffset = {};
	additionalOffset.right = -(TIMEBAR_LENGTH - timeBarLength);
	m_TimeBar->GetTimeBarImage()->SetAdditionalOffset(additionalOffset);

	// ��
	if (m_IsDay && isNight == false && isMorning == false)
	{
		// �ð��� �귯��
		m_TimePassed += deltaTime;

		if (m_QuestManager->m_ApplyFarm != true)
		{
			m_QuestManager->m_ApplyFarm = true;
			m_QuestManager->m_FailWildBoar = true;
		}

		// ����Ʈ ����
		if (m_IsCreateQuest)
		{
			m_QuestManager->m_WildBoar->GetObjectChild("wildBoarExclamationMark")->SetActivate(false);
			// ����� Ȱ��ȭ
			if (m_QuestManager->m_SaveCat)
			{
				m_QuestManager->m_CatObject->SetActivate(true);
				/*	dynamic_cast<FarmerScript*>(m_QuestManager->m_Player->GetComponent<MonoBehaviour>())->speed = 500;
					dynamic_cast<FarmerScript*>(m_QuestManager->m_Player->GetComponent<MonoBehaviour>())->diagonalSpeed = 300;*/
			}

			// ��� ���� �ʱ�ȭ 
			InitializeFarmFertilizer();
			// ����� �ʱ�ȭ
			InitializeWildBoar();
			// ���� �Ϸ簡 ������ �� �� ���¸� üũ�ؼ� ������ ������ ����Ѵ�.
			CheckFarmState();
			// ������ ����Ʈ �ܰ踦 üũ�Ѵ�.
			CheckCornQuest();
			// ��� ����Ʈ �������� üũ �� �߻� �ð��� ����
			CheckEmergencyQuest();
			// ����Ʈ ����
			m_QuestManager->CreateQuest(m_DayCount, m_IsDay);
			// ������ �ٲ�� ������ ����� �Ұ���
			m_IsCreateQuest = false;
			// ������ ��� ����
			//m_ItemManager->DeleteAllItem();
			m_QuestManager->DeleteAllItem();
		}

		// ��� ����Ʈ ���� �����ϴٸ�
		if (m_IsCreateEmergencyQuest)
		{
			// ���� �ð��� ��� ����Ʈ ���� �ð��� �Ѵ´ٸ� 
			if (m_TimePassed > m_EmergencyQuestTime)
			{
				// ��� ����Ʈ ����
				m_QuestManager->CreateEmergencyRandomQuest();
				m_IsCreateEmergencyQuest = false;
			}
		}

		// 		/// ų �׽�Ʈ
		// 		if (m_TimePassed > DAYTIME_LENGTH - 20.f)
		// 		{
		// 			ShowKillWindow(3, 5, 1);
		// 		}
				//�������� �Ѿ�� ���� ���� ��ȯ
		if (m_TimePassed > DAYTIME_LENGTH)
		{
			//fade in out �� ���� ����
			isNight = true;
			isMorning = false;

			// ����Ʈ ���� ����
			m_IsCreateQuest = true;

			m_TimePassed = 0.f;

			// �ؽ�Ʈ �� �̹��� ����
			wstring dateText = to_wstring(m_DayCount) + L"���� ��";
			m_TimeBar->GetDateText()->SetText(dateText.c_str());
			m_TimeBar->GetTimeBarImage()->SetImage(
				ResourceManager::GetInstance()->LoadResource<Image>(L"TimeBarImageMoon", L"/UI/timebarMoon.png")->GetImage());
			m_TimeBar->GetTimeIconImage()->SetImage(
				ResourceManager::GetInstance()->LoadResource<Image>(L"TimeBarIconMoon", L"/UI/timeiconMoon.png")->GetImage());
			m_TimeBar->GetTimeIconImage()->SetPositionOffset(Vector2(12.f, 0.f));

			// BGM ���
			SoundManager::GetInstance()->StopSoundBGM();
			SoundManager::GetInstance()->PlaySoundBGM(
				ResourceManager::GetInstance()->LoadResource<Sound>(L"NightBGM", L"/Sound/BGM/NIGHT/Evil4 - Witch's Lair.mp3")->GetSound());
			SoundManager::GetInstance()->PlaySoundSFX(
				ResourceManager::GetInstance()->LoadResource<Sound>(L"NightSFX", L"/Sound/SFX/DayNight/Night_Owl.mp3")->GetSound());
			SoundManager::GetInstance()->SetBGMVolume(m_NightBGMVolume);

			// ���� Ȱ��ȭ�� ����Ʈ�� ��� �Ϸ��ߴ��� üũ
			m_QuestManager->CheckCompleteAllActiveQuests();

		}

	}
	//���̿� ȭ�� ��ȯ
	else if (isMorning == true)
	{
		++(m_QuestManager->m_ActiveQuests.find(999)->second.questStep);

		if (m_QuestManager->m_ActiveQuests.find(999)->second.questStep == m_QuestManager->m_ActiveQuests.find(999)->second.maxQuestStep)
		{
			m_QuestManager->m_ActiveQuests.find(999)->second.isComplete = true;

			m_QuestManager->m_QuestUiList->m_CheckBoxImage[m_QuestManager->m_ActiveQuests.find(999)->second.uiOrder]->SetImage
			(
				ResourceManager::GetInstance()->LoadResource<Image>(L"Check", L"/UI/check.png")->GetImage()
			);
		}
		else
		{
			m_QuestManager->m_ActiveQuests.find(999)->second.objectId = m_QuestManager->m_QuestList[999 + 1].objectId;
		}

		EventManager::GetInstance()->SendEvent("ChangeNight", nullptr);
		timeCount += deltaTime;

		/// �� -> �� ��ȯ ��ũ��

		// ���� ȭ�� ���̵���
		if (timeCount > 0.f && timeCount <= 1.f)
		{
			m_BlackScreenScript->m_BlackScreen->FadeIn(1.5f);
		}

		if (timeCount > 1.7f)
		{
			int npcNo1 = killNPCList[0];
			int npcNo2 = killNPCList[1];
			int npcNo3 = killNPCList[2];
			int npcNo4 = killNPCList[3];

			ShowKillWindow(npcNo1, npcNo2, npcNo3, npcNo4);
		}

		if (timeCount > 2.f)
		{
			// ���� �˷��� �� ��
			wstring textData = L"";

			// �ؽ�Ʈ �߰�
			if (m_DeadNPCIndex != -1) textData += L"���� ��� �ϳ��� �����Ǿ���!";
			if (textData != L"") textData += L"\n";
			if (!m_QuestManager->m_ApplyFarm) textData += L"���������� ���� ���Ĺ��ȴ�...";

			// ���� ȭ�� Ȱ��ȭ
			m_BlackScreenScript->PlayText(true, textData);
		}

		if (timeCount > 5.f)
		{
			// ���� ȭ�� ��Ȱ��ȭ
			m_BlackScreenScript->DisabledBlackScreen();

			isMorning = false;
			timeCount = 0;
			NPCSponCheck();
		}


	}

	else if (isNight == true)
	{
		timeCount += deltaTime;

		/// �� -> �� ��ȯ ��ũ��

		//���� ȭ�� ����
		if (timeCount > 0.f && timeCount <= 1.f)
		{
			m_BlackScreenScript->m_BlackScreen->FadeIn(1.5f);
		}

		if (timeCount > 2.f)
		{
			// �㿡 �˷��� �� ��
			wstring textData = L"";

			// �ؽ�Ʈ �߰�
			if (!m_QuestManager->m_CompleteAllActiveQuests)
				textData += L"�� �� �̿Ϸ�. ���� ���� ��ȭ";
			if (textData != L"") textData += L"\n";
			if (!m_QuestManager->m_SaveCat)
				textData += L"����̰� �׾ ����ء�";

			// ���� ȭ�� Ȱ��ȭ
			m_BlackScreenScript->PlayText(false, textData);
		}

		if (timeCount > 5.f)
		{
			// ���� ȭ�� ��Ȱ��ȭ
			m_BlackScreenScript->DisabledBlackScreen();

			isNight = false;
			m_IsDay = !m_IsDay;
			timeCount = 0;

			//���������� ���� ������ �ӵ� ����
			PolicePatrolLevelCheck();
			//������ ���� ����
			PoliceSponCheck();
		}
	}
	// ��
	else
	{
		EventManager::GetInstance()->SendEvent("ChangeNight", nullptr);
		// �ð��� �귯��
		m_TimePassed += deltaTime;

		// ����Ʈ ����
		if (m_IsCreateQuest)
		{
			m_QuestManager->m_PondObject->GetObjectChild("PondCat")->SetActivate(false);
			//m_QuestManager->m_PondObject->GetObjectChild("pondExclamationMark")->SetActivate(false);

			// ����� ���ϱ� ���ο� ���� Ȱ��ȭ ��Ȱ��ȭ 
			if (m_QuestManager->m_SaveCat == false)
			{
				m_QuestManager->m_SaveCat = true;
				/*dynamic_cast<FarmerScript*>(m_QuestManager->m_Player->GetComponent<MonoBehaviour>())->speed = 400;
				dynamic_cast<FarmerScript*>(m_QuestManager->m_Player->GetComponent<MonoBehaviour>())->diagonalSpeed = 200;*/
			}


			// �� ����Ʈ �߻� �ð�
			CheckNightQuest();
			// ����Ʈ ����
			m_QuestManager->CreateQuest(m_DayCount, m_IsDay);
			// ������ �ٲ�� ������ ����� �Ұ���
			m_IsCreateQuest = false;
			// ������ ��� ����
			// m_ItemManager->DeleteAllItem();
			m_QuestManager->DeleteAllItem();
		}

		// �� ����Ʈ ���� �����ϴٸ�
		if (m_IsCreateNightQuest)
		{
			// ���� �ð��� �� ����Ʈ ���� �ð��� �Ѵ´ٸ� 
			if (m_TimePassed > m_NightQuestTime)
			{
				// �� ����Ʈ ����
				m_QuestManager->CreateNightRandomQuest();
				m_IsCreateNightQuest = false;
			}
		}
		//if (m_TimePassed + 3 > NIGHTTIME_LENGTH)
		//{
		//	//���⼭ ���̵� �� ����?
		//	m_BlackScreen->SetActivate(true);
		//	m_BlackScreen->GetComponent<ImageRenderer>()->FadeIn(2);
		//}

		if (m_TimePassed > NIGHTTIME_LENGTH)
		{
			//fade in out �� ���� ����
			isNight = false;
			isMorning = true;

			// ����Ʈ ���� ����
			m_IsCreateQuest = true;

			m_IsDay = !m_IsDay;
			m_TimePassed = 0.f;
			m_DayCount++;

			if (m_DayCount > DAY_MAX) return;

			m_IsKillAvailable = true;

			// �ؽ�Ʈ �� �̹��� ����
			wstring dateText = to_wstring(m_DayCount) + L"���� ��";
			m_TimeBar->GetDateText()->SetText(dateText.c_str());
			m_TimeBar->GetTimeBarImage()->SetImage(
				ResourceManager::GetInstance()->LoadResource<Image>(L"TimeBarImageSun", L"/UI/timebarSun.png")->GetImage());
			m_TimeBar->GetTimeIconImage()->SetImage(
				ResourceManager::GetInstance()->LoadResource<Image>(L"TimeBarIconSun", L"/UI/timeiconSun.png")->GetImage());
			m_TimeBar->GetTimeIconImage()->SetPositionOffset(Vector2(2.f, 0.f));

			// BGM ���
			SoundManager::GetInstance()->StopSoundBGM();
			SoundManager::GetInstance()->PlaySoundBGM(
				ResourceManager::GetInstance()->LoadResource<Sound>(L"DayBGM", L"/Sound/BGM/DAY/Joy Ride.mp3")->GetSound());
			SoundManager::GetInstance()->PlaySoundSFX(
				ResourceManager::GetInstance()->LoadResource<Sound>(L"DaySFX", L"/Sound/SFX/DayNight/Day_Chicken.mp3")->GetSound());
			SoundManager::GetInstance()->SetBGMVolume(m_DayBGMVolume);

			// ���� ���� ������ �Ͻ����̶�� ���� �ٽ� ���ҽ����ش�.
			if (m_PatrolLevelTemporary)
			{
				EventManager::GetInstance()->SendEvent("PatrolLevelDown", nullptr);
				m_PatrolLevelTemporary = false;
			}
		}


	}
}

/// <summary>
/// ��� ����Ʈ ���� ���θ� üũ�Ѵ�.
/// </summary>
void GameManagerScript::CheckEmergencyQuest()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(30, 60);

	int randomNumber = dis(gen);

	// 30 ~ 60 �����߿� 39 �̻��̶�� ��� ����Ʈ ����  (70�� Ȯ��)
	if (randomNumber > 39)
	{
		m_IsCreateEmergencyQuest = true;
	}

	////�׽�Ʈ
	/*std::uniform_int_distribution<int> dis(1, 2);
	int randomNumber = dis(gen);
	m_IsCreateEmergencyQuest = true;*/

	//��� ����Ʈ �߻� �ð�
	m_EmergencyQuestTime = randomNumber;
}

/// <summary>
/// �� ����Ʈ ���� ���θ� üũ�Ѵ�.
/// </summary>
void GameManagerScript::CheckNightQuest()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(10, 20);

	//// �׽�Ʈ
	//std::uniform_int_distribution<int> dis(1, 2);

	m_IsCreateNightQuest = true;
	// �� ����Ʈ �߻� �ð�
	m_NightQuestTime = dis(gen);
}

void GameManagerScript::CheckFarmState()
{
	for (auto& farmId : m_FarmScripts)
	{
		if (farmId.second->completeQuestCount == 2)
		{
			string farmState = farmId.second->detailState;

			if (farmState == "NoSeed")
			{
				farmId.second->detailState = "Seed";
			}
			else if (farmState == "Seed")
			{
				if (farmId.second->fakeSeed)
				{
					farmId.second->fakeSeed = false;
				}
				else
				{
					farmId.second->detailState = "First";
				}
			}
			else if (farmState == "First")
			{
				farmId.second->detailState = "Second";
			}
			else if (farmState == "Second")
			{
				farmId.second->detailState = "Third";
			}
		}
		else if (farmId.second->completeQuestCount == 1)
		{
			if (farmId.second->fakeSeed)
			{
				farmId.second->detailState = "NoSeed";
				farmId.second->fakeSeed = false;
			}
		}


		if (farmId.second->farmState == "FarmWater")
		{
			farmId.second->farmState = "FarmNoWater";
		}

		farmId.second->completeQuestCount = 0;
		farmId.second->isInteraction = false;
	}
}

void GameManagerScript::InitializeFarmFertilizer()
{
	for (auto& farmId : m_FarmScripts)
	{
		int index = farmId.second->farmNumber;
		for (auto& farmChild : farmId.second->GetGameObject()->GetObjectChildren())
		{
			if (farmChild->GetName() == "FirstFarmChildFertilizer" + to_string(index) || farmChild->GetName() == "SecondFarmChildFertilizer" + to_string(index))
			{
				if (farmChild->GetActivate())
				{
					farmChild->SetActivate(false);
				}
			}
		}
	}
}

void GameManagerScript::InitializeWildBoar()
{
	for (auto& object : m_QuestManager->m_QuestObjects)
	{
		if (object->GetObjectType() == OBJECT_TYPE::WILDBOAR)
		{
			if (object->GetObjectParent() != nullptr)
			{
				object->GetObjectParent()->GetTransform()->SetLocalPosition(Vector2{ -3000.f, -150.f } + Vector2{ 960.f, 540.f });
				object->GetObjectParent()->SetActivate(false);
				return;
			}
			else
			{
				object->GetTransform()->SetLocalPosition(Vector2{ -2000.f, -150.f } + Vector2{ 960.f, 540.f });
				object->SetActivate(false);
				return;
			}
		}
	}
}

/// <summary>
/// �Լ�ȭ �ؾ��ϴµ� �Ӹ��� �ȵ��ư���. ������ ���������� �ٽ� ����
/// </summary>
void GameManagerScript::DestoryFarm()
{
	if ((m_FarmScripts.find(2))->second->isDestroyFarm == false)
	{
		(m_FarmScripts.find(1))->second->isDestroyFarm = true;
		(m_FarmScripts.find(1))->second->detailState = "NoSeed";

		int index = (m_FarmScripts.find(1))->second->farmNumber;
		for (auto& farmChild : (m_FarmScripts.find(1))->second->GetGameObject()->GetObjectChildren())
		{
			if (farmChild->GetName() == "FirstFarmChildPanel" + to_string(index))
			{
				farmChild->SetActivate(true);
			}
		}

		(m_FarmScripts.find(2))->second->isDestroyFarm = true;
		(m_FarmScripts.find(2))->second->detailState = "NoSeed";

		index = (m_FarmScripts.find(2))->second->farmNumber;
		for (auto& farmChild : (m_FarmScripts.find(2))->second->GetGameObject()->GetObjectChildren())
		{
			if (farmChild->GetName() == "FirstFarmChildPanel" + to_string(index))
			{
				farmChild->SetActivate(true);
			}
		}

		(m_FarmScripts.find(3))->second->isDestroyFarm = true;
		(m_FarmScripts.find(3))->second->detailState = "NoSeed";

		index = (m_FarmScripts.find(3))->second->farmNumber;
		for (auto& farmChild : (m_FarmScripts.find(3))->second->GetGameObject()->GetObjectChildren())
		{
			if (farmChild->GetName() == "FirstFarmChildPanel" + to_string(index))
			{
				farmChild->SetActivate(true);
			}
		}
	}
	else if ((m_FarmScripts.find(5))->second->isDestroyFarm == false)
	{
		(m_FarmScripts.find(4))->second->isDestroyFarm = true;
		(m_FarmScripts.find(4))->second->detailState = "NoSeed";

		int index = (m_FarmScripts.find(4))->second->farmNumber;
		for (auto& farmChild : (m_FarmScripts.find(4))->second->GetGameObject()->GetObjectChildren())
		{
			if (farmChild->GetName() == "FirstFarmChildPanel" + to_string(index))
			{
				farmChild->SetActivate(true);
			}
		}

		(m_FarmScripts.find(5))->second->isDestroyFarm = true;
		(m_FarmScripts.find(5))->second->detailState = "NoSeed";

		index = (m_FarmScripts.find(5))->second->farmNumber;
		for (auto& farmChild : (m_FarmScripts.find(5))->second->GetGameObject()->GetObjectChildren())
		{
			if (farmChild->GetName() == "FirstFarmChildPanel" + to_string(index))
			{
				farmChild->SetActivate(true);
			}
		}

		(m_FarmScripts.find(6))->second->isDestroyFarm = true;
		(m_FarmScripts.find(6))->second->detailState = "NoSeed";

		index = (m_FarmScripts.find(6))->second->farmNumber;
		for (auto& farmChild : (m_FarmScripts.find(6))->second->GetGameObject()->GetObjectChildren())
		{
			if (farmChild->GetName() == "FirstFarmChildPanel" + to_string(index))
			{
				farmChild->SetActivate(true);
			}
		}

	}
	else if ((m_FarmScripts.find(8))->second->isDestroyFarm == false)
	{
		(m_FarmScripts.find(7))->second->isDestroyFarm = true;
		(m_FarmScripts.find(7))->second->detailState = "NoSeed";

		int index = (m_FarmScripts.find(7))->second->farmNumber;
		for (auto& farmChild : (m_FarmScripts.find(7))->second->GetGameObject()->GetObjectChildren())
		{
			if (farmChild->GetName() == "FirstFarmChildPanel" + to_string(index))
			{
				farmChild->SetActivate(true);
			}
		}

		(m_FarmScripts.find(8))->second->isDestroyFarm = true;
		(m_FarmScripts.find(8))->second->detailState = "NoSeed";

		index = (m_FarmScripts.find(8))->second->farmNumber;
		for (auto& farmChild : (m_FarmScripts.find(8))->second->GetGameObject()->GetObjectChildren())
		{
			if (farmChild->GetName() == "FirstFarmChildPanel" + to_string(index))
			{
				farmChild->SetActivate(true);
			}
		}

		(m_FarmScripts.find(9))->second->isDestroyFarm = true;
		(m_FarmScripts.find(9))->second->detailState = "NoSeed";

		index = (m_FarmScripts.find(9))->second->farmNumber;
		for (auto& farmChild : (m_FarmScripts.find(9))->second->GetGameObject()->GetObjectChildren())
		{
			if (farmChild->GetName() == "FirstFarmChildPanel" + to_string(index))
			{
				farmChild->SetActivate(true);
			}
		}

	}
	else if ((m_FarmScripts.find(11))->second->isDestroyFarm == false)
	{
		(m_FarmScripts.find(10))->second->isDestroyFarm = true;
		(m_FarmScripts.find(10))->second->detailState = "NoSeed";

		int index = (m_FarmScripts.find(10))->second->farmNumber;
		for (auto& farmChild : (m_FarmScripts.find(10))->second->GetGameObject()->GetObjectChildren())
		{
			if (farmChild->GetName() == "SecondFarmChildPanel" + to_string(index))
			{
				farmChild->SetActivate(true);
			}
		}

		(m_FarmScripts.find(11))->second->isDestroyFarm = true;
		(m_FarmScripts.find(11))->second->detailState = "NoSeed";

		index = (m_FarmScripts.find(11))->second->farmNumber;
		for (auto& farmChild : (m_FarmScripts.find(11))->second->GetGameObject()->GetObjectChildren())
		{
			if (farmChild->GetName() == "SecondFarmChildPanel" + to_string(index))
			{
				farmChild->SetActivate(true);
			}
		}

		(m_FarmScripts.find(12))->second->isDestroyFarm = true;
		(m_FarmScripts.find(12))->second->detailState = "NoSeed";

		index = (m_FarmScripts.find(12))->second->farmNumber;
		for (auto& farmChild : (m_FarmScripts.find(12))->second->GetGameObject()->GetObjectChildren())
		{
			if (farmChild->GetName() == "SecondFarmChildPanel" + to_string(index))
			{
				farmChild->SetActivate(true);
			}
		}

	}
	else if ((m_FarmScripts.find(14))->second->isDestroyFarm == false)
	{
		(m_FarmScripts.find(13))->second->isDestroyFarm = true;
		(m_FarmScripts.find(13))->second->detailState = "NoSeed";

		int index = (m_FarmScripts.find(13))->second->farmNumber;
		for (auto& farmChild : (m_FarmScripts.find(13))->second->GetGameObject()->GetObjectChildren())
		{
			if (farmChild->GetName() == "SecondFarmChildPanel" + to_string(index))
			{
				farmChild->SetActivate(true);
			}
		}

		(m_FarmScripts.find(14))->second->isDestroyFarm = true;
		(m_FarmScripts.find(14))->second->detailState = "NoSeed";

		index = (m_FarmScripts.find(14))->second->farmNumber;
		for (auto& farmChild : (m_FarmScripts.find(14))->second->GetGameObject()->GetObjectChildren())
		{
			if (farmChild->GetName() == "SecondFarmChildPanel" + to_string(index))
			{
				farmChild->SetActivate(true);
			}
		}

		(m_FarmScripts.find(15))->second->isDestroyFarm = true;
		(m_FarmScripts.find(15))->second->detailState = "NoSeed";

		index = (m_FarmScripts.find(15))->second->farmNumber;
		for (auto& farmChild : (m_FarmScripts.find(15))->second->GetGameObject()->GetObjectChildren())
		{
			if (farmChild->GetName() == "SecondFarmChildPanel" + to_string(index))
			{
				farmChild->SetActivate(true);
			}
		}

	}
	else if ((m_FarmScripts.find(17))->second->isDestroyFarm == false)
	{
		(m_FarmScripts.find(16))->second->isDestroyFarm = true;
		(m_FarmScripts.find(16))->second->detailState = "NoSeed";

		int index = (m_FarmScripts.find(16))->second->farmNumber;
		for (auto& farmChild : (m_FarmScripts.find(16))->second->GetGameObject()->GetObjectChildren())
		{
			if (farmChild->GetName() == "SecondFarmChildPanel" + to_string(index))
			{
				farmChild->SetActivate(true);
			}
		}

		(m_FarmScripts.find(17))->second->isDestroyFarm = true;
		(m_FarmScripts.find(17))->second->detailState = "NoSeed";

		index = (m_FarmScripts.find(17))->second->farmNumber;
		for (auto& farmChild : (m_FarmScripts.find(17))->second->GetGameObject()->GetObjectChildren())
		{
			if (farmChild->GetName() == "SecondFarmChildPanel" + to_string(index))
			{
				farmChild->SetActivate(true);
			}
		}

		(m_FarmScripts.find(18))->second->isDestroyFarm = true;
		(m_FarmScripts.find(18))->second->detailState = "NoSeed";

		index = (m_FarmScripts.find(18))->second->farmNumber;
		for (auto& farmChild : (m_FarmScripts.find(18))->second->GetGameObject()->GetObjectChildren())
		{
			if (farmChild->GetName() == "SecondFarmChildPanel" + to_string(index))
			{
				farmChild->SetActivate(true);
			}
		}
	}
}

/// <summary>
/// ���� �б� üũ
/// </summary>
void GameManagerScript::CheckEndingQuarter()
{
	for (auto& item : m_ItemManager->m_ItemList)
	{
		if (item != nullptr)
		{
			if (item->m_ItemKeyName == L"corn")
			{
				if (item->m_ItemCount >= 9)
				{
					/// �� ���� (������ 9�� �̻� ����)
					GameplayStatics::LoadScene("TrueEnding");
					return;
				}
			}
		}
	}

	GameplayStatics::LoadScene("ArrestedEnding");
	return;
}

void GameManagerScript::ShowKillWindow(int npc1, int npc2, int npc3, int npc4 /*= 999*/)
{
	if (m_IsKillAvailable)
	{
		KillWindow* killWindow = new KillWindow(m_DayCount, npc1, npc2, npc3, npc4);
		GameplayStatics::CreateObject(killWindow);
		GameplayStatics::PauseGame();
		m_IsKillAvailable = false;
	}
}

void GameManagerScript::GetKillNpcIndex(std::any index)
{
	m_DeadNPCIndex = std::any_cast<int>(index);
	GameplayStatics::ResumeGame();
}

void GameManagerScript::CheckGameOver()
{
	if (!m_IsGameOver) return;

	if (!m_ShowGameOverWindow)
	{
		GameOverWindow* gameoverWindow = new GameOverWindow();
		GameplayStatics::CreateObject(gameoverWindow);
		m_ShowGameOverWindow = true;

		GameplayStatics::PauseGame();
	}
}

void GameManagerScript::NPCSponCheck()
{
	if (m_DayCount == 2) //1���� ��
	{
		killNPCList.clear();

		if (m_DeadNPCIndex != -1)
		{
			NPCKillCheck(m_DeadNPCIndex);
			killNPCList.push_back(999);
			EventManager::GetInstance()->SendEvent("PatrolLevelUp", nullptr);
		}

		MakeNPCKillList();

	}

	else if (m_DayCount == 3) //2���� ��
	{
		//1������ ����� �׿��� ���,
		if (killNPCList[0] == 999)
			m_NPC3Script->isDead = false;

		killNPCList.clear();

		if (m_DeadNPCIndex != -1)
		{
			NPCKillCheck(m_DeadNPCIndex);
			killNPCList.push_back(999);
			EventManager::GetInstance()->SendEvent("PatrolLevelUp", nullptr);
		}

		MakeNPCKillList();

	}

	else if (m_DayCount == 4) //3���� ��
	{
		//2������ ����� �׿��� ���,
		if (killNPCList[0] == 999)
			m_NPC4Script->isDead = false;

		killNPCList.clear();

		if (m_DeadNPCIndex != -1)
		{
			NPCKillCheck(m_DeadNPCIndex);
			killNPCList.push_back(999);
			EventManager::GetInstance()->SendEvent("PatrolLevelUp", nullptr);
		}

		MakeNPCKillList();
	}

	else if (m_DayCount == 5) //4���� ��
	{

		//3������ ����� �׿��� ���,
		if (killNPCList[0] == 999)
			m_NPC1Script->isDead = false;

		killNPCList.clear();

		if (m_DeadNPCIndex != -1)
		{
			NPCKillCheck(m_DeadNPCIndex);
			killNPCList.push_back(999);
			EventManager::GetInstance()->SendEvent("PatrolLevelUp", nullptr);
		}
		MakeNPCKillList();
	}

	else if (m_DayCount == 5)
	{

		if (m_DeadNPCIndex != -1)
		{
			NPCKillCheck(m_DeadNPCIndex);
			killNPCList.clear();
			MakeNPCKillList();
			killNPCList.push_back(999);
			EventManager::GetInstance()->SendEvent("PatrolLevelUp", nullptr);
		}
	}
}

void GameManagerScript::MakeNPCKillList()
{
	if (m_NPC1Script->isDead == false)
		killNPCList.push_back(1);

	if (m_NPC2Script->isDead == false)
		killNPCList.push_back(2);

	if (m_NPC3Script->isDead == false)
		killNPCList.push_back(3);

	if (m_NPC4Script->isDead == false)
		killNPCList.push_back(4);

	if (m_NPC5Script->isDead == false)
		killNPCList.push_back(5);

	if (m_NPC6Script->isDead == false)
		killNPCList.push_back(6);

}

void GameManagerScript::NPCKillCheck(int NPCIndex)
{
	if (NPCIndex == 1)
		m_NPC1Script->isDead = true;
	else if (NPCIndex == 2)
		m_NPC2Script->isDead = true;
	else if (NPCIndex == 3)
		m_NPC3Script->isDead = true;
	else if (NPCIndex == 4)
		m_NPC4Script->isDead = true;
	else if (NPCIndex == 5)
		m_NPC5Script->isDead = true;
	else if (NPCIndex == 6)
		m_NPC6Script->isDead = true;
}

void GameManagerScript::PoliceSponCheck()
{

	switch (m_DayCount)
	{
		case 1:
			m_Police1Script->isDead = true;
			m_Police2Script->isDead = true;
			m_Police3Script->isDead = false;
			m_Police4Script->isDead = true;
			break;

		case 2:
			m_Police1Script->isDead = false;
			m_Police2Script->isDead = true;
			m_Police3Script->isDead = false;
			m_Police4Script->isDead = true;
			break;

		case 3:
			m_Police1Script->isDead = false;
			m_Police2Script->isDead = true;
			m_Police3Script->isDead = false;
			m_Police4Script->isDead = true;
			break;

		case 4:
			m_Police1Script->isDead = false;
			m_Police2Script->isDead = true;
			m_Police3Script->isDead = false;
			m_Police4Script->isDead = false;
			break;

		case 5:
			m_Police1Script->isDead = false;
			m_Police2Script->isDead = false;
			m_Police3Script->isDead = false;
			m_Police4Script->isDead = false;
			break;
	}
}

/// <summary>
/// �÷��̾ ����Ʈ���� ������Ʈ�� ��ȣ�ۿ� �ϱ�����
/// ������Ʈ ���̵� ���� Ȱ���Ͽ� ����Ʈ�� üũ�Ѵ�.
/// 
/// --> ����Ʈ �Ŵ������� �� ���¸� �˾ƾ� �ϱ⶧���� id ���� �ƴ� ������Ʈ ��ü�� ������� �ߴ�..
/// ������ ���� ���¸�, �������ϰ� �����ϱ� ������ �̺�Ʈó���� �� �����ܰ踦 ��ġ�°ͺ��ٴ� ����������? �� �𸣰ڴ�
/// 
/// </summary>
/// <param name="scanObject"></param>
void GameManagerScript::Interaction(GameObject* scanObject)
{
	int scanObejectId = dynamic_cast<ObjectDataScript*>(scanObject->GetComponent<MonoBehaviour>())->id;
	m_QuestManager->CheckQuest(scanObejectId, scanObject);
}


/// <summary>
/// ����Ʈ
/// </summary>
/// <param name="deltaTime"></param>
void GameManagerScript::TimeEffectManagement(float deltaTime)
{
	if ((isNight == false && isMorning == false))
	{
		m_DayPassed += deltaTime;
		m_EffectTimePassed += deltaTime;
	}

	if (m_DayPassed > DAY_TOTAL_TIME) m_DayPassed = 0.f;

	// �ð��� ����
	if (m_DayPassed >= 0.f) m_CurrentTimeZone = DAY_NIGHT::MORNING_EARLY;																	// 0.f
	if (m_DayPassed >= DAYTIME_LENGTH / 4) m_CurrentTimeZone = DAY_NIGHT::MORNING_LATE;														// 22.5f
	if (m_DayPassed >= DAYTIME_LENGTH / 2) m_CurrentTimeZone = DAY_NIGHT::AFTERNOON_EARLY;													// 45.f
	if (m_DayPassed >= DAYTIME_LENGTH / 2 + DAYTIME_LENGTH / 4) m_CurrentTimeZone = DAY_NIGHT::AFTERNOON_LATE;								// 67.5f

	if (m_DayPassed >= DAYTIME_LENGTH) m_CurrentTimeZone = DAY_NIGHT::EVENING_EARLY;														// 90.f
	if (m_DayPassed >= DAYTIME_LENGTH + NIGHTTIME_LENGTH / 4) m_CurrentTimeZone = DAY_NIGHT::EVENING_LATE;									// 112.5f
	if (m_DayPassed >= DAYTIME_LENGTH + NIGHTTIME_LENGTH / 2) m_CurrentTimeZone = DAY_NIGHT::LATENIGHT_EARLY;								// 135.f
	if (m_DayPassed >= DAYTIME_LENGTH + NIGHTTIME_LENGTH / 2 + NIGHTTIME_LENGTH / 4) m_CurrentTimeZone = DAY_NIGHT::LATENIGHT_LATE;		// 157.5f

	if (m_DayPassed >= DAYTIME_LENGTH + NIGHTTIME_LENGTH) m_CurrentTimeZone = DAY_NIGHT::MORNING_EARLY;										// 180.f

	// ���ص� �ֱ���� ����
	if (m_EffectTimePassed > EFFECT_CHANGE_LENGTH)
	{
		// ����Ʈ ����
		switch (m_CurrentTimeZone)
		{
			case DAY_NIGHT::MORNING_EARLY:
			{
				if (m_Brightness > 0.f) m_Brightness -= m_BrightnessValue * m_EffectTimePassed;
				if (m_Temperature < 0.f) m_Temperature += m_TemperatureValue * m_EffectTimePassed;
				if (m_ColorTint < 0.f) m_ColorTint += m_ColorTintValue * m_EffectTimePassed;
			}
			break;
			case DAY_NIGHT::MORNING_LATE:
			{
				// �µ� �ø���
				if (m_Temperature < 0.8f) m_Temperature += m_TemperatureValue * m_EffectTimePassed;
			}
			break;
			case DAY_NIGHT::AFTERNOON_EARLY:
			{
				// �µ�, ���� �ø���
				if (m_Temperature < 0.8f) m_Temperature += m_TemperatureValue * m_EffectTimePassed * 2;
				if (m_ColorTint < 0.5f) m_ColorTint += m_ColorTintValue * m_EffectTimePassed;
			}
			break;
			case DAY_NIGHT::AFTERNOON_LATE:
			{
				// ��ο����� ����
				if (m_Brightness < 0.06f) m_Brightness += m_BrightnessValue * m_EffectTimePassed;

				// ���� �ø���
				if (m_ColorTint < 0.5f) m_ColorTint += m_ColorTintValue * m_EffectTimePassed;
				if (m_Temperature > -0.8f) m_Temperature -= m_TemperatureValue * m_EffectTimePassed * 2;
			}
			break;
			case DAY_NIGHT::EVENING_EARLY:
			{
				if (m_Brightness < 0.06f) m_Brightness += m_BrightnessValue * m_EffectTimePassed;

				// �µ� ������
				if (m_Temperature > -0.8f) m_Temperature -= m_TemperatureValue * m_EffectTimePassed * 2;
			}
			break;
			case DAY_NIGHT::EVENING_LATE:
			{
				// �µ� ������
				if (m_Temperature > -0.8f) m_Temperature -= m_TemperatureValue * m_EffectTimePassed;
			}
			break;
			case DAY_NIGHT::LATENIGHT_EARLY:
			{
				if (m_Temperature < 0.8f) m_Temperature += m_TemperatureValue * m_EffectTimePassed;
				if (m_ColorTint > 0.f) m_ColorTint -= m_ColorTintValue * m_EffectTimePassed;
			}
			break;
			case DAY_NIGHT::LATENIGHT_LATE:
			{
				// ������� ����
				if (m_Brightness > 0.f) m_Brightness -= m_BrightnessValue * m_EffectTimePassed;

				// �µ� �ø���, ���� ������
				if (m_Temperature < 0.8f) m_Temperature += m_TemperatureValue * m_EffectTimePassed * 2;
				if (m_ColorTint > 0.f) m_ColorTint -= m_ColorTintValue * m_EffectTimePassed;
			}
			break;
			default:
				break;
		}

		m_EffectTimePassed = 0.f;
		m_EffectManager->ApplyTimeFlowEffect(m_Temperature, m_ColorTint, m_Brightness);

		// 		OutputDebugString(TEXT("Brightness: "));
		// 		OutputDebugString(to_wstring(m_Brightness).c_str());
		// 		OutputDebugString(TEXT("\n"));
	}

}