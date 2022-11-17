#pragma once
#include "Sprite.h"
#include "DirectXCommon.h"



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
	Sprite* sprite = nullptr;
	Sprite* black = nullptr;
	DirectXCommon* dxCommon = nullptr;
	float blackOutAlpha;
	float blackInAlpha;
	bool blackOutFlag;
	bool blackInFlag;

};

