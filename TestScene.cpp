#include "TestScene.h"
#include "GameObject.h"
#include "PrimitiveRenderer.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "Define.h"
#include "Animator.h"
#include "Animation.h"
#include "ResourceManager.h"
#include "Image.h"
#include "Camera.h"


//�׽�Ʈ ��ũ��Ʈ
#include "FarmerScript.h"
#include "EventManager.h"


void TestScene::Enter()
{
	// CollisionManager â���� �̺�Ʈ�� ����ٰ� ��
	// CheckCollisionType(OBJECT_TYPEPLAYER, OBJECT_TYPENPC);

	/// �� ���� �� �׻� ī�޶� ���� ������Ʈ�� ����� �д�.
	/// ī�޶� ������ Ÿ���� ���ϰ� �ʹٸ�
	/// camera - SetTargetObject(������Ʈ ���� �̸�);
	GameObject* cameraObject = new GameObject("CameraObject", OBJECT_TYPE::CAMERA);
	Camera* camera = new Camera;
	cameraObject->AddComponent(camera);
	AddGameObject(cameraObject);


	GameObject* backMirror = new GameObject("BackMirror", OBJECT_TYPE::CAMERA);
	Camera* camera2 = new Camera;
	backMirror->AddComponent(camera2);
	backMirror->GetTransform()->SetLocalScale({0.5f, 0.5f});
	backMirror->GetTransform()->SetLocalPosition({500, 500});
	AddGameObject(backMirror);

	{
		// ���� �簢��
		GameObject* testObject1 = new GameObject("testObject1", OBJECT_TYPE::NPC);
		PrimitiveRenderer* testRenderer1 = new PrimitiveRenderer;
		Collider* testCollider1 = new BoxCollider(Vector2{ 100, 100 });
		testObject1->GetTransform()->SetLocalPosition(Vector2(1000.f, 500.f));
		testRenderer1->SetType("Rectangle");
		testObject1->AddComponent(testRenderer1);
		testObject1->AddComponent(testCollider1);
		AddGameObject(testObject1);

		// ���� ��
		GameObject* testObject2 = new GameObject("testObject2", OBJECT_TYPE::NPC);
		PrimitiveRenderer* testRenderer2 = new PrimitiveRenderer;
		Collider* testCollider2 = new CircleCollider(80);
		testObject2->GetTransform()->SetLocalPosition(Vector2(1000.f, 300.f));
		testRenderer2->SetType("Circle");
		testObject2->AddComponent(testRenderer2);
		testObject2->AddComponent(testCollider2);
		AddGameObject(testObject2);

		// ī�޶� Ÿ�� ���
		
	}

	// FSM �׽�Ʈ�� 
	GameObject* farmer = new GameObject("farmer", OBJECT_TYPE::NPC);
	Collider* testCollider4 = new BoxCollider(Vector2{ 60,120 });
	testCollider4->SetOffset(Vector2{ 0.f,20.f });
	PrimitiveRenderer* farmerRenderer = new PrimitiveRenderer;
	//farmerRenderer->SetType("Circle");
	//farmer->AddComponent(farmerRenderer);
	Animator* animator2 = new Animator(ResourceManager::GetInstance()->LoadResource<Image>(L"FarmerImage", L"/Image/Animation/farmer/idle_run.png")->GetImage());
	farmer->AddComponent(animator2);
	animator2->CreateAnimation(L"farmerIdleFront", Vector2(0, 0), Vector2(0, 0), Vector2(140, 160), Vector2(140, 0), 0.1f, 4);
	animator2->CreateAnimation(L"farmerIdleLeft", Vector2(0, 160), Vector2(0, 0), Vector2(140, 160), Vector2(140, 0), 0.1f, 4);
	animator2->CreateAnimation(L"farmerIdleRight", Vector2(0, 320), Vector2(0, 0), Vector2(140, 160), Vector2(140, 0), 0.1f, 4);
	animator2->CreateAnimation(L"farmerIdleBack", Vector2(0, 480), Vector2(0, 0), Vector2(140, 160), Vector2(140, 0), 0.1f, 4);
	animator2->CreateAnimation(L"farmerWalkFront", Vector2(0, 640), Vector2(0, 0), Vector2(140, 160), Vector2(140, 0), 0.1f, 4);
	animator2->CreateAnimation(L"farmerWalkLeft", Vector2(0, 800), Vector2(0, 0), Vector2(140, 160), Vector2(140, 0), 0.1f, 4);
	animator2->CreateAnimation(L"farmerWalkRight", Vector2(0, 960), Vector2(0, 0), Vector2(140, 160), Vector2(140, 0), 0.1f, 4);
	animator2->CreateAnimation(L"farmerWalkBack", Vector2(0, 1120), Vector2(0, 0), Vector2(140, 160), Vector2(140, 0), 0.1f, 4);
	//animator2->Play(L"farmerWalkFront", true);

	FarmerScript* farmerScript = new FarmerScript;
	farmer->AddComponent(farmerScript);
	farmer->AddComponent(testCollider4);
	AddGameObject(farmer);
}
