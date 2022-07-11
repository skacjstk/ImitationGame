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
	class Line* m_pGroundLine = nullptr;
public:	//생 소
	Town();
	~Town();
public:	//override
	void Update() override;
	void Render() override;
	void ChangeScene() override;
	void ExitScene() override;
	Line* GetLines() override { return m_pGroundLine; }

public:	//public instance method
	void TestGroundCheck();
};
