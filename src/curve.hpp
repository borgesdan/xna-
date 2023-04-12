#ifndef XNA_CURVE_HPP
#define XNA_CURVE_HPP

#include "enumerations.hpp"
#include "curvekeycollection.hpp"
#include "curvekey.hpp"
#include "csharp/integralnumeric.hpp"
#include <cmath>

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