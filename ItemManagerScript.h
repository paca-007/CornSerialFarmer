/// [2023/8/15] �ڿ���
/// --------------------------
/// �������� �����ϴ� ������ �Ŵ���
/// �������� �����ִ� UI�� �Բ� �����Ѵ�

#pragma once

#include "MonoBehaviour.h"
#include <vector>

class ItemSlot;

enum class ITEM_TYPE
{
	AX,
	BAG,
	CATFOOD,
	CORN,
	FERTILIZER,
	FIREWOOD,
	SEED,
	EMPTYWATERING,
	WATERING,
	KEY,

	END
};

class Item
{
public:
	Item(std::wstring itemKeyName,
		std::wstring itemName,
		ITEM_TYPE itemType,
		bool isCountable,
		int itemCount = 1);
	~Item();

public:
	void SetItemCount(int itemCount) { m_ItemCount = itemCount; }
	void SetItemFocus(bool isFocus) { m_ItemFocus = isFocus; }
	
public:
	std::wstring m_ItemKeyName;	// �������� Ű �̸�
	std::wstring m_ItemName;	// �������� �̸�
	ITEM_TYPE m_ItemType;		// �������� ����
	bool m_ItemFocus;			// ��Ŀ�� ����
	bool m_IsCountable;			// �� �� �ִ� ����������
	int m_ItemCount;			// �������� ����
};

class ItemManagerScript : public MonoBehaviour
{
public:
	ItemManagerScript();
	~ItemManagerScript();

public:
	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void Finalize();

public:
	void FindFocusItem();
	void ApplyItemSlot();
	void AddItem(Item* item);
	void DeleteItem(Item* item);
	void DeleteAllItem();

public:
	// Getter & Setter
	Item* GetCurrentFocusItem() { return m_ItemList[m_CurrentFocusItemIndex]; 
	}

public:
	static const int SLOTCOUNT = 5;
	std::vector<Item*> m_ItemList;

	// ������ ���� UI
	ItemSlot* m_ItemSlot;

	// ���� ��Ŀ�̵� ������ �ε���
	int m_CurrentFocusItemIndex;
	bool m_ChangeFocus;

};

