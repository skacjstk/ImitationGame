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
	Texture* mountain_ = nullptr;
	Texture* forest_ = nullptr;
	Texture* townTerrain_ = nullptr;
	class Line* m_pGroundLine = nullptr;
	class Line* m_pCeilingLine = nullptr;
	class NPC* npc = nullptr;
	class Player* tempPlayer = nullptr;
	float cameraMoveX = 0.0f;
	float cameraMoveY = 0.0f;
public:	//생 소
	Town();
	~Town();
public:	//override
	void Update() override;
	void Render() override;
	void ChangeScene() override;
	void ExitScene() override;
	Line* GetGroundLines() override { return m_pGroundLine; }
	Line* GetCeilingLines() override { return m_pCeilingLine; }

};
