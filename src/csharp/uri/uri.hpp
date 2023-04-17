#ifndef CS_URI_URI_HPP
#define CS_URI_URI_HPP

#include <string>
#include <memory>
#include <map>
#include "../integralnumeric.hpp"

namespace cs {
	enum class Flags : csulong
	{
		Zero = 0x00000000,

		SchemeNotCanonical = 0x1,
		UserNotCanonical = 0x2,
		HostNotCanonical = 0x4,
		PortNotCanonical = 0x8,
		PathNotCanonical = 0x10,
		QueryNotCanonical = 0x20,
		FragmentNotCanonical = 0x40,
		CannotDisplayCanonical = 0x7F,

		E_UserNotCanonical = 0x80,
		E_HostNotCanonical = 0x100,
		E_PortNotCanonical = 0x200,
		E_PathNotCanonical = 0x400,
		E_QueryNotCanonical = 0x800,
		E_FragmentNotCanonical = 0x1000,
		E_CannotDisplayCanonical = 0x1F80,

		ShouldBeCompressed = 0x2000,
		FirstSlashAbsent = 0x4000,
		BackslashInPath = 0x8000,

		IndexMask = 0x0000FFFF,
		HostTypeMask = 0x00070000,
		HostNotParsed = 0x00000000,
		IPv6HostType = 0x00010000,
		IPv4HostType = 0x00020000,
		DnsHostType = 0x00030000,
		UncHostType = 0x00040000,
		BasicHostType = 0x00050000,
		UnusedHostType = 0x00060000,
		UnknownHostType = 0x00070000,

		UserEscaped = 0x00080000,
		AuthorityFound = 0x00100000,
		HasUserInfo = 0x00200000,
		LoopbackHost = 0x00400000,
		NotDefaultPort = 0x00800000,

		UserDrivenParsing = 0x01000000,
		CanonicalDnsHost = 0x02000000,
		ErrorOrParsingRecursion = 0x04000000,

		DosPath = 0x08000000,
		UncPath = 0x10000000,
		ImplicitFile = 0x20000000,
		MinimalUriInfoSet = 0x40000000,
		AllUriInfoSet = 0x80000000,
		IdnHost = 0x100000000,
		HasUnicode = 0x200000000,
		HostUnicodeNormalized = 0x400000000,
		RestUnicodeNormalized = 0x800000000,
		UnicodeHost = 0x1000000000,
		IntranetUri = 0x2000000000,

		UserIriCanonical = 0x8000000000,
		PathIriCanonical = 0x10000000000,
		QueryIriCanonical = 0x20000000000,
		FragmentIriCanonical = 0x40000000000,
		IriCanonical = 0x78000000000,
		UnixPath = 0x100000000000,

		DisablePathAndQueryCanonicalization = 0x200000000000,

		CustomParser_ParseMinimalAlreadyCalled = 0x4000000000000000,

		Debug_LeftConstructor = 0x8000000000000000
	};

	struct Offset {
		csushort Scheme{ 0 };
		csushort User{ 0 };
		csushort Host{ 0 };
		csushort PortValue{ 0 };
		csushort Path{ 0 };
		csushort Query{ 0 };
		csushort Fragment{ 0 };
		csushort End{ 0 };
	};

	struct MoreInfo {
		std::string Path;
		std::string Query;
		std::string Fragment;
		std::string AbsoluteUri;
		std::string RemoteUrl;
	};

	struct UriInfo {
		std::shared_ptr<Offset> Offset_;
		std::string String;
		std::string Host;
		std::string IdnHost;
		std::string PathAndQuery;
		std::string ScopeId;

		std::shared_ptr<MoreInfo> MoreInfo_() {
			return _moreInfo;
		}

	private:
		std::shared_ptr<MoreInfo> _moreInfo;
	};

	enum class UriSyntaxFlags : csint {
		None = 0x0,

		MustHaveAuthority = 0x1,
		OptionalAuthority = 0x2,
		MayHaveUserInfo = 0x4,
		MayHavePort = 0x8,
		MayHavePath = 0x10,
		MayHaveQuery = 0x20,
		MayHaveFragment = 0x40,

		AllowEmptyHost = 0x80,
		AllowUncHost = 0x100,
		AllowDnsHost = 0x200,
		AllowIPv4Host = 0x400,
		AllowIPv6Host = 0x800,
		AllowAnInternetHost = AllowDnsHost | AllowIPv4Host | AllowIPv6Host,
		AllowAnyOtherHost = 0x1000,

		FileLikeUri = 0x2000,
		MailToLikeUri = 0x4000,

		V1_UnknownUri = 0x10000,
		SimpleUserSyntax = 0x20000,
		BuiltInSyntax = 0x40000,
		ParserSchemeOnly = 0x80000,

		AllowDOSPath = 0x100000,
		PathIsRooted = 0x200000,
		ConvertPathSlashes = 0x400000,
		CompressPath = 0x800000,
		CanonicalizeAsFilePath = 0x1000000,
		UnEscapeDotsAndSlashes = 0x2000000,
		AllowIdn = 0x4000000,
		AllowIriParsing = 0x10000000,
	};
	
	enum class UriKind : csint {
		RelativeOrAbsolute = 0,
		Absolute = 1,
		Relative = 2
	};
	
	enum class UriComponents : csint {
		Scheme = 0x1,
		UserInfo = 0x2,
		Host = 0x4,
		Port = 0x8,
		Path = 0x10,
		Query = 0x20,
		Fragment = 0x40,

		StrongPort = 0x80,
		NormalizedHost = 0x100,

		KeepDelimiter = 0x40000000,

		SerializationInfoString = toint(0x80000000),

		AbsoluteUri = Scheme | UserInfo | Host | Port | Path | Query | Fragment,
		HostAndPort = Host | StrongPort,
		StrongAuthority = UserInfo | Host | StrongPort,
		SchemeAndServer = Scheme | Host | Port,
		HttpRequestUrl = Scheme | Host | Port | Path | Query,
		PathAndQuery = Path | Query,
	};
	
	enum class UriFormat : csint {
		UriEscaped = 1,
		Unescaped = 2,
		SafeUnescaped = 3
	};
	
	enum class UriIdnScope : csint {
		None,
		AllExceptIntranet,
		All
	};

	enum class ParsingError : csint {
		None = 0,

		BadFormat = 1,
		BadScheme = 2,
		BadAuthority = 3,
		EmptyUriString = 4,
		LastRelativeUriOkErrIndex = 4,

		SchemeLimit = 5,
		SizeLimit = 6,
		MustRootedPath = 7,

		BadHostName = 8,
		NonEmptyHost = 9,
		BadPort = 10,
		BadAuthorityTerminator = 11,

		CannotCreateRelative = 12
	};
	
	enum class UnescapeMode : csint {
		CopyOnly = 0x0,
		Escape = 0x1,
		Unescape = 0x2,
		EscapeUnescape = Unescape | Escape,
		V1ToStringFlag = 0x4,
		UnescapeAll = 0x8,
		UnescapeAllOrThrow = 0x10 | UnescapeAll,
	};

	struct UriCreationOptions {
		bool DangerousDisablePathAndQueryCanonicalization{ false };
	};
}

//UriParser
namespace cs {

	class UriParser {
	public:
		constexpr UriParser(UriSyntaxFlags flags):
		_flags(flags),
		_scheme(std::string()) {
		}

		constexpr std::string SchemeName() {
			return _scheme;
		}

	public:
		static constexpr UriParser HttpUri();
		static constexpr UriParser HttpsUri();
		static constexpr UriParser WsUri();
		static constexpr UriParser WssUri();
		static constexpr UriParser FtpUri();
		static constexpr UriParser FileUri();
		static constexpr UriParser UnixFileUri();
		static constexpr UriParser GopherUri();
		static constexpr UriParser NntpUri();
		static constexpr UriParser NewsUri();
		static constexpr UriParser MailToUri();
		static constexpr UriParser UuidUri();
		static constexpr UriParser TelnetUri();
		static constexpr UriParser LdapUri();
		static constexpr UriParser NetTcpUri();
		static constexpr UriParser NetPipeUri();
		static constexpr UriParser VsMacrosUri();

		static constexpr csint NoDefaultPort = -1;

	private:

	protected:
		csint _port{ 0 };
		std::string _scheme;

	private:
		UriSyntaxFlags _flags{ UriSyntaxFlags::None };			
		
		static constexpr csint c_InitialTableSize = 25;
		static std::map<std::string, std::string> s_table;

		static constexpr UriSyntaxFlags UnknownV1SyntaxFlags = static_cast<UriSyntaxFlags>(
			toulong(UriSyntaxFlags::V1_UnknownUri) |
			toulong(UriSyntaxFlags::OptionalAuthority) |
			toulong(UriSyntaxFlags::MayHaveUserInfo) |
			toulong(UriSyntaxFlags::MayHavePort) |
			toulong(UriSyntaxFlags::MayHavePath) |
			toulong(UriSyntaxFlags::MayHaveQuery) |
			toulong(UriSyntaxFlags::MayHaveFragment) |
			toulong(UriSyntaxFlags::AllowEmptyHost) |
			toulong(UriSyntaxFlags::AllowUncHost) |
			toulong(UriSyntaxFlags::AllowAnInternetHost) |
			toulong(UriSyntaxFlags::PathIsRooted) |
			toulong(UriSyntaxFlags::AllowDOSPath) |
			toulong(UriSyntaxFlags::ConvertPathSlashes) |
			toulong(UriSyntaxFlags::CompressPath) |
			toulong(UriSyntaxFlags::AllowIdn) |
			toulong(UriSyntaxFlags::AllowIriParsing));

		static constexpr UriSyntaxFlags HttpSyntaxFlags = static_cast<UriSyntaxFlags>(
			toulong(UriSyntaxFlags::MustHaveAuthority) |
			toulong(UriSyntaxFlags::MayHaveUserInfo) |
			toulong(UriSyntaxFlags::MayHavePort) |
			toulong(UriSyntaxFlags::MayHavePath) |
			toulong(UriSyntaxFlags::MayHaveQuery) |
			toulong(UriSyntaxFlags::MayHaveFragment) |
			toulong(UriSyntaxFlags::AllowUncHost) |
			toulong(UriSyntaxFlags::AllowAnInternetHost) |
			toulong(UriSyntaxFlags::PathIsRooted) |
			toulong(UriSyntaxFlags::ConvertPathSlashes) |
			toulong(UriSyntaxFlags::CompressPath) |
			toulong(UriSyntaxFlags::CanonicalizeAsFilePath) |
			toulong(UriSyntaxFlags::AllowIdn) |
			toulong(UriSyntaxFlags::AllowIriParsing));

		static constexpr UriSyntaxFlags FtpSyntaxFlags = static_cast<UriSyntaxFlags>(
			toulong(UriSyntaxFlags::MustHaveAuthority) |
			toulong(UriSyntaxFlags::MayHaveUserInfo) |
			toulong(UriSyntaxFlags::MayHavePort) |
			toulong(UriSyntaxFlags::MayHavePath) |
			toulong(UriSyntaxFlags::MayHaveFragment) |
			toulong(UriSyntaxFlags::AllowUncHost) |
			toulong(UriSyntaxFlags::AllowAnInternetHost) |
			toulong(UriSyntaxFlags::PathIsRooted) |
			toulong(UriSyntaxFlags::ConvertPathSlashes) |
			toulong(UriSyntaxFlags::CompressPath) |
			toulong(UriSyntaxFlags::CanonicalizeAsFilePath) |
			toulong(UriSyntaxFlags::AllowIdn) |
			toulong(UriSyntaxFlags::AllowIriParsing));

		static constexpr UriSyntaxFlags FileSyntaxFlags = static_cast<UriSyntaxFlags>(
			toulong(UriSyntaxFlags::MustHaveAuthority) |
			toulong(UriSyntaxFlags::AllowEmptyHost) |
			toulong(UriSyntaxFlags::AllowUncHost) |
			toulong(UriSyntaxFlags::AllowAnInternetHost) |
			toulong(UriSyntaxFlags::MayHavePath) |
			toulong(UriSyntaxFlags::MayHaveFragment) |
			toulong(UriSyntaxFlags::MayHaveQuery) |
			toulong(UriSyntaxFlags::FileLikeUri) |
			toulong(UriSyntaxFlags::PathIsRooted) |
			toulong(UriSyntaxFlags::AllowDOSPath) |
			toulong(UriSyntaxFlags::ConvertPathSlashes) |
			toulong(UriSyntaxFlags::CompressPath) |
			toulong(UriSyntaxFlags::CanonicalizeAsFilePath) |
			toulong(UriSyntaxFlags::UnEscapeDotsAndSlashes) |
			toulong(UriSyntaxFlags::AllowIdn) |
			toulong(UriSyntaxFlags::AllowIriParsing));

		static constexpr UriSyntaxFlags UnixFileSyntaxFlags = static_cast<UriSyntaxFlags>(
			toulong(FileSyntaxFlags) & ~toulong(UriSyntaxFlags::ConvertPathSlashes));

		static constexpr UriSyntaxFlags VsmacrosSyntaxFlags = static_cast<UriSyntaxFlags>(
			toulong(UriSyntaxFlags::MustHaveAuthority) |
			toulong(UriSyntaxFlags::AllowEmptyHost) |
			toulong(UriSyntaxFlags::AllowUncHost) |
			toulong(UriSyntaxFlags::AllowAnInternetHost) |
			toulong(UriSyntaxFlags::MayHavePath) |
			toulong(UriSyntaxFlags::MayHaveFragment) |
			toulong(UriSyntaxFlags::FileLikeUri) |
			toulong(UriSyntaxFlags::AllowDOSPath) |
			toulong(UriSyntaxFlags::ConvertPathSlashes) |
			toulong(UriSyntaxFlags::CompressPath) |
			toulong(UriSyntaxFlags::CanonicalizeAsFilePath) |
			toulong(UriSyntaxFlags::UnEscapeDotsAndSlashes) |
			toulong(UriSyntaxFlags::AllowIdn) |
			toulong(UriSyntaxFlags::AllowIriParsing));

		static constexpr UriSyntaxFlags GopherSyntaxFlags = static_cast<UriSyntaxFlags>(
			toulong(UriSyntaxFlags::MustHaveAuthority) |
			toulong(UriSyntaxFlags::MayHaveUserInfo) |
			toulong(UriSyntaxFlags::MayHavePort) |
			toulong(UriSyntaxFlags::MayHavePath) |
			toulong(UriSyntaxFlags::MayHaveFragment) |
			toulong(UriSyntaxFlags::AllowUncHost) |
			toulong(UriSyntaxFlags::AllowAnInternetHost) |
			toulong(UriSyntaxFlags::PathIsRooted) |
			toulong(UriSyntaxFlags::AllowIdn) |
			toulong(UriSyntaxFlags::AllowIriParsing));

		static constexpr UriSyntaxFlags NewsSyntaxFlags = static_cast<UriSyntaxFlags>(
			toulong(UriSyntaxFlags::MayHavePath) |
			toulong(UriSyntaxFlags::MayHaveFragment) |
			toulong(UriSyntaxFlags::AllowIriParsing));

		static constexpr UriSyntaxFlags NntpSyntaxFlags = static_cast<UriSyntaxFlags>(
			toulong(UriSyntaxFlags::MustHaveAuthority) |
			toulong(UriSyntaxFlags::MayHaveUserInfo) |
			toulong(UriSyntaxFlags::MayHavePort) |
			toulong(UriSyntaxFlags::MayHavePath) |
			toulong(UriSyntaxFlags::MayHaveFragment) |
			toulong(UriSyntaxFlags::AllowUncHost) |
			toulong(UriSyntaxFlags::AllowAnInternetHost) |
			toulong(UriSyntaxFlags::PathIsRooted) |
			toulong(UriSyntaxFlags::AllowIdn) |
			toulong(UriSyntaxFlags::AllowIriParsing));

		static constexpr UriSyntaxFlags TelnetSyntaxFlags = static_cast<UriSyntaxFlags>(
			toulong(UriSyntaxFlags::MustHaveAuthority) |
			toulong(UriSyntaxFlags::MayHaveUserInfo) |
			toulong(UriSyntaxFlags::MayHavePort) |
			toulong(UriSyntaxFlags::MayHavePath) |
			toulong(UriSyntaxFlags::MayHaveFragment) |
			toulong(UriSyntaxFlags::AllowUncHost) |
			toulong(UriSyntaxFlags::AllowAnInternetHost) |
			toulong(UriSyntaxFlags::PathIsRooted) |
			toulong(UriSyntaxFlags::AllowIdn) |
			toulong(UriSyntaxFlags::AllowIriParsing));

		static constexpr UriSyntaxFlags LdapSyntaxFlags = static_cast<UriSyntaxFlags>(
			toulong(UriSyntaxFlags::MustHaveAuthority) |
			toulong(UriSyntaxFlags::AllowEmptyHost) |
			toulong(UriSyntaxFlags::AllowUncHost) |
			toulong(UriSyntaxFlags::AllowAnInternetHost) |
			toulong(UriSyntaxFlags::MayHaveUserInfo) |
			toulong(UriSyntaxFlags::MayHavePort) |
			toulong(UriSyntaxFlags::MayHavePath) |
			toulong(UriSyntaxFlags::MayHaveQuery) |
			toulong(UriSyntaxFlags::MayHaveFragment) |
			toulong(UriSyntaxFlags::PathIsRooted) |
			toulong(UriSyntaxFlags::AllowIdn) |
			toulong(UriSyntaxFlags::AllowIriParsing));

		static constexpr UriSyntaxFlags MailtoSyntaxFlags = static_cast<UriSyntaxFlags>(
			toulong(UriSyntaxFlags::AllowEmptyHost) |
			toulong(UriSyntaxFlags::AllowUncHost) |
			toulong(UriSyntaxFlags::AllowAnInternetHost) |
			toulong(UriSyntaxFlags::MayHaveUserInfo) |
			toulong(UriSyntaxFlags::MayHavePort) |
			toulong(UriSyntaxFlags::MayHavePath) |
			toulong(UriSyntaxFlags::MayHaveFragment) |
			toulong(UriSyntaxFlags::MayHaveQuery) |
			toulong(UriSyntaxFlags::MailToLikeUri) |
			toulong(UriSyntaxFlags::AllowIdn) |
			toulong(UriSyntaxFlags::AllowIriParsing));

		static constexpr UriSyntaxFlags NetPipeSyntaxFlags = static_cast<UriSyntaxFlags>(
			toulong(UriSyntaxFlags::MustHaveAuthority) |
			toulong(UriSyntaxFlags::MayHavePath) |
			toulong(UriSyntaxFlags::MayHaveQuery) |
			toulong(UriSyntaxFlags::MayHaveFragment) |
			toulong(UriSyntaxFlags::AllowAnInternetHost) |
			toulong(UriSyntaxFlags::PathIsRooted) |
			toulong(UriSyntaxFlags::ConvertPathSlashes) |
			toulong(UriSyntaxFlags::CompressPath) |
			toulong(UriSyntaxFlags::CanonicalizeAsFilePath) |
			toulong(UriSyntaxFlags::UnEscapeDotsAndSlashes) |
			toulong(UriSyntaxFlags::AllowIdn) |
			toulong(UriSyntaxFlags::AllowIriParsing));

		static constexpr UriSyntaxFlags NetTcpSyntaxFlags = static_cast<UriSyntaxFlags>(
			toulong(NetPipeSyntaxFlags) | toulong(UriSyntaxFlags::MayHavePort));
	};	

	struct BuiltInUriParser : public UriParser {
		constexpr BuiltInUriParser(std::string lwrCaseScheme, csint defaultPort, UriSyntaxFlags syntaxFlags) :
		UriParser(static_cast<UriSyntaxFlags>(toulong(syntaxFlags) | toulong(UriSyntaxFlags::SimpleUserSyntax) | toulong(UriSyntaxFlags::BuiltInSyntax))){
			_scheme = lwrCaseScheme;
			_port = defaultPort;
		}
	};

	constexpr UriParser UriParser::HttpUri() { return BuiltInUriParser("http", 80, HttpSyntaxFlags); }
	constexpr UriParser UriParser::HttpsUri() { return BuiltInUriParser("https", 443, HttpUri()._flags); }
	constexpr UriParser UriParser::WsUri() { return BuiltInUriParser("ws", 80, HttpSyntaxFlags); }
	constexpr UriParser UriParser::WssUri() { return BuiltInUriParser("wss", 443, HttpSyntaxFlags); }
	constexpr UriParser UriParser::FtpUri() { return BuiltInUriParser("ftp", 21, FtpSyntaxFlags); }
	constexpr UriParser UriParser::FileUri() { return BuiltInUriParser("file", NoDefaultPort, FileSyntaxFlags); }
	constexpr UriParser UriParser::UnixFileUri() { return BuiltInUriParser("file", NoDefaultPort, UnixFileSyntaxFlags); }
	constexpr UriParser UriParser::GopherUri() { return BuiltInUriParser("gopher", 70, GopherSyntaxFlags); }
	constexpr UriParser UriParser::NntpUri() { return BuiltInUriParser("nntp", 119, NntpSyntaxFlags); }
	constexpr UriParser UriParser::NewsUri() { return BuiltInUriParser("news", NoDefaultPort, NewsSyntaxFlags); }
	constexpr UriParser UriParser::MailToUri() { return BuiltInUriParser("mailto", 25, MailtoSyntaxFlags); }
	constexpr UriParser UriParser::UuidUri() { return BuiltInUriParser("uuid", NoDefaultPort, NewsUri()._flags); }
	constexpr UriParser UriParser::TelnetUri() { return BuiltInUriParser("telnet", 23, TelnetSyntaxFlags); }
	constexpr UriParser UriParser::LdapUri() { return BuiltInUriParser("ldap", 389, LdapSyntaxFlags); }
	constexpr UriParser UriParser::NetTcpUri() { return BuiltInUriParser("net.tcp", 808, NetTcpSyntaxFlags); }
	constexpr UriParser UriParser::NetPipeUri() { return BuiltInUriParser("net.pipe", NoDefaultPort, NetPipeSyntaxFlags); }
	constexpr UriParser UriParser::VsMacrosUri() { return BuiltInUriParser("vsmacros", NoDefaultPort, VsmacrosSyntaxFlags); }
}

//Uri
namespace cs {
	class Uri {
	public:
		constexpr Uri(std::string const& uriString) {
		}

		constexpr Uri(Uri const& baseUri, std::string const& relativeUri) {
		}

		constexpr bool UserDrivenParsing() const {
			return (toulong(_flags) & toulong(Flags::UserDrivenParsing)) != 0;
		}

		std::string LocalPath() const {
			if (IsNotAbsoluteUri())
				return std::string();

			return GetLocalPath();
		}

	public:
		static constexpr csint StackallThreshold = 512;
		static constexpr csint c_MaxUriBufferSize = 0xFFF0;
		static constexpr csint c_MaxUriSchemeName = 1024;
		Flags _flags{ Flags::Zero };

		static constexpr std::string UriSchemeFile() { return UriParser::FileUri().SchemeName();  }
		static constexpr std::string UriSchemeFtp() { return UriParser::FtpUri().SchemeName();  }
		static constexpr std::string UriSchemeSftp() { return "sftp";  }
		static constexpr std::string UriSchemeFtps() { return "ftps";  }
		static constexpr std::string UriSchemeGopher() { return UriParser::GopherUri().SchemeName();  }
		static constexpr std::string UriSchemeHttp() { return UriParser::HttpUri().SchemeName();  }
		static constexpr std::string UriSchemeHttps() { return UriParser::HttpsUri().SchemeName();  }
		static constexpr std::string UriSchemeWs() { return UriParser::WsUri().SchemeName();  }
		static constexpr std::string UriSchemeWss() { return UriParser::WssUri().SchemeName();  }
		static constexpr std::string UriSchemeMailto() { return UriParser::MailToUri().SchemeName();  }
		static constexpr std::string UriSchemeNews() { return UriParser::NewsUri().SchemeName();  }
		static constexpr std::string UriSchemeNntp() { return UriParser::NntpUri().SchemeName();  }
		static constexpr std::string UriSchemeSsh() { return "ssh";  }
		static constexpr std::string UriSchemeTelnet() { return UriParser::TelnetUri().SchemeName();  }
		static constexpr std::string UriSchemeNetTcp() { return UriParser::NetTcpUri().SchemeName();  }
		static constexpr std::string UriSchemeNetPipe() { return UriParser::NetPipeUri().SchemeName();  }
		static constexpr std::string SchemeDelimiter() { return "://";  }

	private:
		std::string _string;
		std::string _originalUnicodeString;
		std::shared_ptr<UriInfo> _info;
		std::shared_ptr<UriParser> _syntax;

	private:
		constexpr bool IsImplicitFile() const {
			return (toulong(_flags) & toulong(Flags::ImplicitFile)) != 0;
		}

		constexpr bool IsUncOrDosPath() const {
			return (toulong(_flags) & (toulong(Flags::UncPath) | toulong(Flags::DosPath))) != 0;
		}

		constexpr bool IsDosPath() const {
			return (toulong(_flags) & toulong(Flags::DosPath)) != 0;
		}

		constexpr bool IsUncPath() const {
			return (toulong(_flags) & toulong(Flags::UncPath)) != 0;
		}

		constexpr bool IsUnixPath() const {
			return (toulong(_flags) & toulong(Flags::UnixPath)) != 0;
		}

		constexpr Flags HostType() const {
			return static_cast<Flags>(toulong(_flags) & toulong(Flags::HostTypeMask));
		}

		bool IsNotAbsoluteUri() const {
			return _syntax == nullptr;
		}

		constexpr csint SecuredPathIndex() const {
			if (IsDosPath()) {
				const auto ch = _string[_info->Offset_->Path];
				return  (ch == '/' || ch == '\\') ? 3 : 2;
			}

			return 0;
		}

		constexpr bool NotAny(Flags flags) const {
			return (toulong(_flags) & toulong(flags)) == 0;
		}

		constexpr bool InFact(Flags flags) const {
			return (toulong(_flags) & toulong(flags)) != 0;
		}

		static constexpr bool StaticNotAny(Flags allFlags, Flags checkFlags) {
			return (toulong(allFlags) & toulong(checkFlags)) == 0;
		}

		static constexpr bool StaticInFact(Flags allFlags, Flags checkFlags) {
			return (toulong(allFlags) & toulong(checkFlags)) != 0;
		}		

		std::string GetLocalPath() const {
			return std::string(); //TODO Uri.GetLocalPath();
		}

		constexpr void CreateThis(std::string uri, bool dontEscape, UriKind uriKind, UriCreationOptions creationOptions) {
			
			if (toint(uriKind) < toint(UriKind::RelativeOrAbsolute) || toint(uriKind) > toint(UriKind::Relative)) {
				return;
			}

			_string = uri;

			if (dontEscape)
				_flags = static_cast<Flags>(toint(_flags) | toint(Flags::UserEscaped));

			if (creationOptions.DangerousDisablePathAndQueryCanonicalization)
				_flags = static_cast<Flags>(toint(_flags) | toint(Flags::DisablePathAndQueryCanonicalization));
			
			ParsingError err = ParseScheme(_string, _flags, _syntax);

			//InitializeUri(err, uriKind, out UriFormatException ? e);
			
			/*if (e != null)
				throw e;*/			
		}

		static constexpr ParsingError ParseScheme(std::string& uriString, Flags& flags, std::shared_ptr<UriParser>& syntax) {
			auto length = uriString.size();

			if (length == 0)
				return ParsingError::EmptyUriString;

			if (length >= c_MaxUriBufferSize)
				return ParsingError::SizeLimit;
			
			/*fixed(char* pUriString = uriString)
			{
				ParsingError err = ParsingError.None;
				int idx = ParseSchemeCheckImplicitFile(pUriString, length, ref err, ref flags, ref syntax);

				if (err != ParsingError.None)
					return err;

				flags |= (Flags)idx;
			}*/

			return ParsingError::None;
		}
	};
}

#endif