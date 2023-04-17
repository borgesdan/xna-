#ifndef CS_STREAM_STREAM_HPP
#define CS_STREAM_STREAM_HPP

#include <cmath>
#include <vector>
#include <memory>
#include <string>
#include "../../mathhelper.hpp"
#include "../integralnumeric.hpp"
#include "../array.hpp"

//Stream
namespace cs {
	enum class SeekOrigin {
		Begin = 0,
		Current = 1,
		End = 2
	};

	class Stream {
	public:
		virtual constexpr bool CanRead() { return false; }
		virtual constexpr bool CanWrite() { return false; }
		virtual constexpr bool CanSeek() { return false; }
		virtual constexpr cslong Length() { return -1; }
		virtual constexpr cslong Position() { return -1; }
		virtual constexpr void Position(cslong value) {}
		virtual constexpr void Flush() {}
		virtual constexpr bool CanTimeout() { return false; }
		virtual constexpr csint ReadTimeout() { return 0; }
		virtual constexpr void ReadTimeout(csint value) {}
		virtual constexpr csint WriteTimeout() { return 0; }
		virtual constexpr void WriteTimeout(csint value) {}
		virtual constexpr void Close() {}
		virtual cslong Seek(cslong offset, SeekOrigin origin) { return -1; }
		virtual csint Read(std::vector<csbyte>& buffer, csint offset, csint count) { return -1; }
		virtual csint ReadByte() { return 0; }
		virtual void Write(std::vector<csbyte>& buffer, csint offset, csint count) {}
		virtual void WriteByte(csbyte value) {}
	};

	using PtrStream = std::shared_ptr<cs::Stream>;
}

//MemoryStream
namespace cs {
	class MemoryStream : public Stream {
	public:
		constexpr MemoryStream(csint capacity = 0) :
			_buffer(capacity != 0 ? std::vector<csbyte>(capacity) : std::vector<csbyte>()),
			_capacity(capacity),
			_writable(true),
			_exposable(true),
			_isOpen(true) {
		}

		constexpr MemoryStream(std::vector<csbyte> const& buffer, bool writable = true) :
			_buffer(buffer),
			_length(buffer.size()),
			_capacity(buffer.size()),
			_writable(writable),
			_isOpen(true) {
		}

		constexpr MemoryStream(std::vector<csbyte> const& buffer, csint index, csint count,
			bool writable = true, bool publiclyVisible = false) :
			_buffer(buffer),
			_origin(index),
			_position(index),
			_length(index + count),
			_capacity(index + count),
			_writable(writable),
			_exposable(publiclyVisible),
			_isOpen(true) {
		}

		virtual constexpr bool CanRead() override {
			return _isOpen;
		}

		virtual constexpr bool CanWrite() override {
			return _writable;
		}

		virtual constexpr bool CanSeek() override {
			return _isOpen;
		}

		virtual constexpr cslong Length() override {
			if (!_isOpen)
				return -1;

			return _length - _origin;
		}

		virtual constexpr cslong Position() override {
			if (!_isOpen)
				return -1;

			return _position - _origin;
		}

		virtual constexpr void Position(cslong value) override {
			if (value > MemStreamMaxLength)
				return;

			_position = _origin + toint(value);
		}

		virtual constexpr void Close() override {
			_isOpen = false;
			_writable = false;
			_expandable = false;
		}

		virtual constexpr void Flush() override {
		}

		virtual std::vector<csbyte> GetBuffer() {
			if (!_exposable)
				return std::vector<csbyte>();

			return _buffer;
		}

		virtual csint Capacity() {
			if (!_isOpen)
				return -1;

			return _capacity - _origin;
		}

		virtual void Capacity(csint value) {
			if (value < Length())
				return;

			if (!_isOpen)
				return;

			if (!_expandable && (value != Capacity()))
				return;


			if (_expandable && value != _capacity) {
				if (value > 0) {
					_buffer.resize(value);
				}
				else {
					_buffer.clear();
				}

				_capacity = value;
			}
		}

		virtual csint ReadByte() override {
			if (_isOpen)
				return -1;

			if (_position >= _length)
				return -1;

			return _buffer[_position++];
		}

		virtual cslong Seek(cslong offset, SeekOrigin loc) override {
			if (!_isOpen)
				return -1;

			if (offset > MemStreamMaxLength)
				return -1;

			switch (loc)
			{
			case SeekOrigin::Begin: {
				csint tempPosition = _origin + toint(offset);

				if (offset < 0 || tempPosition < _origin)
					return -1;

				_position = tempPosition;
				break;
			}
			case SeekOrigin::Current: {
				int tempPosition = _position + toint(offset);

				if ((_position + offset) < _origin || tempPosition < _origin)
					return -1;

				_position = tempPosition;
				break;
			}
			case SeekOrigin::End: {
				int tempPosition = _length + toint(offset);

				if ((_length + offset) < _origin || tempPosition < _origin)
					return -1;

				_position = tempPosition;
				break;
			}
			default:
				return -1;
			}

			return _position;
		}

		virtual csint Read(std::vector<csbyte>& buffer, csint offset, csint count) override {
			if (!_isOpen)
				return 0;

			csint n = _length - _position;
			if (n > count)
				n = count;

			if (n <= 0)
				return 0;

			if (n <= 8) {
				csint byteCount = n;

				while (--byteCount >= 0)
					buffer[offset + byteCount] = _buffer[_position + byteCount];
			}
			else {
				for (size_t i = 0; i < n; ++i) {
					buffer[offset + i] = _buffer[_position + i];
				}
			}

			_position += n;

			return n;
		}

		virtual void Write(std::vector<csbyte>& buffer, csint offset, csint count) override {
			if (!_isOpen)
				return;

			if (!_writable)
				return;

			csint i = _position + count;

			if (i < 0)
				return;

			if (i > _length) {
				bool mustZero = _position > _length;

				if (i > _capacity) {
					bool allocatedNewArray = EnsureCapacity(i);

					if (allocatedNewArray) {
						mustZero = false;
					}
				}
				if (mustZero) {
					for (size_t j = _length; j < (i - _length); ++j) {
						_buffer[j] = 0;
					}
				}

				_length = i;
			}
			if ((count <= 8) && (buffer != _buffer)) {
				csint byteCount = count;

				while (--byteCount >= 0) {
					_buffer[_position + byteCount] = buffer[offset + byteCount];
				}
			}
			else {
				for (size_t j = 0; j < count; ++j) {
					buffer[offset + j] = _buffer[_position + j];
				}
			}

			_position = i;
		}

		virtual void WriteByte(csbyte value) override {
			if (!_isOpen)
				return;

			if (_writable)
				return;

			if (_position >= _length) {
				csint newLength = _position + 1;
				bool mustZero = _position > _length;

				if (newLength >= _capacity) {
					bool allocatedNewArray = EnsureCapacity(newLength);

					if (allocatedNewArray) {
						mustZero = false;
					}
				}

				if (mustZero) {
					for (size_t j = _length; j < (_position - _length); ++j) {
						_buffer[j] = 0;
					}
				}

				_length = newLength;
			}

			_buffer[_position++] = value;
		}

	private:
		std::vector<csbyte> _buffer;
		csint _origin{ 0 };
		csint _position{ 0 };
		csint _length{ 0 };
		csint _capacity{ 0 };
		bool _expandable{ false };
		bool _writable{ false };
		bool _exposable{ false };
		bool _isOpen{ false };

		static constexpr csint MemStreamMaxLength = int_max;

		bool EnsureCapacity(csint value) {
			if (value < 0)
				return false;

			if (value > _capacity) {
				csint newCapacity = xna::Math::Max<csint>(value, 256);

				if (newCapacity < _capacity * 2) {
					newCapacity = _capacity * 2;
				}

				if (touint(_capacity * 2) > Array::MaxLength) {
					newCapacity = xna::Math::Max(value, Array::MaxLength);
				}

				Capacity(newCapacity);

				return true;
			}
			return false;
		}
	};
}

#endif