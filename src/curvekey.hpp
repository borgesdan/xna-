#ifndef XNA_CURVEKEY_HPP
#define XNA_CURVEKEY_HPP

#include "enumerations.hpp"

namespace xna {
	class CurveKey {
	public:
		constexpr CurveKey() = default;

		constexpr CurveKey(double position, double value) :
			position(position), internalValue(value){}

		constexpr CurveKey(double position, double value, double tangentIn, double tangentOut):
			position(position), internalValue(value), tangentIn(tangentIn), tangentOut(tangentOut){}

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

#endif