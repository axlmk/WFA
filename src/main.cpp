#include "../header/Utils.h"
#include "../header/WallpaperManager.h"
#include "../header/ImageDownloader.h"
#include "../header/ImageTransformer.h"

int main(int argc, char* argv[]) {

	Logs logs(Logs::TRACE);
	ImageDownloader imgD;
	ImageTransformer imgT;
	WallpaperManager wm;
	int max = 6;

	if (argc == 2) {
		max = atoi(argv[1]);
	}
	
	logs.log("Number of monitors detected: " + std::to_string(wm.GetMonitorCount()), Logs::TRACE);
	wm.displayMonitors();
	Image *img = imgD.getNewImage();
	imgD.downloadImage(img);
	imgT.transform(img, 2560, 1440);
	// wm.changeLeft(img);
	// return 0;
	// for (int i = 0; i < max; i++) {
	// 	Image *img = imgD.getNewImage();
	// 	if(img != nullptr) {
	// 		imgD.downloadImage(img);
	// 		imgT.transform(img, 2560, 1440);
	// 		imgD.removeImage(img);
	// 	} else {
	// 		logs.log("failed", Logs::ERR);
	// 	}
	// 	logs.log("- -  - - -- - - -- -- - -  - - ---  - -- - ", Logs::DEBUG);
	// }
	return 0;
}