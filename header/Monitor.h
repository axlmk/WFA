#pragma once
#include "utils.h"

enum class MonitorFormat {
	LANDSCAPE,
	PORTRAIT,
	SQUARE
};

class Monitor {
	private:
	    RECT m_rect = {0, 0, 0, 0};
		MonitorFormat m_format = MonitorFormat::LANDSCAPE;
		std::string m_id;
	public:
	    int getWidth();
	    int getHeight();
		std::string getId();
		void setId(std::string id);
		void setRect(RECT rect);
		std::string toString();
};