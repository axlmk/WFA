#pragma once

#include <ctime>
#include <string>
#include "utils.h"

typedef struct s_Author {
	std::string link;     // Link to the author's page
	std::string fullname; // Name + Lastname
	std::string pseudo;
} Author;

typedef struct s_Project {
	std::string link;
	std::string title;
	std::string description; 
	struct tm dateOfPublication;
} Project;

typedef struct s_Image {
		std::string link;
		unsigned int width = 0;
		unsigned int height = 0;
		unsigned int id = 0; // ID defined by the website
} ImageInfo;

class Image {
	private:

	    // Gather informations about components
		Author m_author;
		Project m_project;
		ImageInfo m_image;

		// Represent the image extension
		std::string m_extension;

		// Represent the image filename (name.extension)
		fs::path m_filename;

		// Represent the image path (path\filename)
		fs::path m_path;

		// Inform the user when the image has been downloaded
		struct tm m_dateOfDownload;

		// Specify all the format that are accepted
		// Currently : {png, jpg}
		static std::string m_acceptedFileFormats[];

	public:

	    // Classical attributes setters
		void setAuthor(Author author);
		void setProject(Project project);
		void setPath(fs::path path);
		void setDownloadDate(tm date);

		// Set the image structure as well as the filename and the extension 
		void setImage(ImageInfo image);

		// Classical attributes getters
		int getId();
		fs::path getFilename();
		fs::path getPath();
		std::string getLink();
		int getHeight();
		int getWidth();

		// Represent the object as a string, giving all useful informations
		std::string to_string();

		// From a fullname, get the extension type
		// Only extensions defined in the acceptedFileFormats are considered valid
		// Return nullptr if no valid extension has been found
		static std::string getExtension(std::string filename);
};