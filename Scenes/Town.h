#pragma once

/*
// Input Assembly �ܰ� -> Vertex Stage -> Resster Stage -> Pixel Stage -> Out merger 
IA = �ڵ�
VS, RS, PS = Shader Language
*/
class Town : public Scene
{
private:	//private instance variable
	Texture* backGround_ = nullptr;
	Texture* townTerrain_ = nullptr;
	class Line* m_pGroundLine = nullptr;
	class Line* m_pCeilingLine = nullptr;
	class NPC* npc = nullptr;
public:	//�� ��
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
