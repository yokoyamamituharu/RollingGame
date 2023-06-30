#pragma once
#include "Sprite.h"
#include "DirectXCommon.h"
#include "ObjectOBJ.h"
#include "SceneLoader.h"

class TitleScene
{
private:
	//ブラックアウトの段階
	enum {
		black_none,		//何もしていない
		black_being,	//処理中
		black_end,		//処理終わり
	};
public:
	TitleScene();
	~TitleScene();

	void Initialize(DirectXCommon* dxCommon);

	void Update(int& sceneNo, bool& initFlag);

	void Draw();

	/// <summary>
	/// フラグが立ったらブラックアウトをさせる
	/// </summary>
	/// <returns>ブラックアウトが完了したらtrueを返す</returns>
	int BlackOut();

	/// <summary>
	/// フラグが立ったらブラックインをさせる
	/// </summary>
	/// <returns>ブラックインが完了したらtrueを返す</returns>
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

