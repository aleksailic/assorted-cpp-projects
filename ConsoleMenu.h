#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <exception>
#include <iomanip>
#include <sstream>
#include <algorithm>

namespace ConsoleMenu {
	class ConsoleMenuException : public std::exception {
		const char * mMsg;
	public:
		ConsoleMenuException(const char * msg = "Unknown ConsoleMenu exception") :mMsg(msg) {}
		const char * what() const noexcept { return mMsg; }
	};
	class UI {
	public:
		class Menu {
			UI&ui;
			class Option {
			public:
				const int id;
				std::string desc;
				void(*fn)();

				Option(int o_id, std::string description, void(*callback)() = nullptr) :desc(description), id(o_id), fn(callback) {}
			};
			std::string mHandle;
			std::string mName;
			std::string mFooter;
			std::vector<Option*>options;
			const int line_padding;
			int longest_line = 0;
			unsigned int last_id = 0;

			void printCenteredText(const std::string& txt)const {
				int line_size = longest_line + line_padding;
				bool isEven = (line_size - txt.size()) % 2;
				int fill = (line_size - txt.size() - 2) / 2;
				ui.output << std::setfill('-') << std::setw(fill) << (txt.empty() ? '-' : ' ') << txt << (txt.empty() ? '-' : ' ');
				ui.output << std::setfill('-') << std::setw(fill) << (isEven ? '-' : ' ') << std::endl;
			}
		public:
			Menu(std::string handle, UI&parent, std::string name = "", std::string footer="",int padding = 10) :mHandle(handle), ui(parent), line_padding(padding), mName(name),mFooter(footer) {}
			~Menu() {
				std::for_each(options.begin(), options.end(), [](Option* const ptr) {
					delete ptr;
				});
			}

			Menu& createOption(std::string description, unsigned int id = 0, void(*callback)() = nullptr) {
				if (description.empty()) throw ConsoleMenuException("Option description was empty");
				if (id == 0) id = ++last_id;
				else if (id > last_id) last_id = id;


				options.push_back(new Option(id, description, callback));
				if (description.size() > longest_line) longest_line = description.size();
				return *this;
			}
			Menu& print() const{
				ui.lastPrinted = mHandle;
				
				printCenteredText(mName);
				for (auto& option : options) {
					std::stringstream ss;
					ss << "-" << option->id << ": " << option->desc;
					ss << std::setw(longest_line + line_padding - ss.str().size() - 2) << '-' << std::endl;
					ui.output << ss.str();
				}
				printCenteredText(mFooter);
				return const_cast<Menu&>(*this);
			}
			void setName(std::string name) {
				this->mName = name;
			}
			void setFooter(std::string footer) {
				this->mFooter = footer;
			}
			bool checkId(unsigned int id)const {
				if (id > last_id) return false;
				else {
					for (auto& option : options)
						if (option->id == id)return true;
				}
				return false;
			}
		};
	private:
		std::istream& input;
		std::ostream& output;
		mutable std::string lastPrinted = "";

		std::map<std::string, Menu*> menus;
		bool enabled = true;
	public:
		UI() :UI(std::cin, std::cout) {}
		UI(std::istream& in, std::ostream&out) :input(in), output(out) {}
		~UI() {
			std::for_each(menus.begin(), menus.end(), [](auto& itr) {
				delete itr.second;
			});
		}

		Menu& createMenu(std::string handle, std::string name = "", std::string footer="") {
			if (name.empty()) name = handle;

			auto it = menus.find(handle);
			if (it == menus.end())
				menus[handle] = new Menu(handle, *this, name,footer);
			else
				throw ConsoleMenuException("Error creating menu, handle already present");
			return *menus[handle];
		}
		Menu& printMenu(std::string handle) {
			auto it = menus.find(handle);
			if (it != menus.end())
				it->second->print();
			else
				throw ConsoleMenuException("Menu with the given handle not found");

			return *it->second;
		}
		unsigned int getCommand(std::string menu_handle = "") {
			if (menu_handle.empty()) menu_handle = lastPrinted;
			if (menu_handle.empty()) throw ConsoleMenuException("Menu hasn't been printed yet, failed to provide new handle for print");

			auto isnum=[](std::string str)->bool {
				for (char&c : str)
					if (!isdigit(c))return false;
				return true;
			};
			std::string str;
			output << "-biram: ";	
			input >> str;
			if (str[0] == '?') {
				menus[menu_handle]->print();
				return getCommand(menu_handle);
			}else if (isnum(str) && menus[menu_handle]->checkId(std::stoi(str)))
				return std::stoi(str);
			else throw ConsoleMenuException("Non existant id");
		}
		bool active()const {
			return enabled;
		}
		void disable() { enabled = false; }
		std::string getCurrentMenuHandle() { return lastPrinted; }
	};
}
