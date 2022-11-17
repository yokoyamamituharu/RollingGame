#pragma once
#include "Sprite.h"
#include "DirectXCommon.h"



class TitleScene
{
private:
	//�u���b�N�A�E�g�̒i�K
	enum {
		black_none,		//�������Ă��Ȃ�
		black_being,	//������
		black_end,		//�����I���
	};
public:
	void Initialize(DirectXCommon* dxCommon);

	void Update(int& sceneNo, bool& initFlag);

	void Draw();

	/// <summary>
	/// �t���O����������u���b�N�A�E�g��������
	/// </summary>
	/// <returns>�u���b�N�A�E�g������������true��Ԃ�</returns>
	int BlackOut();

	/// <summary>
	/// �t���O����������u���b�N�C����������
	/// </summary>
	/// <returns>�u���b�N�C��������������true��Ԃ�</returns>
	int BlackIn();

private:
	Sprite* sprite = nullptr;
	Sprite* black = nullptr;
	DirectXCommon* dxCommon = nullptr;
	float blackOutAlpha;
	float blackInAlpha;
	bool blackOutFlag;
	bool blackInFlag;

};

