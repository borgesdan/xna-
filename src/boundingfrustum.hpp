#ifndef XNA_BOUNDINGFRUSTUM_HPP
#define XNA_BOUNDINGFRUSTUM_HPP

//BoundingFrustum implementado do arquivo do MonoGame.

#include <vector>
#include "vector.hpp"
#include "matrix.hpp"
#include "boundingbox.hpp"
#include "boundingsphere.hpp"
#include "ray.hpp"
#include "plane.hpp"
#include "csharp/integralnumeric.hpp"

namespace xna {
	struct BoundingFrustum {
		static constexpr csint PlaneCount{ 6 };
		static constexpr csint CornerCount{ 8 };

		constexpr BoundingFrustum(Matrix const& value) :
			_matrix(value) {
			CreatePlanes();
			CreateCorners();
		}

		constexpr Matrix Matrix_() const {
			return _matrix;
		}

		constexpr void Matrix_(Matrix const& value) {
			_matrix = value;
			CreatePlanes();
			CreateCorners();
		}

		constexpr Plane Near() const {
			return _planes[0];
		}

		constexpr Plane Far() const {
			return _planes[1];
		}

		constexpr Plane Left() const {
			return _planes[2];
		}

		constexpr Plane Right() const {
			return _planes[3];
		}

		constexpr Plane Top() const {
			return _planes[4];
		}

		constexpr Plane Bottom() const {
			return _planes[5];
		}

		constexpr ContainmentType Contains(BoundingBox const& box) const {
			auto intersects = false;

			for (size_t i = 0; i < PlaneCount; ++i) {
				auto planeIntersectionType = box.Intersects(_planes[i]);
				switch (planeIntersectionType)
				{
				case PlaneIntersectionType::Front:
					return ContainmentType::Disjoint;
				case PlaneIntersectionType::Intersecting:
					intersects = true;
					break;
				}
			}

			return intersects ? ContainmentType::Intersects : ContainmentType::Contains;
		}

		constexpr ContainmentType Contains(BoundingFrustum const& frustum) const {
			if (*this == frustum)
				return ContainmentType::Contains;

			auto intersects = false;

			for (size_t i = 0; i < PlaneCount; ++i) {
				PlaneIntersectionType planeIntersectionType = frustum.Intersects(_planes[i]);
				switch (planeIntersectionType)
				{
				case PlaneIntersectionType::Front:
					return ContainmentType::Disjoint;
				case PlaneIntersectionType::Intersecting:
					intersects = true;
					break;
				}
			}
			return intersects ? ContainmentType::Intersects : ContainmentType::Contains;
		}

		constexpr ContainmentType Contains(BoundingSphere const& sphere) const {
			auto intersects = false;
			for (size_t i = 0; i < PlaneCount; ++i) {
				auto planeIntersectionType = sphere.Intersects(_planes[i]);

				switch (planeIntersectionType)
				{
				case PlaneIntersectionType::Front:
					return ContainmentType::Disjoint;
				case PlaneIntersectionType::Intersecting:
					intersects = true;
					break;
				}
			}
			return intersects ? ContainmentType::Intersects : ContainmentType::Contains;
		}

		constexpr ContainmentType Contains(Vector3 const& point) const {
			for (size_t i = 0; i < PlaneCount; ++i) {
				if (PlaneHelper::ClassifyPoint(point, _planes[i]) > 0) {
					return ContainmentType::Disjoint;
				}
			}

			return ContainmentType::Contains;
		}

		constexpr std::vector<Vector3> GetCorners() const {
			return _corners;
		}

		constexpr void GetCorners(std::vector<Vector3>& corners) {
			if (corners.size() < 8)
				corners.resize(8);

			corners = _corners;
		}

		constexpr bool Intersects(BoundingBox const& box) const {
			return Contains(box) != ContainmentType::Disjoint;
		}

		constexpr bool Intersects(BoundingFrustum const& frustum) const {
			return Contains(frustum) != ContainmentType::Disjoint;
		}

		constexpr bool Intersects(BoundingSphere const& sphere) const {
			return Contains(sphere) != ContainmentType::Disjoint;
		}

		constexpr PlaneIntersectionType Intersects(Plane const& plane) const {
			auto result = plane.Intersects(_corners[0]);

			const auto size = _corners.size();
			for (size_t i = 1; i < size; i++)
				if (plane.Intersects(_corners[i]) != result)
					result = PlaneIntersectionType::Intersecting;

			return result;
		}

		constexpr cs::n_double Intersects(Ray const& ray) const {
			ContainmentType ctype = Contains(ray.Position);

			switch (ctype)
			{
			case ContainmentType::Disjoint:
				return nullptr;
			case ContainmentType::Contains:
				return 0.0;
			case ContainmentType::Intersects:
				return nullptr;
			default:
				return nullptr;
			}
		}

		constexpr bool Equals(BoundingFrustum const& other) const {
			return _matrix == other._matrix;
		}

		friend constexpr bool operator ==(BoundingFrustum const& a, BoundingFrustum const& b) {
			return a.Equals(b);
		}

		friend constexpr bool operator !=(BoundingFrustum const& a, BoundingFrustum const& b) {
			return !a.Equals(b);
		}

	private:
		Matrix _matrix;
		std::vector<Vector3> _corners = std::vector<Vector3>();
		std::vector<Plane> _planes = std::vector<Plane>();


		constexpr void CreateCorners() {
			IntersectionPoint(_planes[0], _planes[2], _planes[4], _corners[0]);
			IntersectionPoint(_planes[0], _planes[3], _planes[4], _corners[1]);
			IntersectionPoint(_planes[0], _planes[3], _planes[5], _corners[2]);
			IntersectionPoint(_planes[0], _planes[2], _planes[5], _corners[3]);
			IntersectionPoint(_planes[1], _planes[2], _planes[4], _corners[4]);
			IntersectionPoint(_planes[1], _planes[3], _planes[4], _corners[5]);
			IntersectionPoint(_planes[1], _planes[3], _planes[5], _corners[6]);
			IntersectionPoint(_planes[1], _planes[2], _planes[5], _corners[7]);
		}

		constexpr void CreatePlanes() {
			_planes[0] = Plane(-_matrix.M13, -_matrix.M23, -_matrix.M33, -_matrix.M43);
			_planes[1] = Plane(_matrix.M13 - _matrix.M14, _matrix.M23 - _matrix.M24, _matrix.M33 - _matrix.M34, _matrix.M43 - _matrix.M44);
			_planes[2] = Plane(-_matrix.M14 - _matrix.M11, -_matrix.M24 - _matrix.M21, -_matrix.M34 - _matrix.M31, -_matrix.M44 - _matrix.M41);
			_planes[3] = Plane(_matrix.M11 - _matrix.M14, _matrix.M21 - _matrix.M24, _matrix.M31 - _matrix.M34, _matrix.M41 - _matrix.M44);
			_planes[4] = Plane(_matrix.M12 - _matrix.M14, _matrix.M22 - _matrix.M24, _matrix.M32 - _matrix.M34, _matrix.M42 - _matrix.M44);
			_planes[5] = Plane(-_matrix.M14 - _matrix.M12, -_matrix.M24 - _matrix.M22, -_matrix.M34 - _matrix.M32, -_matrix.M44 - _matrix.M42);

			NormalizePlane(_planes[0]);
			NormalizePlane(_planes[1]);
			NormalizePlane(_planes[2]);
			NormalizePlane(_planes[3]);
			NormalizePlane(_planes[4]);
			NormalizePlane(_planes[5]);
		}

		constexpr void NormalizePlane(Plane& p) {
			const auto factor = 1.0 / p.Normal.Length();
			p.Normal.X *= factor;
			p.Normal.Y *= factor;
			p.Normal.Z *= factor;
			p.D *= factor;
		}

		static constexpr void IntersectionPoint(Plane const& a, Plane const& b, Plane const& c, Vector3& result) {

			Vector3 v1, v2, v3;
			Vector3 cross = Vector3::Cross(b.Normal, c.Normal);

			auto f = Vector3::Dot(a.Normal, cross);
			f *= -1.0;

			cross = Vector3::Cross(b.Normal, c.Normal);
			v1 = Vector3::Multiply(cross, a.D);

			cross = Vector3::Cross(c.Normal, a.Normal);
			v2 = Vector3::Multiply(cross, b.D);

			cross = Vector3::Cross(a.Normal, b.Normal);
			v3 = Vector3::Multiply(cross, c.D);

			result.X = (v1.X + v2.X + v3.X) / f;
			result.Y = (v1.Y + v2.Y + v3.Y) / f;
			result.Z = (v1.Z + v2.Z + v3.Z) / f;
		}
	};
}

namespace xna {
	constexpr bool BoundingBox::Intersects(BoundingFrustum& frustum) const {
		return frustum.Intersects(*this);
	}

	constexpr ContainmentType BoundingBox::Contains(BoundingFrustum& frustum) const {
		if (!frustum.Intersects(*this))
			return ContainmentType::Disjoint;

		const auto corners = frustum.GetCorners();
		const auto size = corners.size();

		for (size_t i = 0; i < size; ++i) {
			const auto& corner = corners[i];

			if (Contains(corner) == ContainmentType::Disjoint)
				return ContainmentType::Intersects;
		}

		return ContainmentType::Contains;
	}
}

namespace xna {
	constexpr bool BoundingSphere::Intersects(BoundingFrustum& frustum) const {
		return frustum.Intersects(*this);
	}

	constexpr ContainmentType BoundingSphere::Contains(BoundingFrustum const& frustum) const {
		bool inside = true;

		const auto corners = frustum.GetCorners();
		const auto size = corners.size();
		for (size_t i = 0; i < size; ++i) {
			const auto& corner = corners[i];

			if (Contains(corner) == ContainmentType::Disjoint) {
				inside = false;
				break;
			}
		}
		if (inside)
			return ContainmentType::Contains;

		const auto dmin = 0.0;

		if (dmin <= Radius * Radius)
			return ContainmentType::Intersects;

		return ContainmentType::Disjoint;
	}
}

namespace xna {
	constexpr cs::n_double Ray::Intersects(BoundingFrustum& frustum) {
		return frustum.Intersects(*this);
	}
}

namespace xna {
	constexpr PlaneIntersectionType Plane::Intersects(BoundingFrustum const& frustum) const {
		return frustum.Intersects(*this);
	}
}

#endif