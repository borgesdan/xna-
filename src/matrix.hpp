#ifndef XNA_MATRIX_HPP
#define XNA_MATRIX_HPP

#include <cmath>
#include "basic-structs.hpp"
#include "csharp/nullable.hpp"

namespace xna {
	struct Plane;
	struct Quaternion;

	struct Matrix {
		double M11{ 0.0 };
		double M12{ 0.0 };
		double M13{ 0.0 };
		double M14{ 0.0 };
		double M21{ 0.0 };
		double M22{ 0.0 };
		double M23{ 0.0 };
		double M24{ 0.0 };
		double M31{ 0.0 };
		double M32{ 0.0 };
		double M33{ 0.0 };
		double M34{ 0.0 };
		double M41{ 0.0 };
		double M42{ 0.0 };
		double M43{ 0.0 };
		double M44{ 0.0 };

		constexpr Matrix() = default;

		constexpr Matrix(double m11, double m12, double m13, double m14,
			double m21, double m22, double m23, double m24,
			double m31, double m32, double m33, double m34,
			double m41, double m42, double m43, double m44) :
			M11(m11), M12(m12), M13(m13), M14(m14),
			M21(m21), M22(m22), M23(m23), M24(m24),
			M31(m31), M32(m32), M33(m33), M34(m34),
			M41(m41), M42(m42), M43(m43), M44(m44) {}

		constexpr Matrix(Vector4 row1, Vector4 row2, Vector4 row3, Vector4 row4) :
			M11(row1.X), M12(row1.Y), M13(row1.Z), M14(row1.W),
			M21(row2.X), M22(row2.Y), M23(row2.Z), M24(row2.W),
			M31(row3.X), M32(row3.Y), M33(row3.Z), M34(row3.W),
			M41(row4.X), M42(row4.Y), M43(row4.Z), M44(row4.W) {}

		constexpr Matrix Identity() const {
			return Matrix(
				1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);
		}

		constexpr Vector3 Up() const {
			Vector3 up;
			up.X = M21;
			up.Y = M22;
			up.Z = M23;
			return up;
		}

		constexpr void Up(Vector3 const& value) {
			M21 = value.X;
			M22 = value.Y;
			M23 = value.Z;
		}

		constexpr Vector3 Down() const {
			Vector3 down;
			down.X = -M21;
			down.Y = -M22;
			down.Z = -M23;
			return down;
		}

		constexpr void Down(Vector3 const& value) {
			M21 = -value.X;
			M22 = -value.Y;
			M23 = -value.Z;
		}

		constexpr Vector3 Right() const {
			Vector3 right;
			right.X = M11;
			right.Y = M12;
			right.Z = M13;
			return right;
		}

		constexpr void Right(Vector3 const& value) {
			M11 = value.X;
			M12 = value.Y;
			M13 = value.Z;
		}

		constexpr Vector3 Left() const {
			Vector3 left;
			left.X = -M11;
			left.Y = -M12;
			left.Z = -M13;
			return left;
		}

		constexpr void Left(Vector3 const& value) {
			M11 = -value.X;
			M12 = -value.Y;
			M13 = -value.Z;
		}

		constexpr Vector3 Forward() const {
			Vector3 forward;
			forward.X = -M31;
			forward.Y = -M32;
			forward.Z = -M33;
			return forward;
		}

		constexpr void Forward(Vector3 const& value) {
			M31 = -value.X;
			M32 = -value.Y;
			M33 = -value.Z;
		}

		constexpr Vector3 Backward() const {
			Vector3 backward;
			backward.X = M31;
			backward.Y = M32;
			backward.Z = M33;
			return backward;
		}

		constexpr void Backward(Vector3 const& value) {
			M31 = value.X;
			M32 = value.Y;
			M33 = value.Z;
		}

		constexpr Vector3 Translation() const {
			Vector3 translation;
			translation.X = M41;
			translation.Y = M42;
			translation.Z = M43;
			return translation;
		}

		constexpr void Translation(Vector3 const& value) {
			M41 = value.X;
			M42 = value.Y;
			M43 = value.Z;
		}

		static Matrix CreateBillboard(Vector3 const& objectPosition, Vector3 const& cameraPosition, Vector3 cameraUpVector, cs::Nullable<Vector3> const& cameraForwardVector);
		static Matrix CreateConstrainedBillboard(Vector3 const& objectPosition, Vector3 const& cameraPosition, Vector3 const& rotateAxis,
			cs::Nullable<Vector3> const& cameraForwardVector, cs::Nullable<Vector3> const& objectForwardVector);

		static constexpr Matrix CreateTranslation(Vector3 const& position) {
			Matrix translation;
			translation.M11 = 1.0f;
			translation.M12 = 0.0f;
			translation.M13 = 0.0f;
			translation.M14 = 0.0f;
			translation.M21 = 0.0f;
			translation.M22 = 1.0f;
			translation.M23 = 0.0f;
			translation.M24 = 0.0f;
			translation.M31 = 0.0f;
			translation.M32 = 0.0f;
			translation.M33 = 1.0f;
			translation.M34 = 0.0f;
			translation.M41 = position.X;
			translation.M42 = position.Y;
			translation.M43 = position.Z;
			translation.M44 = 1.0f;
			return translation;
		}

		static constexpr Matrix CreateTranslation(double xPosition, double yPosition, double zPosition) {
			Matrix translation;
			translation.M11 = 1.0;
			translation.M12 = 0.0f;
			translation.M13 = 0.0f;
			translation.M14 = 0.0f;
			translation.M21 = 0.0f;
			translation.M22 = 1.0;
			translation.M23 = 0.0f;
			translation.M24 = 0.0f;
			translation.M31 = 0.0f;
			translation.M32 = 0.0f;
			translation.M33 = 1.0;
			translation.M34 = 0.0f;
			translation.M41 = xPosition;
			translation.M42 = yPosition;
			translation.M43 = zPosition;
			translation.M44 = 1.0;
			return translation;
		}

		static constexpr Matrix CreateScale(double xScale, double yScale, double zScale)	{
			const auto num1 = xScale;
			const auto num2 = yScale;
			const auto num3 = zScale;

			Matrix scale;
			scale.M11 = num1;
			scale.M12 = 0.0;
			scale.M13 = 0.0;
			scale.M14 = 0.0;
			scale.M21 = 0.0;
			scale.M22 = num2;
			scale.M23 = 0.0;
			scale.M24 = 0.0;
			scale.M31 = 0.0;
			scale.M32 = 0.0;
			scale.M33 = num3;
			scale.M34 = 0.0;
			scale.M41 = 0.0;
			scale.M42 = 0.0;
			scale.M43 = 0.0;
			scale.M44 = 1.0;
			return scale;
		}

		static Matrix CreateScale(Vector3 const& scales) {
			const auto x = scales.X;
			const auto y = scales.Y;
			const auto z = scales.Z;
			
			Matrix scale;
			scale.M11 = x;
			scale.M12 = 0.0;
			scale.M13 = 0.0;
			scale.M14 = 0.0;
			scale.M21 = 0.0;
			scale.M22 = y;
			scale.M23 = 0.0;
			scale.M24 = 0.0;
			scale.M31 = 0.0;
			scale.M32 = 0.0;
			scale.M33 = z;
			scale.M34 = 0.0;
			scale.M41 = 0.0;
			scale.M42 = 0.0;
			scale.M43 = 0.0;
			scale.M44 = 1.0;
			return scale;
		}

		static constexpr Matrix CreateScale(double scale) {
			const auto num = scale;
			
			Matrix scale1;
			scale1.M11 = num;
			scale1.M12 = 0.0;
			scale1.M13 = 0.0;
			scale1.M14 = 0.0;
			scale1.M21 = 0.0;
			scale1.M22 = num;
			scale1.M23 = 0.0;
			scale1.M24 = 0.0;
			scale1.M31 = 0.0;
			scale1.M32 = 0.0;
			scale1.M33 = num;
			scale1.M34 = 0.0;
			scale1.M41 = 0.0;
			scale1.M42 = 0.0;
			scale1.M43 = 0.0;
			scale1.M44 = 1.0;
			return scale1;
		}

		static Matrix CreateRotationX(double radians);
		static Matrix CreateRotationY(double radians);
		static Matrix CreateRotationZ(double radians);
		static Matrix CreateFromAxisAngle(Vector3 const& axis, double angle);
		
		static Matrix CreatePerspectiveFieldOfView(double fieldOfView, double aspectRatio, double nearPlaneDistance, double farPlaneDistance) {
			if (fieldOfView <= 0.0 || fieldOfView >= 3.1415927410125732)
			{
				if (fieldOfView <= 0.0)
					fieldOfView = 0.5;
				else
					fieldOfView = 3.0;
			}

			if (nearPlaneDistance <= 0.0)
				nearPlaneDistance = 0.1;

			if (farPlaneDistance <= 0.0)
				farPlaneDistance = 1.0;

			if (nearPlaneDistance >= farPlaneDistance)
				farPlaneDistance = farPlaneDistance - 1.0;

			const auto num1 = 1.0 / tan(fieldOfView * 0.5);
			const auto num2 = num1 / aspectRatio;

			Matrix perspectiveFieldOfView;
			perspectiveFieldOfView.M11 = num2;
			perspectiveFieldOfView.M12 = perspectiveFieldOfView.M13 = perspectiveFieldOfView.M14 = 0.0;
			perspectiveFieldOfView.M22 = num1;
			perspectiveFieldOfView.M21 = perspectiveFieldOfView.M23 = perspectiveFieldOfView.M24 = 0.0;
			perspectiveFieldOfView.M31 = perspectiveFieldOfView.M32 = 0.0;
			perspectiveFieldOfView.M33 = farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
			perspectiveFieldOfView.M34 = -1.0;
			perspectiveFieldOfView.M41 = perspectiveFieldOfView.M42 = perspectiveFieldOfView.M44 = 0.0;
			perspectiveFieldOfView.M43 = (nearPlaneDistance * farPlaneDistance / (nearPlaneDistance - farPlaneDistance));
			
			return perspectiveFieldOfView;
		}

		static constexpr Matrix CreatePerspective(double width, double height, double nearPlaneDistance, double farPlaneDistance) {
			if (nearPlaneDistance <= 0.0)
				nearPlaneDistance = 0.1;

			if (farPlaneDistance <= 0.0)
				farPlaneDistance = 1.0;

			if (nearPlaneDistance >= farPlaneDistance)
				farPlaneDistance = farPlaneDistance - 1.0;

			Matrix perspective;
			perspective.M11 = 2.0 * nearPlaneDistance / width;
			perspective.M12 = perspective.M13 = perspective.M14 = 0.0;
			perspective.M22 = 2.0 * nearPlaneDistance / height;
			perspective.M21 = perspective.M23 = perspective.M24 = 0.0;
			perspective.M33 = farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
			perspective.M31 = perspective.M32 = 0.0;
			perspective.M34 = -1.0;
			perspective.M41 = perspective.M42 = perspective.M44 = 0.0;
			perspective.M43 = (nearPlaneDistance * farPlaneDistance / (nearPlaneDistance - farPlaneDistance));
			return perspective;
		}

		static constexpr Matrix CreatePerspectiveOffCenter(double left, double right, double bottom, double top, double nearPlaneDistance, double farPlaneDistance)
		{
			if (nearPlaneDistance <= 0.0)
				nearPlaneDistance = 0.1;

			if (farPlaneDistance <= 0.0)
				farPlaneDistance = 1.0;

			if (nearPlaneDistance >= farPlaneDistance)
				farPlaneDistance = farPlaneDistance - 1.0;

			Matrix perspectiveOffCenter;
			perspectiveOffCenter.M11 = (2.0 * nearPlaneDistance / (right - left));
			perspectiveOffCenter.M12 = perspectiveOffCenter.M13 = perspectiveOffCenter.M14 = 0.0f;
			perspectiveOffCenter.M22 = (2.0 * nearPlaneDistance / (top - bottom));
			perspectiveOffCenter.M21 = perspectiveOffCenter.M23 = perspectiveOffCenter.M24 = 0.0f;
			perspectiveOffCenter.M31 = ((left + right) / (right - left));
			perspectiveOffCenter.M32 = ((top + bottom) / (top - bottom));
			perspectiveOffCenter.M33 = farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
			perspectiveOffCenter.M34 = -1.0;
			perspectiveOffCenter.M43 = (nearPlaneDistance * farPlaneDistance / (nearPlaneDistance - farPlaneDistance));
			perspectiveOffCenter.M41 = perspectiveOffCenter.M42 = perspectiveOffCenter.M44 = 0.0f;
			return perspectiveOffCenter;
		}

		static constexpr Matrix CreateOrthographic(double width, double height,	double zNearPlane, double zFarPlane) {
			Matrix orthographic;
			orthographic.M11 = 2.0 / width;
			orthographic.M12 = orthographic.M13 = orthographic.M14 = 0.0f;
			orthographic.M22 = 2.0 / height;
			orthographic.M21 = orthographic.M23 = orthographic.M24 = 0.0f;
			orthographic.M33 = (1.0 / (zNearPlane - zFarPlane));
			orthographic.M31 = orthographic.M32 = orthographic.M34 = 0.0f;
			orthographic.M41 = orthographic.M42 = 0.0f;
			orthographic.M43 = zNearPlane / (zNearPlane - zFarPlane);
			orthographic.M44 = 1.0;
			return orthographic;
		}

		static constexpr Matrix CreateOrthographicOffCenter(double left, double right, double bottom, double top, double zNearPlane, double zFarPlane) {
			Matrix orthographicOffCenter;
			orthographicOffCenter.M11 = (2.0 / (right - left));
			orthographicOffCenter.M12 = orthographicOffCenter.M13 = orthographicOffCenter.M14 = 0.0f;
			orthographicOffCenter.M22 = (2.0 / (top - bottom));
			orthographicOffCenter.M21 = orthographicOffCenter.M23 = orthographicOffCenter.M24 = 0.0f;
			orthographicOffCenter.M33 = (1.0 / (zNearPlane - zFarPlane));
			orthographicOffCenter.M31 = orthographicOffCenter.M32 = orthographicOffCenter.M34 = 0.0f;
			orthographicOffCenter.M41 = ((left + right) / (left - right));
			orthographicOffCenter.M42 = ((top + bottom) / (bottom - top));
			orthographicOffCenter.M43 = zNearPlane / (zNearPlane - zFarPlane);
			orthographicOffCenter.M44 = 1.0;
			return orthographicOffCenter;
		}

		static Matrix CreateLookAt(Vector3 cameraPosition, Vector3 cameraTarget, Vector3 cameraUpVector) {
			Vector3 vector3_1 = Vector3::Normalize(cameraPosition - cameraTarget);
			Vector3 vector3_2 = Vector3::Normalize(Vector3::Cross(cameraUpVector, vector3_1));
			Vector3 vector1 = Vector3::Cross(vector3_1, vector3_2);
			
			Matrix lookAt;
			lookAt.M11 = vector3_2.X;
			lookAt.M12 = vector1.X;
			lookAt.M13 = vector3_1.X;
			lookAt.M14 = 0.0f;
			lookAt.M21 = vector3_2.Y;
			lookAt.M22 = vector1.Y;
			lookAt.M23 = vector3_1.Y;
			lookAt.M24 = 0.0f;
			lookAt.M31 = vector3_2.Z;
			lookAt.M32 = vector1.Z;
			lookAt.M33 = vector3_1.Z;
			lookAt.M34 = 0.0f;
			lookAt.M41 = -Vector3::Dot(vector3_2, cameraPosition);
			lookAt.M42 = -Vector3::Dot(vector1, cameraPosition);
			lookAt.M43 = -Vector3::Dot(vector3_1, cameraPosition);
			lookAt.M44 = 1.0;
			return lookAt;
		}		

		bool constexpr Equals(Matrix const& other) const {
			return M11 == other.M11
				&& M22 == other.M22
				&& M33 == other.M33
				&& M44 == other.M44 
				&& M12 == other.M12 
				&& M13 == other.M13 
				&& M14 == other.M14 
				&& M21 == other.M21 
				&& M23 == other.M23 
				&& M24 == other.M24 
				&& M31 == other.M31 
				&& M32 == other.M32 
				&& M34 == other.M34 
				&& M41 == other.M41 
				&& M42 == other.M42 
				&& M43 == other.M43;
		}

		static constexpr Matrix Transpose(Matrix const& matrix)
		{
			Matrix matrix1;
			matrix1.M11 = matrix.M11;
			matrix1.M12 = matrix.M21;
			matrix1.M13 = matrix.M31;
			matrix1.M14 = matrix.M41;
			matrix1.M21 = matrix.M12;
			matrix1.M22 = matrix.M22;
			matrix1.M23 = matrix.M32;
			matrix1.M24 = matrix.M42;
			matrix1.M31 = matrix.M13;
			matrix1.M32 = matrix.M23;
			matrix1.M33 = matrix.M33;
			matrix1.M34 = matrix.M43;
			matrix1.M41 = matrix.M14;
			matrix1.M42 = matrix.M24;
			matrix1.M43 = matrix.M34;
			matrix1.M44 = matrix.M44;
			return matrix1;
		}

		double Determinant() {
			const auto m11 = M11;
			const auto m12 = M12;
			const auto m13 = M13;
			const auto m14 = M14;
			const auto m21 = M21;
			const auto m22 = M22;
			const auto m23 = M23;
			const auto m24 = M24;
			const auto m31 = M31;
			const auto m32 = M32;
			const auto m33 = M33;
			const auto m34 = M34;
			const auto m41 = M41;
			const auto m42 = M42;
			const auto m43 = M43;
			const auto m44 = M44;
			const auto num1 = (m33 * m44 - m34 * m43);
			const auto num2 = (m32 * m44 - m34 * m42);
			const auto num3 = (m32 * m43 - m33 * m42);
			const auto num4 = (m31 * m44 - m34 * m41);
			const auto num5 = (m31 * m43 - m33 * m41);
			const auto num6 = (m31 * m42 - m32 * m41);
			return (m11 * (m22 * num1 - m23 * num2 + m24 * num3) - m12 * (m21 * num1 - m23 * num4 + m24 * num5) + m13 * (m21 * num2 - m22 * num4 + m24 * num6) - m14 * (m21 * num3 - m22 * num5 + m23 * num6));
		}

		static constexpr Matrix Invert(Matrix const& matrix) {
			const auto m11 = matrix.M11;
			const auto m12 = matrix.M12;
			const auto m13 = matrix.M13;
			const auto m14 = matrix.M14;
			const auto m21 = matrix.M21;
			const auto m22 = matrix.M22;
			const auto m23 = matrix.M23;
			const auto m24 = matrix.M24;
			const auto m31 = matrix.M31;
			const auto m32 = matrix.M32;
			const auto m33 = matrix.M33;
			const auto m34 = matrix.M34;
			const auto m41 = matrix.M41;
			const auto m42 = matrix.M42;
			const auto m43 = matrix.M43;
			const auto m44 = matrix.M44;
			const auto num1 = (m33 * m44 - m34 * m43);
			const auto num2 = (m32 * m44 - m34 * m42);
			const auto num3 = (m32 * m43 - m33 * m42);
			const auto num4 = (m31 * m44 - m34 * m41);
			const auto num5 = (m31 * m43 - m33 * m41);
			const auto num6 = (m31 * m42 - m32 * m41);
			const auto num7 = (m22 * num1 - m23 * num2 + m24 * num3);
			const auto num8 = -(m21 * num1 - m23 * num4 + m24 * num5);
			const auto num9 = (m21 * num2 - m22 * num4 + m24 * num6);
			const auto num10 = -(m21 * num3 - m22 * num5 + m23 * num6);
			const auto num11 = (1.0 / (m11 * num7 + m12 * num8 + m13 * num9 + m14 * num10));
			
			Matrix matrix1;
			matrix1.M11 = num7 * num11;
			matrix1.M21 = num8 * num11;
			matrix1.M31 = num9 * num11;
			matrix1.M41 = num10 * num11;
			matrix1.M12 = -(m12 * num1 - m13 * num2 + m14 * num3) * num11;
			matrix1.M22 = (m11 * num1 - m13 * num4 + m14 * num5) * num11;
			matrix1.M32 = -(m11 * num2 - m12 * num4 + m14 * num6) * num11;
			matrix1.M42 = (m11 * num3 - m12 * num5 + m13 * num6) * num11;
			const auto num12 = (m23 * m44 - m24 * m43);
			const auto num13 = (m22 * m44 - m24 * m42);
			const auto num14 = (m22 * m43 - m23 * m42);
			const auto num15 = (m21 * m44 - m24 * m41);
			const auto num16 = (m21 * m43 - m23 * m41);
			const auto num17 = (m21 * m42 - m22 * m41);
			matrix1.M13 = (m12 * num12 - m13 * num13 + m14 * num14) * num11;
			matrix1.M23 = -(m11 * num12 - m13 * num15 + m14 * num16) * num11;
			matrix1.M33 = (m11 * num13 - m12 * num15 + m14 * num17) * num11;
			matrix1.M43 = -(m11 * num14 - m12 * num16 + m13 * num17) * num11;
			const auto num18 = (m23 * m34 - m24 * m33);
			const auto num19 = (m22 * m34 - m24 * m32);
			const auto num20 = (m22 * m33 - m23 * m32);
			const auto num21 = (m21 * m34 - m24 * m31);
			const auto num22 = (m21 * m33 - m23 * m31);
			const auto num23 = (m21 * m32 - m22 * m31);
			matrix1.M14 = -(m12 * num18 - m13 * num19 + m14 * num20) * num11;
			matrix1.M24 = (m11 * num18 - m13 * num21 + m14 * num22) * num11;
			matrix1.M34 = -(m11 * num19 - m12 * num21 + m14 * num23) * num11;
			matrix1.M44 = (m11 * num20 - m12 * num22 + m13 * num23) * num11;
			return matrix1;
		}

		static constexpr Matrix Lerp(Matrix const& matrix1, Matrix const& matrix2, double amount) {
			Matrix matrix;
			matrix.M11 = matrix1.M11 + (matrix2.M11 - matrix1.M11) * amount;
			matrix.M12 = matrix1.M12 + (matrix2.M12 - matrix1.M12) * amount;
			matrix.M13 = matrix1.M13 + (matrix2.M13 - matrix1.M13) * amount;
			matrix.M14 = matrix1.M14 + (matrix2.M14 - matrix1.M14) * amount;
			matrix.M21 = matrix1.M21 + (matrix2.M21 - matrix1.M21) * amount;
			matrix.M22 = matrix1.M22 + (matrix2.M22 - matrix1.M22) * amount;
			matrix.M23 = matrix1.M23 + (matrix2.M23 - matrix1.M23) * amount;
			matrix.M24 = matrix1.M24 + (matrix2.M24 - matrix1.M24) * amount;
			matrix.M31 = matrix1.M31 + (matrix2.M31 - matrix1.M31) * amount;
			matrix.M32 = matrix1.M32 + (matrix2.M32 - matrix1.M32) * amount;
			matrix.M33 = matrix1.M33 + (matrix2.M33 - matrix1.M33) * amount;
			matrix.M34 = matrix1.M34 + (matrix2.M34 - matrix1.M34) * amount;
			matrix.M41 = matrix1.M41 + (matrix2.M41 - matrix1.M41) * amount;
			matrix.M42 = matrix1.M42 + (matrix2.M42 - matrix1.M42) * amount;
			matrix.M43 = matrix1.M43 + (matrix2.M43 - matrix1.M43) * amount;
			matrix.M44 = matrix1.M44 + (matrix2.M44 - matrix1.M44) * amount;
			return matrix;
		}

		static constexpr Matrix Negate(Matrix const& matrix)	{
			Matrix matrix1;
			matrix1.M11 = -matrix.M11;
			matrix1.M12 = -matrix.M12;
			matrix1.M13 = -matrix.M13;
			matrix1.M14 = -matrix.M14;
			matrix1.M21 = -matrix.M21;
			matrix1.M22 = -matrix.M22;
			matrix1.M23 = -matrix.M23;
			matrix1.M24 = -matrix.M24;
			matrix1.M31 = -matrix.M31;
			matrix1.M32 = -matrix.M32;
			matrix1.M33 = -matrix.M33;
			matrix1.M34 = -matrix.M34;
			matrix1.M41 = -matrix.M41;
			matrix1.M42 = -matrix.M42;
			matrix1.M43 = -matrix.M43;
			matrix1.M44 = -matrix.M44;
			return matrix1;
		}

		static constexpr Matrix Add(Matrix const& matrix1, Matrix const& matrix2) {
			Matrix matrix;
			matrix.M11 = matrix1.M11 + matrix2.M11;
			matrix.M12 = matrix1.M12 + matrix2.M12;
			matrix.M13 = matrix1.M13 + matrix2.M13;
			matrix.M14 = matrix1.M14 + matrix2.M14;
			matrix.M21 = matrix1.M21 + matrix2.M21;
			matrix.M22 = matrix1.M22 + matrix2.M22;
			matrix.M23 = matrix1.M23 + matrix2.M23;
			matrix.M24 = matrix1.M24 + matrix2.M24;
			matrix.M31 = matrix1.M31 + matrix2.M31;
			matrix.M32 = matrix1.M32 + matrix2.M32;
			matrix.M33 = matrix1.M33 + matrix2.M33;
			matrix.M34 = matrix1.M34 + matrix2.M34;
			matrix.M41 = matrix1.M41 + matrix2.M41;
			matrix.M42 = matrix1.M42 + matrix2.M42;
			matrix.M43 = matrix1.M43 + matrix2.M43;
			matrix.M44 = matrix1.M44 + matrix2.M44;
			return matrix;
		}

		static constexpr Matrix Subtract(Matrix const& matrix1, Matrix const& matrix2) {
			Matrix matrix;
			matrix.M11 = matrix1.M11 - matrix2.M11;
			matrix.M12 = matrix1.M12 - matrix2.M12;
			matrix.M13 = matrix1.M13 - matrix2.M13;
			matrix.M14 = matrix1.M14 - matrix2.M14;
			matrix.M21 = matrix1.M21 - matrix2.M21;
			matrix.M22 = matrix1.M22 - matrix2.M22;
			matrix.M23 = matrix1.M23 - matrix2.M23;
			matrix.M24 = matrix1.M24 - matrix2.M24;
			matrix.M31 = matrix1.M31 - matrix2.M31;
			matrix.M32 = matrix1.M32 - matrix2.M32;
			matrix.M33 = matrix1.M33 - matrix2.M33;
			matrix.M34 = matrix1.M34 - matrix2.M34;
			matrix.M41 = matrix1.M41 - matrix2.M41;
			matrix.M42 = matrix1.M42 - matrix2.M42;
			matrix.M43 = matrix1.M43 - matrix2.M43;
			matrix.M44 = matrix1.M44 - matrix2.M44;
			return matrix;
		}

		static constexpr Matrix Multiply(Matrix const& matrix1, Matrix const& matrix2) {
			Matrix matrix;
			matrix.M11 = (matrix1.M11 * matrix2.M11 + matrix1.M12 * matrix2.M21 + matrix1.M13 * matrix2.M31 + matrix1.M14 * matrix2.M41);
			matrix.M12 = (matrix1.M11 * matrix2.M12 + matrix1.M12 * matrix2.M22 + matrix1.M13 * matrix2.M32 + matrix1.M14 * matrix2.M42);
			matrix.M13 = (matrix1.M11 * matrix2.M13 + matrix1.M12 * matrix2.M23 + matrix1.M13 * matrix2.M33 + matrix1.M14 * matrix2.M43);
			matrix.M14 = (matrix1.M11 * matrix2.M14 + matrix1.M12 * matrix2.M24 + matrix1.M13 * matrix2.M34 + matrix1.M14 * matrix2.M44);
			matrix.M21 = (matrix1.M21 * matrix2.M11 + matrix1.M22 * matrix2.M21 + matrix1.M23 * matrix2.M31 + matrix1.M24 * matrix2.M41);
			matrix.M22 = (matrix1.M21 * matrix2.M12 + matrix1.M22 * matrix2.M22 + matrix1.M23 * matrix2.M32 + matrix1.M24 * matrix2.M42);
			matrix.M23 = (matrix1.M21 * matrix2.M13 + matrix1.M22 * matrix2.M23 + matrix1.M23 * matrix2.M33 + matrix1.M24 * matrix2.M43);
			matrix.M24 = (matrix1.M21 * matrix2.M14 + matrix1.M22 * matrix2.M24 + matrix1.M23 * matrix2.M34 + matrix1.M24 * matrix2.M44);
			matrix.M31 = (matrix1.M31 * matrix2.M11 + matrix1.M32 * matrix2.M21 + matrix1.M33 * matrix2.M31 + matrix1.M34 * matrix2.M41);
			matrix.M32 = (matrix1.M31 * matrix2.M12 + matrix1.M32 * matrix2.M22 + matrix1.M33 * matrix2.M32 + matrix1.M34 * matrix2.M42);
			matrix.M33 = (matrix1.M31 * matrix2.M13 + matrix1.M32 * matrix2.M23 + matrix1.M33 * matrix2.M33 + matrix1.M34 * matrix2.M43);
			matrix.M34 = (matrix1.M31 * matrix2.M14 + matrix1.M32 * matrix2.M24 + matrix1.M33 * matrix2.M34 + matrix1.M34 * matrix2.M44);
			matrix.M41 = (matrix1.M41 * matrix2.M11 + matrix1.M42 * matrix2.M21 + matrix1.M43 * matrix2.M31 + matrix1.M44 * matrix2.M41);
			matrix.M42 = (matrix1.M41 * matrix2.M12 + matrix1.M42 * matrix2.M22 + matrix1.M43 * matrix2.M32 + matrix1.M44 * matrix2.M42);
			matrix.M43 = (matrix1.M41 * matrix2.M13 + matrix1.M42 * matrix2.M23 + matrix1.M43 * matrix2.M33 + matrix1.M44 * matrix2.M43);
			matrix.M44 = (matrix1.M41 * matrix2.M14 + matrix1.M42 * matrix2.M24 + matrix1.M43 * matrix2.M34 + matrix1.M44 * matrix2.M44);
			return matrix;
		}

		static constexpr Matrix Multiply(Matrix const& matrix1, double scaleFactor) {
			const auto num = scaleFactor;
			Matrix matrix;
			matrix.M11 = matrix1.M11 * num;
			matrix.M12 = matrix1.M12 * num;
			matrix.M13 = matrix1.M13 * num;
			matrix.M14 = matrix1.M14 * num;
			matrix.M21 = matrix1.M21 * num;
			matrix.M22 = matrix1.M22 * num;
			matrix.M23 = matrix1.M23 * num;
			matrix.M24 = matrix1.M24 * num;
			matrix.M31 = matrix1.M31 * num;
			matrix.M32 = matrix1.M32 * num;
			matrix.M33 = matrix1.M33 * num;
			matrix.M34 = matrix1.M34 * num;
			matrix.M41 = matrix1.M41 * num;
			matrix.M42 = matrix1.M42 * num;
			matrix.M43 = matrix1.M43 * num;
			matrix.M44 = matrix1.M44 * num;
			return matrix;
		}

		static constexpr Matrix Divide(Matrix const& matrix1, Matrix const& matrix2) {
			Matrix matrix;
			matrix.M11 = matrix1.M11 / matrix2.M11;
			matrix.M12 = matrix1.M12 / matrix2.M12;
			matrix.M13 = matrix1.M13 / matrix2.M13;
			matrix.M14 = matrix1.M14 / matrix2.M14;
			matrix.M21 = matrix1.M21 / matrix2.M21;
			matrix.M22 = matrix1.M22 / matrix2.M22;
			matrix.M23 = matrix1.M23 / matrix2.M23;
			matrix.M24 = matrix1.M24 / matrix2.M24;
			matrix.M31 = matrix1.M31 / matrix2.M31;
			matrix.M32 = matrix1.M32 / matrix2.M32;
			matrix.M33 = matrix1.M33 / matrix2.M33;
			matrix.M34 = matrix1.M34 / matrix2.M34;
			matrix.M41 = matrix1.M41 / matrix2.M41;
			matrix.M42 = matrix1.M42 / matrix2.M42;
			matrix.M43 = matrix1.M43 / matrix2.M43;
			matrix.M44 = matrix1.M44 / matrix2.M44;
			return matrix;
		}

		static constexpr Matrix Divide(Matrix const& matrix1, double divider) {
			double num = 1.0 / divider;
			Matrix matrix;
			matrix.M11 = matrix1.M11 * num;
			matrix.M12 = matrix1.M12 * num;
			matrix.M13 = matrix1.M13 * num;
			matrix.M14 = matrix1.M14 * num;
			matrix.M21 = matrix1.M21 * num;
			matrix.M22 = matrix1.M22 * num;
			matrix.M23 = matrix1.M23 * num;
			matrix.M24 = matrix1.M24 * num;
			matrix.M31 = matrix1.M31 * num;
			matrix.M32 = matrix1.M32 * num;
			matrix.M33 = matrix1.M33 * num;
			matrix.M34 = matrix1.M34 * num;
			matrix.M41 = matrix1.M41 * num;
			matrix.M42 = matrix1.M42 * num;
			matrix.M43 = matrix1.M43 * num;
			matrix.M44 = matrix1.M44 * num;
			return matrix;
		}

		constexpr Matrix operator-() {
			return Matrix::Negate(*this);
		}

		friend constexpr bool operator ==(Matrix const& matrix1, Matrix const& matrix2) {
			return matrix1.Equals(matrix2);
		}

		friend constexpr bool operator !=(Matrix const& matrix1, Matrix const& matrix2) {
			return !matrix1.Equals(matrix2);
		}

		friend constexpr Matrix operator +(Matrix const& matrix1, Matrix const& matrix2)	{
			return Matrix::Add(matrix1, matrix2);
		}

		friend constexpr Matrix operator -(Matrix const& matrix1, Matrix const& matrix2) {
			return Matrix::Subtract(matrix1, matrix2);
		}

		friend constexpr Matrix operator *(Matrix const& matrix1, Matrix const& matrix2) {
			return Matrix::Multiply(matrix1, matrix2);
		}

		friend constexpr Matrix operator *(Matrix const& matrix1, double scaleFactor) {
			return Matrix::Multiply(matrix1, scaleFactor);
		}

		friend constexpr Matrix operator *(double scaleFactor, Matrix const& matrix1) {
			return Matrix::Multiply(matrix1, scaleFactor);
		}

		friend constexpr Matrix operator /(Matrix const& matrix1, Matrix const& matrix2) {
			return Matrix::Divide(matrix1, matrix2);
		}

		friend constexpr Matrix operator /(Matrix const& matrix1, double divider) {
			return Matrix::Divide(matrix1, divider);
		}

		//implementado em quaternion.hpp

		static constexpr Matrix CreateFromQuaternion(Quaternion const& quaternion);
		static Matrix CreateFromYawPitchRoll(double yaw, double pitch, double roll);
		static constexpr Matrix Transform(Matrix const& value, Quaternion const& rotation);
		bool Decompose(Vector3& scale, Quaternion& rotation, Vector3& translation);
		
		static Matrix CreateShadow(Vector3 const& lightDirection, Plane const& plane);
		static Matrix CreateReflection(Plane const& value);
	};	
}

namespace xna {
	constexpr Vector2 Vector2::Transform(Vector2 const& position, Matrix const& matrix) {
		const auto num1 = (position.X * matrix.M11 + position.Y * matrix.M21) + matrix.M41;
		const auto num2 = (position.X * matrix.M12 + position.Y * matrix.M22) + matrix.M42;

		return Vector2(num1, num2);
	}

	constexpr Vector2 Vector2::TransformNormal(Vector2 const& normal, Matrix const& matrix) {
		const auto num1 = normal.X * matrix.M11 + normal.Y * matrix.M21;
		const auto num2 = normal.X * matrix.M12 + normal.Y * matrix.M22;
		return Vector2(num1, num2);
	}
}

namespace xna {
	constexpr Vector3 Vector3::Transform(Vector3 const& position, Matrix const& matrix) {
		const auto num1 = (position.X * matrix.M11 + position.Y * matrix.M21 + position.Z * matrix.M31) + matrix.M41;
		const auto num2 = (position.X * matrix.M12 + position.Y * matrix.M22 + position.Z * matrix.M32) + matrix.M42;
		const auto num3 = (position.X * matrix.M13 + position.Y * matrix.M23 + position.Z * matrix.M33) + matrix.M43;

		Vector3 vector3;
		vector3.X = num1;
		vector3.Y = num2;
		vector3.Z = num3;

		return vector3;
	}

	constexpr Vector3 Vector3::TransformNormal(Vector3 const& normal, Matrix const& matrix) {
		const auto num1 = (normal.X * matrix.M11 + normal.Y * matrix.M21 + normal.Z * matrix.M31);
		const auto num2 = (normal.X * matrix.M12 + normal.Y * matrix.M22 + normal.Z * matrix.M32);
		const auto num3 = (normal.X * matrix.M13 + normal.Y * matrix.M23 + normal.Z * matrix.M33);

		Vector3 vector3;
		vector3.X = num1;
		vector3.Y = num2;
		vector3.Z = num3;
		return vector3;
	}
}

namespace xna {
	constexpr Vector4 Vector4::Transform(Vector2 const& position, Matrix const& matrix) {
		const auto num1 = (position.X * matrix.M11 + position.Y * matrix.M21) + matrix.M41;
		const auto num2 = (position.X * matrix.M12 + position.Y * matrix.M22) + matrix.M42;
		const auto num3 = (position.X * matrix.M13 + position.Y * matrix.M23) + matrix.M43;
		const auto num4 = (position.X * matrix.M14 + position.Y * matrix.M24) + matrix.M44;
		Vector4 vector4;
		vector4.X = num1;
		vector4.Y = num2;
		vector4.Z = num3;
		vector4.W = num4;
		return vector4;
	}

	constexpr Vector4 Vector4::Transform(Vector3 const& position, Matrix const& matrix) {
		const auto num1 = (position.X * matrix.M11 + position.Y * matrix.M21 + position.Z * matrix.M31) + matrix.M41;
		const auto num2 = (position.X * matrix.M12 + position.Y * matrix.M22 + position.Z * matrix.M32) + matrix.M42;
		const auto num3 = (position.X * matrix.M13 + position.Y * matrix.M23 + position.Z * matrix.M33) + matrix.M43;
		const auto num4 = (position.X * matrix.M14 + position.Y * matrix.M24 + position.Z * matrix.M34) + matrix.M44;
		Vector4 vector4;
		vector4.X = num1;
		vector4.Y = num2;
		vector4.Z = num3;
		vector4.W = num4;
		return vector4;
	}

	constexpr Vector4 Vector4::Transform(Vector4 const& vector, Matrix const& matrix) {
		const auto num1 = (vector.X * matrix.M11 + vector.Y * matrix.M21 + vector.Z * matrix.M31 + vector.W * matrix.M41);
		const auto num2 = (vector.X * matrix.M12 + vector.Y * matrix.M22 + vector.Z * matrix.M32 + vector.W * matrix.M42);
		const auto num3 = (vector.X * matrix.M13 + vector.Y * matrix.M23 + vector.Z * matrix.M33 + vector.W * matrix.M43);
		const auto num4 = (vector.X * matrix.M14 + vector.Y * matrix.M24 + vector.Z * matrix.M34 + vector.W * matrix.M44);
		Vector4 vector4;
		vector4.X = num1;
		vector4.Y = num2;
		vector4.Z = num3;
		vector4.W = num4;
		return vector4;
	}
}

#endif