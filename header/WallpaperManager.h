#pragma once
#include "utils.h"

#include <vector>
#include <stringapiset.h>
#include <shobjidl.h>

#include "Monitor.h"
#include "Image.h"

class WallpaperManager {
	private:
	    //
	    IDesktopWallpaper *m_wallpaper;
		
		std::vector<Monitor> m_monitors;
	public:

	    // Initialize the COM object iDesktopWallpaper
	    WallpaperManager();

		// Destroy COM object
		~WallpaperManager();

		// Get the number of monitors detected
		int GetMonitorCount();
	
		// temp 
		void changeLeft(Image *img);

		void setMonitors();

		void setImageOnWallpaper(Image *img, Monitor monitor);

		void displayMonitors();
};