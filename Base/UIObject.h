#pragma once
class UIObject : public GameObject
{
protected:
	bool _activate = false;
	UIObject* tabGroup[4] = { nullptr, };	// 상하좌우를 클릭할 때 버튼의 포커스를 옮기기 위한 포인터
	std::function<void(int)> _fpEnterCallback = nullptr;	// 해당 버튼이 클릭되었을 때 수행할 함수의 콜백
	int _ID;	// 콜백함수 인자(버튼의 index용)
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
