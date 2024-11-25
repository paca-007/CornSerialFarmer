/// [2023/8/14] �ڿ���
/// --------------------------
/// ��ü���� ���� �帧�� �����ϴ� ���� �Ŵ���
/// ���� �帧�� �����ִ� UI�� �Բ� �����Ѵ�
/// 
/// [2023/08/18] ������ 
/// -----------------------
/// ������ ���η����� ���� ����

#pragma once

#include "MonoBehaviour.h"

#include <map>
#include <vector>
using std::vector;

class TimeBar;
class QuestManagerScript;
class ItemManagerScript;
class EffectManagerScript;
class GameObject;
class ImageRenderer;
class Animator;
class FarmScript;
class NPC1Script;
class NPC2Script;
class PoliceScript;
class Police2Script;
class BlackScreenScript;

enum class ITEM_TYPE;

enum class DAY_NIGHT
{
    MORNING_EARLY,
    MORNING_LATE,
    AFTERNOON_EARLY,
    AFTERNOON_LATE,
    EVENING_EARLY,
    EVENING_LATE,
    LATENIGHT_EARLY,
    LATENIGHT_LATE,
};

class GameManagerScript : public MonoBehaviour
{
public:
    GameManagerScript();
    ~GameManagerScript();

public:
    virtual void Start();
    virtual void Update(float deltaTime);
    virtual void Finalize();

public:
    // �̺�Ʈ

    // ����
    void PatrolLevelUp(std::any); // ���� ���� ����
    void PatrolLevelDown(std::any); // ���� ���� ����
    void PatrolLevelTemporary(std::any); // ���� ���� ������ �Ͻ����ΰ�
    void PolicePatrolLevelCheck();

    //�˶� ������ �̺�Ʈ
    void alarmAlert(std::any);

    // �����
    void CatBowlEmpty(std::any);
    void CatBowlFull(std::any);
    
public:
    // ��¥ ����
    void CheckDayProgress(float deltaTime);
    
    // ����Ʈ
    void TimeEffectManagement(float deltaTime);

    /// ����Ʈ
    // ��� ����Ʈ üũ
    void CheckEmergencyQuest();
    // �� ����Ʈ üũ
    void CheckNightQuest();
    // �÷��̾� ��ȣ �ۿ�
    void Interaction(GameObject* scanObject);

    /// �� (������)
    // ������ ����Ʈ ����
    void CheckCornQuest();
    // �� ���� üũ
    void CheckFarmState();
    // �� ��� �̹��� �ʱ�ȭ
    void InitializeFarmFertilizer();

    /// �����
    void InitializeWildBoar();
    void DestoryFarm();

    /// �����б�
    void CheckEndingQuarter();

    /// ���ϱ�?
    void ShowKillWindow(int npc1, int npc2, int npc3, int npc4 = 999);
    void GetKillNpcIndex(std::any index);

    /// ���ӿ���
    void CheckGameOver();

    ///NPC ����
    void NPCSponCheck();
    void MakeNPCKillList();
    void NPCKillCheck(int NPCIndex);
    
    void PoliceSponCheck();

public:
    /// ���� ���� 

    // ��, ��, ���� �÷��� ��¥ ���� ����
    const float DAYTIME_LENGTH = 10.f;
    const float NIGHTTIME_LENGTH = 10.f;


    const int DAY_MAX = 5;
    
    // ���� ������ ��/���� �����ϴ� ����
    int m_DayCount;
    bool m_IsDay;
    // �ð��� �帧�� �˻�
    double m_TimePassed;
    // �ð� ���� UI
    TimeBar* m_TimeBar;

    ///NPC����
    // ��ü�� ��� �ִ���
    Vector2 m_DeadBodySpot;
    //spon�� ���� script
    NPC1Script* m_NPC1Script;
    NPC2Script* m_NPC2Script;
    NPC1Script* m_NPC3Script;
    NPC1Script* m_NPC4Script;
    NPC1Script* m_NPC5Script;
    NPC2Script* m_NPC6Script;
    PoliceScript* m_Police1Script;
    PoliceScript* m_Police2Script;
    Police2Script* m_Police3Script;
    Police2Script* m_Police4Script;
    vector<int> killNPCList;

    /// ����
    float m_DayBGMVolume;
    float m_NightBGMVolume;

    /// ����Ʈ
    // ����Ʈ ���� ��������
    bool m_IsCreateQuest;
    // ��� ����Ʈ ���� ��������
    bool m_IsCreateEmergencyQuest;
    // ��� ����Ʈ ���� �ð�
    int m_EmergencyQuestTime;
    // �� ����Ʈ ���� �������� üũ
    bool m_IsCreateNightQuest;
    // �� ����Ʈ ���� �ð�
    int m_NightQuestTime;

    /// ����
    // ���� ������ �Ͻ����̿��°�
    bool m_PatrolLevelTemporary;
    // ���� ���� 
    int m_PatrolLevel; 
   
    /// �����
    Animator* m_CatBowlAnimator;

    /// ��
  /*  vector<FarmScript*> m_FarmScripts;*/
    std::map<int, FarmScript*> m_FarmScripts;

    /// ������ �ε���
    bool m_IsKillAvailable = true;
    int m_DeadNPCIndex = -1;


    /// ����Ʈ

    // ����Ʈ �ֱ� ����
    const float EFFECT_CHANGE_LENGTH = 0.1f;
    float m_EffectTimePassed;

    // ����Ʈ�� ���� �ð��� ������
    float DAY_TOTAL_TIME;
    DAY_NIGHT m_CurrentTimeZone;
    float m_DayPassed;

    // ����Ʈ ��ġ
    float m_Temperature;
    float m_ColorTint;

    float m_TemperatureValue;
    float m_ColorTintValue;

    float m_Brightness;
    float m_BrightnessValue;

    //ȭ����ȯ �� Fade In out
    BlackScreenScript* m_BlackScreenScript;
    bool isNight;
    bool isMorning;
    float timeCount;

    /// ���� ����
    bool m_IsGameOver;
    bool m_ShowGameOverWindow;

    /// �Ŵ���
    ItemManagerScript* m_ItemManager;
    QuestManagerScript* m_QuestManager;
    EffectManagerScript* m_EffectManager;
};


