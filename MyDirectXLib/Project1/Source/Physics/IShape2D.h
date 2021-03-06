#pragma once
class Circle2D;
class Box2D;

class IShape2D
{
public:
	virtual ~IShape2D() {};
	
	//四角形との当たり判定
	virtual bool isIntersect(const Box2D& box2D) const = 0;
	//円との当たり判定
	virtual bool isIntersect(const Circle2D& circle2D) const = 0;

protected:
	IShape2D() {};
};