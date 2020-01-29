#pragma once
#include "Physics\IShape3D.h"

class Box3D
	: public IShape3D
{
public:
	Box3D();

	//���W��ݒ�
	void setPosition(const Vec3& position) { m_Position = position; }
	//���W���擾
	const Vec3& getPosition() const { return m_Position; }

	//�T�C�Y��ݒ�
	void setSize(const Vec3& size) { m_Size = size; }
	//�T�C�Y���擾
	const Vec3& getSize() const { return m_Size; }

	virtual bool isIntersect(const IShape3D & shape3D) const override;
	virtual bool isIntersect(const Box3D & box3D) const override;
	virtual bool isIntersect(const Sphere3D & sphere3D) const override;

	virtual Vec3 getFixPosition(const IShape3D & shape3D) const override;
	virtual Vec3 getFixPosition(const Box3D & box3D) const override;
	virtual Vec3 getFixPosition(const Sphere3D & sphere3D) const override;

private:
	Vec3 m_Position;
	Vec3 m_Size;
};