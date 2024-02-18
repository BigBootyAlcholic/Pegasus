#pragma once
#include "stdafx.hpp"
#include "util/math.hpp"

struct Color {
	int r;
	int g;
	int b;
	int a = 255;

	Color()
		: r(0), g(0), b(0), a(0) {}

	Color(int red, int green, int blue, int alpha = 255)
		: r(red), g(green), b(blue), a(alpha) {}

	Color(uint32_t hex)
		: r(hex >> 24), g(((hex >> 16) & 0x00FF)), b(((hex >> 8) & 0x0000FF)), a(hex & 0xFF) {}

	Color Opacity(int opacity) {
		return Color(r, g, b, opacity);
	}

	Math::Vector3_<int> AsVector() {
		return { r, g, b };
	}

	std::initializer_list<int> as_initializer_list() {
		return std::initializer_list<int> { r, g, b, a };
	}

	uint32_t to_argb() {
		uint8_t val[4];
		val[0] = a;
		val[1] = r;
		val[2] = g;
		val[3] = b;

		return *(uint32_t*)&val;
	}
};

enum class eOptionAction {
	left_click,
	right_click,
	click
};

enum eJustify {
	JUSTIFY_LEFT,
	JUSTIFY_RIGHT,
	JUSTIFY_CENTER,
};

enum class eFont : std::int32_t {
	ChaletLondon = 0,
	HouseScript = 1,
	Monospace = 2,
	Wingdings = 3,
	ChaletComprimeCologne = 4,
	Pricedown = 7
};

enum class eOptionFlag {
	Indicator = (1 << 0),
	ToggleOption = (1 << 1),
	ScrollOption = (1 << 2),
	NumberOption = (1 << 3),
	SimpleOption = (1 << 4),
	ToggleNumberOption = (1 << 5),
	KeyboardOption = (1 << 6),
	BreakOption = (1 << 7),
	ToggleScrollOption = (1 << 8),
	VehicleOption = (1 << 9),
	ColorOption = (1 << 10)
};


class MemoryStreamBuf : public std::streambuf
{
public:
	MemoryStreamBuf(char* array, std::size_t size)
	{
		std::fill_n(array, size, '\0');
		std::streambuf::setp(array, array + size - 1);
	}

	template <std::size_t size>
	MemoryStreamBuf(char(&array)[size]) :
		MemoryStreamBuf(&array[0], size)
	{
	}
};
class MemoryStringStream : virtual MemoryStreamBuf, public std::iostream
{
public:
	MemoryStringStream(char* array, std::size_t size) :
		MemoryStreamBuf(array, size),
		std::iostream(static_cast<MemoryStreamBuf*>(this))
	{
	}

	template <std::size_t size>
	MemoryStringStream(char(&array)[size]) :
		MemoryStreamBuf(array),
		std::iostream(static_cast<MemoryStreamBuf*>(this))
	{
	}
};

template <std::size_t N>
class MemoryStringStreamWithBuffer : public MemoryStringStream
{
public:
	MemoryStringStreamWithBuffer() :
		MemoryStringStream(m_Buffer)
	{}
private:
	char m_Buffer[N];
};

class Timer {
public:
	explicit Timer(std::chrono::milliseconds delay) :
		m_Timer(std::chrono::high_resolution_clock::now()),
		m_Delay(delay),
		m_ElapsedTime(0ms)
	{
	}

	bool Update() {
		auto now = std::chrono::high_resolution_clock::now();
		std::chrono::milliseconds delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_Timer);
		if (delta >= m_Delay) {
			m_Timer = now;
			m_ElapsedTime = delta;
			return true;
		}

		return false;
	}


	void SetDelay(std::chrono::milliseconds delay) {
		m_Delay = delay;
	}

	std::chrono::milliseconds GetDelay() const {
		return m_Delay;
	}

	std::chrono::milliseconds GetElapsedTime() const {
		return m_ElapsedTime;
	}

	void Reset() {
		m_Timer = std::chrono::high_resolution_clock::now();
		m_ElapsedTime = 0ms;
	}

private:
	std::chrono::high_resolution_clock::time_point m_Timer;
	std::chrono::milliseconds m_Delay;
	std::chrono::milliseconds m_ElapsedTime;
};