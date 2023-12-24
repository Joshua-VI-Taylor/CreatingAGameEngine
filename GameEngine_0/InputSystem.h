#pragma once

#include "InputListener.h"
#include <unordered_set>
#include "Point.h"
class InputSystem
{
private:
	InputSystem();
	~InputSystem();
public:
	void update();
	void addListener(InputListener* listener);
	void removeListener(InputListener* listener);

	void SetCursorPosition(const Point& pos);
	void showCursor(bool show);
public:
	static InputSystem* get();
	static void create();
	static void release();
private:
	std::unordered_set<InputListener*> m_set_listeners;
	short m_keys_state[256] = {};
	short m_old_keys_state[256] = {};
	Point m_old_mouse_pos;
	bool m_first_time = 1;
	static InputSystem* m_system;
};