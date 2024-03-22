#include "../header/Image.h"

std::string Image::m_acceptedFileFormats[] = {
	"png",
	"jpg"
};

void Image::setAuthor(Author author) {
	m_author = author;
}

void Image::setProject(Project project) {
	m_project = project;
}

void Image::setImage(ImageInfo image) {
	m_extension = getExtension(image.link);
	if(m_extension.empty()) {
		throw std::invalid_argument("The image extension is unvalid");
	}
	m_filename = std::to_string(image.id) + "." + m_extension;
	Log::log("The filename of the image: " + m_filename.string());
	m_image = image;
}

void Image::setPath(fs::path path) {
	m_path = path;
}

void Image::setDownloadDate(tm date) {
	m_dateOfDownload = date;
}


std::string Image::getLink() {
	return m_image.link;
}

std::string Image::to_string() {
	std::ostringstream ss;
	ss << "Author's name: " << m_author.fullname << "\nAuthor's username: " << m_author.pseudo << "\nAuthor's link: " << m_author.link << "\n\n";
	ss << "Project's title: " << m_project.title << "\nProject's Description: " << m_project.description << "\nProject's link: " << m_project.link << "\n\n"; 
	ss << "Image's dimensions: " << m_image.width << "x" << m_image.height << "\nImage's link: " << m_image.link << "\nImage's id: " << m_image.id << "\n\n";
	ss << "Image's filename: " << m_filename.string();
	return ss.str();
}

int Image::getId() {
	return m_image.id;
}

std::string Image::getExtension(std::string filename) {
	// Link provided by Artstation ends like this : filename.extension?random_number
	// To get the extension, we take everything located between the last dot of the link
	// and the question mark located behind
	auto foundDot = filename.find_last_of(".");
	auto foundQuestionMark = filename.find_last_of("?");
	if(foundDot == std::string::npos || foundQuestionMark == std::string::npos) {
		Log::log("The filename doesn't contain a recognizable extension", Log::ERR);
		return "";
	}

	std::string extension = filename.substr(foundDot + 1, foundQuestionMark - foundDot - 1);
	for(auto i : m_acceptedFileFormats) {
		if(extension == i)
			return i;

	} Log::log("The following extension is not allowed: " + extension, Log::ERR);
	return "";
}

fs::path Image::getFilename() {
	return m_filename;
}

fs::path Image::getPath() {
	return m_path;
}

int Image::getHeight() {
	return m_image.height;
}

int Image::getWidth() {
	return m_image.width;

}