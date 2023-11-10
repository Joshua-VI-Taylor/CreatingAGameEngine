#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputListener.h"
#include "Matrix4x4.h"
#include "Vector2D.h"
class AppWindow : public Window, public InputListener
{
public:
	AppWindow();

	void update();

	~AppWindow();

	// Inherited via Window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
	virtual void onKillFocus() override;
	virtual void onFocus() override;
	// Inherited via InputListener
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& mouse_pos) override;
	virtual void onLeftMouseDown(const Point& mouse_pos)override;
	virtual void onLeftMouseUp(const Point& mouse_pos)override;
	virtual void onRightMouseDown(const Point& mouse_pos)override;
	virtual void onRightMouseUp(const Point& mouse_pos)override;
	virtual void onMouseWheelTurn(int axis) override;

private:
	SwapChainPtr m_swap_chain;
	VertexBufferPtr m_vb;
	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;
	ConstantBufferPtr m_cb;
	IndexBufferPtr m_ib;
private:
	long m_old_delta;
	long m_new_delta;
	float m_delta_time;

	float m_delta_pos;
	float m_delta_scale;
	float m_delta_rot;
	bool m_MouseRightDown = false;
	float m_rot_x = 0.0f;
	float m_rot_y = 0.0f;
	//float m_forward = 0.0f;
	Vector2D m_cam_dir;
	Matrix4x4 m_world_cam;
};