#include "../header/ImageTransformer.h"

ImageTransformer::ImageTransformer() {
	Magick::InitializeMagick("");
	m_imageFolder = fs::current_path() / "image";
}

void ImageTransformer::transform(Image *img, int screenWidth, int screenHeight) {
	Magick::Image original {img->getPath().string()};
	int width = static_cast<int>(original.columns());
	int height = static_cast<int>(original.rows());

	// If the image suits to the monitor, there is no need to transform it
	if(width != screenWidth || height != screenHeight) {
		Magick::Image background = fitBackgroundToScreen(&original, screenWidth, screenHeight);

		// If the image is larger or higher than the monitor, then it is resized to suit to it
		if(width > screenWidth || height > screenHeight) {
			original.resize(std::to_string(screenWidth) + "x" +std::to_string(screenHeight));
		}
		Magick::Image composed = background;
		composed.composite(original, Magick::CenterGravity, Magick::OverCompositeOp);
		storeImage(&composed, img);
		return;
	}
	Log::log("The image suits the montinor, therefore it hasn't been transformed", Log::TRACE);
}

bool ImageTransformer::storeImage(Magick::Image *composed, Image *img) {
	if(!fs::exists(m_imageFolder)) {
		if(!fs::create_directory(m_imageFolder)) {
			Log::log("The directory couldn't have been created: " + m_imageFolder.string(), Log::ERR);
			return false;
		}
	} else if(!fs::is_directory(m_imageFolder)) {
		Log::log("The folder cannot be created because a file with the same name already exists" + m_imageFolder.string(), Log::ERR);
		return false;
	}
	img->setPath(m_imageFolder / img->getFilename());
	composed->write(img->getPath().string());
	Log::log("The composed image has been stored here: " + img->getPath().string());
	return true;
}

Magick::Image ImageTransformer::fitBackgroundToScreen(Magick::Image *original, int screenWidth, int screenHeight) {
	Log::log("The original dimension of the image: " + std::to_string(original->columns()) + "x" + std::to_string(original->rows()), Log::TRACE);
	Magick::Image background;
	background = createBackground(original, screenWidth, screenHeight);

	// Set up an offset to crop the image around its center
	int offsetX = (background.columns() - screenWidth) / 2,
		offsetY = (background.rows() - screenHeight) / 2;
	std::string geometry = std::to_string(screenWidth) + "x" + std::to_string(screenHeight) +
		"+" + std::to_string(offsetX) + "+" + std::to_string(offsetY);

	Log::log("The geometry used to crop the image is: " + geometry);
	background.crop(geometry.c_str());
	return background;
}

Magick::Image ImageTransformer::createBackground(Magick::Image *original, int width, int height) {
	assert(width != 0 && height != 0);

	Magick::Image background = *original;
	std::string size = std::to_string(width) + "x" + std::to_string(height) + "^";
	Log::log("The size chosen to resize the image is: " + size);
	Magick::Geometry newDimensions(size);

	background.resize(newDimensions);
	Log::log("The size resized image has the following dimensions: " + std::to_string(background.columns()) + "x" + std::to_string(background.rows()));
	return background;
}