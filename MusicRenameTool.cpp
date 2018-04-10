#include "ConsoleMenu.h"
#include <stack>

struct Song {
	std::string path;
	std::string name;

	Song() = default;
	Song(const char * filepath) {
		std::string path(filepath);
		int loc = path.find_last_of('\\') + 1;

		this->path = path.substr(0, loc);
		this->name = path.substr(loc);
	}
};

class RenameTool {
	std::vector<Song> songs;
	std::stack<std::vector<Song>> history;
	int maxId = 0;

	std::vector<std::string> explode(std::string const & s, char delim)
	{
		std::vector<std::string> result;
		std::istringstream iss(s);

		for (std::string token; std::getline(iss, token, delim); )
		{
			result.push_back(std::move(token));
		}

		return result;
	}
	std::string implode(std::vector<std::string> const & vec, char delim) {
		std::ostringstream ss;
		for (auto& word : vec)
			ss << word << delim;
		return ss.str().substr(0, ss.str().find_last_of(delim));
	}
public:
	void addSong(const Song& song) { 
		songs.push_back(song); 
		maxId = song.name.length() > songs[maxId].name.length() ? songs.size()-1 : maxId;
	}
	void removePrefix(const std::string& strmask) {
		history.push(songs);
		for (auto& song : songs)
			song.name=song.name.substr(strmask.length());
	}
	void removeChars(const std::string& chars) {
		history.push(songs);
		for (const char&c : chars) {
			for (auto& song : songs) {
				song.name.erase(std::remove(song.name.begin(), song.name.end(), c), song.name.end());
			}
		}
	}
	void capitalizeEach() {
		history.push(songs);
		for (auto&song : songs) {
			std::vector<std::string> seperated { explode(song.name, ' ') };
			for (auto&word : seperated) {
				for (char&c : word)c=tolower(c);
				word[0]=toupper(word[0]);
			}
			song.name=implode(seperated, ' ');
		}
	}
	void capitalizeFirst() {
		history.push(songs);
		for (auto&song : songs) {
			for (char&c : song.name)
				c = tolower(c);
			song.name[0]=toupper(song.name[0]);
		}			
	}
	void enumerate(std::string seperator=" ",unsigned int from=0) {
		history.push(songs);
		unsigned int counter = from;
		for (auto&song : songs) {
			std::ostringstream ss;
			ss << std::setfill('0') << std::setw(2) << ++counter << seperator << song.name;
			song.name = ss.str();
		}
	}
	void replace(char before, char after) {
		history.push(songs);
		for (auto& song : songs) {
			std::istringstream iss(song.name);
			std::ostringstream oss;

			for (std::string token; std::getline(iss, token, before); ) {
				oss << std::move(token);
				if (!iss.eof()) oss << after;
			}
			song.name = oss.str();
		}
	}


	void undo() throw(std::exception) {
		if (!history.empty()) {
			songs = history.top();
			history.pop();
		}
		else throw std::exception("Stack underflow!");
		
	}
	void commit() {
		if (history.empty())throw std::exception("History stack underflow!");

		while (history.size() > 1)history.pop();

		std::vector<Song> originals = history.top();
		history.pop();

		for (int i = 0; i < songs.size(); i++) {
			std::string original = originals[i].path + originals[i].name;
			std::string changed = songs[i].path + songs[i].name;
			rename(original.c_str(), changed.c_str());
		}
	}

	void print() {
		for (auto& song : songs)
			std::cout << song.name << '\n';
	}
	std::string getSongNameById(unsigned int id) {
		if(id<songs.size())
			return songs[id].name;
		else throw std::exception("Index out of bounds");
	}
};

int main(int argc,char ** argv) {
	RenameTool tool;
	for (int i = 1; i < argc; i++)
		tool.addSong(Song(*(argv + i)));

	ConsoleMenu::UI ui;
	enum option_handles {
		RM_PREFIX = 1,
		RM_SPEC_CHARS,
		REPLACE,
		CAP_FIRST,
		CAP_EACH,
		ENUMERATE,
		COMMIT,
		PRINT,
		UNDO,
		EXIT = 99
	};
	ui.createMenu("main_menu", "Music Rename Tool")
		.createOption("Remove prefix", RM_PREFIX)
		.createOption("Remove special chars", RM_SPEC_CHARS)
		.createOption("Replace chars",REPLACE)
		.createOption("Capitalize first", CAP_FIRST)
		.createOption("Capitalize each", CAP_EACH)
		.createOption("Add numbers", ENUMERATE)
		.createOption("Commit changes", COMMIT)
		.createOption("Print elements", PRINT)
		.createOption("Undo", UNDO)
		.createOption("Exit", EXIT)
		.print();

	std::string buffer;
	while (ui.active()) {
		try {
			buffer.erase();
			switch (ui.getCommand()) {
			case RM_PREFIX:
				std::cout << tool.getSongNameById(0) << std::endl;
				std::cin >> buffer;
				tool.removePrefix(buffer);
				break;
			case RM_SPEC_CHARS:
				std::cout << "input: ";
				std::cin >> buffer;
				tool.removeChars(buffer);
				break;
			case REPLACE:
				std::cout << "replace [b:a]: ";
				std::cin.ignore();
				std::getline(std::cin, buffer);
				if (buffer.size() != 2)throw std::exception("Only before and after should be put");
				tool.replace(buffer[0], buffer[1]);
				break;
			case CAP_FIRST:
				tool.capitalizeFirst();
				break;
			case CAP_EACH:
				tool.capitalizeEach();
				break;
			case ENUMERATE:
				tool.enumerate();
				break;
			case COMMIT:
				tool.commit();
				ui.disable();
				break;
			case PRINT:
				tool.print();
				break;
			case UNDO:
				tool.undo();
				break;
			case EXIT:
				ui.disable();
				break;
			}
		}
		catch (ConsoleMenu::ConsoleMenuException& ex) {
			std::cerr << ex.what() << std::endl;
		}	
		catch (std::exception& ex) {
			std::cerr << ex.what() << std::endl;
			system("pause");
		}
	}

	return 0;
}

