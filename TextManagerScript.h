#pragma once
#include "MonoBehaviour.h"
#include <map>
#include <vector>

using std::map;
using std::vector;
using std::wstring;

/// <summary>
/// �ۼ��� : 2023.08.21
/// �ۼ��� : ������
/// 
/// ĳ���ͺ� ��ȭ�� ����ϱ� ���� �����͸� �����صδ� script
/// </summary>


class TextManagerScript :
    public MonoBehaviour
{

public:
    TextManagerScript();
    ~TextManagerScript();

public:
	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void Finalize();

public:
	map<string, vector<wstring>> ScriptMap;		//�����͸� �����ص� map ���� key : NPC �̸�, value : ���° ���
	map<string, vector<wstring>> explainMap;	//��ȣ�ۿ��� ������ ���ǵ鿡 ���� ���� �����͸� �����ص� map key : ��ȣ�ۿ� ���� ���ǵ� �̸�, value ���

	
};

