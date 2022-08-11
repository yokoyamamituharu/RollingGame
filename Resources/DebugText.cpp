#include "DebugText.h"

void DebugText::Initialize(ID3D12Device* dev, int window_width, int window_height, UINT texnumber)
{
	//���ׂẴX�v���C�g�̃f�[�^�ɂ���
	for (int i = 0; i < _countof(sprites); i++)
	{
		//�X�v���C�g�𐶐�����
		sprites[i] = Sprite::Create(texnumber, { 0, 0 });
	}
}

void DebugText::Print(const std::string& text, float x, float y, float scale)
{
	//���ׂĂ̕����ɂ���
	for (int i = 0; i < text.size(); i++)
	{
		//�ő吔����������
		if (spriteIndex >= maxCharCount) {
			break;
		}

		//1�������o��(*�A�X�L�[�R�[�h�������藧���Ȃ�)
		const unsigned char& character = text[i];

		//�A�X�L�[�R�[�h��2�b�i����΂����ԍ����v�Z
		int fontIndex = character - 32;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		//���W�v�Z
		sprites[spriteIndex]->SetPos({ x + fontWidth * scale * i, y });
		sprites[spriteIndex]->texLeftTop = { (float)fontIndexX + fontWidth,(float)fontIndexY + fontHeight };
		sprites[spriteIndex]->texCutSize = { fontWidth,fontHeight };
		sprites[spriteIndex]->SetSize({ fontWidth * scale,fontHeight * scale });

		//�X�V

		//��������i�߂�
		spriteIndex++;
	}
}

void DebugText::DrawAll()
{
	//���ׂĂ̕����̃X�v���C�g�ɂ���
	for (int i = 0; i < spriteIndex; i++)
	{
		//�X�v���C�g�`��
		sprites[i]->Draw();
	}

	spriteIndex = 0;
}
