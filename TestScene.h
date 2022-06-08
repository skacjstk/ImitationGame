#pragma once

/*
// Input Assembly �ܰ� -> Vertex Stage -> Resster Stage -> Pixel Stage -> Out merger 
IA = �ڵ�
VS, RS, PS = Shader Language
*/
class TestScene : public Scene
{
private:	//private instance variable
	Texture* _texture = nullptr;
	Animation* _animation = nullptr;
public:	//�� ��
	TestScene();
	~TestScene();
public:	//override
	void Update() override;
	void Render() override;
	void ChangeScene() override;

public:	//public instance method
};
