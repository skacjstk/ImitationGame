#pragma once
// 추상화 Class 설계
// C++ class 특징
//	1. 캡슐화 : 데이터, 기능을 같은 장소에 둘 수있다. 데이터 구조와 데이터를 다루는 방법들을 결합 시켜 묶는 것.

//	2. 다형성 : override, overload 부모클레스에서 물려받은 가상 함수를 자식 클래스 내에서 오버라이딩 되어 사용되는 것
//	3. 정보은닉 : public, protected, private 같은거  (얘는 OOP의 중요한건 아니고)
//	4. 상속/추상화:  상위 개념의 특징을 하위 개념이 물려받는 것 / 공통의 속성이나 기능을 묶어 이름을 붙이는 것
		// --> 추상클래스가 뭐였더라?: virtual 달고있는거고, 상속받아 실제 구현한 후 사용해야 하는 거 근데 Scene이 추상클래스인건 아니야
		
class Scene
{
private:
	string m_strTest;
protected:
	string m_strSceneName;	//현재 Scene 이름
	string m_strNextSceneName;	//다음 Scene의 이름
	bool _Active = true;	//현재 Scene의 활성화 유무
	bool _Reset = true;	// GOAL을 만났을 때 잠시 현재 Scene에서 벗어난 후 ChangeScene이 호출될 때 reset이 안되게끔.
	bool _Display = false;// Map의 데이터를 표시 해줄까 말까.
	int _SceneID = 0;
public:
	virtual void Update() = 0;	// 상속받은 클래스에서 구현하라는 뜻
	virtual void Render() = 0;	// 순수 가상함수 : java의 인터페이스와 비슷한데 
	virtual void ChangeScene() = 0; // Scene 변경시 변경할 대상 Scene에 이 함수가 호출됨
	virtual void ExitScene() = 0;	// 씬 나올 때 따로 설정해야 할 함수

	//Setter
	void SetActive(bool active)			{ _Active = active; };	//옜날에는 inline 함수는 인라인 붙였어야 했음
	void SetReset(bool value)			{ _Reset = value; }
	void SetDisplay(bool value)			{ _Display = value; }
	void SetName(string strName)		{ m_strSceneName = strName; };
	void setNextName(string strName)	{ m_strNextSceneName = strName; };
	void SetSceneID(int number)			{ _SceneID = number; }
	//Getter
	bool IsActive()		{ return _Active; };
	bool IsDisplay()	{ return _Display; }
	bool IsReset()		{ return _Reset; }
	int	GetSceneID()	{ return _SceneID; }
	string GetName()	{ return m_strSceneName; };
	auto GetNextName()	{ return m_strNextSceneName; };
	virtual class Line* GetGroundLines() { return nullptr; }
	virtual class Line* GetCeilingLines() { return nullptr; }
	virtual class Line* GetPlatformLines() { return nullptr; }
//	Scene();
//	~Scene();
};

