#pragma once
#include "IShape3D.h"
#include "Math\Vec3.h"

class Sphere3D
	: public IShape3D
{
public:
	Sphere3D();

	//座標を設定
	void setPosition(const Vec3& position);
	//座標を取得
	const Vec3& getPosition() const;

	//半径を設定
	void setRadius(float radius);
	//半径を取得
	float getRadius() const;

	virtual bool isIntersect(const IShape3D& shape3D) const override;
	virtual bool isIntersect(const Box3D & box3D) const override;
	virtual bool isIntersect(const Sphere3D & sphere3D) const override;

	virtual Vec3 getFixPosition(const IShape3D& shape3D) const override;
	virtual Vec3 getFixPosition(const Box3D & box3D) const override;
	virtual Vec3 getFixPosition(const Sphere3D & sphere3D) const override;

private:
	Vec3 m_Position;	//場所
	float m_Radius;		//半径
};