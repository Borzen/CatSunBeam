#include "BoundingBox.h"

BoundingBox::BoundingBox(D3DXVECTOR3 tmin,D3DXVECTOR3 tmax){
	_min = tmin;
	_max = tmax;
}

bool BoundingBox::isPointInside(D3DXVECTOR3 &p){
	if(p.x >= _min.x && p.y >=_min.y && p.z >= _min.z && p.x <= _max.x && p.y <=_max.y && p.z <= _max.z){
		return true;
	}
	else{
		return false;
	}
}