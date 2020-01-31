#include "Tester.h"
#include "Component\Graphics\MeshRenderer.h"
#include "Utility\Color.h"
#include <DirectXColors.h>
#include "Device\Input.h"
#include "Device\GameTime.h"
#include "Component\Transform.h"

float deathTimer = 0.0f;

Tester::Tester(IGameMediator * pGameMediator)
	: GameObject(pGameMediator)
{
}

void Tester::start()
{
	auto renderer = new MeshRenderer(this);
	renderer->setMesh("Sphere");
	renderer->setColor(Color(DirectX::Colors::White));
}

void Tester::update()
{
	getTransform()->setPosition(getTransform()->getPosition() + Input::getLStickValue().toVec3() * 3 * GameTime::getDeltaTime());
	getTransform()->setAngles(getTransform()->getAngles() + Vec3(0, 30 * GameTime::getDeltaTime(), 0));
}

void Tester::onCollisionEnter(GameObject * pHit)
{
	getComponent<MeshRenderer>()->setColor(Color(DirectX::Colors::Red));
}

void Tester::onCollisionStay(GameObject * pHit)
{
	getComponent<MeshRenderer>()->setColor(Color(DirectX::Colors::Yellow));

	//deathTimer += GameTime::getDeltaTime();
	//if (deathTimer >= 3)
	//{
	//	destroy();
	//	pHit->destroy();
	//}
}

void Tester::onCollisionExit(GameObject * pHit)
{
	getComponent<MeshRenderer>()->setColor(Color(DirectX::Colors::Green));
}