//-----------------------------------------------------------------------------
// File: particle.cpp
//
// Desc: 粒子精灵实现简单粒子效果
// 
//-----------------------------------------------------------------------------
#include <d3d9.h>
#include "resource.h"
#include <Windows.h>
#include <d3dx9.h>

//# pragma warning (disable:4819)
//-----------------------------------------------------------------------------
// 全局变量
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D       = NULL; // 用于创建设备的d3d对象
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // 渲染设备
LPDIRECT3DVERTEXBUFFER9 g_pd3dVBuffer =NULL;  //顶点缓存
LPDIRECT3DTEXTURE9      g_pd3dTexture=NULL;  //纹理
//自定义顶点结构体
struct PointVertex
{
  FLOAT x,y,z;
	DWORD color;                         //无符长整形
};

//确定顶点格式，通知dx系统顶点结构由一个空间坐标和一个点颜色组成
// 点的格式是经过光照和变换的，无需设置光照和变换矩阵
#define D3DFVF_POINTVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
//-----------------------------------------------------------------------------
// Name: InitD3D()
// Describe: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D( HWND hWnd )
{
	// Create the D3D object, which is needed to create the D3DDevice.
	if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return E_FAIL;

	// Set up the structure used to create the D3DDevice. Most parameters are
	// zeroed out. We set Windowed to TRUE, since we want to do D3D in a
	// window, and then set the SwapEffect to "discard", which is the most
	// efficient method of presenting the back buffer to the display.  And 
	// we request a back buffer format that matches the current desktop display 
	// format.
	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.BackBufferWidth  =1400;
	d3dpp.BackBufferHeight =900;
	// Create the Direct3D device. Here we are using the default adapter (most
	// systems only have one, unless they have multiple graphics hardware cards
	// installed) and requesting the HAL (which is saying we want the hardware
	// device rather than a software one). Software vertex processing is 
	// specified since we know it will work on all cards. On cards that support 
	// hardware vertex processing, though, we would see a big performance gain 
	// by specifying hardware vertex processing.
	if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice ) ) )
	{
		return E_FAIL;
	}

	//关闭光照
	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE);

	// Device state would normally be set here

	return S_OK;
}





//-----------------------------------------------------------------------------
// Name:InitObject()
// Desc：将填充的顶点信息拷贝到创建好的顶点缓冲区
//-----------------------------------------------------------------------------
HRESULT InitObject(VOID)
{
	//从Image文件创建纹理,如果文件不在当前文件夹 则尝试父文件夹
	if (FAILED(D3DXCreateTextureFromFile(g_pd3dDevice,"particle.bmp",&g_pd3dTexture)))
	{
		if (FAILED(D3DXCreateTextureFromFile(g_pd3dDevice,"../particle.bmp",&g_pd3dTexture)))
		{
			return S_FALSE;

		}

	}


//填充顶点
	PointVertex m_Vertices[]=
	{
		{-5.0f, 5.0f, 0.0f, 0xffff0000},
		{ 0.0f, 5.0f, 0.0f, 0xffff0000},
		{ 5.0f, 5.0f, 0.0f, 0xffff0000},

		{-5.0f, 0.0f, 0.0f, 0xff00ff00},
		{ 0.0f, 0.0f, 0.0f, 0xff00ff00},
		{ 5.0f, 0.0f, 0.0f, 0xff00ff00},

		{-5.0f, -5.0f, 0.0f, 0xff0000ff},
		{ 0.0f, -5.0f, 0.0f, 0xff0000ff},
		{ 5.0f, -5.0f, 0.0f, 0xff0000ff}

	};
	//创建顶点缓冲区

	if(FAILED(g_pd3dDevice->CreateVertexBuffer(sizeof(m_Vertices), 
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS,D3DFVF_POINTVERTEX
		,D3DPOOL_DEFAULT,&g_pd3dVBuffer,NULL)))
	return E_FAIL;

	VOID* pVertices;
	g_pd3dVBuffer->Lock(0,sizeof(m_Vertices),(void**)&pVertices,0);
	memcpy(pVertices,m_Vertices,sizeof(m_Vertices));
	g_pd3dVBuffer->Unlock();

    return S_OK;


}
//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{
	if( g_pd3dDevice != NULL) 
		g_pd3dDevice->Release();

	if( g_pD3D != NULL)
		g_pD3D->Release();
}


VOID SetupMatrices()
{
	//// For our world matrix, we will just leave it as the identity
	//D3DXMATRIXA16 matWorld;
	//D3DXMatrixIdentity( &matWorld );
	//D3DXMatrixRotationX( &matWorld, timeGetTime()/1000.0f );
	//// g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt( 0.0f, 0.0f,-3.0f );
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f );
	g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}


//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
VOID Render()
{
	SetupMatrices();
	if( NULL == g_pd3dDevice)
		return;

	// 清理后台缓冲并将背景设为黑色
	g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

	// Begin the scene
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{  
		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);        //开启blend
        g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
        g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
		g_pd3dDevice->SetRenderState(D3DRS_POINTSPRITEENABLE,TRUE);
        g_pd3dDevice->SetRenderState(D3DRS_POINTSCALEENABLE,TRUE);
		g_pd3dDevice->SetRenderState(D3DRS_POINTSIZE,300);

		g_pd3dDevice->SetStreamSource(0,g_pd3dVBuffer,0,sizeof(PointVertex));
		g_pd3dDevice->SetFVF(D3DFVF_POINTVERTEX);
		g_pd3dDevice->SetTexture(0,g_pd3dTexture);
		g_pd3dDevice->DrawPrimitive(D3DPT_POINTLIST,0,9);
		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
		g_pd3dDevice->EndScene();

	}
	//{　　                                                                                                                         
	//


	//	
	//	g_pd3dDevice->EndScene();
	//}

	// Present the backbuffer contents to the display
	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_KEYDOWN:
		if (wParam==VK_ESCAPE)
		{
			::DestroyWindow(hWnd);
		}
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage( 0 );
		return 0;
    
	case WM_PAINT:
		Render();
		ValidateRect( hWnd, NULL );
		return 0;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}




//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	// Register the window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, 
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"D3D Particle", NULL };
	RegisterClassEx( &wc );

	// Create the application's window
	HWND hWnd = CreateWindow( "D3D Particle", "D3D Demo: Particle", 
		WS_OVERLAPPEDWINDOW, 120, 90, 1024, 768,
		GetDesktopWindow(), NULL, wc.hInstance, NULL );

	// Initialize Direct3D
	if( SUCCEEDED( InitD3D( hWnd ) ) )
	{ 
		if (SUCCEEDED(InitObject()))
		{

			
		    ShowWindow( hWnd, SW_SHOWDEFAULT );// Show the window
		    UpdateWindow( hWnd );

		     // Enter the message loop
	        MSG msg; 
		   while( GetMessage( &msg, NULL, 0, 0 ) )
		    {
			    TranslateMessage(&msg);
			    DispatchMessage( &msg );
		    }
		}

	
	
	}

	UnregisterClass( "D3D Particle", wc.hInstance );
	return 0;
}



