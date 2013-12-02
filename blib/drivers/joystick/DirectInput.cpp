#include "DirectInput.h"


#include <dinputd.h>
#include <dinput.h>
#include <Windows.h>
#include <winerror.h>

#include <blib/util/Log.h>
#include <blib/platform/win32/window.h>
using blib::util::Log;

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
				Log::out<<"DirectInput: Found joystick "<<pdidInstance->tszProductName<<Log::newline;
				DirectInput* di = (DirectInput*)pDirectInput;

				DirectInput::DI_ENUM_CONTEXT* pEnumContext = &di->enumContext;
				HRESULT hr;
				LPDIRECTINPUTDEVICE8 joystick;

				//if( g_bFilterOutXinputDevices && IsXInputDevice( &pdidInstance->guidProduct ) )
				//	return DIENUM_CONTINUE;

				// Skip anything other than the perferred joystick device as defined by the control panel.  
				// Instead you could store all the enumerated joysticks and let the user pick.
//				if( pEnumContext->bPreferredJoyCfgValid &&
//					!IsEqualGUID( pdidInstance->guidInstance, pEnumContext->pPreferredJoyCfg->guidInstance ) )
//					return DIENUM_CONTINUE;


				hr = di->g_pDI->CreateDevice( pdidInstance->guidInstance, &joystick, NULL );
				if( FAILED( hr ) )
					return DIENUM_CONTINUE;

				di->joysticks.push_back(joystick);
				return DIENUM_CONTINUE;
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
					diprg.lMin = -10000;
					diprg.lMax = +10000;

					// Set the range for the axis
					if( FAILED( di->joysticks[di->enumJoyId]->SetProperty( DIPROP_RANGE, &diprg.diph ) ) )
						return DIENUM_STOP;

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

//				if( g_bFilterOutXinputDevices )
	//				CleanupForIsXInputDevice();

				for(enumJoyId = 0; enumJoyId < joysticks.size(); enumJoyId++)
				{
					// Set the data format to "simple joystick" - a predefined data format 
					//
					// A data format specifies which controls on a device we are interested in,
					// and how they should be reported. This tells DInput that we will be
					// passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
					if( FAILED( hr = joysticks[enumJoyId]->SetDataFormat( &c_dfDIJoystick2 ) ) )
						return;

					// Set the cooperative level to let DInput know how this device should
					// interact with the system and with other DInput applications.
					if( FAILED( hr = joysticks[enumJoyId]->SetCooperativeLevel( hWnd, DISCL_EXCLUSIVE | DISCL_BACKGROUND ) ) )
						return;

					// Enumerate the joystick objects. The callback function enabled user
					// interface elements for objects that are found, and sets the min/max
					// values property for discovered axes.
					if( FAILED( hr = joysticks[enumJoyId]->EnumObjects( EnumObjectsCallback, ( VOID* )this, DIDFT_ALL ) ) )
						return;
				}
			}




			DirectInput::~DirectInput()
			{


			}

			void DirectInput::update()
			{
				HRESULT hr;
				DIJOYSTATE2 js;           // DInput joystick state 


				for(size_t i = 0; i < joysticks.size(); i++)
				{
					// Poll the device to read the current state
					hr = joysticks[i]->Poll();
					if( FAILED( hr ) )
					{
						// DInput is telling us that the input stream has been
						// interrupted. We aren't tracking any state between polls, so
						// we don't have any special reset that needs to be done. We
						// just re-acquire and try again.
						hr = joysticks[i]->Acquire();
						while( hr == DIERR_INPUTLOST )
							hr = joysticks[i]->Acquire();

						// hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
						// may occur when the app is minimized or in the process of 
						// switching, so just try again later 
						joystates[i].connected = false;
						return;
					}

					// Get the input's device state
					if( FAILED( hr = joysticks[i]->GetDeviceState( sizeof( DIJOYSTATE2 ), &js ) ) )
						return; // The device should have been acquired during the Poll()

					joystates[i].connected = true;
					joystates[i].leftStick.x = js.lX / 10000.0f;
					joystates[i].leftStick.y = js.lY / 10000.0f;
					joystates[i].rightStick.x = js.lRx / 10000.0f;
					joystates[i].rightStick.y = js.lRy / 10000.0f;
					joystates[i].leftTrigger = js.lZ > 0 ? (js.lZ / 10000.0f) : 0;
					joystates[i].rightTrigger = js.lZ < 0 ? (-js.lZ / 10000.0f) : 0;

					joystates[i].button = 0;
					for(int ii = 0; ii < 10; ii++)
						if(js.rgbButtons[ii] != 0)
							joystates[i].button |= 1<<ii;
				}

			}


			JoyState DirectInput::getJoyState(int id)
			{
				return joystates[id];
			}

		}
	}
}
