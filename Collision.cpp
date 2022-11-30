#include "Collision.h"

bool CollisitonBoxToBox(XMFLOAT3 pos1,XMFLOAT3 radius1, XMFLOAT3 pos2,XMFLOAT3 radius2)
{
	float disX1 = pos1.x + radius1.x;
	float disX2 = pos2.x - radius2.x;
	float disX3 = pos1.x - radius1.x;
	float disX4 = pos2.x + radius2.x;
	float disY1 = pos1.y + radius1.y;
	float disY2 = pos2.y - radius2.y;
	float disY3 = pos1.y - radius1.y;
	float disY4 = pos2.y + radius2.y;
	float disZ1 = pos1.z + radius1.z;
	float disZ2 = pos2.z - radius2.z;
	float disZ3 = pos1.z - radius1.z;
	float disZ4 = pos2.z + radius2.z;

	//x‚Æy‚Í‚»‚ê‚¼‚ê’†SÀ•W‚Æ‚µ‚ÄŒvŽZ‚·‚é
	return disX1 > disX2 && disX4 > disX3 && disY1 > disY2 && disY4 > disY3 && disZ1 > disZ2 && disZ4 > disZ3;
}
