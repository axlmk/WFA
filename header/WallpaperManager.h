#pragma once
#include "utils.h"

#include <vector>

#include <Windows.h>
#include <initguid.h>
#include <stringapiset.h>
#include <shobjidl.h>
#include <comdef.h>

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

		// Singleton
		WallpaperManager(const WallpaperManager&) = delete;
		WallpaperManager operator=(WallpaperManager) = delete;
		static WallpaperManager& Get();

		// Get all the monitors' IDs
		std::vector<std::string> GetMonitorsIDs();

		// <widht, height>
		std::tuple<int, int> GetMonitorDimensions(std::string ID);

		// Get the number of monitors detected
		int GetMonitorsCount();
	
		// A setting up function filling the m_monitors vector with useful information obtained from the COM library
		void setMonitors();

		// Change the wallpaper on the specified monitor
		void setImageOnWallpaper(Image *img, std::string MonitorID);

		// Print some data about each monitors detected
		void displayMonitors();

		// Convert the HRESULT from the win32api to a readable message
		std::string HRESULT2str(HRESULT hr);
};