#include "contentreader.hpp"

namespace xna {
	void ContentTypeReader::Initialize(ContentTypeReaderManager& manager) {
	}

	std::vector<std::shared_ptr<ContentTypeReader>> ContentTypeReaderManager::LoadAssetReaders(ContentReader& reader) {
		return std::vector<std::shared_ptr<ContentTypeReader>>();
	}

	std::shared_ptr<ContentReader> ContentManager::GetContentReaderFromXnb(std::string const& originalAssetName, std::shared_ptr<cs::Stream>& stream, std::shared_ptr<cs::BinaryReader>& xnbReader) {
		const auto x = xnbReader->ReadByte();
		const auto n = xnbReader->ReadByte();
		const auto b = xnbReader->ReadByte();
		const auto platform = xnbReader->ReadByte();

		const auto identifiers = targetPlatformIdentifiers();
		const auto pFinder = std::find(identifiers.begin(), identifiers.end(), static_cast<char>(platform));

		if (x != 'X' || n != 'N' || b != 'B' || pFinder == identifiers.end()) {
			return nullptr;
		}

		const auto version = xnbReader->ReadByte();
		const auto flags = xnbReader->ReadByte();

		const auto compressedLzx = (flags & ContentCompressedLzx) != 0;
		const auto compressedLz4 = (flags & ContentCompressedLz4) != 0;

		if (version != 5 && version != 4) {
			return nullptr;
		}

		const auto xnbLength = xnbReader->ReadInt32();

		std::shared_ptr<cs::Stream> decompressedStream;

		if (compressedLzx || compressedLz4) {

			const auto decompressedSize = xnbReader->ReadInt32();

			if (compressedLzx) {
				csint compressedSize = xnbLength - 14;
				//decompressedStream = std::make_shared<LzxDecoderStream>(stream, decompressedSize, compressedSize);
			}
			else if (compressedLz4)
			{
				//decompressedStream = new Lz4DecoderStream(stream);
			}
		}
		else
		{
			decompressedStream = stream;
		}

		auto reader = make_shared<ContentReader>(this->shared_from_this(), decompressedStream, originalAssetName, version);

		return reader;
	}
}