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

class Player:public BINDU::SceneObject
{
private:
	BINDU::Sprite m_sprite;
	BINDU::Animator m_animator;
	MoveSet moves;
	BINDU::Vec2f m_velocity{};
	BINDU::Bnd_Rect_F m_srcRect{};
public:
	Player();
	~Player();
	void Init();
	virtual void Update(float dt) override;
	virtual void Move();
	virtual void Animate();
	virtual void Draw(BINDU::Graphics* graphics) override;
	virtual void ProcessInput() override;
};
