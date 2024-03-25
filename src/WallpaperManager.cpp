#include "../header/WallpaperManager.h"

WallpaperManager WallpaperManager::mInstance;

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

			m_monitors[idS] = monitor;
		} else {
			Log::log("The monitor's ID has not been loaded properly", Log::ERR);
		}
	}
}

WallpaperManager::~WallpaperManager() {
	m_wallpaper->Release();
	CoUninitialize();
}

WallpaperManager& WallpaperManager::Get() {
	return mInstance;
}

int WallpaperManager::GetMonitorsCount() {
	return m_monitors.size();
}

std::vector<std::string> WallpaperManager::GetMonitorsIDs() {
	std::vector<std::string> monitorsIDs;
	for(auto ptr = m_monitors.begin(); ptr != m_monitors.end(); ptr++) {
		monitorsIDs.push_back(ptr->first);
	}
	return monitorsIDs;
}

std::tuple<int, int> WallpaperManager::GetMonitorDimensions(std::string ID) {
	return {m_monitors[ID].getWidth(), m_monitors[ID].getHeight()};
}

void WallpaperManager::displayMonitors() {
	int it = 0;
	for(auto ptr = m_monitors.begin(); ptr != m_monitors.end(); ptr++) {
		Log::log("Monitor number " + std::to_string(it++) + "\n\t" + ptr->second.toString() + "\n");
		it++;
	}
}

void WallpaperManager::setImageOnWallpaper(Image *img, std::string MonitorID) {
	LPCWSTR id = str2lp(MonitorID);
	LPCWSTR pathLw = str2lp(img->getPath().string());
	HRESULT hr = m_wallpaper->SetWallpaper(id, pathLw);
	if(hr != S_OK) {
		Log::log(HRESULT2str(hr), Log::ERR);
	} else {
		Log::log("The wallpaper has successfully been set");
	}
}

std::string WallpaperManager::HRESULT2str(HRESULT hr) {
	LPSTR mB = nullptr;
		FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&mB),
        0,
	NULL);
	return mB;
}