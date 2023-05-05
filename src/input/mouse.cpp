#include "mouse.hpp"

namespace xna {
	MouseCursor::MouseCursor(HCURSOR cursor) : _cursor(cursor) {
	}

	MouseCursor MouseCursor::Arrow = MouseCursor(LoadCursor(NULL, IDC_ARROW));
	MouseCursor MouseCursor::IBeam = MouseCursor(LoadCursor(NULL, IDC_IBEAM));
	MouseCursor MouseCursor::Wait = MouseCursor(LoadCursor(NULL, IDC_WAIT));
	MouseCursor MouseCursor::Crosshair = MouseCursor(LoadCursor(NULL, IDC_CROSS));
	MouseCursor MouseCursor::WaitArrow = MouseCursor(LoadCursor(NULL, IDC_APPSTARTING));
	MouseCursor MouseCursor::SizeNWSE = MouseCursor(LoadCursor(NULL, IDC_SIZENWSE));
	MouseCursor MouseCursor::SizeNESW = MouseCursor(LoadCursor(NULL, IDC_SIZENESW));
	MouseCursor MouseCursor::SizeWE = MouseCursor(LoadCursor(NULL, IDC_SIZEWE));
	MouseCursor MouseCursor::SizeNS = MouseCursor(LoadCursor(NULL, IDC_SIZENS));
	MouseCursor MouseCursor::SizeAll = MouseCursor(LoadCursor(NULL, IDC_SIZEALL));
	MouseCursor MouseCursor::No = MouseCursor(LoadCursor(NULL, IDC_NO));
	MouseCursor MouseCursor::Hand = MouseCursor(LoadCursor(NULL, IDC_HAND));
}