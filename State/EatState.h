#pragma once
class HS01_Intro : public GameObject
{
public: // 생성자 & 소멸자
	HS01_Intro();
	~HS01_Intro();
public:	// 공개 인스턴스 변수 
	void  Update(Matrix V, Matrix P);
	void  Render();
	void  UpdateColor(Color color);

};
