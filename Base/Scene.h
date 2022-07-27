#pragma once
// �߻�ȭ Class ����
// C++ class Ư¡
//	1. ĸ��ȭ : ������, ����� ���� ��ҿ� �� ���ִ�. ������ ������ �����͸� �ٷ�� ������� ���� ���� ���� ��.

//	2. ������ : override, overload �θ�Ŭ�������� �������� ���� �Լ��� �ڽ� Ŭ���� ������ �������̵� �Ǿ� ���Ǵ� ��
//	3. �������� : public, protected, private ������  (��� OOP�� �߿��Ѱ� �ƴϰ�)
//	4. ���/�߻�ȭ:  ���� ������ Ư¡�� ���� ������ �����޴� �� / ������ �Ӽ��̳� ����� ���� �̸��� ���̴� ��
		// --> �߻�Ŭ������ ��������?: virtual �ް��ִ°Ű�, ��ӹ޾� ���� ������ �� ����ؾ� �ϴ� �� �ٵ� Scene�� �߻�Ŭ�����ΰ� �ƴϾ�
		
class Scene
{
private:
	string m_strTest;
protected:
	string m_strSceneName;	//���� Scene �̸�
	string m_strNextSceneName;	//���� Scene�� �̸�
	bool _Active = true;	//���� Scene�� Ȱ��ȭ ����
	bool _Reset = true;	// GOAL�� ������ �� ��� ���� Scene���� ��� �� ChangeScene�� ȣ��� �� reset�� �ȵǰԲ�.
	bool _Display = false;// Map�� �����͸� ǥ�� ���ٱ� ����.
	int _SceneID = 0;
public:
	virtual void Update() = 0;	// ��ӹ��� Ŭ�������� �����϶�� ��
	virtual void Render() = 0;	// ���� �����Լ� : java�� �������̽��� ����ѵ� 
	virtual void ChangeScene() = 0; // Scene ����� ������ ��� Scene�� �� �Լ��� ȣ���
	virtual void ExitScene() = 0;	// �� ���� �� ���� �����ؾ� �� �Լ�

	//Setter
	void SetActive(bool active)			{ _Active = active; };	//�������� inline �Լ��� �ζ��� �ٿ���� ����
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

