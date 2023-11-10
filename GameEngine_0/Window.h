#pragma once
#include "Windows.h"
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
class Window
{

public:
	//Initialize the window
	Window();

	bool isRun();

	RECT getClientWindowRect();
	//EVENTS
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();

	virtual void onMouseWheelTurn(int axis)=0;
	//Release the window
	~Window();
private:
	bool broadcast();
protected:
	HWND m_hwnd;
	bool m_is_run;
	bool m_is_init = false;
	friend LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};