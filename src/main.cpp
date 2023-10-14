#include <iostream>
#include <Magick++.h>
#include "../header/utils.h"
#include "../header/WallpaperManager.h"
#include "../header/ImageDownloader.h"
#include "../header/ImageTransformer.h"

int main(int argc, char* argv[]) {
	spdlog::set_level(spdlog::level::trace);
	ImageDownloader imgD;
	ImageTransformer imgT;
	WallpaperManager wm;
	int max = 6;

	if(argc == 2) {
		max = atoi(argv[1]);
	}

	spdlog::debug("Number of monitors detected: {}", wm.GetMonitorCount());
	wm.displayMonitors();
	Image *img = imgD.getNewImage();
	imgD.downloadImage(img);
	imgT.transform(img, 2560, 1440);
	wm.changeLeft(img);
	return 0;

	for(int i = 0; i < max; i++) {
		Image *img = imgD.getNewImage();
		if(img != nullptr) {
			imgD.downloadImage(img);
			imgT.transform(img, 2560, 1440);
			imgD.removeImage(img);
		} else {
			spdlog::error("failed");
		}
		spdlog::debug("- -  - - -- - - -- -- - -  - - ---  - -- - ");
	}
	return 0;
}