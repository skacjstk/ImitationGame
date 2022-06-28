#pragma once

/*
// Input Assembly �ܰ� -> Vertex Stage -> Resster Stage -> Pixel Stage -> Out merger 
IA = �ڵ�
VS, RS, PS = Shader Language
*/
class Intro : public Scene
{
private:	//private instance variable
	Texture* titleImage_ = nullptr;
	Texture* backGround_ = nullptr;
	class Button* introButton_[3] = { nullptr, };
public:	//�� ��
	Intro();
	~Intro();
public:	//override
	void Update() override;
	void Render() override;
	void ChangeScene() override;

public:	//public instance method
private:
	void OnButton(int id);
};
