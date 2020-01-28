#pragma once
class Circle2D;
class Box2D;

class IShape2D
{
public:
	virtual ~IShape2D() {};
	
	//�l�p�`�Ƃ̓����蔻��
	virtual bool isIntersect(const Box2D& box2D) const = 0;
	//�~�Ƃ̓����蔻��
	virtual bool isIntersect(const Circle2D& circle2D) const = 0;

protected:
	IShape2D() {};
};