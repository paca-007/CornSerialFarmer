#include "ItemManagerScript.h"

#include <cassert>

#include "ResourceManager.h"
#include "Image.h"
#include "Font.h"

#include "Input.h"
#include "ItemSlot.h"
#include "ImageRenderer.h"
#include "TextRenderer.h"

// Sound
#include "ResourceManager.h"
#include "SoundManager.h"
#include "Sound.h"

Item::Item(std::wstring itemKeyName, std::wstring itemName, ITEM_TYPE itemType, bool isCountable, int itemCount)
	: m_ItemKeyName(itemKeyName)
	, m_ItemName(itemName)
	, m_ItemType(itemType)
	, m_IsCountable(isCountable)
	, m_ItemCount(itemCount)
	, m_ItemFocus(false)
{

}

Item::~Item()
{

}

ItemManagerScript::ItemManagerScript()
	: MonoBehaviour(typeid(this).name())
	, m_ItemList{}
	, m_ItemSlot(nullptr)
	, m_CurrentFocusItemIndex(0)
	, m_ChangeFocus(false)
{

}

ItemManagerScript::~ItemManagerScript()
{

}

void ItemManagerScript::Start()
{
	// �ϴ� �׽�Ʈ������ ������ ����
// 	Item* ax = new Item(L"ax", L"����", ITEM_TYPE::AX, false);
// 	Item* bag = new Item(L"bag", L"�� �ڷ�", ITEM_TYPE::BAG, false);
// 	Item* catfood = new Item(L"catfood", L"�������", ITEM_TYPE::CATFOOD, false);
// 	Item* corn = new Item(L"corn", L"������", ITEM_TYPE::CORN, true, 26);
// 	Item* firewood = new Item(L"firewood", L"����", ITEM_TYPE::FIREWOOD, true, 8);
// 	Item* fertilizer = new Item(L"fertilizer", L"�������", ITEM_TYPE::FERTILIZER, true, 3);
// 	Item* seed = new Item(L"seed", L"����", ITEM_TYPE::SEED, true, 13);
// 	Item* watering = new Item(L"watering", L"���Ѹ���", ITEM_TYPE::WATERING, false);
// 	Item* emptyWatering = new Item(L"emptyWatering", L"���Ѹ���", ITEM_TYPE::EMPTYWATERING, false);

	// SLOTCOUNT ��ŭ�� ũ��, nullptr�� �ʱ�ȭ
	m_ItemList.assign(SLOTCOUNT, nullptr);

	ApplyItemSlot();
}

void ItemManagerScript::Update(float deltaTime)
{
	FindFocusItem();
}

void ItemManagerScript::Finalize()
{

}

/// <summary>
/// ��Ŀ���� ������ ã�� �Լ�
/// </summary>
void ItemManagerScript::FindFocusItem()
{

	if (Input::GetKeyDown(KEY::NUM1)) { m_ChangeFocus = true; m_CurrentFocusItemIndex = 0; }
	else if (Input::GetKeyDown(KEY::NUM2)) { m_ChangeFocus = true; m_CurrentFocusItemIndex = 1; }
	else if (Input::GetKeyDown(KEY::NUM3)) { m_ChangeFocus = true; m_CurrentFocusItemIndex = 2; }
	else if (Input::GetKeyDown(KEY::NUM4)) { m_ChangeFocus = true; m_CurrentFocusItemIndex = 3; }
	else if (Input::GetKeyDown(KEY::NUM5)) { m_ChangeFocus = true; m_CurrentFocusItemIndex = 4; }

	if (!m_ChangeFocus) return;

	m_ChangeFocus = false;
	SoundManager::GetInstance()->PlaySoundSFX(
		ResourceManager::GetInstance()->LoadResource<Sound>(L"ScrollSFX", L"/Sound/SFX/Button/Button_Scroll.mp3")->GetSound());

	for (int item = 0; item < SLOTCOUNT; item++)
	{
		if (!m_ItemList[item]) continue;

		if (item == m_CurrentFocusItemIndex)
			m_ItemList[item]->m_ItemFocus = true;
		else m_ItemList[item]->m_ItemFocus = false;
	}

	ApplyItemSlot();
} 

/// <summary> 
/// ������ ������ UI�� �����ϴ� �Լ�
/// ��Ŀ���� ����ǰų� ������ ���� �̺�Ʈ�� �߻��� ������ ȣ��������
/// </summary>
void ItemManagerScript::ApplyItemSlot()
{
	for (int currentItemSlot = 0; currentItemSlot < SLOTCOUNT; currentItemSlot++)
	{
		/// ��Ŀ�� ������ ���� ������ ���� �̹��� ��� ---------------------------------------------------------

 		std::wstring ResourceKeyName = L"ItemSlot";
		std::wstring ResourcePath = L"/UI/itemslot";
		std::wstring ItemCount = L"";
		std::wstring ItemName = L"";

		ResourceKeyName += currentItemSlot == m_CurrentFocusItemIndex ? L"Focus" : L"";
		ResourcePath += currentItemSlot == m_CurrentFocusItemIndex ? L"Focus.png" : L".png";

		m_ItemSlot->m_ItemSlotImage[currentItemSlot]->SetImage(
			ResourceManager::GetInstance()->LoadResource<Image>(ResourceKeyName.c_str(), ResourcePath.c_str())->GetImage());

		/// �������� ���� ��� ------------------------------------------------------------------------------

		if (!m_ItemList[currentItemSlot])
		{
			m_ItemSlot->m_ItemNameBox[currentItemSlot]->SetImage(
				ResourceManager::GetInstance()->LoadResource<Image>(L"NameBox", L"/UI/namebox.png")->GetImage());
			m_ItemSlot->m_ItemImage[currentItemSlot]->SetImage(
				ResourceManager::GetInstance()->LoadResource<Image>(L"Empty", L"/Image/item/none.png")->GetImage());
			m_ItemSlot->m_ItemCountText[currentItemSlot]->SetText(L"");
			m_ItemSlot->m_ItemNameBox[currentItemSlot]->SetImage(
				ResourceManager::GetInstance()->LoadResource<Image>(L"NameBox", L"/UI/namebox.png")->GetImage());
			m_ItemSlot->m_ItemNameText[currentItemSlot]->SetText(L"");

			continue;
		}

		/// �������� ���� ��� ------------------------------------------------------------------------------

		// �� �� �ִ� �������� ��� ������ ���� ����
		if (m_ItemList[currentItemSlot]->m_IsCountable)
		{
			ResourceKeyName = L"ItemSlotCount";
			ResourcePath = L"/UI/itemslotcount";

			ResourceKeyName += currentItemSlot == m_CurrentFocusItemIndex ? L"Focus" : L"";
			ResourcePath += currentItemSlot == m_CurrentFocusItemIndex ? L"Focus.png" : L".png";

			m_ItemSlot->m_ItemSlotImage[currentItemSlot]->SetImage(
				ResourceManager::GetInstance()->LoadResource<Image>(ResourceKeyName.c_str(), ResourcePath.c_str())->GetImage());
		}

		// Key Name
		ResourceKeyName = L"Item" + m_ItemList[currentItemSlot]->m_ItemKeyName;
		ResourceKeyName += m_ItemList[currentItemSlot]->m_ItemFocus ? L"Focus" : L"";

		// Key Path
		ResourcePath = L"/Image/item/" + m_ItemList[currentItemSlot]->m_ItemKeyName;
		ResourcePath += m_ItemList[currentItemSlot]->m_ItemFocus ? L"Focus.png" : L".png";

		// Item Count
		if (m_ItemList[currentItemSlot]->m_ItemCount == 1) ItemCount = L"";
		else
		{
			if (m_ItemList[currentItemSlot]->m_ItemCount < 10) ItemCount += L"0";
			ItemCount += std::to_wstring(m_ItemList[currentItemSlot]->m_ItemCount);
		}

		/// Apply
		m_ItemSlot->m_ItemImage[currentItemSlot]->SetImage(
			ResourceManager::GetInstance()->LoadResource<Image>(ResourceKeyName.c_str(), ResourcePath.c_str())->GetImage());
		m_ItemSlot->m_ItemCountText[currentItemSlot]->SetText(ItemCount.c_str());
		
		// Item Name
		if (m_ItemList[currentItemSlot]->m_ItemFocus) ItemName = m_ItemList[currentItemSlot]->m_ItemName;

		// Item Name Box Image
		ResourceKeyName = m_ItemList[currentItemSlot]->m_ItemFocus ? L"NameBoxFocus" : L"NameBox";
		ResourcePath = m_ItemList[currentItemSlot]->m_ItemFocus ? L"/UI/nameboxFocus.png" : L"/UI/namebox.png";

		/// Apply
		m_ItemSlot->m_ItemNameText[currentItemSlot]->SetText(ItemName);
		m_ItemSlot->m_ItemNameBox[currentItemSlot]->SetImage(
			ResourceManager::GetInstance()->LoadResource<Image>(ResourceKeyName.c_str(), ResourcePath.c_str())->GetImage());

	}
}

void ItemManagerScript::AddItem(Item* item)
{
	for (int item = 0; item < SLOTCOUNT; item++)
	{
		if (!m_ItemList[item]) continue;
		m_ItemList[item]->m_ItemFocus = false;
	}

	for (int index = 0; index < SLOTCOUNT; index++)
	{
		if (!m_ItemList[index])
		{
			m_ItemList[index] = item;
			m_ItemList[index]->m_ItemFocus = true;

			m_CurrentFocusItemIndex = index;
			ApplyItemSlot();

			return;
		}
	}

	assert(nullptr && "�������� ���� á��!");
}

void ItemManagerScript::DeleteItem(Item* item)
{
	for (auto& index : m_ItemList)
	{
		if (index == item)
		{
			delete item;
			index = nullptr;

			ApplyItemSlot();

			return;
		}
	}

	assert(nullptr && "���� �������� �������� ����!");
}

/// <summary>
/// ��� �������� ������Ų��
/// </summary>
void ItemManagerScript::DeleteAllItem()
{
	for (auto& item : m_ItemList)
	{
		if (item != nullptr)
		{
			delete item;
			item = nullptr;
		}
	}

	m_CurrentFocusItemIndex = 0;
	ApplyItemSlot();
}

