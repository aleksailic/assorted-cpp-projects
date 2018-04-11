#include "ConsoleMenu.h"
#include <vector>
#include <random>
#include <algorithm>
#include <stack>
#include <iomanip>
#include <time.h>

#ifndef CLEARCONSOLE()
#ifdef __unix__  
#define CLEARCONSOLE() system("clear")
#elif defined(_WIN32) || defined(WIN32)
#define CLEARCONSOLE() system("cls")
#endif
#endif // !CLEARCONSOLE()

class Game {
public:
	enum STATUS_FLAG {
		INACTIVE = 0x0,
		ACTIVE = 0x1,
		WIN = 0x2,
		LOSE = 0x4
	};
private:
	enum FIELD_FLAG {
		VISIBLE=0x10,
		NUM=0x20,
		FLAG=0x40,
		MINE=0x80
	};
	std::vector<std::vector<uint8_t>>matrix;
	uint8_t rows;
	uint8_t cols;

	time_t start_time;
	uint8_t mine_counter;
	uint8_t status;

	template<typename T>
	std::vector<std::pair<T,T>> cartesian_product(std::initializer_list<T>groupA, std::initializer_list<T>groupB) {
		std::vector<std::pair<T, T>> product;
		for (auto& elA : groupA)
			for (auto& elB : groupB)
				product.push_back(std::make_pair(elA, elB));
		return product;
	}
	bool inBounds(int x, int y) {
		if (x < 0 || x >= cols) return false;
		if (y < 0 || y >= rows) return false;
		return true;
	}
	void crawlFrom(uint8_t x, uint8_t y) {
		struct info {
			uint8_t x;
			uint8_t y;
			uint8_t offset;
			info(uint8_t xt, uint8_t yt) :x(xt), y(yt),offset(0) {}
		};
		std::pair<uint8_t, uint8_t>offsets[4] = { {1,0},{0,-1},{0,1},{-1,0} };
		std::stack<info> stack;
		stack.push(info(x,y));
		while (!stack.empty()) {
			info& el = stack.top();
			uint8_t x = el.x + offsets[el.offset].first;
			uint8_t y = el.y + offsets[el.offset].second;
			if (++el.offset == sizeof(offsets)/sizeof(std::pair<uint8_t,uint8_t>)) //ovaj je odsviro svoje
				stack.pop();
			if (inBounds(x, y) && !(matrix[y][x] & VISIBLE)) {
				matrix[y][x] &= ~FLAG; //disable flag once picked
				matrix[y][x] |= VISIBLE;
				if(!(matrix[y][x] & NUM))
					stack.push(info(x, y));
			}
		}
	}
	public:
		Game(uint8_t rows = 8, uint8_t cols = 8, uint8_t mine_number = 8){
			generate(rows, cols,mine_number);
		}
		void generate(uint8_t rows, uint8_t cols, uint8_t mine_number) {
			matrix.clear();
			this->rows = rows;
			this->cols = cols;
			mine_counter = mine_number;
			matrix.resize(cols);
			for (auto& arr : matrix)
				arr.resize(rows);

			std::random_device rd; 
			std::mt19937 gen(rd()); 
			std::uniform_int_distribution<> dis(0,rows*cols-1); //genereate random mines

			auto inVec = [](std::vector<int>& vec,int& el)->bool{
				return std::find(vec.begin(), vec.end(), el) != vec.end() ? true : false;
			};

			//populate with mines
			std::vector<int>mines;
			for (uint8_t i = 0; i < mine_number; i++) {
				int val = dis(gen);
				while (inVec(mines, val))val = dis(gen);
				mines.push_back(val);
				matrix[val / cols][val%cols] = MINE;
			}
			std::vector<std::pair<int,int>> offsets = cartesian_product({ -1,0,1 }, { -1,0,1 }); //get offset of all surroundings
			offsets.erase(std::remove(offsets.begin(), offsets.end(), std::make_pair(0,0)), offsets.end()); //remove 0,0 from offsets
			//count mines
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++) {
					if (matrix[i][j] & MINE) continue;
					int count = 0;
					for (auto& offset : offsets) {
						int x = j + offset.first;
						int y = i + offset.second;
						if (inBounds(x, y))
							if (matrix[y][x] & MINE)count++;
					}
					matrix[i][j] |= count;		
					if (count)
						matrix[i][j] |= NUM;
				}
			}
		}
		//print in termnal
		void print() { 
			CLEARCONSOLE();
			//get fill offset which is number of digits
			int off = 1;
			for (int num = cols; num >= 1; num/= 10, off++);

			std::cout << std::setfill(' ') << std::setw(2*off);
			for (int i = 0; i < cols; i++)
				std::cout << i << std::setw(off);
			std::cout << std::endl;
			for (int i = 0; i < cols;i++) {
				std::cout << std::setw(off) << i << std::setw(off);
				for (auto&el : matrix[i]) {
					if (el&VISIBLE) {
						if (el & MINE)
							std::cout << '#';
						else if (el & NUM)
							std::cout << (int)(el & 0xF);
						else
							std::cout << ' ';
					}
					else if (el & FLAG)
						std::cout << 'F';
					else
						std::cout << '.';
					std::cout << std::setw(off);
				}
				std::cout << std::endl;		
			}
		}
		void pick(uint8_t x, uint8_t y) {
			matrix[y][x] &= ~FLAG; //disable flag once picked
			matrix[y][x] |= VISIBLE; //make the field visible once picked
			if (!(matrix[y][x] & (MINE + NUM))) //if on blank spot
				crawlFrom(x, y);

			if (matrix[y][x] & MINE) { //game over if stepped on mine
				status |= LOSE;
				status &= ~ACTIVE;
			}
		}
		void flag(uint8_t x, uint8_t y) {
			if (!(matrix[y][x] & VISIBLE))
				matrix[y][x] |= FLAG;
		}
		void run() {
			start_time = std::time(0);
			status |= ACTIVE;
			while (status & ACTIVE) {
				print();
				std::cout << "-input (f/p y x): ";
				int x, y;
				char fp;
				do {
					std::cin >> fp >> y >> x;
				} while (!inBounds(x, y) || (fp!='f' && fp!='p'));
				if (fp == 'p')
					pick(x, y);
				else if (fp == 'f')
					flag(x, y);
			}
		}
		uint8_t getStatus()const { return status; }
};

int main() {
	ConsoleMenu::UI ui;
	enum main_menu_handles {
		START = 1,
		HIGHSCORES,
		EXIT,
	};
	enum game_handles {
		BEGINNER=1,
		ADVANCED,
		MASTER,
		CUSTOM
	};

	auto& main_menu = ui.createMenu("main_menu", "Minesweper")
		.createOption("Start", START)
		.createOption("High scores", HIGHSCORES)
		.createOption("Exit", EXIT)
		.print();
	auto& game_menu = ui.createMenu("game_menu", "Select game")
		.createOption("Beginner (8x8)", BEGINNER)
		.createOption("Advanced (14x14)", ADVANCED)
		.createOption("Master (20x20)", MASTER)
		.createOption("Custom", CUSTOM);

	std::string buffer;
	Game * game = new Game();
	while (ui.active()) {
		try {
			buffer.erase();
			if(ui.getCurrentMenuHandle() == "main_menu")
				switch (ui.getCommand()) {
				case START:
					ui.printMenu("game_menu");
					break;
				case HIGHSCORES:
				
					break;
				case EXIT:
					ui.disable();
					break;
				}
			else if (ui.getCurrentMenuHandle() == "game_menu") {
				switch (ui.getCommand()) {
				case BEGINNER:
					game->generate(8, 8, 8);
					break;
				case ADVANCED:
					game->generate(14,14,14);
					break;
				case MASTER:
					game->generate(20,20,20);
					break;
				case CUSTOM:
					std::cout << "-unesi (rows cols mine_number):";
					int rows, cols, mine_number;
					std::cin >> rows >> cols >> mine_number;
					game->generate(rows, cols, mine_number);
					break;
				}
				game->run();
				if (game->getStatus() & Game::WIN)
					std::cout << "BRAVO MACAK, CESTITAMO!" << std::endl;
				else
					std::cout << "GAME OVER! YOU LOSE." << std::endl;

				if (!ui.prompt("Play again?"))
					ui.disable();
				else
					ui.printMenu("main_menu");
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
	delete game;
	return 0;
}
