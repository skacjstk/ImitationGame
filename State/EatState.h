#pragma once
class HS01_Intro : public GameObject
{
public: // ������ & �Ҹ���
	HS01_Intro();
	~HS01_Intro();
public:	// ���� �ν��Ͻ� ���� 
	void  Update(Matrix V, Matrix P);
	void  Render();
	void  UpdateColor(Color color);

};
