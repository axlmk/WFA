#include "../header/WallpaperManager.h"

WallpaperManager::WallpaperManager() {
	HRESULT hr;
	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);
	if(FAILED(hr)) {
		Log::log("The COM library cannot be loaded", Log::ERR);
		throw std::runtime_error("COM library could not be loaded");
	}
	m_wallpaper = nullptr;
	hr = CoCreateInstance(CLSID_DesktopWallpaper, nullptr, CLSCTX_ALL, IID_IDesktopWallpaper, reinterpret_cast<void**>(&m_wallpaper));
	if(FAILED(hr) || m_wallpaper == nullptr) {
		Log::log("The DesktopWallpaper object could not be created", Log::ERR);
		throw std::runtime_error("Cannot create COM object");
	}

	setMonitors();
}

void WallpaperManager::setMonitors() {
	UINT n;
	m_wallpaper->GetMonitorDevicePathCount(&n);

	// Reset the monitor's array by removing all existing monitors
	if(!m_monitors.empty()) {
		m_monitors.clear();
	}

	for(UINT i = 0; i < n; i++) {
		LPWSTR id = nullptr;
		RECT rect;
		Monitor monitor;
		std::string strId;

		m_wallpaper->GetMonitorDevicePathAt(i, &id);
		if(id) {
			m_wallpaper->GetMonitorRECT(id, &rect);

			std::string idS = lp2str(id);
			monitor.setId(idS);
			monitor.setRect(rect);

			m_monitors.push_back(monitor);
		} else {
			Log::log("The monitor's ID has not been loaded properly", Log::ERR);
		}
	}
}

int WallpaperManager::GetMonitorCount() {
	return m_monitors.size();
}

void WallpaperManager::displayMonitors() {
	int it = 0;
	for(Monitor i : m_monitors) {
		Log::log("Monitor number " + std::to_string(it++) + "\n\t" + i.toString() + "\n");
	}
}

void WallpaperManager::setImageOnWallpaper(Image *img, Monitor monitor) {
	std::string idS = monitor.getId();
	LPWSTR id = str2lp(idS);
	fs::path path = img->getPath();
	LPWSTR pathLw = str2lp(path.string());
	m_wallpaper->SetWallpaper(id, pathLw);
}

WallpaperManager::~WallpaperManager() {
	m_wallpaper->Release();
	CoUninitialize();
}

void WallpaperManager::changeLeft(Image *img) {
	setImageOnWallpaper(img, m_monitors.at(1));
}
