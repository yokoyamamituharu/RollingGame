#pragma once
class Sprite;

class SpriteManager
{
public:
	enum SpriteName {
		texture,
		debugfont,
		white1x1,
		title,
		kati,
		make,
		black,
		cloudBurt,
		effect_1,
		effect_2,
		effect_3,
		effect_4,
		effect_5,
		fragment,
		tex1,
		arrowSymbol,
		damageHp,
		hit,
		hp,
		map,
		mousepos,
		pose,
		sprite_0001,
		sprite_0002,
		tikai,
		torisetu,		
		num0,
		num1,
		num2,
		num3,
		num4,
		num5,
		num6,
		num7,
		num8,
		num9,
		slash,
		setumei1,
		setumei2
	};

	SpriteManager();
	~SpriteManager();

	static void StaticInitialize();
};

