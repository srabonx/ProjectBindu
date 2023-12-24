#pragma once
#include <Bindu.h>
#include <map>

enum MoveSet {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	ATTACKING,
	NONE,
};

// struct AnimationValue
// {
// 	int index;
// 	int frameCount;
// };

enum Animations
{
	IDLE,
	RUNNING,
	WALKING,
	FIRE_BREATH = 19,

};

class Player:public BINDU::Entity
{
private:
	BINDU::Sprite m_sprite;
	MoveSet moves;
public:
	Player();
	~Player();
	void Init();
	virtual void Update(float dt);
	virtual void Move();
	virtual void Animate();
	virtual void Draw(BINDU::Graphics* graphics);
	void ProcessInput();
};
