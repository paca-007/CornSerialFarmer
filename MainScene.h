#pragma once


#include "Scene.h"
#include "ZicoMath.h"

class GameManagerScript;

class MainScene: public Scene
{//
private:
	using Scene::Scene;
	virtual void Enter() override;

private:
	// �ӽ� ����
	// �츮�� ī�޶� ������Ʈ �ڵ�� ī�޶� ��ġ�� ȭ���� �߽����� ���� ������ ȭ���� ������ ī�޶��� �߾����� �Ű����.
	// �츮�� ���� ȭ���� �� �۾��� ������ ������̱� ������ ȭ���� �߽���ǥ�� 960,540 �̴�.
	// �� ��ġ�� ������Ʈ���� ȭ���� �߽���ǥ�� 960,540 �� �������� �����ϱ� ������ �Ʒ� startPoint�� Ȱ���ؾ���
	Vector2 m_StartPoint{ 960.f, 540.f };
};