#ifndef XNA_CONTENT_DECOMPRESS_STREAM_HPP
#define XNA_CONTENT_DECOMPRESS_STREAM_HPP

#include <memory>
#include <vector>
#include "../csharp/integralnumeric.hpp"
#include "../csharp/stream/stream.hpp"

namespace xna {
	class DecompressStream : cs::Stream {
	public:

	private:
		static constexpr csint CompressedBufferSize = 65536;
		static constexpr csint DecompressedBufferSize = 65536;
		std::shared_ptr<cs::Stream> baseStream;
		csint compressedTodo;
		csint compressedSize{ 0 };
		csint compressedPosition{ 0 };
		std::vector<csbyte> compressedBuffer;
		csint decompressedTodo;
		csint decompressedSize{ 0 };
		csint decompressedPosition{ 0 };
		std::vector<csbyte> decompressedBuffer;
		//IntPtr decompressionContext;

		DecompressStream(std::shared_ptr<cs::Stream>& baseStream, csint compressedTodo, csint decompressedTodo) :
			baseStream(baseStream),
			compressedTodo(compressedTodo),
			decompressedTodo(decompressedTodo),
			compressedBuffer(std::vector<csbyte>(CompressedBufferSize)),
			decompressedBuffer(std::vector<csbyte>(DecompressedBufferSize)) {
		}

		virtual csint ReadByte() override {
			return decompressedPosition >= decompressedSize && !DecompressNextBuffer()
				? -1
				: toint(decompressedBuffer[decompressedPosition++]);

			return 0;
		}

		virtual csint Read(std::vector<csbyte>& buffer, csint offset, csint count) override {
			if (decompressedPosition >= decompressedSize && !DecompressNextBuffer())
				return 0;

			const auto num = decompressedSize - decompressedPosition;

			if (count > num)
				count = num;			

			for (size_t i = 0; i < count; ++i) {
				buffer[offset + i] = decompressedBuffer[decompressedPosition + i];
			}

			decompressedPosition += count;
			return count;
		}

		bool DecompressNextBuffer() {
			if (decompressedTodo <= 0)
				return false;

			do
			{
				if (compressedPosition >= compressedSize)
					ReadNextBufferFromDisk();

				auto sourceSize = compressedSize - compressedPosition;
				auto outputSize = DecompressedBufferSize;

				//byte* numPtr = compressedBuffer;
				//byte* outputData = decompressedBuffer;
				/*{
					if (NativeMethods.Decompress(decompressionContext, (void*)outputData, ref outputSize, (void*)(numPtr + compressedPosition), ref sourceSize) != 0)
						throw new InvalidOperationException(FrameworkResources.DecompressionError);
				}*/

				if (outputSize == 0 && sourceSize == 0)
					return false;

				compressedPosition += sourceSize;
				decompressedTodo -= outputSize;
				decompressedSize = outputSize;
				decompressedPosition = 0;

			} while (decompressedSize == 0);

			return true;
		}

		void ReadNextBufferFromDisk() {
			if (compressedTodo <= 0)
				return;

			ReadBufferFromDisk(compressedBuffer, compressedTodo, compressedSize);
			compressedPosition = 0;
		}

		void ReadBufferFromDisk(std::vector<csbyte>& buffer, csint& bufferTodo, csint& bufferSize) {
			auto num1 = CompressedBufferSize;

			if (num1 > bufferTodo)
				num1 = bufferTodo;

			auto num2 = 0;

			for (size_t offset = 0; offset < num1; offset += num2)	{
				num2 = baseStream->Read(buffer, offset, num1 - offset);

				if (num2 == 0)
					return;
			}

			bufferTodo -= num1;
			bufferSize = num1;
		}
	};
}

#endif