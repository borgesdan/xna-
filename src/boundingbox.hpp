#ifndef XNA_BOUNDINGBOX_HPP
#define XNA_BOUNDINGBOX_HPP

#include <vector>
#include "csharp/integralnumeric.hpp"
#include "csharp/nullable.hpp"
#include "basic-structs.hpp"
#include "enumerations.hpp"

namespace xna {
	struct BoundingSphere;
	struct BoundingFrustum;
	struct Plane;
	struct Ray;

	struct BoundingBox {
		static constexpr csint CORNER_COUNT = 8;

		Vector3 Min{ Vector3::Zero() };
		Vector3 Max{ Vector3::Zero() };

		constexpr BoundingBox() = default;

		constexpr BoundingBox(Vector3 const& min, Vector3 const& max) :
			Min(min), Max(max) {}

		constexpr std::vector<Vector3> GetCorners() const {
			return std::vector<Vector3>{
				Vector3(Min.X, Max.Y, Max.Z),
					Vector3(Max.X, Max.Y, Max.Z),
					Vector3(Max.X, Min.Y, Max.Z),
					Vector3(Min.X, Min.Y, Max.Z),
					Vector3(Min.X, Max.Y, Min.Z),
					Vector3(Max.X, Max.Y, Min.Z),
					Vector3(Max.X, Min.Y, Min.Z),
					Vector3(Min.X, Min.Y, Min.Z)
			};
		}

		constexpr void GetCorners(std::vector<Vector3>& corners) const {
			if (corners.size() < 8)
				corners.resize(8);

			corners[0].X = Min.X;
			corners[0].Y = Max.Y;
			corners[0].Z = Max.Z;
			corners[1].X = Max.X;
			corners[1].Y = Max.Y;
			corners[1].Z = Max.Z;
			corners[2].X = Max.X;
			corners[2].Y = Min.Y;
			corners[2].Z = Max.Z;
			corners[3].X = Min.X;
			corners[3].Y = Min.Y;
			corners[3].Z = Max.Z;
			corners[4].X = Min.X;
			corners[4].Y = Max.Y;
			corners[4].Z = Min.Z;
			corners[5].X = Max.X;
			corners[5].Y = Max.Y;
			corners[5].Z = Min.Z;
			corners[6].X = Max.X;
			corners[6].Y = Min.Y;
			corners[6].Z = Min.Z;
			corners[7].X = Min.X;
			corners[7].Y = Min.Y;
			corners[7].Z = Min.Z;
		}

		constexpr bool Equals(BoundingBox const& other) const {
			return Min == other.Min && Max == other.Max;
		}

		static constexpr BoundingBox CreateMerged(BoundingBox const& original, BoundingBox const& additional) {
			BoundingBox merged;
			merged.Min = Vector3::Min(original.Min, additional.Min);
			merged.Max = Vector3::Max(original.Max, additional.Max);
			return merged;
		}

		static constexpr BoundingBox CreateFromPoints(std::vector<Vector3>& points) {
			bool flag = false;
			Vector3 result1 = Vector3(std::numeric_limits<double>::max());
			Vector3 result2 = Vector3(std::numeric_limits<double>::min());

			auto size = points.size();

			for (size_t i = 0; i < size; ++i) {
				auto& point = points[i];
				result1 = Vector3::Min(result1, point);
				result2 = Vector3::Max(result2, point);
				flag = true;
			}

			if (!flag)
				BoundingBox();

			return BoundingBox(result1, result2);
		}

		constexpr bool Intersects(BoundingBox const& box) const {
			return Max.X >= box.Min.X
				&& Min.X <= box.Max.X
				&& Max.Y >= box.Min.Y
				&& Min.Y <= box.Max.Y
				&& Max.Z >= box.Min.Z
				&& Min.Z <= box.Max.Z;
		}

		constexpr ContainmentType Contains(BoundingBox const& box) const {
			if (Max.X < box.Min.X || Min.X > box.Max.X || Max.Y < box.Min.Y || Min.Y > box.Max.Y || Max.Z < box.Min.Z || Min.Z > box.Max.Z)
				return ContainmentType::Disjoint;

			return Min.X > box.Min.X || box.Max.X > Max.X || Min.Y > box.Min.Y || box.Max.Y > Max.Y || Min.Z > box.Min.Z || box.Max.Z > Max.Z
				? ContainmentType::Intersects
				: ContainmentType::Contains;
		}

		constexpr ContainmentType Contains(Vector3 const& point) const {
			return Min.X > point.X || point.X > Max.X || Min.Y > point.Y || point.Y > Max.Y || Min.Z > point.Z || point.Z > Max.Z ? ContainmentType::Disjoint : ContainmentType::Contains;
		}

		constexpr Vector3 SupportMapping(Vector3 const& v) const {
			Vector3 result;
			result.X = v.X >= 0.0 ? Max.X : Min.X;
			result.Y = v.Y >= 0.0 ? Max.Y : Min.Y;
			result.Z = v.Z >= 0.0 ? Max.Z : Min.Z;

			return result;
		}

		constexpr friend bool operator==(BoundingBox const& a, BoundingBox const& b) {
			return a.Equals(b);
		}

		constexpr friend bool operator!=(BoundingBox const& a, BoundingBox const& b) {
			return !a.Equals(b);
		}

		//Implementado em BoundingFrustum

		constexpr bool Intersects(BoundingFrustum& frustum) const;
		constexpr ContainmentType Contains(BoundingFrustum& frustum) const;

		//Implementado em BoundingSphere

		static constexpr BoundingBox CreateFromSphere(BoundingSphere const& sphere);
		constexpr bool Intersects(BoundingSphere const& sphere) const;
		constexpr ContainmentType Contains(BoundingSphere const& sphere) const;

		//Implementado em Plane e Ray

		constexpr PlaneIntersectionType Intersects(Plane const& plane) const;
		constexpr cs::n_double Intersects(Ray const& ray) const;

	};
}

#endif