#pragma once
#include "Sprite.h"
#include "DirectXCommon.h"
#include "ObjectOBJ.h"
#include "SceneLoader.h"

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
	TitleScene();
	~TitleScene();

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
	Sprite* titleSprite = nullptr;
	Sprite* setumei1Sprite = nullptr;
	Sprite* setumei2Sprite = nullptr;
	Sprite* setumei3Sprite = nullptr;

	int index = 0;

	Sprite* black = nullptr;
	DirectXCommon* dxCommon = nullptr;
	float blackOutAlpha;
	float blackInAlpha;
	bool blackOutFlag;
	bool blackInFlag;

	Sprite* sceneEffect[5];
	int effectIndex = 0;
	int effectTime = 0;
	bool effectFlag = false;

	ObjectObj* titleObj = nullptr;
	ObjectObj* tenQ = nullptr;
	SceneLoader* scene = nullptr;



	Camera* camera;
};

