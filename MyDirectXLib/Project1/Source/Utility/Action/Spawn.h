#pragma once
#include "AbstractAction.h"
#include <vector>

namespace Action
{
	class Spawn
		: public AbstractAction
	{
	public:
		Spawn(std::vector<AbstractAction*> actionList);
		Spawn(int count, AbstractAction* first, ...);
		~Spawn();

		virtual void init() override;
		virtual void update(float time) override;
		virtual void onSuspend() override;
		virtual bool isEnd() override;

		virtual Action::Spawn* clone() override;

	private:
		std::vector<AbstractAction*> m_ActionList;
	};

};