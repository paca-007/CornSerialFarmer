#include "TextManagerScript.h"

TextManagerScript::TextManagerScript()
	: MonoBehaviour(typeid(this).name())
{

}

TextManagerScript::~TextManagerScript()
{

}

void TextManagerScript::Start()
{
	vector<wstring> NPC1 = { L"������ �����\n���� �̻縦 �ߴ�?" , L"�����̳���\n�� �ǰ��°�~", L"���� ����� ���̷���\n�Ȼ��� ��ο�!", L"��������\n�Ϲڰ����� �����ա�", L"���� ������\n��� �����հ�!", L"������ �����\n�Ͻ��M�µ𸻿���"};
	vector<wstring> NPC2 = { L"�ȳ��ϼ���!\n������ ����?" , L"���� ���͵帱��\n�������?", L"�� ���� �����̳���?\n����� �Ӹ�����", L"�� �����°ž�?", L"������\n����̴١�", L"�� �����\n�����ѵ���" };
	vector<wstring> NPC3 = { L"�츮 ���\n���þ��?\n����� �Ӹ��ε�" , L"�� ������\n���� ��������\n�ſ�?", L"����\n������ �ſ�?", L"���ƾ�~!\n�����~!!", L"�� ���\n������~!", L"��!\n�̻��� ����̾�!"};
	vector<wstring> NPC4 = { L"Ƽ���� ���Ͷ�\n�ο�� �����̱���?" , L"�� ��ó�� ������\n���� �������?", L"�̰� Ƽ���Կ�,\n�˷λ�츣���Կ�?", L"�ȳ��� �輼��~", L"���� �Ⱦ��Ͻó���", L"��� ����\nȭ�������� ������"};
	vector<wstring> NPC5 = { L"��� �ð�\n����������?" , L"������! ����� ��\n���� ���̼���.", L"����Բ��� �� ����\n�̸� �����϶�ó׿�", L"�Ѹ�����\n������!", L"������\n�����ʾҽ��ϴ�!", L"���ס�~~\n�� �� �簡����" };
	vector<wstring> NPC6 = { L"���� ���κ���\n��𰣰ſ���!" , L"���� ���κ���\n���ܵа���!", L"���,\n���ۻ������!", L"�ΰ� ��! ����\n������ ã�Ƴ��ž�!", L"�� ���, �и�\n���� ����� �־�!", L"�� ���, �и�\n���� ����̾�!" };
	vector<wstring> Player = { L"�� ����." , L"�Ű� ����.", L"����!" };
	vector<wstring> Police = { L"���� ��\n�̻� ��!" , L"������ ����� ����?", L"��� �����ڰ�\n��½ �þ��"};

	ScriptMap["NPC1"] = NPC1;
	ScriptMap["NPC2"] = NPC2;
	ScriptMap["NPC3"] = NPC3;
	ScriptMap["NPC4"] = NPC4;
	ScriptMap["NPC5"] = NPC5;
	ScriptMap["NPC6"] = NPC6;
	ScriptMap["Player"] = Player;
	ScriptMap["Police"] = Police;

	vector<wstring> farm = { L"�������� Ű�� �� �ִ�\n�� ���̴�.", L"������ ������\n���� ���̴�.", L"�翡 ������ �ڶ���. ", L"�������� ������ �ӵ���\n �������� ���̴�.", L"�������� �� �ڶ���. \n���� ��Ȯ�� �� �ְڱ�."};
	vector<wstring> house = {L"����� �������̴�.\n���� �������� �ʴ´ٸ�\n��ü�� ����� ������ ���̴�."};
	vector<wstring> wareHouseA = {L"�������� ������ �����ϴ�\n���� â���. �� ���̶� ���� ��ü��\n���� ���� �i�Ƴ��� ���� ���̴�."};
	vector<wstring> wareHouseB = {L"���Ѹ��� �� �����縦 �����ϴ�\nū â���. ��ü �ϳ��� ���ܵ�\n�� ������ �д�."};
	vector<wstring> wareHouseC = {L"��Ȯ�� ���� �ڷ� ���� �����ϴ�\n���� â���. �� ���̶� �Ҿ�������,\n��ü�� ���� �� �������� �����̴�."};
	vector<wstring> wareHouseD = {L"������ �������ó�� ���� ����\n�͵��� �����ϴ� â���.\n������ �幰�� ��ü�� ����� ����."};
	vector<wstring> truck = {L"��Ȯ���� ����ϴ� Ʈ���̴�.\n������ �� �� ������ ��ü������\n���� �� ���� ���̴�."};
	vector<wstring> pond = {L"�� ���� ���� ���������̴�.\n������ �̲������\n������ �ʰ� ��������.", L"���� �����ż� ����� �� ����."};
	vector<wstring> catHouse = {L"����̰� ��� ���̴�.\n���Ϳ��� �༮ �����϶��."};
	vector<wstring> catbowl = {L"������� ��׸��̴�.\n ������ϰ� �������."};
	vector<wstring> tree = {L"���� ������ �������� ����\n���ƺ��δ�. "};
	vector<wstring> NPC1ex = {L"�������� �龥�ð� �ٴϴ�\n���� �ҹ��."};
	vector<wstring> NPC2ex = {L"�������� �������� ����\n�����̴�."};
	vector<wstring> NPC3ex = {L"¥������ �����̴�."};
	vector<wstring> NPC4ex = {L"�ǳ� ���� ��¼�� �Ÿ���\n�����̴�."};
	vector<wstring> NPC5ex = {L"�����̴�. �峭�� �ƴϰ�,\n��¥ ���� �ȴ���ϱ�."};
	vector<wstring> NPC6ex = {L"�ò����� �����̴�."};
	vector<wstring> wildBoar = {L"������ �������\n�� ���� ��ġ�� �Ա���!"};
	vector<wstring> cat = { L"�Ϳ��� ����̴�.\n ���ٵ�� �ְ� �ͱ�..." };
	
	explainMap["FARM"] = farm;
	explainMap["House"] = house;
	explainMap["WareHouseA"] = wareHouseA;
	explainMap["WarehouseB"] = wareHouseB;
	explainMap["WarehouseC"] = wareHouseC;
	explainMap["WarehouseD"] = wareHouseD;
	explainMap["Truck"] = truck;
	explainMap["Pond"] = pond;
	explainMap["CatHouse"] = catHouse;
	explainMap["CatBowl"] = catbowl;
	explainMap["Tree"] = tree;
	explainMap["npc1"] = NPC1ex;
	explainMap["npc2"] = NPC2ex;
	explainMap["npc3"] = NPC3ex;
	explainMap["npc4"] = NPC4ex;
	explainMap["npc5"] = NPC5ex;
	explainMap["npc6"] = NPC6ex;
	explainMap["wildBoar"] = wildBoar;
	explainMap["blackCatChild"] = cat;

	
}

void TextManagerScript::Update(float deltaTime)
{

}

void TextManagerScript::Finalize()
{

}
