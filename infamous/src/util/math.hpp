#pragma once
#include "stdafx.hpp"

#define PI (float)3.14159265358979323846264338327950288
namespace Math {
	template<typename T>
	struct Vector3_;

	template<typename T>
	struct Vector2 {
		T m_X;
		T m_Y;

		Vector2<T>() {}

		Vector2<T>(T in_x, T in_y)
			: m_X(in_x), m_Y(in_y) {}

		Vector2<T> operator*(float value) {
			return Vector2<T>(m_X * value, m_Y * value);
		}

		Vector2<T> operator*=(float value) {
			return Vector2<T>(m_X * value, m_Y * value);
		}

		Vector2<T> operator*(Vector2<T>& value) {
			return Vector2<T>(m_X * value.m_X, m_Y * value.m_Y);
		}

		Vector2<T> operator+=(Vector2<T>& value) {
			m_X += value.m_X;
			m_Y += value.m_Y;
			return Vector2<T>(m_X, m_Y);
		}

		Vector2<T> operator-(float value) {
			return Vector2<T>(m_X - value, m_Y - value);
		}

		Vector2<T> operator-=(float value) {
			return Vector2<T>(m_X - value, m_Y - value);
		}

		Vector2<T> operator-(Vector2<T>& value) {
			return Vector2<T>(m_X - value.m_X, m_Y - value.m_Y);
		}

		Vector2<T> operator+(float value) {
			return Vector2<T>(m_X + value, m_Y + value);
		}

		Vector2<T> operator+=(float value) {
			return Vector2<T>(m_X + value, m_Y + value);
		}

		Vector2<T> operator+(Vector2<T>& value) {
			return Vector2<T>(m_X + value.m_X, m_Y + value.m_Y);
		}

		Vector2<T> operator/(float value) {
			if (value == 0.f || m_X == 0 || m_Y == 0) return Vector2<T>(0, 0);
			return Vector2<T>(m_X / value, m_Y / value);
		}

		Vector2<T> operator/=(float value) {
			if (value == 0.f || m_X == 0 || m_Y == 0) return Vector2<T>(0, 0);
			return Vector2<T>(m_X / value, m_Y / value);
		}

		Vector2<T> operator/(Vector2<T>& value) {
			return Vector2<T>(m_X / value.m_X, m_Y / value.m_Y);
		}

		bool operator==(Vector2<T>& value) {
			return m_X == value.m_X && m_Y == value.m_Y;
		}

		float GetMathmaticalDistance(Vector2<T>& value) {
			/*float a = value.m_X - m_X;
			float b = value.m_Y - m_Y;
			return (float)sqrt((a * a) + (b * b));*/

			return ((float)sqrt(pow(value.m_X - m_X, 2) + pow(value.m_Y - m_Y, 2) * 1.0));
		}

		float GetLength() {
			return (float)sqrt((m_X * m_X) + (m_Y * m_Y));
		}

		void Normalize() {
			float Length = GetLength();
			m_X /= Length;
			m_Y /= Length;
		}
	};

	template<typename T>
	struct Vector3 {
		T m_X;
		DWORD AAAAA;
		T m_Y;
		DWORD BBBBB;
		T m_Z;
		DWORD CCCCC;

		Vector3<T>() {}

		Vector3<T>(T in_x, T in_y, T in_z)
			: m_X(in_x), m_Y(in_y), m_Z(in_z) {}

		Vector3<T> operator*(float value) {
			return Vector3<T>(m_X * value, m_Y * value, m_Z * value);
		}

		Vector3<T> operator*=(float value) {
			return Vector3<T>(m_X * value, m_Y * value, m_Z * value);
		}

		Vector3<T> operator*(Vector3<T>& value) {
			return Vector3<T>(m_X * value.m_X, m_Y * value.m_Y, m_Z * value.m_Z);
		}

		Vector3<T> operator+=(Vector3<T>& value) {
			return Vector3<T>(m_X + value.m_X, m_Y + value.m_Y, m_Z + value.m_Z);
		}

		Vector3<T> operator-(float value) {
			return Vector3<T>(m_X - value, m_Y - value, m_Z - value);
		}

		Vector3<T> operator-=(float value) {
			return Vector3<T>(m_X - value, m_Y - value, m_Z - value);
		}

		Vector3<T> operator-(Vector3<T>& value) {
			return Vector3<T>(m_X - value.m_X, m_Y - value.m_Y, m_Z - value.m_Z);
		}

		Vector3<T> operator+(float value) {
			return Vector3<T>(m_X + value, m_Y + value, m_Z + value);
		}

		Vector3<T> operator+=(float value) {
			return Vector3<T>(m_X + value, m_Y + value, m_Z + value);
		}

		Vector3<T> operator-=(Vector3<T>& value) {
			return Vector3<T>(m_X - value.m_X, m_Y - value.m_Y, m_Z - value.m_Z);
		}

		Vector3<T> operator+(Vector3<T>& value) {
			return Vector3<T>(m_X + value.m_X, m_Y + value.m_Y, m_Z + value.m_Z);
		}

		Vector3<T> operator/(float value) {
			if (value == 0.f || m_X == 0 || m_Y == 0 || m_Z == 0) return Vector3<T>(0, 0);
			return Vector3<T>(m_X / value, m_Y / value, m_Z / value);
		}

		Vector3<T> operator/=(float value) {
			if (value == 0.f || m_X == 0 || m_Y == 0) return Vector3<T>(0, 0);
			return Vector3<T>(m_X / value, m_Y / value, m_Z / value);
		}

		Vector3<T> operator/(Vector3<T>& value) {
			return Vector3<T>(m_X / value.m_X, m_Y / value.m_Y, m_Z / value.m_Z);
		}

		bool operator==(Vector3<T>& value) {
			return m_X == value.m_X && m_Y == value.m_Y && m_Z == value.m_Z;
		}

		float GetMathmaticalDistance(Vector3<T>& value) {
			/*float a = value.m_X - m_X;
			float b = value.m_Y - m_Y;
			float c = value.m_Z - m_Z;
			return (float)sqrt((a * a) + (b * b) + (c * c));*/

			return ((float)sqrt(pow(value.m_X - m_X, 2) + pow(value.m_Y - m_Y, 2) * 1.0));
		}

		bool IsZero() {
			return (m_X == fabs(0.0f) && m_Y == fabs(0.0f) && m_Z == fabs(0.0f));
		}

		float Get2DLength() {
			return (float)sqrt((m_X * m_X) + (m_Z * m_Z));
		}

		float GetLength() {
			return (float)sqrt((m_X * m_X) + (m_Y * m_Y) + (m_Z * m_Z));
		}

		void Normalize() {
			float Length = GetLength();
			m_X /= Length;
			m_Y /= Length;
			m_Z /= Length;
		}

		static Vector3_<T> ToSerialized(Vector3<T> value) {
			return Vector3_<T>(value.m_X, value.m_Y, value.m_Z);
		}

		float Dot(Vector3<T> value) {
			return (m_X * m_X + m_Y * m_Y + m_Z * m_Z);
		}
	};

	template<typename T>
	struct Vector3_ {
		T m_X;
		T m_Y;
		T m_Z;

		Vector3_<T>() {}

		Vector3_<T>(T in_x, T in_y, T in_z)
			: m_X(in_x), m_Y(in_y), m_Z(in_z) {}

		Vector3_<T> operator*(float value) {
			return Vector3_<T>(m_X * value, m_Y * value, m_Z * value);
		}

		Vector3_<T> operator*=(float value) {
			return Vector3_<T>(m_X * value, m_Y * value, m_Z * value);
		}

		Vector3_<T> operator*(Vector3_<T>& value) {
			return Vector3_<T>(m_X * value.m_X, m_Y * value.m_Y, m_Z * value.m_Z);
		}

		Vector3_<T> operator-(float value) {
			return Vector3_<T>(m_X - value, m_Y - value, m_Z - value);
		}

		Vector3_<T> operator-(Vector3_<T> value) {
			return Vector3_<T>(m_X - value.m_X, m_Y - value.m_Y, m_Z - value.m_Z);
		}

		Vector3_<T> operator-=(float value) {
			return Vector3_<T>(m_X - value, m_Y - value, m_Z - value);
		}

		Vector3_<T> operator-(Vector3_<T>& value) {
			return Vector3_<T>(m_X - value.m_X, m_Y - value.m_Y, m_Z - value.m_Z);
		}

		Vector3_<T> operator+(float value) {
			return Vector3_<T>(m_X + value, m_Y + value, m_Z + value);
		}

		Vector3_<T> operator+=(float value) {
			return Vector3_<T>(m_X + value, m_Y + value, m_Z + value);
		}

		Vector3_<T> operator+=(Vector3_<T> value) {
			return Vector3_<T>(m_X + value.m_X, m_Y + value.m_Y, m_Z + value.m_Z);
		}

		Vector3_<T> operator+(Vector3_<T>& value) {
			return Vector3_<T>(m_X + value.m_X, m_Y + value.m_Y, m_Z + value.m_Z);
		}

		Vector3_<T> operator/(float value) {
			if (value == 0.f || m_X == 0 || m_Y == 0 || m_Z == 0) return Vector3_<T>(0, 0);
			return Vector3_<T>(m_X / value, m_Y / value, m_Z / value);
		}

		Vector3_<T> operator/=(float value) {
			if (value == 0.f || m_X == 0 || m_Y == 0) return Vector3_<T>(0, 0);
			return Vector3_<T>(m_X / value, m_Y / value, m_Z / value);
		}

		Vector3_<T> operator/(Vector3_<T>& value) {
			return Vector3_<T>(m_X / value.m_X, m_Y / value.m_Y, m_Z / value.m_Z);
		}

		bool operator==(Vector3_<T>& value) {
			return m_X == value.m_X && m_Y == value.m_Y && m_Z == value.m_Z;
		}

		float GetMathmaticalDistance(Vector3_<T>& value) {
			return ((float)sqrt(pow(value.m_X - m_X, 2) + pow(value.m_Y - m_Y, 2) * 1.0));
		}

		static Vector3<T> ToPadded(Vector3_<T> value) {
			return Vector3<T>(value.m_X, value.m_Y, value.m_Z);
		}

		float GetLength() {
			return (float)sqrt((m_X * m_X) + (m_Y * m_Y) + (m_Z * m_Z));
		}

		void Normalize() {
			float Length = GetLength();
			m_X /= Length;
			m_Y /= Length;
			m_Z /= Length;
		}
	};

	template<typename T>
	struct Vector4 {
		T m_X;
		T m_Y;
		T m_Z;
		T m_W;
	};

	struct Matrix {
		union {
			struct {
				Vector4<float> m_Left;
				Vector4<float> m_Up;
				Vector4<float> m_Forward;
				Vector4<float> m_Translation;
			};

			float m_Elements[4][4];
		};

		Matrix() {}

		float& operator () (int Row, int Col) {
			return m_Elements[Row][Col];
		}
	};

	template<typename T>
	bool Within(T val, T min, T max) {
		return val <= max && val >= min;
	}

	template<typename T>
	T Clamp(T val, T min, T max) {
		return val < min ? min : val > max ? max : val;
	}

	inline float Lerp(float toEase, float easeFrom, float percent) {
		return (toEase + percent * (easeFrom - toEase));
	}

	inline void Ease(float& toEase, float& easeFrom, float multiplier) {
		toEase += toEase < easeFrom ? abs(toEase - easeFrom) / multiplier : -abs(toEase - easeFrom) / multiplier;
	}

	inline float Repeat(float t, float length) {
		return Clamp(t - floor(t / length) * length, 0.f, length);
	}

	inline float PingPong(float t, float length) {
		t = Repeat(t, length * 2.f);
		return length - fabs(t - length);
	}
}