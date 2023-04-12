#ifndef CS_NUMERIC_HPP
#define CS_NUMERIC_HPP

#include <cmath>
#include <cstdint>
#include <limits>

//ref: https://learn.microsoft.com/pt-br/dotnet/csharp/language-reference/builtin-types/integral-numeric-types

namespace cs {
    using Sbyte = int8_t;
    using Byte = uint8_t;
    using Short = int16_t;
    using Ushort = uint16_t;
    using Int = int32_t;
    using Uint = uint32_t;
    using Long = int64_t;
    using Ulong = uint64_t;    

    struct Limits {
        static constexpr Sbyte SbyteMax = std::numeric_limits<Sbyte>::max();
        static constexpr Sbyte SbyteMin = std::numeric_limits<Sbyte>::min();

        static constexpr Byte ByteMax = std::numeric_limits<Byte>::max();
        static constexpr Byte ByteMin = std::numeric_limits<Byte>::min();

        static constexpr Short ShortMax = std::numeric_limits<Short>::max();
        static constexpr Short ShortMin = std::numeric_limits<Short>::min();

        static constexpr Ushort UshortMax = std::numeric_limits<Ushort>::max();
        static constexpr Ushort UshortMin = std::numeric_limits<Ushort>::min();

        static constexpr Int IntMax = std::numeric_limits<Int>::max();
        static constexpr Int IntMin = std::numeric_limits<Int>::min();

        static constexpr Uint UintMax = std::numeric_limits<Uint>::max();
        static constexpr Uint UintMin = std::numeric_limits<Uint>::min();

        static constexpr Long LongMax = std::numeric_limits<Long>::max();
        static constexpr Long LongMin = std::numeric_limits<Long>::min();

        static constexpr Ulong UlongMax = std::numeric_limits<Ulong>::max();
        static constexpr Ulong UlongMin = std::numeric_limits<Ulong>::min();

        static constexpr float FloatMax = std::numeric_limits<float>::max();
        static constexpr float FloatMin = std::numeric_limits<float>::min();

        static constexpr double DoubleMax = std::numeric_limits<double>::max();
        static constexpr double DoubleMin = std::numeric_limits<double>::min();
    };

    struct Floating {
        static constexpr bool IsInfinity(float value) { return value == std::numeric_limits<float>::infinity(); }
        static constexpr bool IsInfinity(double value) { return value == std::numeric_limits<double>::infinity(); }
        
        static constexpr bool IsNaN(float value) { return value == std::numeric_limits<float>::quiet_NaN(); }
        static constexpr bool IsNaN(double value) { return value == std::numeric_limits<double>::quiet_NaN(); }
        
        static constexpr bool IsNegativeInfinity(float value) { 
            /*if (!std::numeric_limits<double>::is_iec559)
                return false;*/

            return value == -std::numeric_limits<float>::infinity();
        }

        static constexpr bool IsNegativeInfinity(double value) {
            /*if (!std::numeric_limits<double>::is_iec559)
                return false;*/
            
            return value == -std::numeric_limits<double>::infinity();            
        }
    };

    struct Convert {
        template <typename T>
        static constexpr Int ToInt(T value) { return static_cast<Int>(value); }

        template <typename T>
        static constexpr Uint ToUint(T value) { return static_cast<Uint>(value); }

        template <typename T>
        static constexpr Long ToLong(T value) { return static_cast<Long>(value); }

        template <typename T>
        static constexpr Ulong ToUlong(T value) { return static_cast<Ulong>(value); }

        template <typename T>
        static constexpr Sbyte ToSbyte(T value) { return static_cast<Sbyte>(value); }    
        
        template <typename T>
        static constexpr Byte ToByte(T value) { return static_cast<Byte>(value); }

        template <typename T>
        static constexpr Short ToShort(T value) { return static_cast<Short>(value); }
        
        template <typename T>
        static constexpr Ushort ToUshort(T value) { return static_cast<Ushort>(value); }

        template <typename T>
        static constexpr float ToFloat(T value) { return static_cast<float>(value); }

        template <typename T>
        static constexpr double ToDouble(T value) { return static_cast<double>(value); }
    };
}

#endif