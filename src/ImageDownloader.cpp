#include "../header/ImageDownloader.h"

ImageDownloader::ImageDownloader() {

	// Initialize the curl library to be able to use it later
	m_curl = curl_easy_init();
	if(!m_curl) {
		Log::log("Curl failed creating Curl object", Log::ERR);
	}
	m_tempFolder = fs::current_path() / "temp";
}

ImageDownloader::~ImageDownloader() {

	// Clear the curl library
	curl_easy_cleanup(m_curl);
}

size_t ImageDownloader::storeToMemoryCB(void *contents, size_t size, size_t nmemb, void *userp) {
	auto realsize = size * nmemb;
	auto &mem = *static_cast<std::string *>(userp);
	mem.append(static_cast<char *>(contents), realsize);
	return realsize;
}

struct tm ImageDownloader::arstationDateToTm(std::string date) {
	struct tm tm;
	if(sscanf(date.c_str(), "%d-%d-%dT%d:%d:%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec) != 6) {
		Log::log("Date's string could not be parsed correctly: " + date, Log::ERR);
		throw std::invalid_argument("The provided date doesn't have the good format");
	} 
	Log::log("Date's string has been parsed successfully: " + date, Log::TRACE);
	return tm;
}

Image *ImageDownloader::jsonToImage(json const j) {
	Image *img = new Image();

	try {
		ImageInfo imageInfo = {
			j["assets"][0]["image_url"],
			j["assets"][0]["width"],
			j["assets"][0]["height"],
			j["assets"][0]["id"]
		}; img->setImage(imageInfo);

		Author author = {
			j["user"]["permalink"],
			j["user"]["full_name"],
			j["user"]["username"]
		}; img->setAuthor(author);
	} catch(json::type_error const &e) {
		Log::log("Json file doesn't have the good format.", Log::ERR);
		return nullptr;
	} catch(std::invalid_argument const &e) {
		Log::log(e.what(), Log::ERR);
		return nullptr;
	}

	try {
	struct tm date;
		date = ImageDownloader::arstationDateToTm(j["published_at"]);
		Project project = {
			j["permalink"],
			j["title"],
			j["description"],
			date
		}; img->setProject(project);
	} catch(std::invalid_argument const &e) {
		return nullptr;
	} catch(json::type_error const &e) {
		Log::log("Json file doesn't have the good format.", Log::ERR);
		return nullptr;
	}

	return img;
}

Image *ImageDownloader::getNewImage() {
	auto rawContent = webRequest(m_siteLink);
	if(rawContent.empty()) {
		Log::log("The web request returned null");
		return nullptr;
	}

	json content;
	try {
		content = json::parse(rawContent);
	} catch(json::exception const& e) {
		Log::log("The raw content is not a JSON format. The image object could not be generated", Log::ERR);
		return nullptr;
	} Log::log("The raw content has successfully been parsed as a JSON format", Log::TRACE);

	return jsonToImage(content);
}

std::string ImageDownloader::webRequest(std::string link) {
	CURLcode ret;
	std::string rawContent;

	Log::log("The queried link: " + link, Log::TRACE);
	curl_easy_setopt(m_curl, CURLOPT_URL, link.c_str());
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, storeToMemoryCB);
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &rawContent);
	curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);
	Log::log("Curl options are set", Log::TRACE);

	ret = curl_easy_perform(m_curl);

	if(ret != CURLE_OK) {
		Log::log("Curl couldn't perform the request: " + std::string(curl_easy_strerror(ret)), Log::ERR);
		return "";
	}
	Log::log("Curl request has been performed successfully", Log::TRACE);
	return rawContent;
}

int ImageDownloader::downloadImage(Image *image) {
	std::string rawContent = webRequest(image->getLink());
	if(rawContent.empty()) {
		return 1;
	}
	auto ret = storeImage(image, rawContent);
	if(!ret) {
		return 2;
	} return 0;

}

bool ImageDownloader::storeImage(Image *img, std::string const rawContent) {
	if(!fs::exists(m_tempFolder)) {
		if(!fs::create_directory(m_tempFolder)) {
			Log::log("The directory couldn't have been created", Log::ERR);
			return false;
		}
	} else if(!fs::is_directory(m_tempFolder)) {
		Log::log("The folder cannot be created because a file with the same name already exists", Log::ERR);
		return false;
	}

	fs::path pOriginal = m_tempFolder;
	pOriginal /= img->getFilename();
	Log::log("The original path of the downloaded image: " + pOriginal.string());
	img->setPath(pOriginal);

	std::ofstream original(pOriginal, ios::binary);
	if(!original) {
		Log::log("The file could not be opened correctly: " + pOriginal.string(), Log::ERR);
		return false;
	}
	original << rawContent;
	return true;
}

void ImageDownloader::removeImage(Image *img) {
	fs::path tempFolder = m_tempFolder / img->getFilename();
	if(!fs::remove(tempFolder)) {
		Log::log("The provided image doesn't exist", Log::ERR);
	}
}