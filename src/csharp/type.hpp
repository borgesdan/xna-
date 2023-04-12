#ifndef XNA_TYPE_HPP
#define XNA_TYPE_HPP

#include <any>
#include <typeinfo>
#include <typeindex>
#include <type_traits>

namespace cs {
	struct Type {
		constexpr Type() = default;

		template <typename T>
		Type(T const& obj) : obj(typeid(T)) {
			isArray = std::is_array<T>::value;
			isIntegral = std::is_integral<T>::value;
			isFloatingPoint = std::is_floating_point<T>::value;
			isEnum = std::is_enum<T>::value;
			isClass = std::is_class<T>::value;
			isVoid = std::is_void<T>::value;
			isPointer = std::is_pointer<T>::value;
		}

		bool Equals(Type const& other) const {
			return obj == other.obj;
		}

		friend bool operator==(Type const& t1, Type const& t2) {
			return t1.Equals(t2);
		}

		friend bool operator!=(Type const& t1, Type const& t2) {
			return !t1.Equals(t2);
		}		

		template <typename T>
		static Type TypeOf(T const& obj) {
			return Type(obj);
		}

		template <typename T>
		static Type TypeOf() {
			Type type;
			type.isArray = std::is_array<T>::value;
			type.isIntegral = std::is_integral<T>::value;
			type.isFloatingPoint = std::is_floating_point<T>::value;
			type.isEnum = std::is_enum<T>::value;
			type.isClass = std::is_class<T>::value;
			type.isVoid = std::is_void<T>::value;
			type.isPointer = std::is_pointer<T>::value;

			return type;
		}

	private:
		std::type_index obj{ std::any().type() };
		bool isArray{ false };
		bool isIntegral{ false };
		bool isFloatingPoint{ false };		
		bool isEnum{ false };		
		bool isClass{ false };		
		bool isVoid{ false };		
		bool isPointer{ false };		

	public:
		constexpr bool IsArray() const {
			return isArray;			
		}

		constexpr bool IsIntegral() const {
			return isIntegral;
		}

		constexpr bool IsFloatingPoint() const {
			return isFloatingPoint;
		}

		constexpr bool IsEnum() const {
			return isEnum;
		}

		constexpr bool IsClass() const {
			return isClass;
		}

		constexpr bool IsVoid() const {
			return isVoid;
		}

		constexpr bool IsPointer() const {
			return isPointer;
		}

		std::type_index TypeIndex() const {
			return obj;
		}
	};	

	template <typename T>
	Type typeof(T const& obj) {
		return Type::TypeOf(obj);
	}

	template <typename T>
	Type typeof() {
		return Type::TypeOf<T>();
	}
}

#endif