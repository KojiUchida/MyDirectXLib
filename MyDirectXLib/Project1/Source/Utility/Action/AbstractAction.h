#pragma once
class GameObject;
class Timer;

namespace Action
{
	class AbstractAction
	{
	public:
		AbstractAction(float time = 0.0f);
		virtual ~AbstractAction();

		void setUser(GameObject* pUser) { m_pUser = pUser; }

		void baseInit();
		void baseUpdate();

		float getActionTime();

		virtual void init() = 0;				//������
		virtual void update(float time) = 0;	//�X�V
		virtual void onSuspend() = 0;			//���~��
		virtual bool isEnd();					//�I���������ǂ���

		virtual AbstractAction* clone() = 0;	//���g���R�s�[����


	protected:
		GameObject* m_pUser;
		Timer* m_pTimer;
	};
};