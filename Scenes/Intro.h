#pragma once

/*
// Input Assembly 단계 -> Vertex Stage -> Resster Stage -> Pixel Stage -> Out merger 
IA = 코딩
VS, RS, PS = Shader Language
*/
class Intro : public Scene
{
private:	//private instance variable
	Texture* titleImage_ = nullptr;
	Texture* backGround_ = nullptr;
	class Button* introButton_[3] = { nullptr, };
public:	//생 소
	Intro();
	~Intro();
public:	//override
	void Update() override;
	void Render() override;
	void ChangeScene() override;
	void ExitScene() override;

public:	//public instance method
private:
	void OnButton(int id);
};
