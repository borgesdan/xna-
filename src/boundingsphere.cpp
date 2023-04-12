#include "boundingsphere.hpp"
#include <limits>

namespace xna {
    BoundingSphere BoundingSphere::CreateMerged(BoundingSphere const& original, BoundingSphere const& additional) {
        Vector3 result = Vector3::Subtract(additional.Center, original.Center);
        const auto num1 = result.Length();
        const auto radius1 = original.Radius;
        const auto radius2 = additional.Radius;

        if (radius1 + radius2 >= num1) {
            if (radius1 - radius2 >= num1)
                return original;
            if (radius2 - radius1 >= num1)
                return additional;
        }

        Vector3 vector3 = result * (1.0 / num1);
        const auto num2 = MathHelper::Min(-radius1, num1 - radius2);
        const auto num3 = ((MathHelper::Max(radius1, num1 + radius2) - num2) * 0.5);

        BoundingSphere merged;
        merged.Center = original.Center + vector3 * (num3 + num2);
        merged.Radius = num3;

        return merged;
    }

    BoundingSphere BoundingSphere::CreateFromBoundingBox(BoundingBox const& box) {
        BoundingSphere fromBoundingBox;
        fromBoundingBox.Center = Vector3::Lerp(box.Min, box.Max, 0.5);

        const auto result = Vector3::Distance(box.Min, box.Max);
        fromBoundingBox.Radius = result * 0.5;

        return fromBoundingBox;
    }

    BoundingSphere BoundingSphere::CreateFromPoints(std::vector<Vector3> const& points)
    {
        //Implementado do MonoGame

        auto minx = Vector3(std::numeric_limits<double>::max());
        auto maxx = -minx;
        auto miny = minx;
        auto maxy = -minx;
        auto minz = minx;
        auto maxz = -minx;

        auto numPoints = 0;
        auto size = points.size();

        for (size_t i = 0; i < points.size(); ++i) {

            ++numPoints;

            const auto& pt = points[i];

            if (pt.X < minx.X)
                minx = pt;
            if (pt.X > maxx.X)
                maxx = pt;
            if (pt.Y < miny.Y)
                miny = pt;
            if (pt.Y > maxy.Y)
                maxy = pt;
            if (pt.Z < minz.Z)
                minz = pt;
            if (pt.Z > maxz.Z)
                maxz = pt;
        }

        if (numPoints == 0)
            throw BoundingSphere();

        auto sqDistX = Vector3::DistanceSquared(maxx, minx);
        auto sqDistY = Vector3::DistanceSquared(maxy, miny);
        auto sqDistZ = Vector3::DistanceSquared(maxz, minz);

        auto min = minx;
        auto max = maxx;

        if (sqDistY > sqDistX && sqDistY > sqDistZ) {
            max = maxy;
            min = miny;
        }

        if (sqDistZ > sqDistX && sqDistZ > sqDistY) {
            max = maxz;
            min = minz;
        }

        auto center = (min + max) * 0.5f;
        auto radius = Vector3::Distance(max, center);
        auto sqRadius = radius * radius;

        for (size_t i = 0; i < size; ++i) {
            const auto& pt = points[i];
            const auto diff = (pt - center);
            const auto sqDist = diff.LengthSquared();

            if (sqDist > sqRadius) {
                const auto distance = sqrt(sqDist);
                const auto direction = diff / distance;
                const auto G = center - radius * direction;

                center = (G + pt) / 2;
                radius = Vector3::Distance(pt, center);
                sqRadius = radius * radius;
            }
        }

        return BoundingSphere(center, radius);
    }

    ContainmentType BoundingSphere::Contains(BoundingSphere const& sphere) const {
        const auto  result = Vector3::Distance(Center, sphere.Center);
        const auto radius1 = Radius;
        const auto radius2 = sphere.Radius;

        if (radius1 + radius2 < result)
            return ContainmentType::Disjoint;

        return radius1 - radius2 < result
            ? ContainmentType::Intersects
            : ContainmentType::Contains;
    }

    BoundingSphere BoundingSphere::Transform(Matrix const& matrix) const {
        BoundingSphere boundingSphere;
        boundingSphere.Center = Vector3::Transform(Center, matrix);

        const auto d = std::max(
            (matrix.M11 * matrix.M11 + matrix.M12 * matrix.M12 + matrix.M13 * matrix.M13),
            std::max((matrix.M21 * matrix.M21 + matrix.M22 * matrix.M22 + matrix.M23 * matrix.M23), (matrix.M31 * matrix.M31 + matrix.M32 * matrix.M32 + matrix.M33 * matrix.M33))
        );

        boundingSphere.Radius = Radius * sqrt(d);

        return boundingSphere;
    }
}