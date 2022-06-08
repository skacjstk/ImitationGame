#pragma once

/*
// Input Assembly 단계 -> Vertex Stage -> Resster Stage -> Pixel Stage -> Out merger 
IA = 코딩
VS, RS, PS = Shader Language
*/
class TestScene : public Scene
{
private:	//private instance variable
	Texture* _texture = nullptr;
	Animation* _animation = nullptr;
public:	//생 소
	TestScene();
	~TestScene();
public:	//override
	void Update() override;
	void Render() override;
	void ChangeScene() override;

public:	//public instance method
};
