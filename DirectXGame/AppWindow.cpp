#include "AppWindow.h"


AppWindow::AppWindow()
{
}


AppWindow::~AppWindow()
{
}
void AppWindow::onCreate()
{
	Window::onCreate();
	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();
	RECT rc = getClientWindowRect();
	
	m_swap_chain->init(this->m_hwnd, rc.right- rc.left, rc.bottom - rc.top);
	


}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	GraphicsEngine::get()->getImmediateDeviceContext()->ClearRenderTargetColor
	(this->m_swap_chain, 1, 1, 1, 1);




	m_swap_chain->present(false);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_swap_chain->release();
	GraphicsEngine::get()->release();

}
