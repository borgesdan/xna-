#include "plane.hpp"

namespace xna {
	void Plane::Normalize() {
		const auto d = (Normal.X * Normal.X + Normal.Y * Normal.Y + Normal.Z * Normal.Z);

		if (abs(d - 1.0) < 1.1920928955078125E-07)
			return;

		const auto num = 1.0 / sqrt(d);
		Normal.X *= num;
		Normal.Y *= num;
		Normal.Z *= num;
		D *= num;
	}
	
	Plane Plane::Normalize(Plane const& value)
	{
		const auto d = (value.Normal.X * value.Normal.X + value.Normal.Y * value.Normal.Y + value.Normal.Z * value.Normal.Z);
		
		if (abs(d - 1.0) < 1.1920928955078125E-07) {
			Plane plane;
			plane.Normal = value.Normal;
			plane.D = value.D;
			return plane;
		}

		const auto num = 1.0 / sqrt(d);
		Plane plane1;
		plane1.Normal.X = value.Normal.X * num;
		plane1.Normal.Y = value.Normal.Y * num;
		plane1.Normal.Z = value.Normal.Z * num;
		plane1.D = value.D * num;
		return plane1;
	}
}
