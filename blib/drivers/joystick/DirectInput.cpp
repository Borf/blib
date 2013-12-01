#include "DirectInput.h"


#include <dinputd.h>
#include <dinput.h>
#include <Windows.h>
#include <winerror.h>


#include <blib/platform/win32/window.h>

namespace blib
{
	namespace drivers
	{
		namespace joystick
		{

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }


			struct XINPUT_DEVICE_NODE
			{
				DWORD dwVidPid;
				XINPUT_DEVICE_NODE* pNext;
			};


			bool                    g_bFilterOutXinputDevices = false;
			XINPUT_DEVICE_NODE*     g_pXInputDeviceList = NULL;


			void CleanupForIsXInputDevice()
			{
				// Cleanup linked list
				XINPUT_DEVICE_NODE* pNode = g_pXInputDeviceList;
				while( pNode )
				{
					XINPUT_DEVICE_NODE* pDelete = pNode;
					pNode = pNode->pNext;
					SAFE_DELETE( pDelete );
				}
			}

			bool IsXInputDevice( const GUID* pGuidProductFromDirectInput )
			{
				// Check each xinput device to see if this device's vid/pid matches
				XINPUT_DEVICE_NODE* pNode = g_pXInputDeviceList;
				while( pNode )
				{
					if( pNode->dwVidPid == pGuidProductFromDirectInput->Data1 )
						return true;
					pNode = pNode->pNext;
				}

				return false;
			}



			BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pDirectInput )
			{
				DirectInput* di = (DirectInput*)pDirectInput;

				DirectInput::DI_ENUM_CONTEXT* pEnumContext = &di->enumContext;
				HRESULT hr;

				if( g_bFilterOutXinputDevices && IsXInputDevice( &pdidInstance->guidProduct ) )
					return DIENUM_CONTINUE;

				// Skip anything other than the perferred joystick device as defined by the control panel.  
				// Instead you could store all the enumerated joysticks and let the user pick.
				if( pEnumContext->bPreferredJoyCfgValid &&
					!IsEqualGUID( pdidInstance->guidInstance, pEnumContext->pPreferredJoyCfg->guidInstance ) )
					return DIENUM_CONTINUE;

				// Obtain an interface to the enumerated joystick.
				hr = di->g_pDI->CreateDevice( pdidInstance->guidInstance, &di->g_pJoystick, NULL );

				// If it failed, then we can't use this joystick. (Maybe the user unplugged
				// it while we were in the middle of enumerating it.)
				if( FAILED( hr ) )
					return DIENUM_CONTINUE;

				// Stop enumeration. Note: we're just taking the first joystick we get. You
				// could store all the enumerated joysticks and let the user pick.
				return DIENUM_STOP;
			}

			BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext )
			{
				DirectInput* di = (DirectInput*)pContext;
//				HWND hDlg = ( HWND )pContext;

				static int nSliderCount = 0;  // Number of returned slider controls
				static int nPOVCount = 0;     // Number of returned POV controls

				// For axes that are returned, set the DIPROP_RANGE property for the
				// enumerated axis in order to scale min/max values.
				if( pdidoi->dwType & DIDFT_AXIS )
				{
					DIPROPRANGE diprg;
					diprg.diph.dwSize = sizeof( DIPROPRANGE );
					diprg.diph.dwHeaderSize = sizeof( DIPROPHEADER );
					diprg.diph.dwHow = DIPH_BYID;
					diprg.diph.dwObj = pdidoi->dwType; // Specify the enumerated axis
					diprg.lMin = -1000;
					diprg.lMax = +1000;

					// Set the range for the axis
					if( FAILED( di->g_pJoystick->SetProperty( DIPROP_RANGE, &diprg.diph ) ) )
						return DIENUM_STOP;

				}


				// Set the UI to reflect what objects the joystick supports
				if( pdidoi->guidType == GUID_XAxis )
				{
//					EnableWindow( GetDlgItem( hDlg, IDC_X_AXIS ), TRUE );
//					EnableWindow( GetDlgItem( hDlg, IDC_X_AXIS_TEXT ), TRUE );
				}
				if( pdidoi->guidType == GUID_YAxis )
				{
//					EnableWindow( GetDlgItem( hDlg, IDC_Y_AXIS ), TRUE );
//					EnableWindow( GetDlgItem( hDlg, IDC_Y_AXIS_TEXT ), TRUE );
				}
				if( pdidoi->guidType == GUID_ZAxis )
				{
//					EnableWindow( GetDlgItem( hDlg, IDC_Z_AXIS ), TRUE );
//					EnableWindow( GetDlgItem( hDlg, IDC_Z_AXIS_TEXT ), TRUE );
				}
				if( pdidoi->guidType == GUID_RxAxis )
				{
//					EnableWindow( GetDlgItem( hDlg, IDC_X_ROT ), TRUE );
//					EnableWindow( GetDlgItem( hDlg, IDC_X_ROT_TEXT ), TRUE );
				}
				if( pdidoi->guidType == GUID_RyAxis )
				{
//					EnableWindow( GetDlgItem( hDlg, IDC_Y_ROT ), TRUE );
//					EnableWindow( GetDlgItem( hDlg, IDC_Y_ROT_TEXT ), TRUE );
				}
				if( pdidoi->guidType == GUID_RzAxis )
				{
//					EnableWindow( GetDlgItem( hDlg, IDC_Z_ROT ), TRUE );
//					EnableWindow( GetDlgItem( hDlg, IDC_Z_ROT_TEXT ), TRUE );
				}
				if( pdidoi->guidType == GUID_Slider )
				{
					switch( nSliderCount++ )
					{
					case 0 :
//						EnableWindow( GetDlgItem( hDlg, IDC_SLIDER0 ), TRUE );
//						EnableWindow( GetDlgItem( hDlg, IDC_SLIDER0_TEXT ), TRUE );
						break;

					case 1 :
//						EnableWindow( GetDlgItem( hDlg, IDC_SLIDER1 ), TRUE );
//						EnableWindow( GetDlgItem( hDlg, IDC_SLIDER1_TEXT ), TRUE );
						break;
					}
				}
				if( pdidoi->guidType == GUID_POV )
				{
					switch( nPOVCount++ )
					{
					case 0 :
//						EnableWindow( GetDlgItem( hDlg, IDC_POV0 ), TRUE );
//						EnableWindow( GetDlgItem( hDlg, IDC_POV0_TEXT ), TRUE );
						break;

					case 1 :
//						EnableWindow( GetDlgItem( hDlg, IDC_POV1 ), TRUE );
//						EnableWindow( GetDlgItem( hDlg, IDC_POV1_TEXT ), TRUE );
						break;

					case 2 :
//						EnableWindow( GetDlgItem( hDlg, IDC_POV2 ), TRUE );
//						EnableWindow( GetDlgItem( hDlg, IDC_POV2_TEXT ), TRUE );
						break;

					case 3 :
//						EnableWindow( GetDlgItem( hDlg, IDC_POV3 ), TRUE );
//						EnableWindow( GetDlgItem( hDlg, IDC_POV3_TEXT ), TRUE );
						break;
					}
				}

				return DIENUM_CONTINUE;
			}


			DirectInput::DirectInput(blib::Window* window)
			{
				HRESULT hr;

				HWND hWnd = window->hWnd;


				// Register with the DirectInput subsystem and get a pointer
				// to a IDirectInput interface we can use.
				// Create a DInput object
				if( FAILED( hr = DirectInput8Create( GetModuleHandle( NULL ), DIRECTINPUT_VERSION,
					IID_IDirectInput8, ( VOID** )&g_pDI, NULL ) ) )
					return;

				DIJOYCONFIG PreferredJoyCfg = {0};
				enumContext.pPreferredJoyCfg = &PreferredJoyCfg;
				enumContext.bPreferredJoyCfgValid = false;

				IDirectInputJoyConfig8* pJoyConfig = NULL;
				if( FAILED( hr = g_pDI->QueryInterface( IID_IDirectInputJoyConfig8, ( void** )&pJoyConfig ) ) )
					return;

				PreferredJoyCfg.dwSize = sizeof( PreferredJoyCfg );
				if( SUCCEEDED( pJoyConfig->GetConfig( 0, &PreferredJoyCfg, DIJC_GUIDINSTANCE ) ) ) // This function is expected to fail if no joystick is attached
					enumContext.bPreferredJoyCfgValid = true;
				SAFE_RELEASE( pJoyConfig );

				// Look for a simple joystick we can use for this sample program.
				if( FAILED( hr = g_pDI->EnumDevices( DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, this, DIEDFL_ATTACHEDONLY ) ) )
					return;

				if( g_bFilterOutXinputDevices )
					CleanupForIsXInputDevice();

				// Make sure we got a joystick
				if( NULL == g_pJoystick )
				{
					MessageBox( NULL, TEXT( "Joystick not found. The sample will now exit." ),
						TEXT( "DirectInput Sample" ),
						MB_ICONERROR | MB_OK );
					return;
				}

				// Set the data format to "simple joystick" - a predefined data format 
				//
				// A data format specifies which controls on a device we are interested in,
				// and how they should be reported. This tells DInput that we will be
				// passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
				if( FAILED( hr = g_pJoystick->SetDataFormat( &c_dfDIJoystick2 ) ) )
					return;

				// Set the cooperative level to let DInput know how this device should
				// interact with the system and with other DInput applications.
				if( FAILED( hr = g_pJoystick->SetCooperativeLevel( hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND | DISCL_BACKGROUND ) ) )
					return;

				// Enumerate the joystick objects. The callback function enabled user
				// interface elements for objects that are found, and sets the min/max
				// values property for discovered axes.
				if( FAILED( hr = g_pJoystick->EnumObjects( EnumObjectsCallback, ( VOID* )this, DIDFT_ALL ) ) )
					return;
			}




			DirectInput::~DirectInput()
			{


			}

			void DirectInput::update()
			{
				HRESULT hr;
				TCHAR strText[512] = {0}; // Device state text
				DIJOYSTATE2 js;           // DInput joystick state 

				if( NULL == g_pJoystick )
					return;

				// Poll the device to read the current state
				hr = g_pJoystick->Poll();
				if( FAILED( hr ) )
				{
					// DInput is telling us that the input stream has been
					// interrupted. We aren't tracking any state between polls, so
					// we don't have any special reset that needs to be done. We
					// just re-acquire and try again.
					hr = g_pJoystick->Acquire();
					while( hr == DIERR_INPUTLOST )
						hr = g_pJoystick->Acquire();

					// hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
					// may occur when the app is minimized or in the process of 
					// switching, so just try again later 
					return;
				}

				// Get the input's device state
				if( FAILED( hr = g_pJoystick->GetDeviceState( sizeof( DIJOYSTATE2 ), &js ) ) )
					return; // The device should have been acquired during the Poll()

				joystates[0].leftStick.x = (js.lX-32768) / 32768.0f;
				joystates[0].leftStick.y = (js.lY-32768) / 32768.0f;


			}


			JoyState DirectInput::getJoyState(int id)
			{
				return joystates[0];
			}

		}
	}
}
