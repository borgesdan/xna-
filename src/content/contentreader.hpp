#ifndef XNA_CONTENT_CONTENTREADER_HPP
#define XNA_CONTENT_CONTENTREADER_HPP

#include <memory>
#include <string>
#include <vector>
#include <any>
#include <map>
#include "../csharp/integralnumeric.hpp"
#include "../csharp/stream/stream.hpp"
#include "../csharp/stream/reader.hpp"
#include "contentmanager.hpp"
#include "../collision.hpp"
#include "../color.hpp"
#include "../basic-structs.hpp"
#include "../quaternion.hpp"
#include "../matrix.hpp"
#include "../csharp/type.hpp"
#include "lzxdecoder.hpp"

//ContentTypeReader
namespace xna {
	class ContentReader;
	class ContentTypeReaderManager;

	class ContentTypeReader {
	public:
		constexpr ContentTypeReader() = default;

		virtual ~ContentTypeReader() = 0;

		ContentTypeReader(cs::Type const& targetType) :
			_targetType(targetType) {
		}

		virtual std::any Read(ContentReader& input, std::any const& existingInstance) {
			return existingInstance;
		}

		virtual void Initialize(ContentTypeReaderManager& manager);

		cs::Type TargetType() const{
			return _targetType;
		}

		virtual csint TypeVersion() const {
			return 0;
		}

	private:
		cs::Type _targetType;
	};

	template<typename T>
	class ContentTypeReaderT : public ContentTypeReader {
	public:
		ContentTypeReaderT() :
			ContentTypeReader(cs::typeof<T>()){
		}

		virtual std::any Read(ContentReader& input, std::any const& existingInstance) override {
			auto t = any_cast<T>(existingInstance);
			return Read(t);
		}

		virtual T Read(ContentReader& input, T& existingInstance) {
			return existingInstance;
		}
	};
}

//ContentTypeReaderManager
namespace xna {
	class ContentTypeReaderManager {
	public:
		ContentTypeReaderManager() = default;

		std::vector<std::shared_ptr<ContentTypeReader>> LoadAssetReaders(ContentReader& reader);

		std::shared_ptr<ContentTypeReader> GetTypeReader(cs::Type const& targetType) {
			/*if (_contentReaders.contains(targetType))
				return _contentReaders[targetType];*/

			return nullptr;
		}

	private:
		static std::map<cs::Type, std::shared_ptr<ContentTypeReader>> _contentReadersCache;
		std::map<cs::Type, std::shared_ptr<ContentTypeReader>> _contentReaders;
	};

	void ContentTypeReader::Initialize(ContentTypeReaderManager& manager) {
	}
}

//ContentReader
namespace xna {
	class ContentReader : public cs::BinaryReader {
	public:
		ContentReader(std::shared_ptr<ContentManager> manager, std::shared_ptr <cs::Stream> stream, std::string assetName, csint version) :
			contentManager(manager),
			assetName(assetName),
			version(version),
			BinaryReader(stream) {
		}

		template <typename T>
		T ReadAsset() {
			InitializeTypeReaders();
		}

		template <typename T>
		T ReadAsset(T& existingInstance) {
			InitializeTypeReaders();
			return ReadObject(existingInstance);
		}

		void InitializeTypeReaders() {
			typeReaderManager = std::make_shared<ContentTypeReaderManager>();
			typeReaders = typeReaderManager->LoadAssetReaders(*this);
			sharedResourceCount = Read7BitEncodedInt();
		}

		template <typename T>
		T ReadExternalReference() {
			auto externalReference = ReadString();

			if (externalReference.empty())
				//return contentManager.Load<T>(FileHelpers.ResolveRelativePath(assetName, externalReference));
				return T();

			return T();
		}

		template <typename T>
		T ReadObject() {
			auto t = T();
			return InnerReadObject(t);
		}

		template <typename T>
		T ReadObject(ContentTypeReader& typeReader) {
			auto defaultT = T();
			auto result = typeReader.Read(*this, defaultT);
			return result;
		}

		template <typename T>
		T ReadObject(T& existingInstance) {
			return InnerReadObject(existingInstance);
		}

		template <typename T>
		T ReadObject(ContentTypeReader& typeReader, T& existingInstance) {
			auto result = std::any_cast<T>(typeReader.Read(*this, existingInstance));
			return result;
		}

		template <typename T>
		T InnerReadObject(T& existingInstance) {
			auto typeReaderIndex = Read7BitEncodedInt();

			if (typeReaderIndex == 0)
				return existingInstance;

			if (typeReaderIndex > typeReaders.size())
				return existingInstance;

			auto& typeReader = typeReaders[typeReaderIndex - 1];
			auto& result = std::any_cast<T>(typeReader->Read(*this, existingInstance));

			return result;
		}		

		template <typename T>
		T ReadRawObject() {
			auto t = T();
			auto result = ReadRawObject(t);
			return result;
		}

		template <typename T>
		T ReadRawObject(T& existingInstance) {
			cs::Type objectType = cs::typeof<T>();
			const auto size = typeReaders.size();

			for (size_t i = 0; i < size; ++i) {
				auto& typeReader = typeReaders[i];

				if (typeReader->TargetType() == objectType) {
					auto result = ReadRawObject(typeReader, existingInstance);
					return result;
				}
			}

			return existingInstance;
		}

		template <typename T>
		T ReadRawObject(ContentTypeReader& typeReader, T& existingInstance) {
			auto result = std::any_cast<T>(typeReader.Read(*this, existingInstance));
			return result;
		}

		constexpr Matrix ReadMatrix() {
			Matrix result;
			result.M11 = ReadSingle();
			result.M12 = ReadSingle();
			result.M13 = ReadSingle();
			result.M14 = ReadSingle();
			result.M21 = ReadSingle();
			result.M22 = ReadSingle();
			result.M23 = ReadSingle();
			result.M24 = ReadSingle();
			result.M31 = ReadSingle();
			result.M32 = ReadSingle();
			result.M33 = ReadSingle();
			result.M34 = ReadSingle();
			result.M41 = ReadSingle();
			result.M42 = ReadSingle();
			result.M43 = ReadSingle();
			result.M44 = ReadSingle();
			return result;
		}

		constexpr Quaternion ReadQuaternion() {
			Quaternion result;
			result.X = ReadSingle();
			result.Y = ReadSingle();
			result.Z = ReadSingle();
			result.W = ReadSingle();
			return result;
		}

		constexpr Vector2 ReadVector2() {
			Vector2 result;
			result.X = ReadSingle();
			result.Y = ReadSingle();
			return result;
		}

		constexpr Vector3 ReadVector3() {
			Vector3 result;
			result.X = ReadSingle();
			result.Y = ReadSingle();
			result.Z = ReadSingle();
			return result;
		}

		constexpr Vector4 ReadVector4() {
			Vector4 result;
			result.X = ReadSingle();
			result.Y = ReadSingle();
			result.Z = ReadSingle();
			result.W = ReadSingle();
			return result;
		}

		constexpr Color ReadColor() {
			Color result;
			result.R(ReadByte());
			result.G(ReadByte());
			result.B(ReadByte());
			result.A(ReadByte());
			return result;
		}

		constexpr BoundingSphere ReadBoundingSphere() {
			auto position = ReadVector3();
			auto radius = ReadSingle();
			return BoundingSphere(position, radius);
		}

	private:
		std::shared_ptr<ContentManager> contentManager;
		std::shared_ptr<ContentTypeReaderManager> typeReaderManager;
		std::string assetName;
		std::vector<std::shared_ptr<ContentTypeReader>> typeReaders;
		csint version{ 0 };
		csint sharedResourceCount{ 0 };
	};

	std::vector<std::shared_ptr<ContentTypeReader>> ContentTypeReaderManager::LoadAssetReaders(ContentReader& reader) {
		return std::vector<std::shared_ptr<ContentTypeReader>>();
	}
}

namespace xna {
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

		if (compressedLzx || compressedLz4)	{
			
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

#endif