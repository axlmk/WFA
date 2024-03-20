#pragma once
#include "utils.h"
#include <Magick++.h>
#include "Image.h"

class ImageTransformer {
	private:

	    // The path to the image folder where final images are stored
	    fs::path m_imageFolder;

		// Get a blured background based on the original image an cropped to monitor's dimensions
		Magick::Image fitBackgroundToScreen(Magick::Image *original, int screenWidth, int screenHeight);

		// Get a blured background based on an image and monitor's dimensions
		// The returned image is bigger or equals than the original
		Magick::Image createBackground(Magick::Image *original, int width, int height);

		// Store the final image on the disk
		bool storeImage(Magick::Image *composed, Image *img);
	public:

	    // Set up the current path as well as Magick properties
	    ImageTransformer();

		// Get an image formatted for the monitor's dimension
		// If the original image doesn't fit perfectly with the monitor's dimensions
		//a blured background image is created and croped to corresponds to these dimensions.
		// Once done, the original image is added upong it.
		void transform(Image *img, int screenWidth, int screenHeight);
};