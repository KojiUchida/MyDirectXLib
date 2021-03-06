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

	void checkCollision2D();
	void checkCollision3D();

	//コンポーネントを取得
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
		//キャストに成功したらループ終了
		if (pComponent != nullptr) break;
	}

	return pComponent;
}
