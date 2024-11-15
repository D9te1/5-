#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>

const int Cell_Size = 30;
const int Count_Cell = 20;

using namespace sf;
using namespace std;

const int PLAYER_SYMBOL = 1;
const int BOT_SYMBOL = 2;

class Menu;
class Game;
class GameLogic;
class EndGame;
class Bot;
class ChooseTurn;

//начальное меню игры

class Menu {
	RenderWindow window;
	RectangleShape playButton;
	RectangleShape exitButton;
	Font font;
public:
	void RunMenu(Game& game);
};

// выбор очередности хода

class ChooseTurn {
	RenderWindow window;
	RectangleShape firstButton;
	RectangleShape secondButton;
	Font font;
public:
	bool isFirstPlayer;
	void RunChooseTurn() {
		if (!font.loadFromFile("arial.ttf")) {
			cerr << "Ошибка: Не удалось загрузить шрифт" << endl;
		}
		window.create(VideoMode(300, 200), L"Выбор хода");
		firstButton.setSize(Vector2f(160, 40));
		firstButton.setFillColor(Color::White);
		firstButton.setOutlineColor(Color::Black);
		firstButton.setOutlineThickness(2);
		firstButton.setPosition(70, 80);

		secondButton.setSize(Vector2f(160, 40));
		secondButton.setFillColor(Color::White);
		secondButton.setOutlineColor(Color::Black);
		secondButton.setOutlineThickness(2);
		secondButton.setPosition(70, 140);

		while (window.isOpen()) {
			Event event;
			while (window.pollEvent(event)) {
				if (event.type == Event::Closed)
					window.close();
				if (event.type == Event::MouseButtonPressed) {
					if (event.mouseButton.button == Mouse::Left) {
						Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
						if (firstButton.getGlobalBounds().contains(mousePos)) {
							isFirstPlayer = true;
							window.close();
							return;
						}
						else if (secondButton.getGlobalBounds().contains(mousePos)) {
							isFirstPlayer = false;
							window.close();
						}
					}
				}
			}
			window.clear(Color::White);
			window.draw(firstButton);

			Text firstText(L"Играть первым", font, 20);
			firstText.setFillColor(Color::Black);
			firstText.setPosition(80, 85);
			window.draw(firstText);

			window.draw(secondButton);
			Text secondText(L"Играть вторым", font, 20);
			secondText.setFillColor(Color::Black);
			secondText.setPosition(80, 145);
			window.draw(secondText);

			window.display();
		}
	}
};

// логика игры

class GameLogic {
	int size;
	int playerScore;
	int botScore;
public:
	vector<vector<int>> board;

	GameLogic(int size) : size(size) {
		board.resize(size, vector<int>(size, 0));
	}
	
	// ход игрок

	void addCircle(int x, int y) {
		board[y][x] = PLAYER_SYMBOL;
	}

	void incrementPlayerScore() {
		playerScore++;
	}

	void incrementBotScore() {
		botScore++;
	}

	int getPlayerScore() const {
		return playerScore;
	}

	int getBotScore() const {
		return botScore;
	}

	//проверка на победу

	bool checkWin(int player);

	//проверка на возможность сделать 4 в ряд

	bool IfHave3(int player);

	//проверка на возможность сделать 3 в ряд

	bool IfHave2(int player);

	//проверка на возможность сделать 2 в ряд

	bool IfHave1(int player);

	// очистка поля

	void reset() {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				board[i][j] = 0;
			}
		}
	}
};

class Bot {
	GameLogic& gameLogic;
	vector<CircleShape>& circles;

	// проверка на возможность выставить 2 в ряд

	bool Attack1() {
		return ToWin1(BOT_SYMBOL);
	}

	// проверка на возможность выставить 3 в ряд

	bool Attack2() {
		return ToWin2(BOT_SYMBOL);
	}

	// проверка на возможность выставить 4 в ряд

	bool Attack3() {
		return ToWin3(BOT_SYMBOL);
	}

	// проверка на возможность выиграть

	bool CanWin() {
		return ToWin4(BOT_SYMBOL);
	}

	// проверка на возможность заблокировать 2 в ряд

	bool Block1() {
		return ToWin1(PLAYER_SYMBOL);
	}

	// проверка на возможность заблокировать 3 в ряд

	bool Block2() {
		return ToWin2(PLAYER_SYMBOL);
	}

	// проверка на возможность заблокировать 4 в ряд

	bool Block3() {
		return ToWin3(PLAYER_SYMBOL);
	}

	// проверка на возможность заблокировать выигрыш

	bool BlockWin() {
		return ToWin4(PLAYER_SYMBOL);
	}


	bool ToWin1(int playerSymbol) {
		for (int y = 0; y < gameLogic.board.size(); y++) {
			for (int x = 0; x < gameLogic.board.size(); x++) {
				if (gameLogic.board[y][x] == 0) {
					gameLogic.board[y][x] = playerSymbol;
					if (gameLogic.IfHave1(playerSymbol)) {

						gameLogic.board[y][x] = 0;
						placeSymbol(x, y);
						return true;
					}
					gameLogic.board[y][x] = 0;
				}
			}
		}
		return false;
	}

	bool ToWin2(int playerSymbol) {
		for (int y = 0; y < gameLogic.board.size(); y++) {
			for (int x = 0; x < gameLogic.board.size(); x++) {
				if (gameLogic.board[y][x] == 0) {
					gameLogic.board[y][x] = playerSymbol;
					if (gameLogic.IfHave2(playerSymbol)) {
						gameLogic.board[y][x] = 0;
						placeSymbol(x, y);
						return true;
					}
					gameLogic.board[y][x] = 0;
				}
			}
		}
		return false;
	}

	bool ToWin3(int playerSymbol) {
		for (int y = 0; y < gameLogic.board.size(); y++) {
			for (int x = 0; x < gameLogic.board.size(); x++) {
				if (gameLogic.board[y][x] == 0) {
					gameLogic.board[y][x] = playerSymbol;
					if (gameLogic.IfHave3(playerSymbol)) {
						gameLogic.board[y][x] = 0;
						placeSymbol(x, y);
						return true;
					}
					gameLogic.board[y][x] = 0;
				}
			}
		}
		return false;
	}

	bool ToWin4(int playerSymbol) {
		for (int y = 0; y < gameLogic.board.size(); y++) {
			for (int x = 0; x < gameLogic.board.size(); x++) {
				if (gameLogic.board[y][x] == 0) {
					gameLogic.board[y][x] = playerSymbol;
					if (gameLogic.checkWin(playerSymbol)) {
						gameLogic.board[y][x] = 0;
						placeSymbol(x, y);
						return true;
					}
					gameLogic.board[y][x] = 0;
				}
			}
		}
		return false;
	}

	// ход бота с прорисовкой

	void placeSymbol(int x, int y) {
		gameLogic.board[y][x] = BOT_SYMBOL;
		CircleShape circle(Cell_Size / 2 - 2);
		circle.setFillColor(Color::Blue);
		circle.setPosition(x * Cell_Size + 1, y * Cell_Size + 1);
		circles.push_back(circle);
	}

	// первый ход для бота(ближе к центру)

	void FirstMove() {
		int centerX = gameLogic.board.size() / 2;
		int centerY = gameLogic.board.size() / 2;
		int bestX = -1;
		int bestY = -1;
		double minDistance = numeric_limits<double>::max();

		for (int y = 0; y < gameLogic.board.size(); y++) {
			for (int x = 0; x < gameLogic.board[y].size(); x++) {
				if (gameLogic.board[y][x] == 0) {
					double distance = sqrt(pow(x - centerX, 2) + pow(y - centerY, 2));
					if (distance < minDistance) {
						minDistance = distance;
						bestX = x;
						bestY = y;
					}
				}
			}
		}

		if (bestX != -1 && bestY != -1) {
			placeSymbol(bestX, bestY);
		}
	}
public:
	Bot(GameLogic& gameLogic, vector<CircleShape>& circles) :gameLogic(gameLogic), circles(circles) {}

	// выбор действия

	void makeMove() {
		if (CanWin()) {
			cout << "CanWin" << endl;
			return;//если бот может выиграть
		}
		if (BlockWin()) {
			cout << "BlockWin" << endl;
			return;//если игрок может выиграть
		}
		if (Attack3()) {
			cout << "Attack3" << endl;
			return;//атака если есть 3 подряд
		}
		if (Block3()) {
			cout << "Block3" << endl;
			return;//блок если есть 3 подряд
		}
		if (Attack2()) {
			cout << "Attack2" << endl;
			return;//атака если есть 2 подряд
		}
		if (Block2()) {
			cout << "Block2" << endl;
			return;//блок если есть 2 подряд
		}
		if (Attack1()) {
			cout << "Attack1" << endl;
			return;//ход рядом со своим
		}
		if (Block1()) {
			cout << "Block1" << endl;
			return;
		}
		FirstMove();
	}
};

class Game {
	RenderWindow window;
	vector<CircleShape> circles;
	GameLogic gamelogic;
	Bot bot;
	bool isPlayerFirst;
	bool isPlayerTurn;
	RectangleShape sidebar;
	RectangleShape exitButton;
	RectangleShape restartButton;
	Font font;
public:
	Game() : gamelogic(Count_Cell), bot(gamelogic, circles) {
		if (!font.loadFromFile("arial.ttf")) {
			cerr << "Ошибка: Не удалось загрузить шрифт" << endl;
		}

		sidebar.setSize(Vector2f(50, 600));
		sidebar.setFillColor(Color::White);
		sidebar.setPosition(700, 0);

		exitButton.setSize(Vector2f(130, 40));
		exitButton.setFillColor(Color::White);
		exitButton.setOutlineColor(Color::Black);
		exitButton.setOutlineThickness(2);
		exitButton.setPosition(620, 520);

		restartButton.setSize(Vector2f(130, 40));
		restartButton.setFillColor(Color::White);
		restartButton.setOutlineColor(Color::Black);
		restartButton.setOutlineThickness(2);
		restartButton.setPosition(620, 470);
		isPlayerFirst = true;
	}

	void setFirstPlayer(bool isFirst) {
		isPlayerFirst = isFirst;
		isPlayerTurn = isPlayerFirst;
	}
	// возвращение к начальному состоянию
	void resetGame() {
		gamelogic.reset();
		circles.clear();
		isPlayerTurn = isPlayerFirst;
	}

	void RunGame();
};

// окно победы

class EndGame {
	RenderWindow window;
	RectangleShape restartButton;
	RectangleShape exitButton;
	Font font;
	Text message;
public:
	bool shouldRestart = false;
	EndGame(const wstring& winner) {
		if (!font.loadFromFile("arial.ttf")) {
			cout << "Ошибка: Не удалось загрузить шрифт" << endl;
		}
		window.create(VideoMode(300, 200), L"Конец игры");

		message.setFont(font);
		message.setString(winner);
		message.setCharacterSize(24);
		message.setFillColor(Color::Black);
		message.setPosition(50, 30);

		restartButton.setSize(Vector2f(100, 40));
		restartButton.setFillColor(Color::White);
		restartButton.setOutlineColor(Color::Black);
		restartButton.setOutlineThickness(2);
		restartButton.setPosition(100, 80);

		exitButton.setSize(Vector2f(100, 40));
		exitButton.setFillColor(Color::White);
		exitButton.setOutlineColor(Color::Black);
		exitButton.setOutlineThickness(2);
		exitButton.setPosition(100, 140);
	}

	void RunEndGame(Game& game) {
		while (window.isOpen()) {
			Event event;
			while (window.pollEvent(event)) {
				if (event.type == Event::Closed)
					window.close();

				if (event.type == Event::MouseButtonPressed) {
					if (event.mouseButton.button == Mouse::Left) {
						Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
						if (restartButton.getGlobalBounds().contains(mousePos)) {
							shouldRestart = true;
							window.close();
							return;
						}
						else if (exitButton.getGlobalBounds().contains(mousePos)) {
							window.close();
						}
					}
				}
			}

			window.clear(Color::White);
			window.draw(message);
			window.draw(restartButton);
			window.draw(exitButton);

			Text restartText(L"Заново", font, 20);
			restartText.setFillColor(Color::Black);
			restartText.setPosition(120, 85);
			window.draw(restartText);

			Text exitText(L"Выход", font, 20);
			exitText.setFillColor(Color::Black);
			exitText.setPosition(120, 145);
			window.draw(exitText);
			window.display();
		}
	}
};

void Menu::RunMenu(Game& game) {
	if (!font.loadFromFile("arial.ttf")) {
		cerr << "Ошибка: Не удалось загрузить шрифт arial.ttf" << endl;
	}
	window.create(VideoMode(300, 200), L"Меню");
	playButton.setSize(Vector2f(100, 40));
	playButton.setFillColor(Color::White);
	playButton.setOutlineColor(Color::Black);
	playButton.setOutlineThickness(2);
	playButton.setPosition(100, 80);

	exitButton.setSize(Vector2f(100, 40));
	exitButton.setFillColor(Color::White);
	exitButton.setOutlineColor(Color::Black);
	exitButton.setOutlineThickness(2);
	exitButton.setPosition(100, 140);

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::MouseButtonPressed) {
				if (event.mouseButton.button == Mouse::Left) {
					if (playButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
						window.close();
						ChooseTurn chooseTurn;
						chooseTurn.RunChooseTurn();
						game.setFirstPlayer(chooseTurn.isFirstPlayer);
						game.RunGame();
					}
					if (exitButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
						window.close();
				}
			}
		}
		window.clear(Color::White);
		window.draw(playButton);
		Text playText(L"Играть", font, 20);
		playText.setFillColor(Color::Black);
		playText.setPosition(120, 85);
		window.draw(playText);
		window.draw(exitButton);
		Text exitText(L"Выход", font, 20);
		exitText.setFillColor(Color::Black);
		exitText.setPosition(120, 145);
		window.draw(exitText);
		window.display();
	}
}

void Game::RunGame() {
	window.create(VideoMode(800, 600), L"5 в ряд");
	// сетка поля
	VertexArray grid(Lines);

	for (int i = 0; i <= Count_Cell; ++i) {
		grid.append(sf::Vertex(sf::Vector2f(i * Cell_Size, 0), sf::Color::Black));
		grid.append(sf::Vertex(sf::Vector2f(i * Cell_Size, Cell_Size * Count_Cell), sf::Color::Black));

		grid.append(sf::Vertex(sf::Vector2f(0, i * Cell_Size), sf::Color::Black));
		grid.append(sf::Vertex(sf::Vector2f(Cell_Size * Count_Cell, i * Cell_Size), sf::Color::Black));
	}

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::MouseButtonPressed) {
				if (event.mouseButton.button == Mouse::Left) {
					Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
					if (exitButton.getGlobalBounds().contains(mousePos)) {
						window.close();
					}
					else if (restartButton.getGlobalBounds().contains(mousePos)) {
						resetGame();
					}
					else if (mousePos.x >= 0 && mousePos.x < Cell_Size * Count_Cell &&
						mousePos.y >= 0 && mousePos.y < Cell_Size * Count_Cell) {
						// ход игрока
						if (isPlayerTurn) {
							int cellX = static_cast<int>(mousePos.x) / Cell_Size;
							int cellY = static_cast<int>(mousePos.y) / Cell_Size;
							if (gamelogic.board[cellY][cellX] == 0) {
								CircleShape circle(Cell_Size / 2 - 2);
								circle.setFillColor(Color::Red);
								circle.setPosition(cellX * Cell_Size + 1, cellY * Cell_Size + 1);
								circles.push_back(circle);
								gamelogic.addCircle(cellX, cellY);
								isPlayerTurn = false;
								window.clear(Color::White);
								window.draw(grid);
								for (const auto& circle : circles) {
									window.draw(circle);
								}
								window.draw(sidebar);
								window.draw(restartButton);
								window.draw(exitButton);
								window.display();
								if (gamelogic.checkWin(PLAYER_SYMBOL)) {
									gamelogic.incrementPlayerScore();
									EndGame endGame(L"Игрок 1 выиграл!");
									endGame.RunEndGame(*this);
									if (endGame.shouldRestart) {
										resetGame();
									}
									else {
										window.close();
									}
								}
							}
						}
					}
				}
			}
			// ход бота
			if (!isPlayerTurn) {
				bot.makeMove();
				window.clear(Color::White);
				window.draw(grid);
				for (const auto& circle : circles) {
					window.draw(circle);
				}
				window.draw(sidebar);
				window.draw(restartButton);
				window.draw(exitButton);
				window.display();
				if (gamelogic.checkWin(BOT_SYMBOL)) {
					gamelogic.incrementBotScore();
					EndGame endGame(L"Бот выиграл!");
					endGame.RunEndGame(*this);
					if (endGame.shouldRestart) {
						resetGame();
					}
					else {
						window.close();
					}
				}
				isPlayerTurn = true;
			}
		}
		window.clear(Color::White);
		window.draw(grid);
		for (const auto& circle : circles) {
			window.draw(circle);
		}
		window.draw(sidebar);
		window.draw(restartButton);
		window.draw(exitButton);

		Text playerScoreText;
		playerScoreText.setFont(font);
		playerScoreText.setCharacterSize(20);
		playerScoreText.setFillColor(Color::Black);
		playerScoreText.setString(L"Счет Игрока: " + to_wstring(gamelogic.getPlayerScore()));
		playerScoreText.setPosition(610, 50);
		window.draw(playerScoreText);

		Text botScoreText;
		botScoreText.setFont(font);
		botScoreText.setCharacterSize(20);
		botScoreText.setFillColor(Color::Black);
		botScoreText.setString(L"Счет Бота: " + to_wstring(gamelogic.getBotScore()));
		botScoreText.setPosition(610, 80);
		window.draw(botScoreText);

		Text restartText(L"Заново", font, 20);
		restartText.setFillColor(Color::Black);
		restartText.setPosition(645, 475);
		window.draw(restartText);

		Text exitText(L"Выход", font, 20);
		exitText.setFillColor(Color::Black);
		exitText.setPosition(645, 525);
		window.draw(exitText);
		window.display();
	}
}

// проверка на победу

bool GameLogic::checkWin(int player) {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (board[i][j] == player) {
				if (j + 4 < size && board[i][j + 1] == player &&
					board[i][j + 2] == player &&
					board[i][j + 3] == player &&
					board[i][j + 4] == player) {
					return true;
				}
				if (i + 4 < size && board[i + 1][j] == player &&
					board[i + 2][j] == player &&
					board[i + 3][j] == player &&
					board[i + 4][j] == player) {
					return true;
				}
				if (i + 4 < size && j + 4 < size &&
					board[i + 1][j + 1] == player &&
					board[i + 2][j + 2] == player &&
					board[i + 3][j + 3] == player &&
					board[i + 4][j + 4] == player) {
					return true;
				}
				if (i + 4 < size && j - 4 >= 0 &&
					board[i + 1][j - 1] == player &&
					board[i + 2][j - 2] == player &&
					board[i + 3][j - 3] == player &&
					board[i + 4][j - 4] == player) {
					return true;
				}
			}
		}
	}
	return false;
}

//проверка на возможность сделать 4 в ряд

bool GameLogic::IfHave3(int player) {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (board[i][j] == player) {
				//вертикаль
				if (j + 4 < size && board[i][j + 1] == player &&
					board[i][j + 2] == player &&
					board[i][j + 3] == player &&
					board[i][j + 4] == 0) {
					return true;
				}
				if (j + 3 < size && j - 1 >= 0 && board[i][j + 1] == player &&
					board[i][j + 2] == player &&
					board[i][j + 3] == player &&
					board[i][j - 1] == 0) {
					return true;
				}
				//горизонталь
				if (i + 4 < size && board[i + 1][j] == player &&
					board[i + 2][j] == player &&
					board[i + 3][j] == player &&
					board[i + 4][j] == 0) {
					return true;
				}
				if (i + 3 < size && i - 1 >= 0 && board[i + 1][j] == player &&
					board[i + 2][j] == player &&
					board[i + 3][j] == player &&
					board[i - 1][j] == 0) {
					return true;
				}
				//диагональ вправо вниз
				if (i + 4 < size && j + 4 < size &&
					board[i + 1][j + 1] == player &&
					board[i + 2][j + 2] == player &&
					board[i + 3][j + 3] == player &&
					board[i + 4][j + 4] == 0) {
					return true;
				}
				if (i + 3 < size && j + 3 < size && i - 1 >= 0 && j - 1 >= 0 &&
					board[i + 1][j + 1] == player &&
					board[i + 2][j + 2] == player &&
					board[i + 3][j + 3] == player &&
					board[i - 1][j - 1] == 0) {
					return true;
				}
				//диагональ влево вниз
				if (i - 4 >= 0 && j + 4 < size &&
					board[i - 1][j + 1] == player &&
					board[i - 2][j + 2] == player &&
					board[i - 3][j + 3] == player &&
					board[i - 4][j + 4] == 0) {
					return true;
				}
				if (i - 3 >= 0 && j + 3 < size && i + 1 < size && j - 1 >= 0 &&
					board[i - 1][j + 1] == player &&
					board[i - 2][j + 2] == player &&
					board[i - 3][j + 3] == player &&
					board[i + 1][j - 1] == 0) {
					return true;
				}
			}
		}
	}
	return false;
}

//проверка на возможность сделать 3 в ряд

bool GameLogic::IfHave2(int player) {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (board[i][j] == player) {
				//вертикаль
				if (j + 4 < size && board[i][j + 1] == player &&
					board[i][j + 2] == player &&
					(board[i][j + 3] == 0 && board[i][j + 4] == 0)) {
					return true;
				}
				if (j + 3 < size && j - 2 >= 0 && board[i][j + 1] == player &&
					board[i][j + 2] == player &&
					((board[i][j - 1] == 0 && board[i][j - 2] == 0) ||
						(board[i][j - 1] == 0 && board[i][j + 3] == 0))) {
					return true;
				}
				// горизонталь
				if (i + 4 < size && board[i + 1][j] == player &&
					board[i + 2][j] == player &&
					(board[i + 3][j] == 0 && board[i + 4][j] == 0)) {
					return true;
				}
				if (i + 3 < size && i - 2 >= 0 && board[i + 1][j] == player &&
					board[i + 2][j] == player &&
					((board[i - 1][j] == 0 && board[i - 2][j] == 0) ||
						(board[i - 1][j] == 0 && board[i + 3][j] == 0))) {
					return true;
				}
				//диагональ вправо вниз
				if (i + 4 < size && j + 4 < size &&
					board[i + 1][j + 1] == player &&
					board[i + 2][j + 2] == player &&
					(board[i + 3][j + 3] == 0 && board[i + 4][j + 4] == 0)) {
					return true;
				}
				if (i - 2 >= 0 && j - 2 >= 0 && i + 3 < size && j + 3 < size &&
					board[i + 1][j + 1] == player &&
					board[i + 2][j + 2] == player &&
					((board[i + 3][j + 3] == 0 && board[i - 1][j - 1] == 0) ||
						(board[i - 1][j - 1] == 0 && board[i - 2][j - 2] == 0))) {
					return true;
				}
				//диагональ влево вниз
				if (i - 4 >= 0 && j + 4 < size &&
					board[i - 1][j + 1] == player &&
					board[i - 2][j + 2] == player &&
					(board[i - 3][j + 3] == 0 && board[i - 4][j + 4] == 0)) {
					return true;
				}
				if (i + 2 < size && j - 2 >= 0 && i - 3 >= 0 && j + 3 < size &&
					board[i - 1][j + 1] == player &&
					board[i - 2][j + 2] == player &&
					((board[i - 3][j + 3] == 0 && board[i + 1][j - 1] == 0) ||
						(board[i + 1][j - 1] == 0 && board[i + 2][j - 2] == 0))) {
					return true;
				}
			}
		}
	}
	return false;
}

//проверка на возможность сделать 2 в ряд

bool GameLogic::IfHave1(int player) {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (board[i][j] == player) {
				//вертикаль
				if (j + 4 < size && board[i][j + 1] == player &&
					board[i][j + 2] == 0 &&
					board[i][j + 3] == 0 &&
					board[i][j + 4] == 0) {
					return true;
				}
				if (j + 3 < size && j - 3 >= 0 && board[i][j + 1] == player &&
					((board[i][j + 2] == 0 && board[i][j + 3] == 0 && board[i][j - 1] == 0)
						|| (board[i][j + 2] == 0 && board[i][j - 2] == 0 && board[i][j - 1] == 0)
						|| (board[i][j - 3] == 0 && board[i][j - 2] == 0 && board[i][j - 1] == 0))) {
					return true;
				}
				//горизонталь
				if (i + 4 < size && board[i + 1][j] == player &&
					board[i + 2][j] == 0 &&
					board[i + 3][j] == 0 &&
					board[i + 4][j] == 0) {
					return true;
				}
				if (i + 3 < size && i - 3 >= 0 && board[i + 1][j] == player &&
					((board[i + 2][j] == 0 && board[i + 3][j] == 0 && board[i - 1][j] == 0)
						|| (board[i + 2][j] == 0 && board[i - 2][j] == 0 && board[i - 1][j] == 0)
						|| (board[i - 3][j] == 0 && board[i - 2][j] == 0 && board[i - 1][j] == 0))) {
					return true;
				}
				//диагональ вправо вниз
				if (i + 4 < size && j + 4 < size &&
					board[i + 1][j + 1] == player &&
					board[i + 2][j + 2] == 0 &&
					board[i + 3][j + 3] == 0 &&
					board[i + 4][j + 4] == 0) {
					return true;
				}
				if (i + 3 < size && j + 3 < size && i - 3 >= 0 && j - 3 >= 0 &&
					board[i + 1][j + 1] == player &&
					((board[i + 2][j + 2] == 0 && board[i + 3][j + 3] == 0 && board[i - 1][j - 1] == 0)
						|| (board[i + 2][j + 2] == 0 && board[i - 2][j - 2] == 0 && board[i - 1][j - 1] == 0)
						|| (board[i - 3][j - 3] == 0 && board[i - 2][j - 2] == 0 && board[i - 1][j - 1] == 0))) {
					return true;
				}
				//даигональ влево вниз
				if (i - 4 >= 0 && j + 4 < size &&
					board[i - 1][j + 1] == player &&
					board[i - 2][j + 2] == 0 &&
					board[i - 3][j + 3] == 0 &&
					board[i - 4][j + 4] == 0) {
					return true;
				}
				if (i - 3 >= 0 && j + 3 < size && i + 3 < size && j - 3 >= 0 &&
					board[i - 1][j + 1] == player &&
					((board[i - 2][j + 2] == 0 && board[i - 3][j + 3] == 0 && board[i + 1][j - 1] == 0)
						|| (board[i - 2][j + 2] == 0 && board[i + 2][j - 2] == 0 && board[i + 1][j - 1] == 0)
						|| (board[i + 3][j - 3] == 0 && board[i + 2][j - 2] == 0 && board[i + 1][j - 1] == 0))) {
					return true;
				}
			}
		}
	}
	return false;
}

int main()
{
	Game game;
	Menu menu;
	menu.RunMenu(game);
	return 0;
}