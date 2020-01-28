#pragma once
#include "Math\Vec3.h"

class Sphere3D;
class Box3D;

class IShape3D
{
public:
	virtual ~IShape3D() {};

	//�����蔻��p�֐�
	virtual bool isIntersect(const IShape3D& shape3D) const = 0;
	//�����̂Ƃ̓����蔻��
	virtual bool isIntersect(const Box3D& box3D) const = 0;
	//���Ƃ̓����蔻��
	virtual bool isIntersect(const Sphere3D & sphere3D) const = 0;

	//�����o�������p�֐�
	virtual Vec3 getFixPosition(const IShape3D& shape3D) const = 0;
	//�����̂Ƃ̉����o������
	virtual Vec3 getFixPosition(const Box3D& box3D) const = 0;
	//���Ƃ̉����o������
	virtual Vec3 getFixPosition(const Sphere3D & sphere3D)  const = 0;

protected:
	IShape3D() {};
};