#include "../header/Monitor.h"

int Monitor::getWidth() {
	return abs(m_rect.left - m_rect.right);
}

int Monitor::getHeight() {
	return abs(m_rect.top - m_rect.bottom);
}

void Monitor::setId(std::string id) {
	m_id = id;
}

void Monitor::setRect(RECT rect) {
	m_rect = rect;
	if(getWidth() > getHeight()) {
		m_format = MonitorFormat::LANDSCAPE;
	} else if(getWidth() < getHeight()) {
		m_format = MonitorFormat::PORTRAIT;
	} else {
		m_format = MonitorFormat::SQUARE;
	}
}

std::string Monitor::toString() {
	std::ostringstream ss;
	ss << "Monitor's id: " << m_id << "\n";
	ss << "\t" << "Left: " << m_rect.left << "\n";
	ss << "\t" << "Top: " << m_rect.top << "\n";
	ss << "\t" << "Right: " << m_rect.right << "\n";
	ss << "\t" << "Bottom: " << m_rect.bottom << "\n";
	switch(m_format) 	{
	case MonitorFormat::LANDSCAPE:
		ss << "\tFormat: Landscape\n";
		break;
	case MonitorFormat::PORTRAIT:
		ss << "\tFormat: Portrait\n";
		break;
	case MonitorFormat::SQUARE:
		ss << "\tFormat: Square\n";
		break;
	default:
		ss << "\tFormat: Unrecognized\n";
		break;
	}
	return ss.str();
}

std::string Monitor::getId() {
	return m_id;
}