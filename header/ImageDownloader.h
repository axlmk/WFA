#pragma once

#include "utils.h"
#include <curl\curl.h>
#include <fstream>
#include <time.h>
#include <nlohmann\json.hpp>
#include "Image.h"

using json = nlohmann::json;
using ios = std::ios;

/*
* Manipulate an Image object. Get random images on Artstation,
* load their properties and download the final image.
*/
class ImageDownloader {
	private:
	    //Brought by the CURL library to perform requests
		CURL *m_curl;

		// The URL site from which the images are downloaded
		std::string m_siteLink = "https://artstation.com/random_project.json";
		
		// The path to the temp folder where original images are stored temporarily
		fs::path m_tempFolder;

		// Callback function used by Curl to store the downloaded content
		static size_t storeToMemoryCB(void *contents, size_t size, size_t nmemb, void *userp);
		
		// Get the date according to the format used by Artstation
		static tm arstationDateToTm(std::string date);

		// Perform a standard download request to get the content from a link
		// Return the request's content or nullptr if the query failed
		std::string webRequest(std::string link);

		// Instanciate an image object with the properties coming from a json file
		// The json corresponds to the one furnished by the Artstation site
		Image *jsonToImage(json const j);

		// Take the image data downloaded from a website, store it into a file and set up the 
		// image object
		bool storeImage(Image *img, std::string const rawContent);

		// Singleton
		ImageDownloader();
		~ImageDownloader();
		static ImageDownloader mInstance;
	
	public:

		// Singleton
		ImageDownloader(const ImageDownloader&) = delete;
		ImageDownloader operator=(ImageDownloader) = delete;
		static ImageDownloader& Get();

		// Get an entire new image object loaded randomdly from a website
		// Currently only artstation is available
		// Return the newly constructed object or a nullptr if a problem occured
		Image *getNewImage();

		// Download the image content to file stored on the system
		// Return an error code
		//  0 : OK
		//  1 : The web request cannot download the image
		//  2 : The image cannot be stored
		int downloadImage(Image *image);

		// Remove the image from the temp folder
		void removeImage(Image *img);
};