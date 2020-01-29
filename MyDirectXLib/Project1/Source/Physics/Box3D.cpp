#include "Box3D.h"
#include <cmath>
#include "Physics\Sphere3D.h"
#include "Math\MathUtility.h"

Box3D::Box3D()
{
}

bool Box3D::isIntersect(const IShape3D & shape3D) const
{
	return shape3D.isIntersect(*this);
}

bool Box3D::isIntersect(const Box3D & box3D) const
{
	Vec3 myPosition = getPosition();
	Vec3 otherPosition = box3D.getPosition();

	Vec3 mySize = getSize();
	Vec3 otherSize = getSize();

	//���݂�X,Y,Z���ꂼ��̋����̐�Βl���Z�o
	const float xDiff = std::fabsf(myPosition.x - otherPosition.x);
	const float yDiff = std::fabsf(myPosition.y - otherPosition.y);
	const float zDiff = std::fabsf(myPosition.z - otherPosition.z);

	//�Փ˂܂ł̍ŒZ�����ƌ��݂̋����̔�r��X,Y,Z���ꂼ��ōs��
	return
		xDiff <= (mySize.x + otherSize.x) &&
		yDiff <= (mySize.y + otherSize.y) &&
		zDiff <= (mySize.z + otherSize.z);
}

bool Box3D::isIntersect(const Sphere3D & sphere3D) const
{
	Vec3 myPosition = getPosition();
	Vec3 otherPosition = sphere3D.getPosition();

	Vec3 mySize = getSize();

	//X, Y, Z�ł��ꂼ���ԋ߂����W�����
	Vec3 closestPos(
		MathUtility::clamp(otherPosition.x, myPosition.x - mySize.x, myPosition.x + mySize.x),
		MathUtility::clamp(otherPosition.y, myPosition.y - mySize.y, myPosition.y + mySize.y),
		MathUtility::clamp(otherPosition.z, myPosition.z - mySize.z, myPosition.z + mySize.z)
	);

	return closestPos.distance(otherPosition) <= sphere3D.getRadius();
}

Vec3 Box3D::getFixPosition(const IShape3D & shape3D) const
{
	return shape3D.getFixPosition(*this);
}

Vec3 Box3D::getFixPosition(const Box3D & box3D) const
{
	Vec3 myPosition = getPosition();
	Vec3 otherPosition = box3D.getPosition();

	Vec3 mySize = getSize();
	Vec3 otherSize = getSize();

	//���݂�X,Y,Z���ꂼ��̋����̍����Z�o
	const float xDiff = otherPosition.x - myPosition.x;
	const float yDiff = otherPosition.y - myPosition.y;
	const float zDiff = otherPosition.z - myPosition.z;

	//�ł����������鎲
	const float farAxis = std::fmaxf(std::fmaxf(std::fabsf(xDiff), std::fabsf(yDiff)), std::fabsf(zDiff));
	//�����o������
	Vec3 fixPosLocal(xDiff, yDiff, zDiff);

	//X�����̉����o��
	if (std::fabsf(xDiff) <= (mySize.x + otherSize.x) &&
		farAxis == std::fabsf(xDiff))
	{
		//X�̍ŒZ�Փˋ����̕������o��
		fixPosLocal.x = (mySize.x + otherSize.x) * MathUtility::sign(xDiff);
	}

	//Y�����̉����o��
	if (std::fabsf(yDiff) <= (mySize.y + otherSize.y) &&
		farAxis == std::fabsf(yDiff))
	{
		//Y�̍ŒZ�Փˋ����̕������o��
		fixPosLocal.y = (mySize.y + otherSize.y) * MathUtility::sign(yDiff);
	}

	//Z�����̉����o��
	if (std::fabsf(zDiff) <= (mySize.z + otherSize.z) &&
		farAxis == std::fabsf(zDiff))
	{
		//Z�̍ŒZ�Փˋ����̕������o��
		fixPosLocal.z = (mySize.z + otherSize.z) * MathUtility::sign(zDiff);
	}

	return myPosition + fixPosLocal;
}

Vec3 Box3D::getFixPosition(const Sphere3D & sphere3D) const
{
	Vec3 myPosition = getPosition();
	Vec3 otherPosition = sphere3D.getPosition();

	Vec3 mySize = getSize();

	//X, Y, Z�ł��ꂼ���ԋ߂����W�����
	Vec3 closestPos(
		MathUtility::clamp(otherPosition.x, myPosition.x - mySize.x, myPosition.x + mySize.x),
		MathUtility::clamp(otherPosition.y, myPosition.y - mySize.y, myPosition.y + mySize.y),
		MathUtility::clamp(otherPosition.z, myPosition.z - mySize.z, myPosition.z + mySize.z)
	);

	//�����o������
	Vec3 fixPosLocal = (otherPosition - closestPos).normalized() * sphere3D.getRadius();

	return closestPos + fixPosLocal;
}
