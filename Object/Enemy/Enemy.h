#pragma once
class Enemy : public GameActor
{

public:
	virtual void Update(Matrix V, Matrix P) override {};
	virtual void Render() override {};
	virtual void Reset() override {};
};