#pragma once

/*
// Input Assembly 단계 -> Vertex Stage -> Resster Stage -> Pixel Stage -> Out merger 
IA = 코딩
VS, RS, PS = Shader Language
*/
class Town : public Scene
{
private:	//private instance variable
	Texture* backGround_ = nullptr;
	class Player* tempPlayer_ = nullptr;
	class Weapon* _tempWeapon = nullptr;
public:	//생 소
	Town();
	~Town();
public:	//override
	void Update() override;
	void Render() override;
	void ChangeScene() override;

public:	//public instance method
};
