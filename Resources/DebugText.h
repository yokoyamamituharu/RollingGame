#pragma once
#include "Sprite.h"
#include <string>

class DebugText
{
public:
	static const int maxCharCount = 256;
	static const int fontWidth = 9;
	static const int fontHeight = 18;
	static const int fontLineCount = 14;

	//�X�v���C�g�̔z��
	Sprite *sprites[maxCharCount];

	//�X�v���C�g�z��̓Y�����ԍ�
	int spriteIndex = 0;

public:
	void Initialize(ID3D12Device* dev, int window_width, int window_height, UINT texnumber);

	void Print(const std::string& text, float x, float y, float scale = 1.0f);

	void DrawAll();
};

