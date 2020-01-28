#pragma once
#include <vector>

class GameObject;
class AbstractComponent;
class AbstractCollider2D;
class AbstractCollider3D;
struct Vec3;

class ComponentManager
{
public:
	ComponentManager(GameObject* pUser);
	~ComponentManager();

	void add(AbstractComponent* pComponent);
	void remove(AbstractComponent* pComponent);

	void update();

	void onEnable();
	void onDisable();
	void onDestroy();

	void onCollisionEnter(GameObject* pHit);
	void onCollisionStay(GameObject* pHit);
	void onCollisionExit(GameObject* pHit);

	//���W���ύX���ꂽ��
	void onPositionChanged(const Vec3& currentPos);
	//�p�x���ύX���ꂽ��
	void onAngleChanged(const Vec3& currentAngle);
	//�T�C�Y���ύX���ꂽ��
	void onSizeChanged(const Vec3& currentSize);

	void checkCollision2D();
	void checkCollision3D();

	//�R���|�[�l���g���擾
	template<typename T>
	T* getComponent();

private:
	GameObject* m_pUser;
	std::vector<AbstractComponent*> m_Components;
};

template<typename T>
inline T* ComponentManager::getComponent()
{
	T* pComponent = nullptr;

	for (auto component : m_Components)
	{
		pComponent = dynamic_cast<T*>(component);
		//�L���X�g�ɐ��������烋�[�v�I��
		if (pComponent != nullptr) break;
	}

	return pComponent;
}
