#include "CoinScript.h"

#include "TestGame.h"

FVector3 position[17] =
{
	FVector3(2,0,2),
	FVector3(124,0,140),
	FVector3(58,-8,131),
	FVector3(-64,0,133),
	FVector3(-64,0,64),
	FVector3(-15,0,25),
	FVector3(36,0,88),
	FVector3(88,-7,86),
	FVector3(137,0,7),
	FVector3(111,8,-34),
	FVector3(13,0,-41),
	FVector3(60,-11,9),
	FVector3(6,6,160),
	FVector3(-4,6,89),
	FVector3(155,2,74),
	FVector3(143,2,168),
	FVector3(16,-8,150)
};

int idx = 0;

void CoinScript::Update(float dt) 
{
	static float accum = 0;
	accum += dt;

	Component<FTransform>().Position.y = position[idx].y + 4 + sinf(accum * 3.0f);
	Component<FTransform>().Orientation = FQuaternion::FromEulerAngles(FVector3(0, accum, 0));

	auto playerPos = game->playerEntity.Component<FTransform>().Position;
	auto thisPos = Component<FTransform>().Position;

	if ((thisPos - playerPos).Length() < 5.0f)
	{
		Relocate();
	}
}
void CoinScript::Relocate()
{
	idx = rand() % 17;

	Component<FTransform>().Position = position[idx];
}