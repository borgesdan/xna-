#ifndef XNA_CURVE_HPP
#define XNA_CURVE_HPP

#include "enumerations.hpp"
#include "csharp/integralnumeric.hpp"
#include <cmath>
#include <algorithm>
#include <vector>

namespace xna {
    class CurveKey {
    public:
        constexpr CurveKey() = default;

        constexpr CurveKey(double position, double value) :
            position(position), internalValue(value) {}

        constexpr CurveKey(double position, double value, double tangentIn, double tangentOut) :
            position(position), internalValue(value), tangentIn(tangentIn), tangentOut(tangentOut) {}

        constexpr CurveKey(double position, double value, double tangentIn, double tangentOut, CurveContinuity continuity) :
            position(position), internalValue(value), tangentIn(tangentIn),
            tangentOut(tangentOut), continuity(continuity)
        {}

        constexpr CurveKey Clone() {
            return CurveKey(position, internalValue, tangentIn, tangentOut, continuity);
        }

        constexpr bool Equals(CurveKey const& other) const {
            return other.position == position
                && other.internalValue == internalValue
                && other.tangentIn == tangentIn
                && other.tangentOut == tangentOut
                && other.continuity == continuity;
        }

        constexpr double Position() const {
            return position;
        }

        constexpr double Value() const {
            return internalValue;
        }

        constexpr void Value(double value) {
            internalValue = value;
        }

        constexpr double TangentIn() const {
            return tangentIn;
        }

        constexpr void TangentIn(double value) {
            tangentIn = value;
        }

        constexpr double TangentOut() const {
            return tangentIn;
        }

        constexpr void TangentOut(double value) {
            tangentIn = value;
        }

        constexpr CurveContinuity Continuity() const {
            return continuity;
        }

        constexpr void Continuity(CurveContinuity const& value) {
            continuity = value;
        }

        constexpr friend bool operator==(CurveKey const& a, CurveKey const& b) {
            return a.Equals(b);
        }

        constexpr friend bool operator!=(CurveKey const& a, CurveKey const& b) {
            return !a.Equals(b);
        }

        static constexpr bool BinaryCompare(const CurveKey& a, const CurveKey& b) {
            return a.position < b.position;
        }

    private:
        double position{ 0.0 };
        double internalValue{ 0.0 };
        double tangentOut{ 0.0 };
        double tangentIn{ 0.0 };
        CurveContinuity continuity{ CurveContinuity::Smooth };
    };
}

namespace xna {
    class CurveKeyCollection {
    public:

        constexpr CurveKeyCollection() = default;

        constexpr double TimeRange() const { return timeRange; }
        constexpr void TimeRange(double value) { timeRange = value; }
        constexpr double InvTimeRange() const { return invTimeRange; }
        constexpr void InvTimeRange(double value) { invTimeRange = value; }
        constexpr bool IsCacheAvailable() const { return isCacheAvailable; }
        constexpr void IsCacheAvailable(bool value) { isCacheAvailable = value; }

        constexpr size_t IndexOf(CurveKey const& item) const {
            auto it = std::find(keys.begin(), keys.end(), item);
            size_t index = -1;

            if (it != keys.end())
                index = it - keys.begin();

            return index;
        }

        constexpr void RemoveAt(size_t index) {
            keys.erase(keys.begin() + index);
        }

        constexpr CurveKey& Index(size_t index) { return keys[index]; }

        constexpr void Index(size_t index, CurveKey const& value) {
            if (keys[index].Position() == value.Position()) {
                keys[index] = value;
            }
            else {
                RemoveAt(index);
                Add(value);
            }
        }

        constexpr void Add(CurveKey const& item) {
            size_t index = binarySearch(item);

            if (index >= 0) {
                while (index < keys.size() && item.Position() == keys[index].Position())
                    ++index;
            }
            else
                index = ~index;

            keys.insert(keys.begin() + index, item);
            isCacheAvailable = false;
        }

        constexpr void Clear() {
            keys.clear();
            timeRange = invTimeRange = 0.0;
            isCacheAvailable = false;
        }

        constexpr bool Contains(CurveKey const& item) {
            //keys.contains(item);
            auto finder = std::find(keys.begin(), keys.end(), item);
            return finder != keys.end();
        }

        constexpr void CopyTo(std::vector<CurveKey>& array, size_t arrayIndex) {
            array = keys;
            isCacheAvailable = false;
        }

        constexpr size_t Count() const { return keys.size(); }

        constexpr bool IsReadOnly() const { return false; }

        constexpr bool Remove(CurveKey const& item) {
            isCacheAvailable = false;
            auto it = std::find(keys.begin(), keys.end(), item);
            keys.erase(it);
            return false;
        }

        constexpr CurveKeyCollection Clone() const {
            CurveKeyCollection collection;
            collection.keys = keys;
            collection.invTimeRange = invTimeRange;
            collection.timeRange = timeRange;
            collection.isCacheAvailable = true;

            return collection;
        }

        constexpr void ComputeCacheValues() {
            timeRange = invTimeRange = 0.0;

            if (keys.size() > 1) {
                timeRange = keys[keys.size() - 1].Position() - keys[0].Position();

                if (timeRange > 1.4012984643248171E-45)
                    invTimeRange = 1.0 / timeRange;
            }

            isCacheAvailable = false;
        }

    private:
        std::vector<CurveKey> keys;
        double timeRange{ 0.0 };
        double invTimeRange{ 0.0 };
        bool isCacheAvailable{ true };

        //https://www.geeksforgeeks.org/binary-search/
        constexpr size_t binarySearch(CurveKey const& To_Find) const {
            size_t lo = 0;
            size_t hi = keys.size() - 1;
            size_t mid = 0;

            while (hi - lo > 1) {
                const auto mid = (hi + lo) / 2;

                if (CurveKey::BinaryCompare(keys[mid], To_Find)) {
                    lo = mid + 1;
                }
                else {
                    hi = mid;
                }
            }

            if (keys[lo] == To_Find)
                return lo;
            else if (keys[hi] == To_Find)
                return hi;

            return -1;
        }
    };
}

namespace xna {
	class Curve {
	public:
        constexpr Curve() = default;

		constexpr CurveLoopType PreLoop() const { return preLoop; }
		constexpr void PreLoop(CurveLoopType const& value) { preLoop = value; }

		constexpr CurveLoopType PostLoop() const { return postLoop; }
		constexpr void PostLoop(CurveLoopType const& value) { postLoop = value; }

		constexpr CurveKeyCollection& Keys() { return keys; }

		constexpr bool IsConstant() const { return keys.Count() <= 1; }

		constexpr Curve Clone() const {
			Curve curve;
			curve.preLoop = preLoop;
			curve.postLoop = postLoop;
			curve.keys = keys.Clone();

            return curve;
		}

		void ComputeTangent(size_t keyIndex, CurveTangent const& tangentType) {
			ComputeTangent(keyIndex, tangentType, tangentType);
		}        

		void ComputeTangent(size_t keyIndex, CurveTangent const& tangentInType, CurveTangent const& tangentOutType) {
            if (keys.Count() <= keyIndex || keyIndex < 0)
                return;

            auto& key = keys.Index(keyIndex);
            double position = 0.0;
            auto num1 = (position = key.Position());
            const auto num2 = position;
            auto num3 = position;
            
            double num4 = 0.0;
            auto num5 = (num4 = key.Value());
            const auto num6 = num4;
            auto num7 = num4;
            
            if (keyIndex > 0) {
                num3 = keys.Index(keyIndex - 1).Position();
                num7 = keys.Index(keyIndex - 1).Value();
            }

            if (keyIndex + 1 < keys.Count()) {
                num1 = keys.Index(keyIndex + 1).Position();
                num5 = keys.Index(keyIndex + 1).Value();
            }

            switch (tangentInType) {
            case CurveTangent::Linear:
                key.TangentIn(num6 - num7);
                break;            
            case CurveTangent::Smooth: {
                const auto num8 = num1 - num3;
                const auto num9 = num5 - num7;
                key.TangentIn(abs(num9) >= 1.1920928955078125E-07 
                    ? num9 * abs(num3 - num2) / num8 
                    : 0.0);
                break;
            }                
            default:
                key.TangentIn(0.0);
                break;
            }

            switch (tangentOutType)
            {
            case CurveTangent::Linear:
                key.TangentOut(num5 - num6);
                break;
            case CurveTangent::Smooth:{
                const auto num10 = num1 - num3;
                const auto num11 = num5 - num7;
                if (abs(num11) < 1.1920928955078125E-07) {
                    key.TangentOut(0.0);
                    break;
                }
                key.TangentOut(num11 * abs(num1 - num2) / num10);
                break;
            }
            default:
                key.TangentOut(0.0);
                break;
            }
		}

        void ComputeTangents(CurveTangent const& tangentType) {
            ComputeTangents(tangentType, tangentType);
        }

        void ComputeTangents(CurveTangent const& tangentInType, CurveTangent const& tangentOutType) {
            for (size_t keyIndex = 0; keyIndex < keys.Count(); ++keyIndex)
                ComputeTangent(keyIndex, tangentInType, tangentOutType);
        }

        double Evaluate(double position) {
            if (keys.Count() == 0)
                return 0.0;

            if (keys.Count() == 1)
                return keys.Index(0).Value();

            auto& key1 = keys.Index(0);
            auto& key2 = keys.Index(keys.Count() - 1);
            auto t = position;
            auto num1 = 0.0;

            if (t < key1.Position()) {
                if (preLoop == CurveLoopType::Constant)
                    return key1.Value();

                if (preLoop == CurveLoopType::Linear)
                    return key1.Value() - key1.TangentIn() * (key1.Position() - t);

                if (!keys.IsCacheAvailable())
                    keys.ComputeCacheValues();

                auto num2 = CalcCycle(t);
                auto num3 = t - (key1.Position() + num2 * keys.TimeRange());

                if (preLoop == CurveLoopType::Cycle)
                    t = key1.Position() + num3;

                else if (preLoop == CurveLoopType::CycleOffset) {
                    t = key1.Position() + num3;
                    num1 = (key2.Value() - key1.Value()) * num2;
                }
                else {
                    t = (static_cast<csint>(num2) & 1) != 0 ? key2.Position() - num3 : key1.Position() + num3;
                }
            }
            else if (key2.Position() < t)
            {
                if (postLoop == CurveLoopType::Constant)
                    return key2.Value();
               
                if (postLoop == CurveLoopType::Linear)
                    return key2.Value() - key2.TangentOut() * (key2.Position() - t);
               
                if (!keys.IsCacheAvailable())
                    keys.ComputeCacheValues();

                auto num4 = CalcCycle(t);
                auto num5 = t - (key1.Position() + num4 * keys.TimeRange());

                if (postLoop == CurveLoopType::Cycle)
                    t = key1.Position() + num5;

                else if (postLoop == CurveLoopType::CycleOffset) {
                    t = key1.Position() + num5;
                    num1 = (key2.Value() - key1.Value()) * num4;
                }
                else
                    t = (static_cast<csint>(num4) & 1) != 0 ? key2.Position() - num5 : key1.Position() + num5;
            }

            CurveKey k0;
            CurveKey k1;
            const auto segment = FindSegment(t, k0, k1);
            return num1 + Curve::Hermite(k0, k1, segment);
        }

	private:
		CurveLoopType preLoop{ CurveLoopType::Constant };
		CurveLoopType postLoop{ CurveLoopType::Constant };
		CurveKeyCollection keys;
        
        constexpr double CalcCycle(double t) {
            auto num = (t - keys.Index(0).Position()) * keys.InvTimeRange();
            
            if (num < 0.0)
                --num;

            return num;
        }

        constexpr double FindSegment(double t, CurveKey& k0, CurveKey& k1) {
            auto segment = t;
            k0 = keys.Index(0);
            const auto size = keys.Count();

            for (size_t index = 1; index < size; ++index) {
                k1 = keys.Index(index);

                if (k1.Position() >= t) {
                    auto position1 = k0.Position();
                    auto position2 = k1.Position();
                    auto num1 = t;
                    auto num2 = position2 - position1;

                    segment = 0.0;

                    if (num2 > 1E-10) {
                        segment = ((num1 - position1) / num2);
                        break;
                    }

                    break;
                }

                k0 = k1;
            }

            return segment;
        }

        static constexpr double Hermite(CurveKey const& k0, CurveKey const& k1, double t) {
           
            if (k0.Continuity() == CurveContinuity::Step)
                return t >= 1.0 ? k1.Value() : k0.Value();

            const auto num1 = t * t;
            const auto num2 = num1 * t;
            const auto internalValue1 = k0.Value();
            const auto internalValue2 = k1.Value();
            const auto tangentOut = k0.TangentOut();
            const auto tangentIn = k1.TangentIn();

            return (internalValue1 * (2.0 * num2 - 3.0 * num1 + 1.0) + internalValue2 * (-2.0 * num2 + 3.0 * num1) + tangentOut * (num2 - 2.0 * num1 + t) + tangentIn * (num2 - num1));
        }
  
	};
}

#endif