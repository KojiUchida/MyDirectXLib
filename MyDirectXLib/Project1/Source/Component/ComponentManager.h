#pragma once
#include <vector>

class GameObject;
class AbstractComponent;
class AbstractCollider2D;
class AbstractCollider3D;

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
