#pragma once
#include "IShape3D.h"
#include "Math\Vec3.h"

class Sphere3D
	: public IShape3D
{
public:
	Sphere3D();

	//���W��ݒ�
	void setPosition(const Vec3& position);
	//���W���擾
	const Vec3& getPosition() const;

	//���a��ݒ�
	void setRadius(float radius);
	//���a���擾
	float getRadius() const;

	virtual bool isIntersect(const IShape3D& shape3D) const override;
	virtual bool isIntersect(const Box3D & box3D) const override;
	virtual bool isIntersect(const Sphere3D & sphere3D) const override;

	virtual Vec3 getFixPosition(const IShape3D& shape3D) const override;
	virtual Vec3 getFixPosition(const Box3D & box3D) const override;
	virtual Vec3 getFixPosition(const Sphere3D & sphere3D) const override;

private:
	Vec3 m_Position;	//�ꏊ
	float m_Radius;		//���a
};