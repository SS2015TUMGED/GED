#include <stdio.h>
#include <tchar.h>

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdint>

#include "dxut.h"
#include "DXUTmisc.h"
#include "DXUTcamera.h"
#include "DXUTgui.h"
#include "DXUTsettingsDlg.h"
#include "SDKmisc.h"

#include "d3dx11effect.h"

#include "Terrain.h"
#include "GameEffect.h"
#include "ConfigParser.h"
#include "Mesh.h"
#include "CustomData.h"

#include "debug.h"
#include <map>
#include "Ememy.h"
#include <random>
#include <time.h>

using namespace std;
using namespace DirectX;

//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------

// Camera
struct CAMERAPARAMS {
	float   fovy;
	float   aspect;
	float   nearPlane;
	float   farPlane;
}                                       g_cameraParams;
float                                   g_cameraMoveScaler = 1000.f;
float                                   g_cameraRotateScaler = 0.01f;
CFirstPersonCamera                      g_camera;               // A first person camera

// User Interface
CDXUTDialogResourceManager              g_dialogResourceManager; // manager for shared resources of dialogs
CD3DSettingsDlg                         g_settingsDlg;          // Device settings dialog
CDXUTTextHelper*                        g_txtHelper = NULL;
CDXUTDialog                             g_hud;                  // dialog for standard controls
CDXUTDialog                             g_sampleUI;             // dialog for sample specific controls

// assignment 05 : remove the var
//ID3D11InputLayout*                      g_terrainVertexLayout; // Describes the structure of the vertex buffer to the input assembler stage


bool                                    g_terrainSpinning = true;
XMMATRIX                                g_terrainWorld; // object- to world-space transformation


// Scene information
XMVECTOR                                g_lightDir;
Terrain									g_terrain;

GameEffect								g_gameEffect; // CPU part of Shader

//Assignment 08: Enemy Handling
float								g_SpawnTimer = 6.0f;
std::default_random_engine Ememy::rng;

//-------------------------------------------------------------------------------------
// Our Killer Variables
// ------------------------------------------------------------------------------------
ConfigParser parser;
//Mesh*        g_cockpitMesh = nullptr;

//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------
#define IDC_TOGGLEFULLSCREEN    1
#define IDC_TOGGLEREF           2
#define IDC_CHANGEDEVICE        3
#define IDC_TOGGLESPIN          4
#define IDC_RELOAD_SHADERS		101

//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                          void* pUserContext );
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );

bool CALLBACK IsD3D11DeviceAcceptable( const CD3D11EnumAdapterInfo *, UINT , const CD3D11EnumDeviceInfo *,
                                       DXGI_FORMAT, bool, void* );
HRESULT CALLBACK OnD3D11CreateDevice( ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
                                     void* pUserContext );
HRESULT CALLBACK OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                         const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
void CALLBACK OnD3D11ReleasingSwapChain( void* pUserContext );
void CALLBACK OnD3D11DestroyDevice( void* pUserContext );
void CALLBACK OnD3D11FrameRender( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime,
                                 float fElapsedTime, void* pUserContext );

void InitApp();
void RenderText();

// our added functions
void DeinitApp();




void ReleaseShader();
HRESULT ReloadShader(ID3D11Device* pd3dDevice);

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

#pragma region Random Number Generation
	int seed = (int)time(NULL);
	cout << "seed: " << seed << endl << endl;

	//seed the rng
	Ememy::rng.seed(seed);
#pragma endregion


    // Old Direct3D Documentation:
    // Start > All Programs > Microsoft DirectX SDK (June 2010) > Windows DirectX Graphics Documentation

    // DXUT Documentaion:
    // Start > All Programs > Microsoft DirectX SDK (June 2010) > DirectX Documentation for C++ : The DirectX Software Development Kit > Programming Guide > DXUT
	
    // New Direct3D Documentaion (just for reference, use old documentation to find explanations):
    // http://msdn.microsoft.com/en-us/library/windows/desktop/hh309466%28v=vs.85%29.aspx


    // Initialize COM library for windows imaging components
    /*HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (hr != S_OK)
    {
        MessageBox(0, L"Error calling CoInitializeEx", L"Error", MB_OK | MB_ICONERROR);
        exit(-1);
    }*/

    // Set DXUT callbacks
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackKeyboard( OnKeyboard );
    DXUTSetCallbackFrameMove( OnFrameMove );
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );

    DXUTSetCallbackD3D11DeviceAcceptable( IsD3D11DeviceAcceptable );
    DXUTSetCallbackD3D11DeviceCreated( OnD3D11CreateDevice );
    DXUTSetCallbackD3D11SwapChainResized( OnD3D11ResizedSwapChain );
    DXUTSetCallbackD3D11SwapChainReleasing( OnD3D11ReleasingSwapChain );
    DXUTSetCallbackD3D11DeviceDestroyed( OnD3D11DestroyDevice );
    DXUTSetCallbackD3D11FrameRender( OnD3D11FrameRender );
    //DXUTSetIsInGammaCorrectMode(false);

    InitApp();
    DXUTInit( true, true, NULL ); // Parse the command line, show msgboxes on error, no extra command line params
    DXUTSetCursorSettings( true, true );
    DXUTCreateWindow( L"TODO: Insert Title Here" ); // You may change the title

    DXUTCreateDevice( D3D_FEATURE_LEVEL_10_0, true, 1280, 720 );

    DXUTMainLoop(); // Enter into the DXUT render loop

	// assignment 06
	DXUTShutdown();
	DeinitApp();


    return DXUTGetExitCode();
}

//--------------------------------------------------------------------------------------
// Initialize the app 
//--------------------------------------------------------------------------------------
void InitApp()
{
    HRESULT hr;
    WCHAR path[MAX_PATH];

    // Parse the config file

    V(DXUTFindDXSDKMediaFileCch(path, MAX_PATH, L"..\\..\\game.cfg"));
	char pathA[MAX_PATH];
	size_t size;
	wcstombs_s(&size, pathA, path, MAX_PATH);


	// load the config file
	parser.load(pathA);
	
	// ************************** Assignment 06 ***************************
	// Set g_terrainSpinning to 0 (= disable spinning by default). You could also add this to your config. 
	// apply terrainSpinning from the parser
	g_terrainSpinning = parser.terrainSpinning;


    // Intialize the user interface

    g_settingsDlg.Init( &g_dialogResourceManager );
    g_hud.Init( &g_dialogResourceManager );
    g_sampleUI.Init( &g_dialogResourceManager );

    g_hud.SetCallback( OnGUIEvent );
    int iY = 30;
    int iYo = 26;
    g_hud.AddButton( IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 0, iY, 170, 22 );
    g_hud.AddButton( IDC_TOGGLEREF, L"Toggle REF (F3)", 0, iY += iYo, 170, 22, VK_F3 );
    g_hud.AddButton( IDC_CHANGEDEVICE, L"Change device (F2)", 0, iY += iYo, 170, 22, VK_F2 );

	g_hud.AddButton (IDC_RELOAD_SHADERS, L"Reload shaders (F5)", 0, iY += 24, 170, 22, VK_F5);
    
    g_sampleUI.SetCallback( OnGUIEvent ); iY = 10;
    iY += 24;
    g_sampleUI.AddCheckBox( IDC_TOGGLESPIN, L"Toggle Spinning", 0, iY += 24, 125, 22, g_terrainSpinning );   


	// ************************************* Assignment 06 ****************************************************
	// create new mesh
	//g_cockpitMesh = parser.g_Meshes["Cockpit"];



}

//****************************************************************************************************
// **************************************** our methods **********************************************
//****************************************************************************************************

void DeinitApp(){
	for (auto iterator = parser.g_Meshes.begin(); iterator != parser.g_Meshes.end(); iterator++) {
		SAFE_DELETE(iterator->second);
	}
	parser.g_Meshes.clear();
	Ememy::g_EnemyInstances.clear();
	//for (auto iterator = parser.enemys.begin(); iterator != parser.enemys.end(); iterator++) {
	//	parser.enemys.erase(iterator);
	//}
	parser.enemys.clear();
}


//--------------------------------------------------------------------------------------
// Render the help and statistics text. This function uses the ID3DXFont interface for 
// efficient text rendering.
//--------------------------------------------------------------------------------------
void RenderText()
{
    g_txtHelper->Begin();
    g_txtHelper->SetInsertionPos( 5, 5 );
    g_txtHelper->SetForegroundColor(XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f));
    g_txtHelper->DrawTextLine( DXUTGetFrameStats(true)); //DXUTIsVsyncEnabled() ) );
    g_txtHelper->DrawTextLine( DXUTGetDeviceStats() );
    g_txtHelper->End();
}

//--------------------------------------------------------------------------------------
// Reject any D3D11 devices that aren't acceptable by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D11DeviceAcceptable( const CD3D11EnumAdapterInfo *, UINT, const CD3D11EnumDeviceInfo *,
        DXGI_FORMAT, bool, void* )
{
    return true;
}

//--------------------------------------------------------------------------------------
// Specify the initial device settings
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
	UNREFERENCED_PARAMETER(pDeviceSettings);
	UNREFERENCED_PARAMETER(pUserContext);

    // For the first device created if its a REF device, optionally display a warning dialog box
    static bool s_bFirstTime = true;
    if( s_bFirstTime )
    {
        s_bFirstTime = false;
        if (pDeviceSettings->d3d11.DriverType == D3D_DRIVER_TYPE_REFERENCE)
        {
            DXUTDisplaySwitchingToREFWarning();
        }
    }
    //// Enable anti aliasing
    pDeviceSettings->d3d11.sd.SampleDesc.Count = 4;
    pDeviceSettings->d3d11.sd.SampleDesc.Quality = 1;

    return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D11 resources that aren't dependant on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D11CreateDevice( ID3D11Device* pd3dDevice,
        const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	

	UNREFERENCED_PARAMETER(pBackBufferSurfaceDesc);
	UNREFERENCED_PARAMETER(pUserContext);

    HRESULT hr;

	D3DX11_PASS_DESC pd2;
	pd2.pIAInputSignature;


    ID3D11DeviceContext* pd3dImmediateContext = DXUTGetD3D11DeviceContext(); // http://msdn.microsoft.com/en-us/library/ff476891%28v=vs.85%29
    V_RETURN( g_dialogResourceManager.OnD3D11CreateDevice( pd3dDevice, pd3dImmediateContext ) );
    V_RETURN( g_settingsDlg.OnD3D11CreateDevice( pd3dDevice ) );
    g_txtHelper = new CDXUTTextHelper( pd3dDevice, pd3dImmediateContext, &g_dialogResourceManager, 15 );

    V_RETURN( ReloadShader(pd3dDevice) );
    
	// In game.cpp: Create the input layout in OnD3D11CreateDevice() using Mesh::createInputLayout() 
	// (as pass use g_gameEffect.meshPass1),
	Mesh::createInputLayout(pd3dDevice, g_gameEffect.meshPass1);

    
    // Initialize the camera
	
	// ************************************************** Assignment 06
	// In game.cpp: In OnD3D11CreateDevice(), search for the part where the camera g_camera is initialized. 
	// Change the parameter Eye to be in the center of the terrain, i.e. set Eye.x and Eye.z to 0, and set Eye.y to an appropriate value 
	// depending on your heightfield (Hint: move  the camera initialization 
	// code to after the position where the heightfield is read; also the terrainHeight read from game.cfg might help). 

	XMVECTOR vEye = XMVectorSet(0.0f, 130.0f, 1.0f, 0.0f);   // Camera eye is here
    XMVECTOR vAt = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);               // ... facing at this position
    g_camera.SetViewParams(vEye, vAt); // http://msdn.microsoft.com/en-us/library/windows/desktop/bb206342%28v=vs.85%29.aspx
	g_camera.SetScalers(g_cameraRotateScaler, g_cameraMoveScaler);


	// Define the input layout
	const D3D11_INPUT_ELEMENT_DESC layout[] = // http://msdn.microsoft.com/en-us/library/bb205117%28v=vs.85%29.aspx
	{
		{ "SV_POSITION",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",         0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",       0, DXGI_FORMAT_R32G32_FLOAT,       0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof( layout ) / sizeof( layout[0] );

	// Create the input layout
    D3DX11_PASS_DESC pd;
	V_RETURN(g_gameEffect.pass0->GetDesc(&pd));
	
	
	//V_RETURN( pd3dDevice->CreateInputLayout( layout, numElements, pd.pIAInputSignature,
    //        pd.IAInputSignatureSize, &g_terrainVertexLayout ) );

	// Create the terrain

	// TODO: You might pass a ConfigParser object to the create function.
	//       Therefore you can adjust the TerrainClass accordingly
	V_RETURN(g_terrain.create(pd3dDevice));


	// *************************** Assignment 06 ******************************************
	// In game.cpp: In OnD3D11CreateDevice() call the create() method of g_CockpitMesh
	//g_cockpitMesh->create(pd3dDevice);

	// Assignment 07: Create all the new meshes
	/*parser.g_Meshes["Gatling"]->create(pd3dDevice);
	parser.g_Meshes["Plasma"]->create(pd3dDevice);
	parser.g_Meshes["Bare"]->create(pd3dDevice);
	parser.g_Meshes["Crazy_Tree"]->create(pd3dDevice);*/
	// TODO: maybe with for()
	for (const auto& kv : parser.g_Meshes) {
		kv.second->create(pd3dDevice);
	}

	/*for (map<string, Mesh*>::iterator it = parser.g_Meshes.begin(); it != parser.g_Meshes.end(); it++)
	{
		it->second->create(pd3dDevice);
	}*/

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11CreateDevice 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11DestroyDevice( void* pUserContext )
{
	UNREFERENCED_PARAMETER(pUserContext);

    g_dialogResourceManager.OnD3D11DestroyDevice();
    g_settingsDlg.OnD3D11DestroyDevice();
    DXUTGetGlobalResourceCache().OnDestroyDevice();
   // SAFE_RELEASE( g_terrainVertexLayout );
    
	// Destroy the terrain
	g_terrain.destroy();

    SAFE_DELETE( g_txtHelper );
    ReleaseShader();

	// don’t forget to release it in OnD3D11DestroyDevice() by calling Mesh::destroyInputLayout()	
	Mesh::destroyInputLayout();
	// assignment 06
	//g_cockpitMesh->destroy();
	// assignment 07
	for (const auto& kv : parser.g_Meshes) {
		kv.second->destroy();
	}
}

//--------------------------------------------------------------------------------------
// Create any D3D11 resources that depend on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
        const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	UNREFERENCED_PARAMETER(pSwapChain);
	UNREFERENCED_PARAMETER(pUserContext);

    HRESULT hr;
    
    // Intialize the user interface

    V_RETURN( g_dialogResourceManager.OnD3D11ResizedSwapChain( pd3dDevice, pBackBufferSurfaceDesc ) );
    V_RETURN( g_settingsDlg.OnD3D11ResizedSwapChain( pd3dDevice, pBackBufferSurfaceDesc ) );

    g_hud.SetLocation( pBackBufferSurfaceDesc->Width - 170, 0 );
    g_hud.SetSize( 170, 170 );
    g_sampleUI.SetLocation( pBackBufferSurfaceDesc->Width - 170, pBackBufferSurfaceDesc->Height - 300 );
    g_sampleUI.SetSize( 170, 300 );

    // Initialize the camera

    g_cameraParams.aspect = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
    g_cameraParams.fovy = 0.785398f;
    g_cameraParams.nearPlane = 1.f;
    g_cameraParams.farPlane = 5000.f;

    g_camera.SetProjParams(g_cameraParams.fovy, g_cameraParams.aspect, g_cameraParams.nearPlane, g_cameraParams.farPlane);
	
	// ********************************************* Assignment 06 *************************************************************
	// In game.cpp: Search for other occurrences of g_camera and disable the position movement of the camera
	g_camera.SetEnablePositionMovement(false);



	g_camera.SetRotateButtons(true, false, false);
	g_camera.SetScalers( g_cameraRotateScaler, g_cameraMoveScaler );
	g_camera.SetDrag( true );

	

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11ResizedSwapChain 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11ReleasingSwapChain( void* pUserContext )
{
	UNREFERENCED_PARAMETER(pUserContext);
    g_dialogResourceManager.OnD3D11ReleasingSwapChain();
}

//--------------------------------------------------------------------------------------
// Loads the effect from file
// and retrieves all dependent variables
//--------------------------------------------------------------------------------------
HRESULT ReloadShader(ID3D11Device* pd3dDevice)
{
    assert(pd3dDevice != NULL);

    HRESULT hr;

    ReleaseShader();
	V_RETURN(g_gameEffect.create(pd3dDevice));

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Release resources created in ReloadShader
//--------------------------------------------------------------------------------------
void ReleaseShader()
{
	g_gameEffect.destroy();
}

//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                          void* pUserContext )
{
	UNREFERENCED_PARAMETER(pUserContext);

    // Pass messages to dialog resource manager calls so GUI state is updated correctly
    *pbNoFurtherProcessing = g_dialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    // Pass messages to settings dialog if its active
    if( g_settingsDlg.IsActive() )
    {
        g_settingsDlg.MsgProc( hWnd, uMsg, wParam, lParam );
        return 0;
    }

    // Give the dialogs a chance to handle the message first
    *pbNoFurtherProcessing = g_hud.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;
    *pbNoFurtherProcessing = g_sampleUI.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;
        
    // Use the mouse weel to control the movement speed
    if(uMsg == WM_MOUSEWHEEL) {
        int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        g_cameraMoveScaler *= (1 + zDelta / 500.0f);
        if (g_cameraMoveScaler < 0.1f)
          g_cameraMoveScaler = 0.1f;
        g_camera.SetScalers(g_cameraRotateScaler, g_cameraMoveScaler);
    }

    // Pass all remaining windows messages to camera so it can respond to user input
    g_camera.HandleMessages( hWnd, uMsg, wParam, lParam );

    return 0;
}

//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	UNREFERENCED_PARAMETER(nChar);
	UNREFERENCED_PARAMETER(bKeyDown);
	UNREFERENCED_PARAMETER(bAltDown);
	UNREFERENCED_PARAMETER(pUserContext);

	// **************************** Assignment 06 ***************************************
	// In game.cpp: For debugging purposes, you still might want to be able to 
	// “fly around”. One possibility to achieve this is binding a hotkey that re-enables
	// the camera movement when pressed. To do so, add a similar line to OnKeyboard(): 
	// Enable position movement when the C-key is pressed if (nChar=='C' && bKeyDown)
	// { /* enable position movement here */ } 

	//Enable position movement when the C-key is pressed 
	if (nChar=='C' && bKeyDown) 
	{ 
		g_camera.SetEnablePositionMovement(true);

		} 

}

//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	UNREFERENCED_PARAMETER(nEvent);
	UNREFERENCED_PARAMETER(pControl);
	UNREFERENCED_PARAMETER(pUserContext);

    switch( nControlID )
    {
        case IDC_TOGGLEFULLSCREEN:
            DXUTToggleFullScreen(); break;
        case IDC_TOGGLEREF:
            DXUTToggleREF(); break;
        case IDC_CHANGEDEVICE:
            g_settingsDlg.SetActive( !g_settingsDlg.IsActive() ); break;
        case IDC_TOGGLESPIN:
            g_terrainSpinning = g_sampleUI.GetCheckBox( IDC_TOGGLESPIN )->GetChecked();
            break;
		case IDC_RELOAD_SHADERS:
			ReloadShader(DXUTGetD3D11Device ());
			break;
    }
}

//checks if a enemy is out of the map
bool out_of_map(Ememy::EnemyInstance &e) {
	return (DirectX::XMVectorGetByIndex(DirectX::XMVector3Length(e.pos), 0) > parser.getTerrainWidth() + 10);
}
//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{
	UNREFERENCED_PARAMETER(pUserContext);
	// Update the camera's position based on user input 
	g_camera.FrameMove(fElapsedTime);

	// Initialize the terrain world matrix
	// http://msdn.microsoft.com/en-us/library/windows/desktop/bb206365%28v=vs.85%29.aspx

	// Start with identity matrix
	g_terrainWorld = XMMatrixIdentity();

	// Assignment 05 Scale the terrainworld to the data form the .cfg file	
	g_terrainWorld *= XMMatrixScaling(parser.getTerrainWidth(), parser.getTerrainHeight(), parser.getTerrainDepth());

	if (g_terrainSpinning)
	{
		// If spinng enabled, rotate the world matrix around the y-axis
		g_terrainWorld *= XMMatrixRotationY(30.0f * DEG2RAD((float)fTime)); // Rotate around world-space "up" axis
	}

	// Set the light vector
	g_lightDir = XMVectorSet(1, 1, 1, 0); // Direction to the directional light in world space    
	g_lightDir = XMVector3Normalize(g_lightDir);

	//Enemy handling
	g_SpawnTimer += fElapsedTime;

	for (auto enemy : parser.enemys)
	{
		//check if it is time to spawn a new enemy
		if ((int)g_SpawnTimer % enemy.second.SpawnRate == 0)
		{
			//spawning new enemy
			if (enemy.second.Spawn == false){
				Ememy::spawn(enemy.second, parser.getTerrainWidth());
				parser.enemys[enemy.first].Spawn = true;
			}
		}
		else{
			if (enemy.second.Spawn == true)
				parser.enemys[enemy.first].Spawn = false;
		}
	}
	//deletes enemys from list
	Ememy::g_EnemyInstances.remove_if(out_of_map);

	//iterate over all list elements
	for (auto it = Ememy::g_EnemyInstances.begin(); it != Ememy::g_EnemyInstances.end(); it++)
	{
		it->pos = it->pos + fElapsedTime * it->vel;
	}
}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D11 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11FrameRender( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime,
        float fElapsedTime, void* pUserContext )
{
	UNREFERENCED_PARAMETER(pd3dDevice);
	UNREFERENCED_PARAMETER(fTime);
	UNREFERENCED_PARAMETER(pUserContext);

    HRESULT hr;

    // If the settings dialog is being shown, then render it instead of rendering the app's scene
    if( g_settingsDlg.IsActive() )
    {
        g_settingsDlg.OnRender( fElapsedTime );
        return;
    }     

    ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
	float clearColor[4] = {0.5f, 0.5f, 0.5f, 1.0f};
    pd3dImmediateContext->ClearRenderTargetView( pRTV, clearColor );
        
	if(g_gameEffect.effect == NULL) {
        g_txtHelper->Begin();
        g_txtHelper->SetInsertionPos( 5, 5 );
        g_txtHelper->SetForegroundColor( XMVectorSet( 1.0f, 1.0f, 0.0f, 1.0f ) );
        g_txtHelper->DrawTextLine( L"SHADER ERROR" );
        g_txtHelper->End();
        return;
    }

    // Clear the depth stencil
    ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
    pd3dImmediateContext->ClearDepthStencilView( pDSV, D3D11_CLEAR_DEPTH, 1.0, 0 );
    
    // Update variables that change once per frame
    XMMATRIX const view = g_camera.GetViewMatrix(); 
	// http://msdn.microsoft.com/en-us/library/windows/desktop/bb206342%28v=vs.85%29.aspx
    
	XMMATRIX const proj = g_camera.GetProjMatrix(); 
	// http://msdn.microsoft.com/en-us/library/windows/desktop/bb147302%28v=vs.85%29.aspx
   
	XMMATRIX worldViewProj = g_terrainWorld * view * proj;
	
	// vars for the transformation
	XMMATRIX mTrans, mScale, mRot, mAnim;

	// tmp vars for the transfomation matrices
	XMMATRIX tmp_worldEV, tmp_worldViewProjectionEV;

	V(g_gameEffect.worldViewProjectionEV->SetMatrix( ( float* )&worldViewProj ));
	V(g_gameEffect.lightDirEV->SetFloatVector((float*)&g_lightDir));

	// get the inverse transposed matrix for g_terrainWorld
	V(g_gameEffect.worldNormalsMatrix->SetMatrix((float*)&XMMatrixTranspose(XMMatrixInverse(nullptr, g_terrainWorld))));

	// Set the effect variable g_gameEffect.cameraPosWorldEV to the camera position in world 
	// space. For this you can use the value from g_camera.GetEyePt()
	V(g_gameEffect.cameraPosWorldEV->SetFloatVector((float*)&g_camera.GetEyePt()));

	g_terrain.render(pd3dImmediateContext, g_gameEffect.pass0);

	//Create the transformation matrices for the cockpit mesh and set the corresponding effect variables of g_gameEffect: 
	for (auto cockpitObject : parser.cockpitObjects) {

		if (parser.g_Meshes.find(cockpitObject.Name) == parser.g_Meshes.end()) {
			cout << "Error: Mesh for name(" << cockpitObject.Name << ") does not exist." << endl;
			system("pause");
		}

		// set the rotation matrix
		//               convert degree to radian
		//               radian     = degree * pi / 180;
		// https://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.matrix.xmmatrixrotationy%28v=vs.85%29.aspx
		mRot = XMMatrixRotationRollPitchYaw(cockpitObject.RotX * XM_PI / 180,
			cockpitObject.RotY * XM_PI / 180,
			cockpitObject.RotZ * XM_PI / 180);
	
		// set translation matrix as given in the config
		// https://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.matrix.xmmatrixtranslation%28v=vs.85%29.aspx
		mTrans = XMMatrixTranslation(cockpitObject.TransX, cockpitObject.TransY, cockpitObject.TransZ);

		// set the scale matrix
		mScale = XMMatrixScaling(cockpitObject.Scale, cockpitObject.Scale, cockpitObject.Scale);

		// apply transformation as given in the config
		tmp_worldEV = mRot * mTrans * mScale *g_camera.GetWorldMatrix();
		tmp_worldViewProjectionEV = tmp_worldEV * g_camera.GetViewMatrix() * g_camera.GetProjMatrix();

		// apply the tmp vars
		V(g_gameEffect.worldEV->SetMatrix((float*) &tmp_worldEV));
	   
		// Set input layout
		//pd3dImmediateContext->IASetInputLayout( g_terrainVertexLayout );

		V(g_gameEffect.worldViewProjectionEV->SetMatrix((float*)&tmp_worldViewProjectionEV));

		//Now call the ->render() method for the mesh
		parser.g_Meshes[cockpitObject.Name]->render(pd3dImmediateContext, g_gameEffect.meshPass1, g_gameEffect.diffuseEV, g_gameEffect.specularEV, g_gameEffect.glowEV);
	}
	
	//Create the transformation matrices for the cockpit mesh and set the corresponding effect variables of g_gameEffect: 
	for (auto groundObject : parser.groundObjects) {

		if (parser.g_Meshes.find(groundObject.Name) == parser.g_Meshes.end()) {
			cout << "Error: Mesh for name(" << groundObject.Name << ") does not exist." << endl;
			system("pause");
		}

		// set the rotation matrix
		//               convert degree to radian
		//               radian     = degree * pi / 180;
		// https://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.matrix.xmmatrixrotationy%28v=vs.85%29.aspx
		mRot = XMMatrixRotationRollPitchYaw(groundObject.RotX * XM_PI / 180,
			groundObject.RotY * XM_PI / 180,
			groundObject.RotZ * XM_PI / 180);

		// set translation matrix as given in the config
		// https://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.matrix.xmmatrixtranslation%28v=vs.85%29.aspx
		mTrans = XMMatrixTranslation(groundObject.TransX, groundObject.TransY, groundObject.TransZ);

		// set the scale matrix
		mScale = XMMatrixScaling(groundObject.Scale, groundObject.Scale, groundObject.Scale);

		// apply transformation as given in the config
		tmp_worldEV = mRot * mTrans * mScale;
		tmp_worldViewProjectionEV = tmp_worldEV * g_camera.GetViewMatrix() * g_camera.GetProjMatrix();

		// apply the tmp vars
		V(g_gameEffect.worldEV->SetMatrix((float*)&tmp_worldEV));

		V(g_gameEffect.worldViewProjectionEV->SetMatrix((float*)&tmp_worldViewProjectionEV));

		//Now call the ->render() method for the mesh
		parser.g_Meshes[groundObject.Name]->render(pd3dImmediateContext, g_gameEffect.meshPass1, g_gameEffect.diffuseEV, g_gameEffect.specularEV, g_gameEffect.glowEV);
	}
	

	for (auto enemy : Ememy::g_EnemyInstances) {

		if (parser.g_Meshes.find(enemy.type.Mesh) == parser.g_Meshes.end()) {
			cout << "Error: Mesh for name(" << enemy.name << ") does not exist." << endl;
			system("pause");
		}

		// set the rotation matrix
		//               convert degree to radian
		//               radian     = degree * pi / 180;
		// https://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.matrix.xmmatrixrotationy%28v=vs.85%29.aspx
		mRot = XMMatrixRotationRollPitchYaw(enemy.type.RotX * XM_PI / 180,
			enemy.type.RotY * XM_PI / 180,
			enemy.type.RotZ * XM_PI / 180);

		DirectX::XMFLOAT3 test;
		DirectX::XMStoreFloat3(&test, enemy.vel);
		float d = atan2(test.x, test.z);

		// set translation matrix as given in the config
		// https://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.matrix.xmmatrixtranslation%28v=vs.85%29.aspx
		mTrans = XMMatrixTranslation(enemy.type.TransX, enemy.type.TransY, enemy.type.TransZ);

		// set the scale matrix
		mScale = XMMatrixScaling(enemy.type.Scale, enemy.type.Scale, enemy.type.Scale);

		mAnim = XMMatrixRotationY(d) * XMMatrixTranslationFromVector(enemy.pos);
		// apply transformation as given in the config
		tmp_worldEV = mRot * mScale * mTrans;
		tmp_worldViewProjectionEV = mAnim * tmp_worldEV * g_camera.GetViewMatrix() * g_camera.GetProjMatrix();

		// apply the tmp vars
		V(g_gameEffect.worldEV->SetMatrix((float*)&tmp_worldEV));

		//V(g_gameEffect.worldViewProjectionEV->SetMatrix((float*)&worldViewProj));

		V(g_gameEffect.worldViewProjectionEV->SetMatrix((float*)&tmp_worldViewProjectionEV));

		//Now call the ->render() method for the mesh
		parser.g_Meshes[enemy.type.Mesh]->render(pd3dImmediateContext, g_gameEffect.meshPass1, g_gameEffect.diffuseEV, g_gameEffect.specularEV, g_gameEffect.glowEV);
	}
	
    DXUT_BeginPerfEvent( DXUT_PERFEVENTCOLOR, L"HUD / Stats" );
    V(g_hud.OnRender( fElapsedTime ));
    V(g_sampleUI.OnRender( fElapsedTime ));
    RenderText();
    DXUT_EndPerfEvent();

    static DWORD dwTimefirst = GetTickCount();
    if ( GetTickCount() - dwTimefirst > 5000 )
    {    
        OutputDebugString( DXUTGetFrameStats( DXUTIsVsyncEnabled() ) );
        OutputDebugString( L"\n" );
        dwTimefirst = GetTickCount();
    }
}
