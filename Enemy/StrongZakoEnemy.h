#pragma once
#include "Enemy.h"
#include "EnemyZako.h"
class StrongZakoEnemy :
    public EnemyZako
{

private:
    int hp = 10;
    OBJobject* tuyoObject = nullptr;
};

