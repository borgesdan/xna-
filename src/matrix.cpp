#include <vector>
#include "matrix.hpp"

namespace xna {
    
    Matrix Matrix::CreateBillboard(Vector3 const& objectPosition, Vector3 const& cameraPosition, Vector3 cameraUpVector, cs::Nullable<Vector3> const& cameraForwardVector) {
		Vector3 result1;
		result1.X = objectPosition.X - cameraPosition.X;
		result1.Y = objectPosition.Y - cameraPosition.Y;
		result1.Z = objectPosition.Z - cameraPosition.Z;

		const auto d = result1.LengthSquared();

		if (d < 9.9999997473787516E-05)
			result1 = cameraForwardVector.HasValue() ? -cameraForwardVector.Value() : Vector3::Forward();
		else
			result1 = Vector3::Multiply(result1, 1.0 / sqrt(d));

		auto result2 = Vector3::Cross(cameraUpVector, result1);
		result2.Normalize();

		const auto result3 = Vector3::Cross(result1, result2);

		Matrix billboard;
		billboard.M11 = result2.X;
		billboard.M12 = result2.Y;
		billboard.M13 = result2.Z;
		billboard.M14 = 0.0;
		billboard.M21 = result3.X;
		billboard.M22 = result3.Y;
		billboard.M23 = result3.Z;
		billboard.M24 = 0.0;
		billboard.M31 = result1.X;
		billboard.M32 = result1.Y;
		billboard.M33 = result1.Z;
		billboard.M34 = 0.0;
		billboard.M41 = objectPosition.X;
		billboard.M42 = objectPosition.Y;
		billboard.M43 = objectPosition.Z;
		billboard.M44 = 1.0;

		return billboard;
	}

	Matrix Matrix::CreateConstrainedBillboard(Vector3 const& objectPosition, Vector3 const& cameraPosition, Vector3 const& rotateAxis,
		cs::Nullable<Vector3> const& cameraForwardVector, cs::Nullable<Vector3> const& objectForwardVector) {

        Vector3 result1;
        result1.X = objectPosition.X - cameraPosition.X;
        result1.Y = objectPosition.Y - cameraPosition.Y;
        result1.Z = objectPosition.Z - cameraPosition.Z;

        const auto d = result1.LengthSquared();

        if (d < 9.9999997473787516E-05)
            result1 = cameraForwardVector.HasValue() ? -cameraForwardVector.Value() : Vector3::Forward();
        else
            result1 = Vector3::Multiply(result1, 1.0 / sqrt(d));

        const auto &vector2 = rotateAxis;
        auto result2 = Vector3::Dot(rotateAxis, result1);

        Vector3 result3;
        Vector3 result4;

        if (abs(result2) > 0.998254656791687) {
            if (objectForwardVector.HasValue()) {
                result3 = objectForwardVector.Value();
                result2 = Vector3::Dot(rotateAxis, result3);

                if (abs(result2) > 0.998254656791687)
                    result3 = abs((rotateAxis.X * Vector3::Forward().X + rotateAxis.Y * Vector3::Forward().Y + rotateAxis.Z * Vector3::Forward().Z)) > 0.998254656791687 ? Vector3::Right() : Vector3::Forward();
            }
            else
                result3 = abs((rotateAxis.X * Vector3::Forward().X + rotateAxis.Y * Vector3::Forward().Y + rotateAxis.Z * Vector3::Forward().Z)) > 0.998254656791687 ? Vector3::Right() : Vector3::Forward();

            result4 = Vector3::Cross(rotateAxis, result3);
            result4.Normalize();

            result3 = Vector3::Cross(result4, rotateAxis);
            result3.Normalize();
        }
        else
        {
            result4 = Vector3::Cross(rotateAxis, result1);
            result4.Normalize();

            result3 = Vector3::Cross(result4, vector2);
            result3.Normalize();
        }

        Matrix constrainedBillboard;
        constrainedBillboard.M11 = result4.X;
        constrainedBillboard.M12 = result4.Y;
        constrainedBillboard.M13 = result4.Z;
        constrainedBillboard.M14 = 0.0;
        constrainedBillboard.M21 = vector2.X;
        constrainedBillboard.M22 = vector2.Y;
        constrainedBillboard.M23 = vector2.Z;
        constrainedBillboard.M24 = 0.0;
        constrainedBillboard.M31 = result3.X;
        constrainedBillboard.M32 = result3.Y;
        constrainedBillboard.M33 = result3.Z;
        constrainedBillboard.M34 = 0.0;
        constrainedBillboard.M41 = objectPosition.X;
        constrainedBillboard.M42 = objectPosition.Y;
        constrainedBillboard.M43 = objectPosition.Z;
        constrainedBillboard.M44 = 1.0;

        return constrainedBillboard;
	}

    Matrix Matrix::CreateRotationX(double radians) {
		const auto num1 = cos(radians);
		const auto num2 = sin(radians);

		Matrix rotationX;
		rotationX.M11 = 1.0;
		rotationX.M12 = 0.0;
		rotationX.M13 = 0.0;
		rotationX.M14 = 0.0;
		rotationX.M21 = 0.0;
		rotationX.M22 = num1;
		rotationX.M23 = num2;
		rotationX.M24 = 0.0;
		rotationX.M31 = 0.0;
		rotationX.M32 = -num2;
		rotationX.M33 = num1;
		rotationX.M34 = 0.0;
		rotationX.M41 = 0.0;
		rotationX.M42 = 0.0;
		rotationX.M43 = 0.0;
		rotationX.M44 = 1.0;
		return rotationX;
	}

	Matrix Matrix::CreateRotationY(double radians) {
		const auto num1 = cos(radians);
		const auto num2 = sin(radians);

		Matrix rotationY;
		rotationY.M11 = num1;
		rotationY.M12 = 0.0;
		rotationY.M13 = -num2;
		rotationY.M14 = 0.0;
		rotationY.M21 = 0.0;
		rotationY.M22 = 1.0;
		rotationY.M23 = 0.0;
		rotationY.M24 = 0.0;
		rotationY.M31 = num2;
		rotationY.M32 = 0.0;
		rotationY.M33 = num1;
		rotationY.M34 = 0.0;
		rotationY.M41 = 0.0;
		rotationY.M42 = 0.0;
		rotationY.M43 = 0.0;
		rotationY.M44 = 1.0;
		return rotationY;
	}

	Matrix Matrix::CreateRotationZ(double radians) {
		const auto num1 = cos(radians);
		const auto num2 = sin(radians);

		Matrix rotationZ;
		rotationZ.M11 = num1;
		rotationZ.M12 = num2;
		rotationZ.M13 = 0.0;
		rotationZ.M14 = 0.0;
		rotationZ.M21 = -num2;
		rotationZ.M22 = num1;
		rotationZ.M23 = 0.0;
		rotationZ.M24 = 0.0;
		rotationZ.M31 = 0.0;
		rotationZ.M32 = 0.0;
		rotationZ.M33 = 1.0;
		rotationZ.M34 = 0.0;
		rotationZ.M41 = 0.0;
		rotationZ.M42 = 0.0;
		rotationZ.M43 = 0.0;
		rotationZ.M44 = 1.0;
		return rotationZ;
	}

	Matrix Matrix::CreateFromAxisAngle(Vector3 const& axis, double angle) {
		const auto x = axis.X;
		const auto y = axis.Y;
		const auto z = axis.Z;
		const auto num1 = sin(angle);
		const auto num2 = cos(angle);
		const auto num3 = x * x;
		const auto num4 = y * y;
		const auto num5 = z * z;
		const auto num6 = x * y;
		const auto num7 = x * z;
		const auto num8 = y * z;
		Matrix fromAxisAngle;
		fromAxisAngle.M11 = num3 + num2 * (1.0 - num3);
		fromAxisAngle.M12 = (num6 - num2 * num6 + num1 * z);
		fromAxisAngle.M13 = (num7 - num2 * num7 - num1 * y);
		fromAxisAngle.M14 = 0.0;
		fromAxisAngle.M21 = (num6 - num2 * num6 - num1 * z);
		fromAxisAngle.M22 = num4 + num2 * (1.0 - num4);
		fromAxisAngle.M23 = (num8 - num2 * num8 + num1 * x);
		fromAxisAngle.M24 = 0.0;
		fromAxisAngle.M31 = (num7 - num2 * num7 + num1 * y);
		fromAxisAngle.M32 = (num8 - num2 * num8 - num1 * x);
		fromAxisAngle.M33 = num5 + num2 * (1.0 - num5);
		fromAxisAngle.M34 = 0.0;
		fromAxisAngle.M41 = 0.0;
		fromAxisAngle.M42 = 0.0;
		fromAxisAngle.M43 = 0.0;
		fromAxisAngle.M44 = 1.0;
		return fromAxisAngle;
	}
}

namespace xna {
	void Vector2::Transform(std::vector<Vector2> const& sourceArray, Matrix const& matrix, std::vector<Vector2>& destinationArray) {
		const auto sourceLength = sourceArray.size();
		const auto destinationLength = destinationArray.size();

		if (destinationLength < sourceLength)
			destinationArray.resize(sourceLength);

		for (size_t index = 0; index < sourceLength; ++index) {
			const auto x = sourceArray[index].X;
			const auto y = sourceArray[index].Y;

			destinationArray[index].X = (x * matrix.M11 + y * matrix.M21) + matrix.M41;
			destinationArray[index].Y = (x * matrix.M12 + y * matrix.M22) + matrix.M42;
		}
	}

	void Vector2::Transform(std::vector<Vector2> const& sourceArray, size_t sourceIndex, Matrix const& matrix,
		std::vector<Vector2>& destinationArray, size_t destinationIndex, size_t length) {

		const auto sourceLength = sourceArray.size();
		const auto destinationLength = destinationArray.size();

		if (sourceLength < sourceIndex + length)
			return;

		if (destinationLength < destinationIndex + length)
			return;

		for (; length > 0; --length) {
			const auto x = sourceArray[sourceIndex].X;
			const auto y = sourceArray[sourceIndex].Y;

			destinationArray[destinationIndex].X = (x * matrix.M11 + y * matrix.M21) + matrix.M41;
			destinationArray[destinationIndex].Y = (x * matrix.M12 + y * matrix.M22) + matrix.M42;

			++sourceIndex;
			++destinationIndex;
		}
	}

	void Vector2::TransformNormal(std::vector<Vector2> const& sourceArray, Matrix const& matrix, std::vector<Vector2>& destinationArray) {
		const auto sourceLength = sourceArray.size();
		const auto destinationLength = destinationArray.size();

		if (destinationLength < sourceLength)
			destinationArray.resize(sourceLength);

		for (size_t index = 0; index < sourceLength; ++index) {
			const auto x = sourceArray[index].X;
			const auto y = sourceArray[index].Y;

			destinationArray[index].X = x * matrix.M11 + y * matrix.M21;
			destinationArray[index].Y = x * matrix.M12 + y * matrix.M22;
		}
	}

	void Vector2::TransformNormal(std::vector<Vector2> const& sourceArray, size_t sourceIndex,
		Matrix& matrix, std::vector<Vector2> destinationArray, size_t destinationIndex, size_t length) {

		const auto sourceLength = sourceArray.size();
		const auto destinationLength = destinationArray.size();

		if (sourceLength < sourceIndex + length)
			return;

		if (destinationLength < destinationIndex + length)
			return;

		for (; length > 0; --length)
		{
			const auto x = sourceArray[sourceIndex].X;
			const auto y = sourceArray[sourceIndex].Y;

			destinationArray[destinationIndex].X = x * matrix.M11 + y * matrix.M21;
			destinationArray[destinationIndex].Y = x * matrix.M12 + y * matrix.M22;

			++sourceIndex;
			++destinationIndex;
		}
	}
}

namespace xna {
	void Vector3::Transform(std::vector<Vector3> const& sourceArray, Matrix const& matrix, std::vector<Vector3>& destinationArray) {
		const auto sourceLength = sourceArray.size();
		const auto destinationLength = destinationArray.size();

		if (destinationLength < sourceLength)
			destinationArray.resize(sourceLength);

		for (size_t index = 0; index < sourceLength; ++index) {
			const auto x = sourceArray[index].X;
			const auto y = sourceArray[index].Y;
			const auto z = sourceArray[index].Z;
			destinationArray[index].X = (x * matrix.M11 + y * matrix.M21 + z * matrix.M31) + matrix.M41;
			destinationArray[index].Y = (x * matrix.M12 + y * matrix.M22 + z * matrix.M32) + matrix.M42;
			destinationArray[index].Z = (x * matrix.M13 + y * matrix.M23 + z * matrix.M33) + matrix.M43;
		}
	}

	void Vector3::Transform(std::vector<Vector3> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector3>& destinationArray, size_t destinationIndex, size_t length) {

		const auto sourceLength = sourceArray.size();
		const auto destinationLength = destinationArray.size();

		if (sourceLength < sourceIndex + length)
			return;

		if (destinationLength < destinationIndex + length)
			return;

		for (; length > 0; --length)
		{
			const auto x = sourceArray[sourceIndex].X;
			const auto y = sourceArray[sourceIndex].Y;
			const auto z = sourceArray[sourceIndex].Z;
			destinationArray[destinationIndex].X = (x * matrix.M11 + y * matrix.M21 + z * matrix.M31) + matrix.M41;
			destinationArray[destinationIndex].Y = (x * matrix.M12 + y * matrix.M22 + z * matrix.M32) + matrix.M42;
			destinationArray[destinationIndex].Z = (x * matrix.M13 + y * matrix.M23 + z * matrix.M33) + matrix.M43;
			++sourceIndex;
			++destinationIndex;
		}
	}

	void Vector3::TransformNormal(std::vector<Vector3> const& sourceArray, Matrix const& matrix, std::vector<Vector3>& destinationArray) {
		const auto sourceLength = sourceArray.size();
		const auto destinationLength = destinationArray.size();

		if (destinationLength < sourceLength)
			destinationArray.resize(sourceLength);

		for (int index = 0; index < sourceLength; ++index)
		{
			const auto x = sourceArray[index].X;
			const auto y = sourceArray[index].Y;
			const auto z = sourceArray[index].Z;
			destinationArray[index].X = (x * matrix.M11 + y * matrix.M21 + z * matrix.M31);
			destinationArray[index].Y = (x * matrix.M12 + y * matrix.M22 + z * matrix.M32);
			destinationArray[index].Z = (x * matrix.M13 + y * matrix.M23 + z * matrix.M33);
		}
	}

	void Vector3::TransformNormal(std::vector<Vector3> const& sourceArray, size_t sourceIndex,
		Matrix& matrix, std::vector<Vector3> destinationArray, size_t destinationIndex, size_t length) {

		const auto sourceLength = sourceArray.size();
		const auto destinationLength = destinationArray.size();

		if (sourceLength < sourceIndex + length)
			return;

		if (destinationLength < destinationIndex + length)
			return;

		for (; length > 0; --length)
		{
			const auto x = sourceArray[sourceIndex].X;
			const auto y = sourceArray[sourceIndex].Y;
			const auto z = sourceArray[sourceIndex].Z;
			destinationArray[destinationIndex].X = (x * matrix.M11 + y * matrix.M21 + z * matrix.M31);
			destinationArray[destinationIndex].Y = (x * matrix.M12 + y * matrix.M22 + z * matrix.M32);
			destinationArray[destinationIndex].Z = (x * matrix.M13 + y * matrix.M23 + z * matrix.M33);
			++sourceIndex;
			++destinationIndex;
		}
	}
}

namespace xna {
	void Vector4::Transform(std::vector<Vector4> const& sourceArray, Matrix const& matrix, std::vector<Vector4>& destinationArray) {

		const auto sourceLength = sourceArray.size();
		const auto destinationLength = destinationArray.size();

		if (destinationLength < sourceLength)
			destinationArray.resize(sourceLength);

		for (size_t index = 0; index < sourceLength; ++index)
		{
			const auto x = sourceArray[index].X;
			const auto y = sourceArray[index].Y;
			const auto z = sourceArray[index].Z;
			const auto w = sourceArray[index].W;
			destinationArray[index].X = (x * matrix.M11 + y * matrix.M21 + z * matrix.M31 + w * matrix.M41);
			destinationArray[index].Y = (x * matrix.M12 + y * matrix.M22 + z * matrix.M32 + w * matrix.M42);
			destinationArray[index].Z = (x * matrix.M13 + y * matrix.M23 + z * matrix.M33 + w * matrix.M43);
			destinationArray[index].W = (x * matrix.M14 + y * matrix.M24 + z * matrix.M34 + w * matrix.M44);
		}
	}

	void Vector4::Transform(std::vector<Vector4> const& sourceArray, size_t sourceIndex, Matrix const& matrix,
		std::vector<Vector4>& destinationArray, size_t destinationIndex, size_t length) {

		const auto sourceLength = sourceArray.size();
		const auto destinationLength = destinationArray.size();

		if (sourceLength < sourceIndex + length)
			return;

		if (destinationLength < destinationIndex + length)
			return;

		for (; length > 0; --length)
		{
			const auto x = sourceArray[sourceIndex].X;
			const auto y = sourceArray[sourceIndex].Y;
			const auto z = sourceArray[sourceIndex].Z;
			const auto w = sourceArray[sourceIndex].W;
			destinationArray[destinationIndex].X = (x * matrix.M11 + y * matrix.M21 + z * matrix.M31 + w * matrix.M41);
			destinationArray[destinationIndex].Y = (x * matrix.M12 + y * matrix.M22 + z * matrix.M32 + w * matrix.M42);
			destinationArray[destinationIndex].Z = (x * matrix.M13 + y * matrix.M23 + z * matrix.M33 + w * matrix.M43);
			destinationArray[destinationIndex].W = (x * matrix.M14 + y * matrix.M24 + z * matrix.M34 + w * matrix.M44);
			++sourceIndex;
			++destinationIndex;
		}
	}
}