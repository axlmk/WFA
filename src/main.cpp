#include "../header/Utils.h"
#include "../header/WallpaperManager.h"
#include "../header/ImageDownloader.h"
#include "../header/ImageTransformer.h"


int main(int argc, char* argv[]) {
	Log::InitiazeLogLevel(Log::TRACE);

	ImageDownloader &imgD = ImageDownloader::Get();
	ImageTransformer &imgT = ImageTransformer::Get();
	WallpaperManager &wm = WallpaperManager::Get();
	
	Log::log("Number of monitors detected: " + std::to_string(wm.GetMonitorsCount()));
	wm.displayMonitors();

	auto monitorsIDs {wm.GetMonitorsIDs()};
	for(auto i = monitorsIDs.begin(); i < monitorsIDs.end(); i++) {
		Image *img = imgD.getNewImage();
		imgD.downloadImage(img);
		std::tuple<int, int> monitorDim {wm.GetMonitorDimensions(*i)};
		imgT.transform(img, std::get<0>(monitorDim), std::get<1>(monitorDim));
	}
	return 0;
}