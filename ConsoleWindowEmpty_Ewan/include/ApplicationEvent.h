#pragma once

#include "Event.h"
#include <cstdint>

class WindowResizeEvent : public Event
{
public:
	virtual ~WindowResizeEvent() {};
	WindowResizeEvent(uint32_t a_width, uint32_t a_height) :
		m_width(a_width), m_height(a_height) {}
	
	static constexpr DescriptorType descriptor = "WindowResizeEvent";
	virtual DescriptorType type() const { return descriptor; }
	inline uint32_t GetWidth() { return m_width; }
	inline uint32_t GetHeight() { return m_height; }
private:
	uint32_t m_width;
	uint32_t m_height;
};