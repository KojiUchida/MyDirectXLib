#pragma once
class GameObject;
class RenderManager;
struct Vec3;

class AbstractComponent
{
public:
	virtual ~AbstractComponent();

	void destroy();
	void setActive(bool value);
	bool isActive();

	void update();

	virtual void onStart() = 0;
	virtual void onUpdate() = 0;

	virtual void onDestroy() {};
	virtual void onEnable() {};
	virtual void onDisable() {};

	//�Փ˔���p�֐�(�����R���|�[�l���g�ł̂ݎg�p)
	virtual void checkCollision2D() {};

	//�Փ˔���p�֐�(�����R���|�[�l���g�ł̂ݎg�p)
	virtual void checkCollision3D() {};

	virtual void onCollisionEnter(GameObject* pHit) {};
	virtual void onCollisionStay(GameObject* pHit) {};
	virtual void onCollisionExit(GameObject* pHit) {};

	//���W���ύX���ꂽ��
	virtual void onPositionChanged(const Vec3& currentPos) {};
	//�p�x���ύX���ꂽ��
	virtual void onAngleChanged(const Vec3& currentAngle) {};
	//�T�C�Y���ύX���ꂽ��
	virtual void onSizeChanged(const Vec3& currentSize) {};

	int getUpdateOrder() const;
	GameObject* getUser();

protected:
	AbstractComponent(GameObject* pUser, int updateOrder = 100);

protected:
	GameObject* m_pUser;
	int m_UpdateOrder;
	bool m_Enabled;
	bool m_FirstUpdate;
};

