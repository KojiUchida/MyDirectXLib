#pragma once
#include <vector>

class AbstractCollider2D;
class AbstractCollider3D;
class IGameMediator;

class PhysicsWorld
{
public:
	PhysicsWorld(IGameMediator* pMediator);
	~PhysicsWorld();

	void update();

	//��������Ŕ���
	void checkCollision2D(AbstractCollider2D* pCollider);
	//�R���C�_�[�̓o�^
	void addCollider2D(AbstractCollider2D* pCollider);
	//�R���C�_�[��o�^����
	void removeCollider2D(AbstractCollider2D* pCollider);

	//��������Ŕ���
	void checkCollision3D(AbstractCollider3D* pCollider);
	//�R���C�_�[�̓o�^
	void addCollider3D(AbstractCollider3D* pCollider);
	//�R���C�_�[��o�^����
	void removeCollider3D(AbstractCollider3D* pCollider);

private:
	bool checkCollisionDistance(AbstractCollider2D* coll1, AbstractCollider2D* coll2);

private:
	//�R���C�_�[�Ǘ��pvector
	std::vector<AbstractCollider2D*> m_ColliderList2D;
	std::vector<AbstractCollider3D*> m_ColliderList3D;

	IGameMediator* m_pGameMediator;
};