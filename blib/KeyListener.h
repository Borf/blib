#pragma once

#include <blib/config.h>

#ifdef BLIB_WIN
#include <Windows.h>
#endif

#include <string>

namespace blib
{
	enum class Key
	{
		NONE = -1,
#ifdef BLIB_WIN
		ESC =		VK_ESCAPE,
		LEFT =		VK_LEFT,
		RIGHT =		VK_RIGHT,
		UP =		VK_UP,
		DOWN =		VK_DOWN,
		SPACE =		VK_SPACE,
		ENTER =		VK_RETURN,
		HOME =		VK_HOME,
		END =		VK_END,
		SHIFT =		VK_SHIFT,
		CONTROL =	VK_CONTROL,
		ALT =		VK_MENU,
		PLUS =	VK_ADD,
		MINUS = VK_SUBTRACT,
		TAB	=		VK_TAB,
		BACKSPACE =	VK_BACK,

		EQUALS = VK_OEM_PLUS,
		UNDERSCORE = VK_OEM_MINUS,

		BRACKETLEFT = VK_OEM_4,
		BRACKETRIGHT = VK_OEM_6,


		F1 =	VK_F1,
		F2 =	VK_F2,
		F3 =	VK_F3,
		F4 =	VK_F4,
		F5 =	VK_F5,
		F6 =	VK_F6,
		F7 =	VK_F7,
		F8 =	VK_F8,
		F9 =	VK_F9,
		F10 =	VK_F10,
		F11 =	VK_F11,
		F12 =	VK_F12,

		DIVIDE = VK_DIVIDE,

		DEL = VK_DELETE,




#else
		ESC,
		LEFT,
		RIGHT,
		UP,
		DOWN,
		SPACE,
		ENTER,
		HOME,
		END,
		SHIFT,
		CONTROL,
		ALT,
		PLUS,
		MINUS,
		TAB,
		DEL,
		BACKSPACE,

		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		DIVIDE,
        BRACKETLEFT,
        BRACKETRIGHT,

#endif
		A = 'A',
		B = 'B',
		C = 'C',
		D = 'D',
		E = 'E',
		F = 'F',
		G = 'G',
		H = 'H',
		I = 'I',
		J = 'J',
		K = 'K',
		L = 'L',
		M = 'M',
		N = 'N',
		O = 'O',
		P = 'P',
		Q = 'Q',
		R = 'R',
		S = 'S',
		T = 'T',
		U = 'U',
		V = 'V',
		W = 'W',
		X = 'X',
		Y = 'Y',
		Z = 'Z',

		_0 = '0',
		_1 = '1',
		_2 = '2',
		_3 = '3',
		_4 = '4',
		_5 = '5',
		_6 = '6',
		_7 = '7',
		_8 = '8',
		_9 = '9',
	};

	class KeyListener
	{
	public:
		virtual bool onKeyDown(Key key) { return false;  };
		virtual bool onKeyUp(Key key) { return false; };
		virtual bool onChar(char character) { return false; };


		static Key fromString(std::string);

	};


}
