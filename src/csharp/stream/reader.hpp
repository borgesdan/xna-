#ifndef CS_STREAM_READER_HPP
#define CS_STREAM_READER_HPP

#include "stream.hpp"

//BinaryReader
namespace cs {
	// https://referencesource.microsoft.com/#mscorlib/system/io/binaryreader.cs,4f6cad84482876ff
	class BinaryReader {
	public:
		BinaryReader(std::shared_ptr<Stream> stream) :
			_stream(stream) {
		}

		virtual void Close() {}

		virtual csint PeekChar() {
			if (!_stream->CanSeek())
				return -1;

			cslong origPos = _stream->Position();
			csint ch = Read();

			_stream->Position(origPos);
			return ch;
		}

		virtual csbyte ReadByte() {
			return 0;
		}

		virtual csint Read() {
			return -1;
		}

		virtual cssbyte ReadSByte() {
			return tosbyte(InternalReadByte());
		}

		virtual bool ReadBoolean() {
			InternalReadByte() != 0;
		}

		virtual char ReadChar() {
			csint value = Read();

			if (value == -1) {
				return 0;
			}

			return static_cast<char>(value);
		}

		virtual csshort ReadInt16() {
			return 0;
		}

		virtual csushort ReadUInt16() {
			return 0;
		}

		virtual csint ReadInt32() {
			return 0;
		}

		virtual csuint ReadUInt32() {
			return 0;
		}

		virtual cslong ReadInt64() {
			return 0;
		}

		virtual csulong ReadUInt64() {
			return 0;
		}

		virtual float ReadSingle() {
			return 0.0;
		}

		virtual double ReadDouble() {
			return 0.0;
		}

		virtual std::string ReadString() {
			return "";
		}

		virtual csint Read(std::vector<char> buffer, csint index, csint count) {

			if (buffer.size() - index < count) {
				return -1;
			}

			//return InternalReadChars(new Span<char>(buffer, index, count));
		}

		virtual csint Read(std::vector<char> buffer) {
			//return InternalReadChars(buffer);
			return 0;
		}

		virtual std::vector<char> ReadChars(csint count) {
			return std::vector<char>();
		}

		virtual csint Read(std::vector<csbyte> buffer, csint index, csint count) {
			return 0;
		}

		virtual csint Read(std::vector<csbyte> buffer) {
			return 0;
		}

		virtual std::vector<csbyte> ReadBytes(csint count) {
			return std::vector<csbyte>();
		}

		virtual csint Read7BitEncodedInt() {
			return 0;
		}

		virtual cslong Read7BitEncodedInt64() {
			return 0;
		}


	protected:
		virtual void FillBuffer(csint numBytes) {
		}

	private:
		static constexpr csint MaxCharBytesSize = 128;
		std::shared_ptr<Stream> _stream;
		std::vector<csbyte> _charBytes;
		bool _2BytesPerChar{ true };

		csbyte InternalReadByte() {
			csint b = _stream->ReadByte();

			if (b == -1) {
				return 0;
			}

			return tobyte(b);
		}

		csint InternalReadChars(std::vector<char> buffer) {
			return 0;
		}

		std::vector<csbyte> InternalRead(csint numBytes) {
			return std::vector<csbyte>();
		}
	};
}

#endif