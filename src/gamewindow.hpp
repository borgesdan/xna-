#ifndef XNA_GAMEWINDOW_HPP
#define XNA_GAMEWINDOW_HPP

#include <string>
#include "basic-structs.hpp"
#include "enumerations.hpp"
#include "eventargs.hpp"
#include "input/mouse.hpp"

namespace xna {
	class Game;

	class GameWindow {	
	public:
		constexpr GameWindow() = default;

		virtual bool AllowUserResizing() const {
			return false;
		}
		
		virtual void AllowUserResizing(bool value) {
		}

		virtual Rectangle ClientBounds() const {
			return Rectangle::Empty();
		}

		virtual bool AllowAltF4() const {
			return true;
		}

		virtual void AllowAltF4(bool value){
		}

		virtual Point Position() const {
			return Point::Zero();
		}

		virtual void Position(Point const& value) {
		}

		virtual DisplayOrientation CurrentOrientation() const {
			return DisplayOrientation::Default;
		}

		virtual std::string ScreenDeviceName() const {
			return std::string();
		}

		constexpr std::string Title() const {
			return _title;
		}

		void Title(std::string const& value) {
			if (value != _title) {
				SetTitle(value);
				_title = value;
			}
		}

		virtual bool IsBorderless() const {
			return false;
		}

		virtual void IsBorderless(bool value){}

		virtual void SetTitle(std::string const& title) {
		}

		virtual void BeginScreenDeviceChange(bool willBeFullScreen) {
		}

		virtual void EndScreenDeviceChange(std::string const& screenDeviceName, int clientWidth, int clientHeight) {
		}

		void EndScreenDeviceChange(std::string const& screenDeviceName) {
			const auto bounds = ClientBounds();
			EndScreenDeviceChange(screenDeviceName, bounds.Width, bounds.Height);
		}

		void OnTextInput(TextInputEventArgs const& e) {
			//EventHelpers.Raise(this, TextInput, e);
		}

		void OnKeyDown(InputKeyEventArgs const& e) {
			//EventHelpers.Raise(this, KeyDown, e);
		}

		void OnKeyUp(InputKeyEventArgs const& e) {
			// EventHelpers.Raise(this, KeyUp, e);
		}

		void OnFileDrop(FileDropEventArgs const& e) {
			// EventHelpers.Raise(this, FileDrop, e);
		}

		virtual void SetSupportedOrientations(DisplayOrientation orientations) {
		}

		static GameWindow Create(Game const& game, int width, int height) {
		}

	protected:
		void OnActivated() {
		}
		
		void OnDeactivated() {
		}

		void OnClientSizeChanged() {
			// EventHelpers.Raise(this, ClientSizeChanged, EventArgs.Empty);
		}

		void OnOrientationChanged() {
			//EventHelpers.Raise(this, OrientationChanged, EventArgs.Empty);
		}

		void OnPaint() {
		}

		void OnScreenDeviceNameChanged() {
			// EventHelpers.Raise(this, ScreenDeviceNameChanged, EventArgs.Empty);
		}

	public:
		xna::MouseState MouseState;

	private:
		std::string _title;
	};
}

#endif