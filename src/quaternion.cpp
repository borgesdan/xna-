#include "quaternion.hpp"
#include "matrix.hpp"

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
		
		if (num1 > 0.0)	{
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