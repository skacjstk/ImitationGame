#pragma once
class UIObject : public GameObject
{
protected:
	bool _activate = false;
	UIObject* tabGroup[4] = { nullptr, };	// �����¿츦 Ŭ���� �� ��ư�� ��Ŀ���� �ű�� ���� ������
	std::function<void(int)> _fpEnterCallback = nullptr;	// �ش� ��ư�� Ŭ���Ǿ��� �� ������ �Լ��� �ݹ�
	int _ID;	// �ݹ��Լ� ����(��ư�� index��)
public:
	UIObject() {};
	~UIObject() {};
	// Setter
	void SetID(int id) { _ID = id; }
	void SetActivate(bool activate) { _activate = activate; }
	void SetLeftTabGroup(UIObject* object) { tabGroup[0] = object; }
	void SetRightTabGroup(UIObject* object) { tabGroup[1] = object; }
	void SetTopTabGroup(UIObject* object) { tabGroup[2] = object; }
	void SetBottomTabGroup(UIObject* object) { tabGroup[3] = object; }
	void SetCallback(std::function<void(int)> cbFunction) { _fpEnterCallback = cbFunction; }
public:
	bool IsActivate() { return _activate; }
};
