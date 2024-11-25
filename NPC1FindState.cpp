#include "NPC1FindState.h"
#include "StateMachine.h"

#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Animation.h"
#include "Animator.h"
#include "Collider.h"
#include "BoxCollider.h"

NPC1FindState::NPC1FindState(StateMachine* owner)
	:FSMState(owner, "NPC1Find")
{

}

NPC1FindState::~NPC1FindState()
{

}

void NPC1FindState::Enter()
{
	m_Machine->m_Owner->GetObjectChild("NPCTextBubble")->GetComponent<Animator>()->Play(L"ExclamationMark", true);
	m_Machine->m_Owner->GetObjectChild("NPCTextBubble")->SetActivate(true);
}

void NPC1FindState::Update(float deltaTime)
{
	Vector2 pos = m_Machine->m_Owner->GetTransform()->GetLocalPosition();
	const float speed = 300;
	//���������� �̵�
	if (m_Machine->m_DetailState.compare("MoveRight") == 0)
	{
		pos.x += speed * deltaTime;
		m_Machine->m_Owner->GetComponent<Animator>()->Play(L"NPCMoveRight", true);
		m_Machine->m_Owner->GetObjectChild("NPCView")->GetComponent<BoxCollider>()->SetOffset(Vector2{ 90.f, 20.f });
		m_Machine->m_Owner->GetObjectChild("NPCView")->GetComponent<BoxCollider>()->SetSize(Vector2{ 120, 60 });

	}

	//�������� �̵�
	if (m_Machine->m_DetailState.compare("MoveLeft") == 0)
	{
		pos.x -= speed * deltaTime;
		m_Machine->m_Owner->GetComponent<Animator>()->Play(L"NPCMoveLeft", true);
		m_Machine->m_Owner->GetObjectChild("NPCView")->GetComponent<BoxCollider>()->SetOffset(Vector2{ -90.f, 20.f });
		m_Machine->m_Owner->GetObjectChild("NPCView")->GetComponent<BoxCollider>()->SetSize(Vector2{ 120, 60 });

	}
	//���� �ö�
	if (m_Machine->m_DetailState.compare("MoveBack") == 0)
	{
		pos.y -= speed * deltaTime;
		m_Machine->m_Owner->GetComponent<Animator>()->Play(L"NPCMoveBack", true);
		m_Machine->m_Owner->GetObjectChild("NPCView")->GetComponent<BoxCollider>()->SetOffset(Vector2{ 0.f, -80.f });
		m_Machine->m_Owner->GetObjectChild("NPCView")->GetComponent<BoxCollider>()->SetSize(Vector2{ 60, 120 });
	}
	//�Ʒ��� ������
	if (m_Machine->m_DetailState.compare("MoveFront") == 0)
	{
		pos.y += speed * deltaTime;
		m_Machine->m_Owner->GetComponent<Animator>()->Play(L"NPCMoveFront", true);
		m_Machine->m_Owner->GetObjectChild("NPCView")->GetComponent<BoxCollider>()->SetOffset(Vector2{ 0.f, 120.f });
		m_Machine->m_Owner->GetObjectChild("NPCView")->GetComponent<BoxCollider>()->SetSize(Vector2{ 60, 120 });
	}

	m_Machine->m_Owner->GetTransform()->SetLocalPosition(pos);
}

void NPC1FindState::Exit()
{
	m_Machine->m_Owner->GetObjectChild("NPCTextBubble")->SetActivate(false);
}
