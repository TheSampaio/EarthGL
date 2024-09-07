#ifndef _EARTHGL_INPUT_
#define _EARTHGL_INPUT_

class Application;
class Window;

// Enumerates all keyboard keys
enum EKeyCode
{
	// Letters keys
	A = 65,
	B = 66,
	C = 67,
	D = 68,
	E = 69,
	F = 70,
	G = 71,
	H = 72,
	I = 73,
	J = 74,
	K = 75,
	L = 76,
	M = 77,
	N = 78,
	O = 79,
	P = 80,
	Q = 81,
	R = 82,
	S = 83,
	T = 84,
	U = 85,
	V = 86,
	W = 87,
	X = 88,
	Y = 89,
	Z = 90,

	// Alpha numbers
	Alpha0 = 48,
	Alpha1 = 49,
	Alpha2 = 50,
	Alpha3 = 51,
	Alpha4 = 52,
	Alpha5 = 53,
	Alpha6 = 54,
	Alpha7 = 55,
	Alpha8 = 56,
	Alpha9 = 57,

	// Numpad keys
	Num0 = 320,
	Num1 = 321,
	Num2 = 322,
	Num3 = 323,
	Num4 = 324,
	Num5 = 325,
	Num6 = 326,
	Num7 = 327,
	Num8 = 328,
	Num9 = 329,

	Decimal = 330,
	Divide = 331,
	Multiply = 332,
	Subtract = 333,
	Add = 334,

	// Functional keys
	F1 = 290,
	F2 = 291,
	F3 = 292,
	F4 = 293,
	F5 = 294,
	F6 = 295,
	F7 = 296,
	F8 = 297,
	F9 = 298,
	F10 = 299,
	F11 = 300,
	F12 = 301,

	// Arrow keys
	Right = 262,
	Left = 263,
	Down = 264,
	Up = 265,

	// Other keys
	Esc = 256,
	Tab = 258,
	End = 269,
	Menu = 348,
	Home = 268,
	Enter = 335,
	Minus = 45,
	Space = 259,
	Comma = 44,
	Pause = 284,
	Return = 257,
	Period = 46,
	Insert = 260,
	Delete = 261,
	Backspace = 259,
	Semicolon = 59,

	Slash = 47,
	Backslash = 92,

	SuperLeft = 343,
	SuperRight = 347,

	AltLeft = 342,
	AltRight = 346,

	ShiftLeft = 340,
	ShiftRight = 344,

	ControlLeft = 341,
	ControlRight = 345,

	PageUp = 266,
	PageDown = 267,

	CapsLock = 280,
	ScrollLock = 281,
	NumLock = 282,
};

class Input
{
public:
    static bool GetKeyPress(EKeyCode key) { return GetInstance().IGetKeyPress(key); }
    static bool GetKeyRelease(EKeyCode key) { return GetInstance().IGetKeyRelease(key); }
    static bool GetKeyTap(EKeyCode key) { return GetInstance().IGetKeyTap(key); }

    friend Application;

private:
    Input() {};

    Input(const Input&) = delete;
    Input operator=(const Input&) = delete;

    void PollEvents();

    bool IGetKeyPress(EKeyCode key);
    bool IGetKeyRelease(EKeyCode key);
    bool IGetKeyTap(EKeyCode key);

    static Input& GetInstance() { static Input sInstance; return sInstance; }
};

#endif
