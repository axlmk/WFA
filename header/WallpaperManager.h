#pragma once
#include "utils.h"

#include <vector>
#include <stringapiset.h>
#include <shobjidl.h>

#include "Monitor.h"
#include "Image.h"

class WallpaperManager {
	private:
	    IDesktopWallpaper *m_wallpaper;
		
		// <ID, object>
		std::unordered_map<std::string, Monitor> m_monitors;
		std::vector<Monitor>::iterator ptr;

		static WallpaperManager mInstance;

		// Initialize the COM object iDesktopWallpaper
	    WallpaperManager();

		// Destroy COM object
		~WallpaperManager();
	public:

		WallpaperManager(const WallpaperManager&) = delete;
		WallpaperManager operator=(WallpaperManager) = delete;

		static WallpaperManager& Get();

		std::vector<std::string> GetMonitorsIDs();

		// <widht, height>
		std::tuple<int, int> GetMonitorDimensions(std::string ID);

		// Get the number of monitors detected
		int GetMonitorsCount();
	
		// temp 
		// void changeLeft(Image *img);

		// A setting up function filling the m_monitors vector with useful information obtained from the COM library
		void setMonitors();

		void setImageOnWallpaper(Image *img, Monitor monitor);

		void displayMonitors();
};