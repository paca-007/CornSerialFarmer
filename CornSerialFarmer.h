///
/// 2023.08.09 
/// �ۼ��� : ��â��
/// dll�� ���鼭 ������ ��� �ؾ����� �;���.
/// �׳� �ĸ� ���ӿ� ���ӿ��� �־ �����½����� �޴�.
///
#pragma once

class GameProcess;

class CornSerialFarmer
{
public:
	CornSerialFarmer(GameProcess* gameProcess);
	~CornSerialFarmer();

	void InitializeGame();

private:
	GameProcess* m_GameProcess;
};

