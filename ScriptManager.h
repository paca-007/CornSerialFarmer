#pragma once
#include "MonoBehaviour.h"
/// <summary>
/// �ۼ��� : 2023.08.21
/// �ۼ��� : ������
/// 
/// ĳ���ͺ� ��ȭ�� ����ϱ� ���� �����͸� �����صδ� script
/// </summary>

class ScriptManager :
    public MonoBehaviour
{

public:
    ScriptManager();
    ~ScriptManager();

public:
	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void Finalize();

};

