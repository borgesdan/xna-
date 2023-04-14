#include "basic-structs.hpp"

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

		const auto& vector2 = rotateAxis;
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

namespace xna {

	Quaternion Quaternion::CreateFromYawPitchRoll(double yaw, double pitch, double roll) {
		const auto num1 = roll * 0.5;
		const auto num2 = sin(num1);
		const auto num3 = cos(num1);
		const auto num4 = pitch * 0.5;
		const auto num5 = sin(num4);
		const auto num6 = cos(num4);
		const auto num7 = yaw * 0.5;
		const auto num8 = sin(num7);
		const auto num9 = cos(num7);

		Quaternion fromYawPitchRoll;
		fromYawPitchRoll.X = (num9 * num5 * num3 + num8 * num6 * num2);
		fromYawPitchRoll.Y = (num8 * num6 * num3 - num9 * num5 * num2);
		fromYawPitchRoll.Z = (num9 * num6 * num2 - num8 * num5 * num3);
		fromYawPitchRoll.W = (num9 * num6 * num3 + num8 * num5 * num2);
		return fromYawPitchRoll;
	}

	Quaternion Quaternion::CreateFromRotationMatrix(Matrix const& matrix) {
		const auto num1 = matrix.M11 + matrix.M22 + matrix.M33;
		Quaternion fromRotationMatrix;

		if (num1 > 0.0) {
			const auto num2 = sqrt(num1 + 1.0);
			fromRotationMatrix.W = num2 * 0.5;
			const auto num3 = 0.5 / num2;
			fromRotationMatrix.X = (matrix.M23 - matrix.M32) * num3;
			fromRotationMatrix.Y = (matrix.M31 - matrix.M13) * num3;
			fromRotationMatrix.Z = (matrix.M12 - matrix.M21) * num3;
		}
		else if (matrix.M11 >= matrix.M22 && matrix.M11 >= matrix.M33) {
			const auto num4 = sqrt(1.0 + matrix.M11 - matrix.M22 - matrix.M33);
			const auto num5 = 0.5 / num4;
			fromRotationMatrix.X = 0.5 * num4;
			fromRotationMatrix.Y = (matrix.M12 + matrix.M21) * num5;
			fromRotationMatrix.Z = (matrix.M13 + matrix.M31) * num5;
			fromRotationMatrix.W = (matrix.M23 - matrix.M32) * num5;
		}
		else if (matrix.M22 > matrix.M33) {
			const auto num6 = sqrt(1.0 + matrix.M22 - matrix.M11 - matrix.M33);
			const auto num7 = 0.5 / num6;
			fromRotationMatrix.X = (matrix.M21 + matrix.M12) * num7;
			fromRotationMatrix.Y = 0.5f * num6;
			fromRotationMatrix.Z = (matrix.M32 + matrix.M23) * num7;
			fromRotationMatrix.W = (matrix.M31 - matrix.M13) * num7;
		}
		else {
			const auto num8 = sqrt(1.0 + matrix.M33 - matrix.M11 - matrix.M22);
			const auto num9 = 0.5 / num8;
			fromRotationMatrix.X = (matrix.M31 + matrix.M13) * num9;
			fromRotationMatrix.Y = (matrix.M32 + matrix.M23) * num9;
			fromRotationMatrix.Z = 0.5f * num8;
			fromRotationMatrix.W = (matrix.M12 - matrix.M21) * num9;
		}
		return fromRotationMatrix;
	}

	Quaternion Quaternion::Slerp(Quaternion const& quaternion1, Quaternion const& quaternion2, double amount) {
		const auto num1 = amount;
		auto d = (quaternion1.X * quaternion2.X + quaternion1.Y * quaternion2.Y + quaternion1.Z * quaternion2.Z + quaternion1.W * quaternion2.W);
		bool flag = false;

		if (d < 0.0) {
			flag = true;
			d = -d;
		}

		double num2;
		double num3;

		if (d > 0.99999898672103882) {
			num2 = 1.0 - num1;
			num3 = flag ? -num1 : num1;
		}
		else {
			const auto a = acos(d);
			const auto num4 = (1.0 / sin(a));
			num2 = sin((1.0 - num1) * a) * num4;
			num3 = flag ? -sin(num1 * a) * num4 : sin(num1 * a) * num4;
		}

		Quaternion quaternion;
		quaternion.X = (num2 * quaternion1.X + num3 * quaternion2.X);
		quaternion.Y = (num2 * quaternion1.Y + num3 * quaternion2.Y);
		quaternion.Z = (num2 * quaternion1.Z + num3 * quaternion2.Z);
		quaternion.W = (num2 * quaternion1.W + num3 * quaternion2.W);
		return quaternion;
	}

	Quaternion Quaternion::Lerp(Quaternion const& quaternion1, Quaternion const& quaternion2, double amount) {
		const auto num1 = amount;
		const auto num2 = 1.0 - num1;
		Quaternion quaternion;

		if (quaternion1.X * quaternion2.X + quaternion1.Y * quaternion2.Y + quaternion1.Z * quaternion2.Z + quaternion1.W * quaternion2.W >= 0.0) {
			quaternion.X = (num2 * quaternion1.X + num1 * quaternion2.X);
			quaternion.Y = (num2 * quaternion1.Y + num1 * quaternion2.Y);
			quaternion.Z = (num2 * quaternion1.Z + num1 * quaternion2.Z);
			quaternion.W = (num2 * quaternion1.W + num1 * quaternion2.W);
		}
		else {
			quaternion.X = (num2 * quaternion1.X - num1 * quaternion2.X);
			quaternion.Y = (num2 * quaternion1.Y - num1 * quaternion2.Y);
			quaternion.Z = (num2 * quaternion1.Z - num1 * quaternion2.Z);
			quaternion.W = (num2 * quaternion1.W - num1 * quaternion2.W);
		}

		const auto num3 = 1.0 / sqrt(quaternion.X * quaternion.X + quaternion.Y * quaternion.Y + quaternion.Z * quaternion.Z + quaternion.W * quaternion.W);
		quaternion.X *= num3;
		quaternion.Y *= num3;
		quaternion.Z *= num3;
		quaternion.W *= num3;
		return quaternion;
	}
}

namespace xna {
	void Vector2::Transform(std::vector<Vector2> const& sourceArray, Quaternion const& rotation, std::vector<Vector2>& destinationArray) {

		const auto sourceLength = sourceArray.size();
		const auto destinationLength = destinationArray.size();

		if (destinationLength < sourceLength)
			destinationArray.resize(sourceLength);

		const auto num1 = rotation.X + rotation.X;
		const auto num2 = rotation.Y + rotation.Y;
		const auto num3 = rotation.Z + rotation.Z;
		const auto num4 = rotation.W * num3;
		const auto num5 = rotation.X * num1;
		const auto num6 = rotation.X * num2;
		const auto num7 = rotation.Y * num2;
		const auto num8 = rotation.Z * num3;
		const auto num9 = 1.0 - num7 - num8;
		const auto num10 = num6 - num4;
		const auto num11 = num6 + num4;
		const auto num12 = 1.0 - num5 - num8;

		for (int index = 0; index < sourceLength; ++index)
		{
			const auto x = sourceArray[index].X;
			const auto y = sourceArray[index].Y;

			destinationArray[index].X = x * num9 + y * num10;
			destinationArray[index].Y = x * num11 + y * num12;
		}
	}

	void Vector2::Transform(std::vector<Vector2> const& sourceArray, size_t sourceIndex,
		Quaternion const& rotation, std::vector<Vector2>& destinationArray,
		size_t destinationIndex, size_t length) {

		const auto sourceLength = sourceArray.size();
		const auto destinationLength = destinationArray.size();

		if (sourceLength < sourceIndex + length)
			return;

		if (destinationLength < destinationIndex + length)
			return;

		const auto num1 = rotation.X + rotation.X;
		const auto num2 = rotation.Y + rotation.Y;
		const auto num3 = rotation.Z + rotation.Z;
		const auto num4 = rotation.W * num3;
		const auto num5 = rotation.X * num1;
		const auto num6 = rotation.X * num2;
		const auto num7 = rotation.Y * num2;
		const auto num8 = rotation.Z * num3;
		const auto num9 = 1.0 - num7 - num8;
		const auto num10 = num6 - num4;
		const auto num11 = num6 + num4;
		const auto num12 = 1.0 - num5 - num8;

		for (; length > 0; --length)
		{
			const auto x = sourceArray[sourceIndex].X;
			const auto y = sourceArray[sourceIndex].Y;

			destinationArray[destinationIndex].X = x * num9 + y * num10;
			destinationArray[destinationIndex].Y = x * num11 + y * num12;

			++sourceIndex;
			++destinationIndex;
		}
	}

	void Vector3::Transform(std::vector<Vector3> const& sourceArray, Quaternion const& rotation, std::vector<Vector3>& destinationArray) {
		const auto sourceLength = sourceArray.size();
		const auto destinationLength = destinationArray.size();

		if (destinationLength < sourceLength)
			destinationArray.resize(sourceLength);

		const auto num1 = rotation.X + rotation.X;
		const auto num2 = rotation.Y + rotation.Y;
		const auto num3 = rotation.Z + rotation.Z;
		const auto num4 = rotation.W * num1;
		const auto num5 = rotation.W * num2;
		const auto num6 = rotation.W * num3;
		const auto num7 = rotation.X * num1;
		const auto num8 = rotation.X * num2;
		const auto num9 = rotation.X * num3;
		const auto num10 = rotation.Y * num2;
		const auto num11 = rotation.Y * num3;
		const auto num12 = rotation.Z * num3;
		const auto num13 = 1.0 - num10 - num12;
		const auto num14 = num8 - num6;
		const auto num15 = num9 + num5;
		const auto num16 = num8 + num6;
		const auto num17 = 1.0 - num7 - num12;
		const auto num18 = num11 - num4;
		const auto num19 = num9 - num5;
		const auto num20 = num11 + num4;
		const auto num21 = 1.0 - num7 - num10;

		for (size_t index = 0; index < sourceLength; ++index)
		{
			const auto x = sourceArray[index].X;
			const auto y = sourceArray[index].Y;
			const auto z = sourceArray[index].Z;
			destinationArray[index].X = x * num13 + y * num14 + z * num15;
			destinationArray[index].Y = x * num16 + y * num17 + z * num18;
			destinationArray[index].Z = x * num19 + y * num20 + z * num21;
		}
	}

	void Vector3::Transform(std::vector<Vector3> const& sourceArray, size_t sourceIndex,
		Quaternion const& rotation, std::vector<Vector3>& destinationArray,
		size_t destinationIndex, size_t length) {

		const auto sourceLength = sourceArray.size();
		const auto destinationLength = destinationArray.size();

		if (sourceLength < sourceIndex + length)
			return;

		if (destinationLength < destinationIndex + length)
			return;

		const auto num1 = rotation.X + rotation.X;
		const auto num2 = rotation.Y + rotation.Y;
		const auto num3 = rotation.Z + rotation.Z;
		const auto num4 = rotation.W * num1;
		const auto num5 = rotation.W * num2;
		const auto num6 = rotation.W * num3;
		const auto num7 = rotation.X * num1;
		const auto num8 = rotation.X * num2;
		const auto num9 = rotation.X * num3;
		const auto num10 = rotation.Y * num2;
		const auto num11 = rotation.Y * num3;
		const auto num12 = rotation.Z * num3;
		const auto num13 = 1.0 - num10 - num12;
		const auto num14 = num8 - num6;
		const auto num15 = num9 + num5;
		const auto num16 = num8 + num6;
		const auto num17 = 1.0 - num7 - num12;
		const auto num18 = num11 - num4;
		const auto num19 = num9 - num5;
		const auto num20 = num11 + num4;
		const auto num21 = 1.0 - num7 - num10;

		for (; length > 0; --length)
		{
			const auto x = sourceArray[sourceIndex].X;
			const auto y = sourceArray[sourceIndex].Y;
			const auto z = sourceArray[sourceIndex].Z;
			destinationArray[destinationIndex].X = x * num13 + y * num14 + z * num15;
			destinationArray[destinationIndex].Y = x * num16 + y * num17 + z * num18;
			destinationArray[destinationIndex].Z = x * num19 + y * num20 + z * num21;
			++sourceIndex;
			++destinationIndex;
		}
	}

	void Vector4::Transform(std::vector<Vector4> const& sourceArray, Quaternion const& rotation, std::vector<Vector4>& destinationArray) {

		const auto sourceLength = sourceArray.size();
		const auto destinationLength = destinationArray.size();

		if (destinationLength < sourceLength)
			destinationArray.resize(sourceLength);

		const auto num1 = rotation.X + rotation.X;
		const auto num2 = rotation.Y + rotation.Y;
		const auto num3 = rotation.Z + rotation.Z;
		const auto num4 = rotation.W * num1;
		const auto num5 = rotation.W * num2;
		const auto num6 = rotation.W * num3;
		const auto num7 = rotation.X * num1;
		const auto num8 = rotation.X * num2;
		const auto num9 = rotation.X * num3;
		const auto num10 = rotation.Y * num2;
		const auto num11 = rotation.Y * num3;
		const auto num12 = rotation.Z * num3;
		const auto num13 = 1.0 - num10 - num12;
		const auto num14 = num8 - num6;
		const auto num15 = num9 + num5;
		const auto num16 = num8 + num6;
		const auto num17 = 1.0 - num7 - num12;
		const auto num18 = num11 - num4;
		const auto num19 = num9 - num5;
		const auto num20 = num11 + num4;
		const auto num21 = 1.0 - num7 - num10;

		for (int index = 0; index < sourceLength; ++index)
		{
			const auto x = sourceArray[index].X;
			const auto y = sourceArray[index].Y;
			const auto z = sourceArray[index].Z;
			destinationArray[index].X = (x * num13 + y * num14 + z * num15);
			destinationArray[index].Y = (x * num16 + y * num17 + z * num18);
			destinationArray[index].Z = (x * num19 + y * num20 + z * num21);
			destinationArray[index].W = sourceArray[index].W;
		}
	}

	void Vector4::Transform(std::vector<Vector4> const& sourceArray, size_t sourceIndex,
		Quaternion const& rotation, std::vector<Vector4>& destinationArray,
		size_t destinationIndex, size_t length) {

		const auto sourceLength = sourceArray.size();
		const auto destinationLength = destinationArray.size();

		if (sourceLength < sourceIndex + length)
			return;

		if (destinationLength < destinationIndex + length)
			return;

		const auto num1 = rotation.X + rotation.X;
		const auto num2 = rotation.Y + rotation.Y;
		const auto num3 = rotation.Z + rotation.Z;
		const auto num4 = rotation.W * num1;
		const auto num5 = rotation.W * num2;
		const auto num6 = rotation.W * num3;
		const auto num7 = rotation.X * num1;
		const auto num8 = rotation.X * num2;
		const auto num9 = rotation.X * num3;
		const auto num10 = rotation.Y * num2;
		const auto num11 = rotation.Y * num3;
		const auto num12 = rotation.Z * num3;
		const auto num13 = 1.0 - num10 - num12;
		const auto num14 = num8 - num6;
		const auto num15 = num9 + num5;
		const auto num16 = num8 + num6;
		const auto num17 = 1.0 - num7 - num12;
		const auto num18 = num11 - num4;
		const auto num19 = num9 - num5;
		const auto num20 = num11 + num4;
		const auto num21 = 1.0 - num7 - num10;

		for (; length > 0; --length)
		{
			const auto x = sourceArray[sourceIndex].X;
			const auto y = sourceArray[sourceIndex].Y;
			const auto z = sourceArray[sourceIndex].Z;
			const auto w = sourceArray[sourceIndex].W;
			destinationArray[destinationIndex].X = (x * num13 + y * num14 + z * num15);
			destinationArray[destinationIndex].Y = (x * num16 + y * num17 + z * num18);
			destinationArray[destinationIndex].Z = (x * num19 + y * num20 + z * num21);
			destinationArray[destinationIndex].W = w;
			++sourceIndex;
			++destinationIndex;
		}
	}
}

namespace xna {
	Matrix Matrix::CreateFromYawPitchRoll(double yaw, double pitch, double roll) {
		Quaternion result1 = Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
		return Matrix::CreateFromQuaternion(result1);
	}

	bool Matrix::Decompose(Vector3& scale, Quaternion& rotation, Vector3& translation) {
		//A implementação do XNA utiliza ponteiros,
		//aqui utiliza-se a implementação do MonoGame

		translation.X = M41;
		translation.Y = M42;
		translation.Z = M43;

		const auto xs = (Math::Sign(M11 * M12 * M13 * M14) < 0) ? -1 : 1;
		const auto ys = (Math::Sign(M21 * M22 * M23 * M24) < 0) ? -1 : 1;
		const auto zs = (Math::Sign(M31 * M32 * M33 * M34) < 0) ? -1 : 1;

		scale.X = xs * sqrt(M11 * M11 + M12 * M12 + M13 * M13);
		scale.Y = ys * sqrt(M21 * M21 + M22 * M22 + M23 * M23);
		scale.Z = zs * sqrt(M31 * M31 + M32 * M32 + M33 * M33);

		if (scale.X == 0.0 || scale.Y == 0.0 || scale.Z == 0.0) {
			rotation = Quaternion::Identity();
			return false;
		}

		Matrix m1 = Matrix(
			M11 / scale.X, M12 / scale.X, M13 / scale.X, 0,
			M21 / scale.Y, M22 / scale.Y, M23 / scale.Y, 0,
			M31 / scale.Z, M32 / scale.Z, M33 / scale.Z, 0,
			0, 0, 0, 1);

		rotation = Quaternion::CreateFromRotationMatrix(m1);

		return true;
	}
}