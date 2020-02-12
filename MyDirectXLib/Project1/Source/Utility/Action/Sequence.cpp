#include "Sequence.h"
#include <stdarg.h>

Action::Sequence::Sequence(std::queue<AbstractAction*> actionQueue)
	:m_ActionQueue(actionQueue),
	m_ActionQueueOrigin(actionQueue)
{
}

Action::Sequence::Sequence(int count, AbstractAction * first, ...)
{
	m_ActionQueueOrigin.push(first);

	va_list args;
	va_start(args, first);
	for (int i = 0; i < count - 1; i++)
	{
		m_ActionQueueOrigin.push(va_arg(args, AbstractAction*));
	}
	va_end(args);
}

Action::Sequence::~Sequence()
{
	while (m_ActionQueueOrigin.size() > 0)
	{
		delete m_ActionQueueOrigin.front();
		m_ActionQueueOrigin.pop();
	}
}

void Action::Sequence::init()
{
	m_ActionQueue = m_ActionQueueOrigin;
	m_ActionQueue.front()->setUser(m_pUser);
	m_ActionQueue.front()->baseInit();
}

void Action::Sequence::update(float time)
{
	if (m_ActionQueue.size() <= 0)
		return;

	m_ActionQueue.front()->baseUpdate();

	if (m_ActionQueue.front()->isEnd())
	{
		m_ActionQueue.pop();

		if (m_ActionQueue.size() == 0) return;

		m_ActionQueue.front()->setUser(m_pUser);
		m_ActionQueue.front()->baseInit();
	}
}

void Action::Sequence::onSuspend()
{
	if (m_ActionQueue.size() <= 0)
		return;

	m_ActionQueue.front()->onSuspend();
}

bool Action::Sequence::isEnd()
{
	return m_ActionQueue.size() == 0;
}

Action::Sequence * Action::Sequence::clone()
{
	std::queue<AbstractAction*> cloneQueue;

	//Queue�̔z����t�����ɃR�s�[
	for (auto itr = m_ActionQueue._Get_container().begin();
			itr != m_ActionQueue._Get_container().end();
			++itr)
	{
		cloneQueue.push((*itr)->clone());
	}

	return new Sequence(cloneQueue);
}
