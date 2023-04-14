#ifndef XNA_CONTENT_CONTENTMANAGER_HPP
#define XNA_CONTENT_CONTENTMANAGER_HPP

#include "../csharp/integralnumeric.hpp"
#include "../csharp/stream/stream.hpp"
#include "../csharp/stream/reader.hpp"
#include "../content/lzxdecoder.hpp"
#include "../csharp/io/path.hpp"
#include "../titlecontainer.hpp"
#include <string>
#include <any>
#include <map>
#include <vector>
#include <memory>

namespace xna {
	class ContentReader;

	class ContentManager : std::enable_shared_from_this<ContentManager> {
	public:
		ContentManager() {
		}

		ContentManager(std::string const& rootDirectory) {
		}

		static void ReloadGraphicsContent() {
		}

		template <typename T>
		T LoadLocalized(std::string const& assetName) {
		}

		template <typename T>
		T Load(std::string const& assetName) {
		}

		virtual void Unload() {
			loadedAssets.clear();
		}
		
		virtual void UnloadAsset(std::string const& assetName) {			
			if (assetName.empty())
				return;

			if (!loadedAssets.contains(assetName))
				return;

			loadedAssets.erase(assetName);
		}

		virtual void UnloadAssets(std::vector<std::string> assetNames) {
			if (assetNames.empty())
				return;

			const auto size = assetNames.size();
			for (size_t i = 0; i < size; i++) {
				UnloadAsset(assetNames[i]);
			}
		}

		std::string RootDirectoryFullPath() const {
			return cs::Path::Combine(TitleContainer::Location(), RootDirectory);
		}	

	protected:
		virtual std::shared_ptr<cs::Stream> OpenStream(std::string const& assetName) {
			auto assetPath = cs::Path::Combine(RootDirectory, assetName) + ".xnb";

			std::shared_ptr<cs::Stream> stream;

			if (cs::Path::IsPathRooted(assetPath)) {
				//
			}

			return stream;
		}

		template <typename T>
		T ReadAsset(std::string const& assetName); //Usa ContentReader

		virtual std::map<std::string, std::any> LoadedAssets() {
		}

		virtual void ReloadGraphicsAssets() {
			const auto size = loadedAssets.size();
			for (size_t i = 0; i < size; ++i) {
				//??
			}
		}

		template <typename T>
		void ReloadAsset(std::string const& originalAssetName, T& currentAsset) {
			if (originalAssetName.empty())
				return;

			const auto assetName = originalAssetName;
			auto stream = OpenStream(assetName);

			auto xnbReader = std::make_shared<cs::BinaryReader>(stream);
			auto reader = GetContentReaderFromXnb(assetName, stream, nullptr);
			
			reader->ReadAsset<T>(currentAsset);

			reader->Close();
			stream->Close();
		}

	public:
		std::string RootDirectory;

	private:
		static constexpr csbyte ContentCompressedLzx = 0x80;
		static constexpr csbyte ContentCompressedLz4 = 0x40;

		static std::vector<std::shared_ptr<ContentManager>> ContentManagers;		
		std::map<std::string, std::any> loadedAssets;

		static constexpr std::vector<char> targetPlatformIdentifiers() {
			return std::vector<char>
			{
				'w', // Windows (XNA & DirectX)
					'x', // Xbox360 (XNA)
					'i', // iOS
					'a', // Android
					'd', // DesktopGL
					'X', // MacOSX
					'W', // WindowsStoreApp
					'n', // NativeClient
					'M', // WindowsPhone8
					'r', // RaspberryPi
					'P', // PlayStation4
					'5', // PlayStation5
					'O', // XboxOne
					'S', // Nintendo Switch
					'G', // Google Stadia
					'b', // WebAssembly and Bridge.NET
					'm', // WindowsPhone7.0 (XNA)
					'p', // PlayStationMobile
					'v', // PSVita
					'g', // Windows (OpenGL)
					'l', // Linux
			};
		}

		static void AddContentManager(std::shared_ptr<ContentManager>& contentManager) {
		}

		static void RemoveContentManager(std::shared_ptr<ContentManager>& contentManager) {
		}

		std::shared_ptr<ContentReader> GetContentReaderFromXnb(std::string const& originalAssetName, std::shared_ptr<cs::Stream>& stream, std::shared_ptr<cs::BinaryReader>& xnbReader);
	};
}

#endif