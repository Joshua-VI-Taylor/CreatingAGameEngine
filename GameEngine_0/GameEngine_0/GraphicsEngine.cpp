#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"


GraphicsEngine::GraphicsEngine()
{
}

bool GraphicsEngine::init()
{
	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT num_driver_types = ARRAYSIZE(driver_types);

	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	UINT num_feature_levels = ARRAYSIZE(feature_levels);

	HRESULT res = 0;

	for (UINT driver_type_index = 0; driver_type_index < num_driver_types;)
	{
		res = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL, feature_levels,
			num_feature_levels, D3D11_SDK_VERSION, &m_d3d_device, &m_feature_level, &m_imm_context);
		if (SUCCEEDED(res))
			break;
		++driver_type_index;
	}
	if (FAILED(res))
	{
		return false;
	}

	m_imm_device_context = new DeviceContext(m_imm_context);

	m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
	m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);

	return true;
}


bool GraphicsEngine::release()
{
	if (m_vs)m_vs->Release();
	if (m_ps)m_ps->Release();

	if (m_vsblob)m_vsblob->Release();
	if (m_psblob)m_psblob->Release();

	m_dxgi_device->Release();
	m_dxgi_adapter->Release();
	m_dxgi_factory->Release();

	m_imm_device_context->release();


	m_d3d_device->Release();
	return true;
}

GraphicsEngine::~GraphicsEngine()
{
}

SwapChain* GraphicsEngine::createSwapChain()
{
	return new SwapChain();
}


DeviceContext* GraphicsEngine::getImmediateDeviceContext()
{
	return this->m_imm_device_context;
}

VertexBuffer* GraphicsEngine::createVertexBuffer()
{
	return new VertexBuffer();
}

bool GraphicsEngine::createShaders()
{
	//A variable the hold messages corresponding to errors 
	ID3DBlob* errblob = nullptr;
	
	//The D3DCompileFromFile Compile the shader code or recipe for creating a shader to binary so a shader can be created and initazlize 
	//@m_vsblob is a Referrence to the Compiled Vertex shader code
	D3DCompileFromFile(L"VertexShader.hlsl", nullptr, nullptr, "vsmain", "vs_5_0", NULL, NULL, &m_vsblob, &errblob);
	//@m_psblob is a Referrence to the Compiled Pixel shader code.
	D3DCompileFromFile(L"PixelShader.hlsl", nullptr, nullptr, "psmain", "ps_5_0", NULL, NULL, &m_psblob, &errblob);
	//Creates a shader object from the Precompiled shader code, which I can use to manipulate vertices in the 3D Grahpics rendering pipline
	m_d3d_device->CreateVertexShader(m_vsblob->GetBufferPointer(), m_vsblob->GetBufferSize(), nullptr, &m_vs);

	m_d3d_device->CreatePixelShader(m_psblob->GetBufferPointer(), m_psblob->GetBufferSize(), nullptr, &m_ps);
	return true;
}

bool GraphicsEngine::setShaders()
{
	m_imm_context->VSSetShader(m_vs, nullptr, 0);
	m_imm_context->PSSetShader(m_ps, nullptr, 0);
	return true;
	//
}

void GraphicsEngine::getShaderBufferAndSize(void** bytecode, UINT* size)
{
	*bytecode = this->m_vsblob->GetBufferPointer();
	*size = (UINT)this->m_vsblob->GetBufferSize();
}

GraphicsEngine* GraphicsEngine::get()
{
	static GraphicsEngine engine;
	return &engine;
}
