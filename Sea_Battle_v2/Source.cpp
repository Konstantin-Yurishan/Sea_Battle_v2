/*/*Первостепенное*/
//заполнение поля двумя игроками
//для каждого игрока в его очередь отображается его поле, и поле противника с туманом войны
//красивое оформление полей - добавление цветов, разметки
//навигация по чужому полю для выстрела
//произведение выстрела и его последствия, вывод сообщений о попаданиях и промахах
//произведение выстрела противником с лёгким ии
//кондиции завершения игры
//произведение выстрела противником с сложным ии
//специальные кнопки для досрочного завершения игры и выхода в главное меню
//разборка с освобождением памяти
//вычищение мусора из кода и комментирование
//оптимизация и сокращение кода

/*второстепенное*/
//	интро
//добавление интро с перелювающейся и выплывающей надписью
//добавление музыки для интро
//прерывание интро
//
//	меню
//ввод именю игрока -> возможность выбора "рандомного" для PC (тип Железный Ублюдок) 
//кнопка "назад" для меню
//выбор пунктов меню с помощью стрелок на клавиатуре 
//подсветка пунктов меню при выборе + при выборе появление стрелки влева "->"
//звук перемещения по пунктам меню
// 
//	звук
//добавить звуковые эффекты при попадании  
// 
//	загрузка
//загрузка перед каждым запуском игры))
//звуковой эффект загрузки - https://www.youtube.com/watch?v=3SJ9ts-Nxfs 1.42   звук работы флопи-дисковода с диском
// 
//	интерфейс
//вывод статистики во время игры - оставшееся количество кораблей у себя, у противника, количество целых, подбитых
//кнопки для выхода и кнопка назад 
//ограничение на длину имени игрока
//
//	сохранение и загрузка
// реализация возможности сохранения и загрузки  
// 
//	разное
//передача всего и вся по указателю
//
//
/**/


#include <iostream>
#include <string>
#include <Windows.h>
#include <thread>
#include <conio.h>
#pragma comment(lib, "winmm.lib")


struct ship {

	int size;
	int** decCoord;
	bool* decStat;
	short genStat;

};


struct playerField {

	std::string playerName = "";
	ship* ship1 = new ship[4];
	ship* ship2 = new ship[3];
	ship* ship3 = new ship[2];
	ship* ship4 = new ship[1];

	char** field;

};

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


playerField createPlayerField(); //создаёт экземпляр структуры, и заполняет в ней поле field
bool checkArea(playerField*, int, int); //проверяет окресности переданной точки на переданном поле
void fillFieldAutomatic(playerField*);
void fillFieldManual(playerField*);
void showField(playerField);
void setCursorPos(int, int);
void showFields(playerField*, playerField*);
void makeShoot(playerField*, playerField*);
bool shootChecker(playerField*, int, int);
//void game(); //входят функции intro, mainMenu
void mainMenu(playerField*, playerField*);
void consoleSize(); //задаёт размер окна
bool terminator_1(playerField*);
bool terminator_2(playerField*);
void startGame(playerField*, playerField*);
bool shooter(playerField*, playerField*); //представляем навигацию по вражескому полю
bool winChecker(playerField*, playerField*);
void accountant(playerField*, int, int); // запись информации по выстрелу


int main()
{

	srand(time(NULL));
	consoleSize();

	//скрывает мигающий курсор
	void* handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(handle, &structCursorInfo);
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &structCursorInfo);


	playerField field_1 = createPlayerField();
	playerField* field_1_ptr = &field_1;
	playerField field_2 = createPlayerField();
	playerField* field_2_ptr = &field_2;


	mainMenu(field_1_ptr, field_2_ptr);
	showFields(field_1_ptr, field_2_ptr);

	//fillFieldAutomatic(field_1_ptr);
	//fillFieldAutomatic(field_2_ptr);
	//showField(field);
	//fillFieldManual(field_1_ptr);

	//makeShoot(field_1_ptr, field_2_ptr);

	return 0;
}

playerField createPlayerField()
{
	playerField newField;

	newField.field = new char* [10];

	for (int i = 0; i < 10; i++) {
		newField.field[i] = new char[10];
	}

	for (int i = 0; i < 10; i++) {
		for (int c = 0; c < 10; c++) {
			newField.field[i][c] = '~';
		}
	}

	return newField;
}

bool checkArea(playerField* field, int X, int Y)
{
	for (int i = X - 1; i <= X + 1; i++) {
		for (int c = Y - 1; c <= Y + 1; c++) {

			if (i < 0 || c < 0 || i > 9 || c > 9 || field->field[i][c] == '~' || field->field[i][c] != '#') {
				continue;
			}
			else {
				return false;
			}
		}
	}
	return true;
}

void fillFieldManual(playerField* field)
{
	system("CLS");

	int coordX, coordY, shipNum, coordX2, coordY2, coordX3, coordY3, coordX4, coordY4;
	char** tempField = field->field;
	char entering;
	bool rota = true;
	enum { UP = 72, DOWN = 80, RIGHT = 77, LEFT = 75, ENTER = '\r', ROTATION = 'r' };


	//однопалубные
	shipNum = 0;
	coordX = 0;
	coordY = 0;
	
	showField(*field);
	setCursorPos(0, 0);
	

	while (shipNum < 4) {

		std::cout << "Place the ship" << field->playerName << std::endl;
		std::cout << "*";
		entering = _getch();

		if (entering == UP) {
			system("CLS");
			if (coordX > 0) {
				coordX -= 1;
			}
			showField(*field);
			setCursorPos(coordX, coordY * 2);
			std::cout << '*';
			setCursorPos(20, 20);
			std::cout << "coordX = " << coordX << " coordY = " << coordY << std::endl;

		}
		else if (entering == DOWN) {
			system("CLS");
			if (coordX < 9) {
				coordX += 1;
			}
			showField(*field);
			setCursorPos(coordX, coordY * 2);
			std::cout << '*';
			setCursorPos(20, 20);
			std::cout << "coordX = " << coordX << " coordY = " << coordY << std::endl;
		}
		else if (entering == RIGHT) {
			system("CLS");
			if (coordY < 9) {
				coordY += 1;
			}
			showField(*field);
			setCursorPos(coordX, coordY * 2);
			std::cout << '*';
			setCursorPos(20, 20);
			std::cout << "coordX = " << coordX << " coordY = " << coordY << std::endl;
		}
		else if (entering == LEFT) {
			system("CLS");
			if (coordY > 0) {
				coordY -= 1;
			}
			showField(*field);
			setCursorPos(coordX, coordY * 2);
			std::cout << '*';
			setCursorPos(20, 20);
			std::cout << "coordX = " << coordX << " coordY = " << coordY << std::endl;
		}
		else if (entering == ENTER) {
			system("CLS");

			if (checkArea(field, coordX, coordY)) {
				field->field[coordX][coordY] = '#';

				field->ship1[shipNum].size = 1;
				field->ship1[shipNum].decCoord = new int* [1];
				field->ship1[shipNum].decCoord[0] = new int[2];
				field->ship1[shipNum].decCoord[0][0] = coordX;
				field->ship1[shipNum].decCoord[0][1] = coordY;
				field->ship1[shipNum].decStat = new bool[1];
				field->ship1[shipNum].decStat[0] = true;
				field->ship1[shipNum].genStat = 1;
				shipNum++;
				showField(*field);
			}
			else {
				showField(*field);
				std::cout << "Wrong position!";
			}
		}
	}


	//двухпалубные
	shipNum = 0;
	coordX = 0;
	coordY = 0;

	while (shipNum < 3) {

		entering = _getch();

		if (entering == UP) {
			system("CLS");

			if (coordX > 0) {
				coordX -= 1;
			}

			showField(*field);
			if (rota) {

				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 2);
				std::cout << "*";
			}
			else {
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY * 2);
				std::cout << "*";
			}
			setCursorPos(20, 20);
			std::cout << "coordX = " << coordX << " coordY = " << coordY << std::endl;

		}
		else if (entering == DOWN) {
			system("CLS");



			showField(*field);

			if (rota) {
				if (coordX < 9) {
					coordX += 1;
				}
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 2);
				std::cout << "*";
			}
			else {
				if (coordX < 8) {
					coordX += 1;
				}
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY * 2);
				std::cout << "*";
			}

			setCursorPos(20, 20);

			std::cout << "coordX = " << coordX << " coordY = " << coordY << std::endl;

		}
		else if (entering == RIGHT) {
			system("CLS");



			showField(*field);
			if (rota) {
				if (coordY < 8) {
					coordY += 1;
				}
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 2);
				std::cout << "*";
			}
			else {
				if (coordY < 9) {
					coordY += 1;
				}
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY * 2);
				std::cout << "*";
			}
			setCursorPos(20, 20);
			std::cout << "coordX = " << coordX << " coordY = " << coordY << std::endl;
		}
		else if (entering == LEFT) {
			system("CLS");

			if (coordY > 0) {
				coordY -= 1;
			}

			showField(*field);

			if (rota) {
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 2);
				std::cout << "*";
			}
			else {
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY * 2);
				std::cout << "*";
			}
			setCursorPos(20, 20);
			std::cout << "coordX = " << coordX << " coordY = " << coordY << std::endl;
		}
		else if (entering == ENTER) {
			system("CLS");



			if (rota) {
				coordX2 = coordX;
				coordY2 = coordY + 1;
			}
			else {
				coordX2 = coordX + 1;
				coordY2 = coordY;
			}

			if (checkArea(field, coordX, coordY) && checkArea(field, coordX2, coordY2)) {
				field->field[coordX2][coordY2] = '#';
				field->field[coordX][coordY] = '#';

				field->ship2[shipNum].size = 2;
				field->ship2[shipNum].decCoord = new int* [2];
				field->ship2[shipNum].decCoord[0] = new int[2];
				field->ship2[shipNum].decCoord[1] = new int[2];
				field->ship2[shipNum].decCoord[0][0] = coordX2;
				field->ship2[shipNum].decCoord[0][1] = coordX2;
				field->ship2[shipNum].decCoord[1][0] = coordX;
				field->ship2[shipNum].decCoord[1][1] = coordY;
				field->ship2[shipNum].decStat = new bool[2];
				field->ship2[shipNum].decStat[0] = true;
				field->ship2[shipNum].decStat[1] = true;
				field->ship2[shipNum].genStat = 1;
				shipNum++;
				showField(*field);
			}
			else {
				showField(*field);
				std::cout << "Wrong position!";
			}
		}
		else if (entering == ROTATION) {

			if (coordX + 1 > 9 || coordY * 2 + 2 > 18) {
				continue;
			}

			system("CLS");

			showField(*field);

			rota = !rota;

			if (rota) {
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 2);
				std::cout << "*";
			}
			else {
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY * 2);
				std::cout << "*";
			}

			setCursorPos(20, 20);

		}
	}

	//трёхпалубные
	shipNum = 0;
	coordX = 0;
	coordY = 0;

	while (shipNum < 2) {

		entering = _getch();

		if (entering == UP) {

			system("CLS");

			if (coordX > 0) {
				coordX -= 1;
			}

			showField(*field);

			if (rota) {
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 2);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 4);
				std::cout << "*";
			}
			else {
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 2, coordY * 2);
				std::cout << "*";
			}

			setCursorPos(20, 20);

			std::cout << "coordX = " << coordX << " coordY = " << coordY << std::endl;

		}
		else if (entering == DOWN) {
			system("CLS");

			showField(*field);

			if (rota) {
				if (coordX < 9) {
					coordX += 1;
				}
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 2);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 4);
				std::cout << "*";
			}
			else {
				if (coordX < 7) {
					coordX += 1;
				}
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 2, coordY * 2);
				std::cout << "*";
			}

			setCursorPos(20, 20);

			std::cout << "coordX = " << coordX << " coordY = " << coordY << std::endl;

		}
		else if (entering == RIGHT) {

			system("CLS");



			showField(*field);

			if (rota) {
				if (coordY < 7) {
					coordY += 1;
				}
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 2);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 4);
				std::cout << "*";
			}
			else {
				if (coordY < 9) {
					coordY += 1;
				}
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 2, coordY * 2);
				std::cout << "*";
			}

			setCursorPos(20, 20);

			std::cout << "coordX = " << coordX << " coordY = " << coordY << std::endl;

		}
		else if (entering == LEFT) {

			system("CLS");

			if (coordY > 0) {
				coordY -= 1;
			}

			showField(*field);

			if (rota) {
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 2);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 4);
				std::cout << "*";
			}
			else {
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 2, coordY * 2);
				std::cout << "*";
			}

			setCursorPos(20, 20);

			std::cout << "coordX = " << coordX << " coordY = " << coordY << std::endl;
		}
		else if (entering == ENTER) {
			system("CLS");

			if (rota) {
				coordX2 = coordX;
				coordY2 = coordY + 1;
				coordX3 = coordX;
				coordY3 = coordY + 2;
			}
			else {
				coordX2 = coordX + 1;
				coordY2 = coordY;
				coordX3 = coordX + 2;
				coordY3 = coordY;
			}

			if (checkArea(field, coordX, coordY) && checkArea(field, coordX2, coordY2) && checkArea(field, coordX3, coordY3)) {
				field->field[coordX][coordY] = '#';
				field->field[coordX2][coordY2] = '#';
				field->field[coordX3][coordY3] = '#';

				field->ship3[shipNum].size = 3;
				field->ship3[shipNum].decCoord = new int* [3];
				field->ship3[shipNum].decCoord[0] = new int[2];
				field->ship3[shipNum].decCoord[1] = new int[2];
				field->ship3[shipNum].decCoord[2] = new int[2];
				field->ship3[shipNum].decCoord[0][0] = coordX;
				field->ship3[shipNum].decCoord[0][1] = coordY;
				field->ship3[shipNum].decCoord[1][0] = coordX2;
				field->ship3[shipNum].decCoord[1][1] = coordX2;
				field->ship3[shipNum].decCoord[2][0] = coordX3;
				field->ship3[shipNum].decCoord[2][1] = coordY3;
				field->ship3[shipNum].decStat = new bool[3];
				field->ship3[shipNum].decStat[0] = true;
				field->ship3[shipNum].decStat[1] = true;
				field->ship3[shipNum].decStat[2] = true;
				field->ship3[shipNum].genStat = 1;
				shipNum++;
				showField(*field);
			}
			else {
				showField(*field);
				std::cout << "Wrong position!";
			}
		}
		else if (entering == ROTATION) {

			if (coordX + 2 > 9 || coordY * 2 + 4 > 18) {
				continue;
			}
			system("CLS");
			showField(*field);

			rota = !rota;

			if (rota) {
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 2);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 4);
				std::cout << "*";
			}
			else {
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 2, coordY * 2);
				std::cout << "*";
			}

			setCursorPos(20, 20);
		}
	}

	//четырёхпалубные
	shipNum = 0;
	coordX = 0;
	coordY = 0;

	while (shipNum < 1) {

		entering = _getch();

		if (entering == UP) {

			system("CLS");

			if (coordX > 0) {
				coordX -= 1;
			}

			showField(*field);

			if (rota) {
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 2);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 4);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 6);
				std::cout << "*";
			}
			else {
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 2, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 3, coordY * 2);
				std::cout << "*";
			}

			setCursorPos(20, 20);

			std::cout << "coordX = " << coordX << " coordY = " << coordY << std::endl;

		}
		else if (entering == DOWN) {
			system("CLS");



			showField(*field);

			if (rota) {
				if (coordX < 9) {
					coordX += 1;
				}
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 2);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 4);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 6);
				std::cout << "*";
			}
			else {
				if (coordX < 6) {
					coordX += 1;
				}
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 2, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 3, coordY * 2);
				std::cout << "*";
			}

			setCursorPos(20, 20);

			std::cout << "coordX = " << coordX << " coordY = " << coordY << std::endl;

		}
		else if (entering == RIGHT) {

			system("CLS");



			showField(*field);

			if (rota) {
				if (coordY < 6) {
					coordY += 1;
				}
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 2);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 4);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 6);
				std::cout << "*";
			}
			else {
				if (coordY < 9) {
					coordY += 1;
				}
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 2, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 3, coordY * 2);
				std::cout << "*";
			}

			setCursorPos(20, 20);

			std::cout << "coordX = " << coordX << " coordY = " << coordY << std::endl;

		}
		else if (entering == LEFT) {

			system("CLS");

			if (coordY > 0) {
				coordY -= 1;
			}

			showField(*field);

			if (rota) {
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 2);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 4);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 6);
				std::cout << "*";
			}
			else {
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 2, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 3, coordY * 2);
				std::cout << "*";
			}

			setCursorPos(20, 20);

			std::cout << "coordX = " << coordX << " coordY = " << coordY << std::endl;
		}
		else if (entering == ENTER) {
			system("CLS");

			if (rota) {
				coordX2 = coordX;
				coordY2 = coordY + 1;
				coordX3 = coordX;
				coordY3 = coordY + 2;
				coordX4 = coordX;
				coordY4 = coordY + 3;
			}
			else {
				coordX2 = coordX + 1;
				coordY2 = coordY;
				coordX3 = coordX + 2;
				coordY3 = coordY;
				coordX4 = coordX + 3;
				coordY4 = coordY;
			}

			if (checkArea(field, coordX, coordY) && checkArea(field, coordX2, coordY2) && checkArea(field, coordX3, coordY3) && checkArea(field, coordX4, coordY4)) {
				field->field[coordX][coordY] = '#';
				field->field[coordX2][coordY2] = '#';
				field->field[coordX3][coordY3] = '#';
				field->field[coordX4][coordY4] = '#';

				field->ship4[shipNum].size = 4;
				field->ship4[shipNum].decCoord = new int* [4];
				field->ship4[shipNum].decCoord[0] = new int[2];
				field->ship4[shipNum].decCoord[1] = new int[2];
				field->ship4[shipNum].decCoord[2] = new int[2];
				field->ship4[shipNum].decCoord[3] = new int[2];
				field->ship4[shipNum].decCoord[0][0] = coordX;
				field->ship4[shipNum].decCoord[0][1] = coordY;
				field->ship4[shipNum].decCoord[1][0] = coordX2;
				field->ship4[shipNum].decCoord[1][1] = coordY2;
				field->ship4[shipNum].decCoord[2][0] = coordX3;
				field->ship4[shipNum].decCoord[2][1] = coordY3;
				field->ship4[shipNum].decCoord[3][0] = coordX4;
				field->ship4[shipNum].decCoord[3][1] = coordY4;
				field->ship4[shipNum].decStat = new bool[4];
				field->ship4[shipNum].decStat[0] = true;
				field->ship4[shipNum].decStat[1] = true;
				field->ship4[shipNum].decStat[2] = true;
				field->ship4[shipNum].decStat[3] = true;
				field->ship4[shipNum].genStat = 1;
				shipNum++;
				showField(*field);
			}
			else {
				showField(*field);
				std::cout << "Wrong position!";
			}
		}
		else if (entering == ROTATION) {


			if (coordX + 3 > 9 || coordY * 2 + 4 > 18) {
				continue;
			}
			system("CLS");

			showField(*field);

			rota = !rota;

			if (rota) {
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 2);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 4);
				std::cout << "*";
				setCursorPos(coordX, coordY * 2 + 6);
				std::cout << "*";
			}
			else {
				setCursorPos(coordX, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 2, coordY * 2);
				std::cout << "*";
				setCursorPos(coordX + 3, coordY * 2);
				std::cout << "*";
			}

			setCursorPos(20, 20);
		}
	}
}

void fillFieldAutomatic(playerField* field)
{
	system("CLS");

	int coordX, coordY, dir, shipNum;

	//однопалубные

	shipNum = 0;

	while (shipNum < 4) {

		coordX = rand() % 10;
		coordY = rand() % 10;

		if (checkArea(field, coordX, coordY)) {
			field->field[coordX][coordY] = '#';

			field->ship1[shipNum].size = 1;
			field->ship1[shipNum].decCoord = new int* [1];
			field->ship1[shipNum].decCoord[0] = new int[2];
			field->ship1[shipNum].decCoord[0][0] = coordX;
			field->ship1[shipNum].decCoord[0][1] = coordY;
			field->ship1[shipNum].decStat = new bool[1];
			field->ship1[shipNum].decStat[0] = true;
			field->ship1[shipNum].genStat = 1;
			shipNum++;
		}
	}

	//двупалубные

	shipNum = 0;

	while (shipNum < 3) {

		coordX = rand() % 10;
		coordY = rand() % 10;
		dir = rand() % 4;

		if ((coordX == 0 && dir == 0) || (coordY == 0 && dir == 2) || (coordX == 9 && dir == 1) || (coordY == 9 && dir == 3)) {
			continue;
		}

		if (checkArea(field, coordX, coordY)) {
			if (dir == 0) {
				if (checkArea(field, coordX - 1, coordY)) {
					field->field[coordX - 1][coordY] = '#';
					field->field[coordX][coordY] = '#';

					field->ship2[shipNum].size = 2;
					field->ship2[shipNum].decCoord = new int* [2];
					field->ship2[shipNum].decCoord[0] = new int[2];
					field->ship2[shipNum].decCoord[1] = new int[2];
					field->ship2[shipNum].decCoord[0][0] = coordX - 1;
					field->ship2[shipNum].decCoord[0][1] = coordY;
					field->ship2[shipNum].decCoord[1][0] = coordX;
					field->ship2[shipNum].decCoord[1][1] = coordY;
					field->ship2[shipNum].decStat = new bool[2];
					field->ship2[shipNum].decStat[0] = true;
					field->ship2[shipNum].decStat[1] = true;
					field->ship2[shipNum].genStat = 1;
					shipNum++;
				}
			}
			else if (dir == 1) {
				if (checkArea(field, coordX + 1, coordY)) {
					field->field[coordX + 1][coordY] = '#';
					field->field[coordX][coordY] = '#';

					field->ship2[shipNum].size = 2;
					field->ship2[shipNum].decCoord = new int* [2];
					field->ship2[shipNum].decCoord[0] = new int[2];
					field->ship2[shipNum].decCoord[1] = new int[2];
					field->ship2[shipNum].decCoord[0][0] = coordX + 1;
					field->ship2[shipNum].decCoord[0][1] = coordY;
					field->ship2[shipNum].decCoord[1][0] = coordX;
					field->ship2[shipNum].decCoord[1][1] = coordY;
					field->ship2[shipNum].decStat = new bool[2];
					field->ship2[shipNum].decStat[0] = true;
					field->ship2[shipNum].decStat[1] = true;
					field->ship2[shipNum].genStat = 1;
					shipNum++;
				}
			}
			else if (dir == 2) {
				if (checkArea(field, coordX, coordY - 1)) {
					field->field[coordX][coordY - 1] = '#';
					field->field[coordX][coordY] = '#';

					field->ship2[shipNum].size = 2;
					field->ship2[shipNum].decCoord = new int* [2];
					field->ship2[shipNum].decCoord[0] = new int[2];
					field->ship2[shipNum].decCoord[1] = new int[2];
					field->ship2[shipNum].decCoord[0][0] = coordX;
					field->ship2[shipNum].decCoord[0][1] = coordY - 1;
					field->ship2[shipNum].decCoord[1][0] = coordX;
					field->ship2[shipNum].decCoord[1][1] = coordY;
					field->ship2[shipNum].decStat = new bool[2];
					field->ship2[shipNum].decStat[0] = true;
					field->ship2[shipNum].decStat[1] = true;
					field->ship2[shipNum].genStat = 1;
					shipNum++;
				}
			}
			else {
				if (checkArea(field, coordX, coordY + 1)) {
					field->field[coordX][coordY + 1] = '#';
					field->field[coordX][coordY] = '#';

					field->ship2[shipNum].size = 2;
					field->ship2[shipNum].decCoord = new int* [2];
					field->ship2[shipNum].decCoord[0] = new int[2];
					field->ship2[shipNum].decCoord[1] = new int[2];
					field->ship2[shipNum].decCoord[0][0] = coordX;
					field->ship2[shipNum].decCoord[0][1] = coordY + 1;
					field->ship2[shipNum].decCoord[1][0] = coordX;
					field->ship2[shipNum].decCoord[1][1] = coordY;
					field->ship2[shipNum].decStat = new bool[2];
					field->ship2[shipNum].decStat[0] = true;
					field->ship2[shipNum].decStat[1] = true;
					field->ship2[shipNum].genStat = 1;
					shipNum++;
				}
			}
		}
	}

	//трёхпалубные

	shipNum = 0;

	while (shipNum < 2) {

		coordX = rand() % 10;
		coordY = rand() % 10;
		dir = rand() % 4;

		if ((coordX < 3 && dir == 0) || (coordY < 3 && dir == 2) || (coordX > 7 && dir == 1) || (coordY > 7 && dir == 3)) {
			continue;
		}

		if (checkArea(field, coordX, coordY)) {
			if (dir == 0) {
				if (checkArea(field, coordX - 1, coordY) && checkArea(field, coordX - 2, coordY)) {
					field->field[coordX - 1][coordY] = '#';
					field->field[coordX - 2][coordY] = '#';
					field->field[coordX][coordY] = '#';

					field->ship3[shipNum].size = 3;
					field->ship3[shipNum].decCoord = new int* [3];
					field->ship3[shipNum].decCoord[0] = new int[2];
					field->ship3[shipNum].decCoord[1] = new int[2];
					field->ship3[shipNum].decCoord[2] = new int[2];
					field->ship3[shipNum].decCoord[0][0] = coordX;
					field->ship3[shipNum].decCoord[0][1] = coordY;
					field->ship3[shipNum].decCoord[1][0] = coordX - 1;
					field->ship3[shipNum].decCoord[1][1] = coordY;
					field->ship3[shipNum].decCoord[2][0] = coordX - 2;
					field->ship3[shipNum].decCoord[2][1] = coordY;
					field->ship3[shipNum].decStat = new bool[3];
					field->ship3[shipNum].decStat[0] = true;
					field->ship3[shipNum].decStat[1] = true;
					field->ship3[shipNum].decStat[2] = true;
					field->ship3[shipNum].genStat = 1;
					shipNum++;
				}
			}
			else if (dir == 1) {
				if (checkArea(field, coordX + 1, coordY) && checkArea(field, coordX + 2, coordY)) {
					field->field[coordX + 1][coordY] = '#';
					field->field[coordX + 2][coordY] = '#';
					field->field[coordX][coordY] = '#';

					field->ship3[shipNum].size = 3;
					field->ship3[shipNum].decCoord = new int* [3];
					field->ship3[shipNum].decCoord[0] = new int[2];
					field->ship3[shipNum].decCoord[1] = new int[2];
					field->ship3[shipNum].decCoord[2] = new int[2];
					field->ship3[shipNum].decCoord[0][0] = coordX;
					field->ship3[shipNum].decCoord[0][1] = coordY;
					field->ship3[shipNum].decCoord[1][0] = coordX + 1;
					field->ship3[shipNum].decCoord[1][1] = coordY;
					field->ship3[shipNum].decCoord[2][0] = coordX + 2;
					field->ship3[shipNum].decCoord[2][1] = coordY;
					field->ship3[shipNum].decStat = new bool[3];
					field->ship3[shipNum].decStat[0] = true;
					field->ship3[shipNum].decStat[1] = true;
					field->ship3[shipNum].decStat[2] = true;
					field->ship3[shipNum].genStat = 1;
					shipNum++;
				}
			}
			else if (dir == 2) {
				if (checkArea(field, coordX, coordY - 1) && checkArea(field, coordX, coordY - 2)) {
					field->field[coordX][coordY - 1] = '#';
					field->field[coordX][coordY - 2] = '#';
					field->field[coordX][coordY] = '#';

					field->ship3[shipNum].size = 3;
					field->ship3[shipNum].decCoord = new int* [3];
					field->ship3[shipNum].decCoord[0] = new int[2];
					field->ship3[shipNum].decCoord[1] = new int[2];
					field->ship3[shipNum].decCoord[2] = new int[2];
					field->ship3[shipNum].decCoord[0][0] = coordX;
					field->ship3[shipNum].decCoord[0][1] = coordY;
					field->ship3[shipNum].decCoord[1][0] = coordX;
					field->ship3[shipNum].decCoord[1][1] = coordY - 1;
					field->ship3[shipNum].decCoord[2][0] = coordX;
					field->ship3[shipNum].decCoord[2][1] = coordY - 2;
					field->ship3[shipNum].decStat = new bool[3];
					field->ship3[shipNum].decStat[0] = true;
					field->ship3[shipNum].decStat[1] = true;
					field->ship3[shipNum].decStat[2] = true;
					field->ship3[shipNum].genStat = 1;
					shipNum++;
				}
			}
			else {
				if (checkArea(field, coordX, coordY + 1) && checkArea(field, coordX, coordY + 2)) {
					field->field[coordX][coordY + 1] = '#';
					field->field[coordX][coordY + 2] = '#';
					field->field[coordX][coordY] = '#';

					field->ship3[shipNum].size = 3;
					field->ship3[shipNum].decCoord = new int* [3];
					field->ship3[shipNum].decCoord[0] = new int[2];
					field->ship3[shipNum].decCoord[1] = new int[2];
					field->ship3[shipNum].decCoord[2] = new int[2];
					field->ship3[shipNum].decCoord[0][0] = coordX;
					field->ship3[shipNum].decCoord[0][1] = coordY;
					field->ship3[shipNum].decCoord[1][0] = coordX;
					field->ship3[shipNum].decCoord[1][1] = coordY + 1;
					field->ship3[shipNum].decCoord[2][0] = coordX;
					field->ship3[shipNum].decCoord[2][1] = coordY + 2;
					field->ship3[shipNum].decStat = new bool[3];
					field->ship3[shipNum].decStat[0] = true;
					field->ship3[shipNum].decStat[1] = true;
					field->ship3[shipNum].decStat[2] = true;
					field->ship3[shipNum].genStat = 1;
					shipNum++;
				}
			}


		}

	}

	//четырёхпалубные

	shipNum = 0;

	while (shipNum < 1) {

		coordX = rand() % 10;
		coordY = rand() % 10;
		dir = rand() % 4;

		if ((coordX < 4 && dir == 0) || (coordY < 4 && dir == 2) || (coordX > 6 && dir == 1) || (coordY > 6 && dir == 3)) {
			continue;
		}

		if (checkArea(field, coordX, coordY)) {
			if (dir == 0) {
				if (checkArea(field, coordX - 1, coordY) && checkArea(field, coordX - 2, coordY) && checkArea(field, coordX - 3, coordY)) {
					field->field[coordX - 1][coordY] = '#';
					field->field[coordX - 2][coordY] = '#';
					field->field[coordX - 3][coordY] = '#';
					field->field[coordX][coordY] = '#';

					field->ship4[shipNum].size = 4;
					field->ship4[shipNum].decCoord = new int* [4];
					field->ship4[shipNum].decCoord[0] = new int[2];
					field->ship4[shipNum].decCoord[1] = new int[2];
					field->ship4[shipNum].decCoord[2] = new int[2];
					field->ship4[shipNum].decCoord[3] = new int[2];
					field->ship4[shipNum].decCoord[0][0] = coordX;
					field->ship4[shipNum].decCoord[0][1] = coordY;
					field->ship4[shipNum].decCoord[1][0] = coordX - 1;
					field->ship4[shipNum].decCoord[1][1] = coordY;
					field->ship4[shipNum].decCoord[2][0] = coordX - 2;
					field->ship4[shipNum].decCoord[2][1] = coordY;
					field->ship4[shipNum].decCoord[3][0] = coordX - 3;
					field->ship4[shipNum].decCoord[3][1] = coordY;
					field->ship4[shipNum].decStat = new bool[4];
					field->ship4[shipNum].decStat[0] = true;
					field->ship4[shipNum].decStat[1] = true;
					field->ship4[shipNum].decStat[2] = true;
					field->ship4[shipNum].decStat[3] = true;
					field->ship4[shipNum].genStat = 1;
					shipNum++;
				}
			}
			else if (dir == 1) {
				if (checkArea(field, coordX + 1, coordY) && checkArea(field, coordX + 2, coordY) && checkArea(field, coordX + 3, coordY)) {
					field->field[coordX + 1][coordY] = '#';
					field->field[coordX + 2][coordY] = '#';
					field->field[coordX + 3][coordY] = '#';
					field->field[coordX][coordY] = '#';

					field->ship4[shipNum].size = 4;
					field->ship4[shipNum].decCoord = new int* [4];
					field->ship4[shipNum].decCoord[0] = new int[2];
					field->ship4[shipNum].decCoord[1] = new int[2];
					field->ship4[shipNum].decCoord[2] = new int[2];
					field->ship4[shipNum].decCoord[3] = new int[2];
					field->ship4[shipNum].decCoord[0][0] = coordX;
					field->ship4[shipNum].decCoord[0][1] = coordY;
					field->ship4[shipNum].decCoord[1][0] = coordX + 1;
					field->ship4[shipNum].decCoord[1][1] = coordY;
					field->ship4[shipNum].decCoord[2][0] = coordX + 2;
					field->ship4[shipNum].decCoord[2][1] = coordY;
					field->ship4[shipNum].decCoord[3][0] = coordX + 3;
					field->ship4[shipNum].decCoord[3][1] = coordY;
					field->ship4[shipNum].decStat = new bool[4];
					field->ship4[shipNum].decStat[0] = true;
					field->ship4[shipNum].decStat[1] = true;
					field->ship4[shipNum].decStat[2] = true;
					field->ship4[shipNum].decStat[3] = true;
					field->ship4[shipNum].genStat = 1;
					shipNum++;
				}
			}
			else if (dir == 2) {
				if (checkArea(field, coordX, coordY - 1) && checkArea(field, coordX, coordY - 2) && checkArea(field, coordX, coordY - 3)) {
					field->field[coordX][coordY - 1] = '#';
					field->field[coordX][coordY - 2] = '#';
					field->field[coordX][coordY - 3] = '#';
					field->field[coordX][coordY] = '#';

					field->ship4[shipNum].size = 4;
					field->ship4[shipNum].decCoord = new int* [4];
					field->ship4[shipNum].decCoord[0] = new int[2];
					field->ship4[shipNum].decCoord[1] = new int[2];
					field->ship4[shipNum].decCoord[2] = new int[2];
					field->ship4[shipNum].decCoord[3] = new int[2];
					field->ship4[shipNum].decCoord[0][0] = coordX;
					field->ship4[shipNum].decCoord[0][1] = coordY;
					field->ship4[shipNum].decCoord[1][0] = coordX;
					field->ship4[shipNum].decCoord[1][1] = coordY - 1;
					field->ship4[shipNum].decCoord[2][0] = coordX;
					field->ship4[shipNum].decCoord[2][1] = coordY - 2;
					field->ship4[shipNum].decCoord[3][0] = coordX;
					field->ship4[shipNum].decCoord[3][1] = coordY - 3;
					field->ship4[shipNum].decStat = new bool[4];
					field->ship4[shipNum].decStat[0] = true;
					field->ship4[shipNum].decStat[1] = true;
					field->ship4[shipNum].decStat[2] = true;
					field->ship4[shipNum].decStat[3] = true;
					field->ship4[shipNum].genStat = 1;
					shipNum++;
				}
			}
			else {
				if (checkArea(field, coordX, coordY + 1) && checkArea(field, coordX, coordY + 2) && checkArea(field, coordX, coordY + 3)) {
					field->field[coordX][coordY + 1] = '#';
					field->field[coordX][coordY + 2] = '#';
					field->field[coordX][coordY + 3] = '#';
					field->field[coordX][coordY] = '#';

					field->ship4[shipNum].size = 4;
					field->ship4[shipNum].decCoord = new int* [4];
					field->ship4[shipNum].decCoord[0] = new int[2];
					field->ship4[shipNum].decCoord[1] = new int[2];
					field->ship4[shipNum].decCoord[2] = new int[2];
					field->ship4[shipNum].decCoord[3] = new int[2];
					field->ship4[shipNum].decCoord[0][0] = coordX;
					field->ship4[shipNum].decCoord[0][1] = coordY;
					field->ship4[shipNum].decCoord[1][0] = coordX;
					field->ship4[shipNum].decCoord[1][1] = coordY + 1;
					field->ship4[shipNum].decCoord[2][0] = coordX;
					field->ship4[shipNum].decCoord[2][1] = coordY + 2;
					field->ship4[shipNum].decCoord[3][0] = coordX;
					field->ship4[shipNum].decCoord[3][1] = coordY + 3;
					field->ship4[shipNum].decStat = new bool[4];
					field->ship4[shipNum].decStat[0] = true;
					field->ship4[shipNum].decStat[1] = true;
					field->ship4[shipNum].decStat[2] = true;
					field->ship4[shipNum].decStat[3] = true;
					field->ship4[shipNum].genStat = 1;
					shipNum++;
				}
			}
		}
	}
}

/*Простой вывод поля без прекрас*/
void showField(playerField field)
{
	for (int i = 0; i < 10; i++) {
		for (int c = 0; c < 10; c++)
		{
			std::cout << field.field[i][c] << " ";
		}
		std::cout << std::endl;
	}
}

void setCursorPos(int y, int x)
{
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x, y };
	SetConsoleCursorPosition(output, pos);
}

void showFields(playerField* field1, playerField* field2)
{

	//сами поля
	//очерёдность хода
	//статистика по кораблям внизу
	//имена игроков над полями
	//"вызов" прицела попробовать вызвать изменение позиции курсора отдельной функцией и тд и тп
	//sea - 177, ships - 254, destr - 206, miss - 153
	char sea = 176;
	char ship = 254;
	char destr = 206;
	char miss = 153;
	char mist = 178;

	std::cout << "\n\n\n";
	std::cout << "\t\t\t\t" << field1->playerName << "\t\t\t\t" << field2->playerName << std::endl;
	std::cout << "\t\t\t   A B C D E F G H I J \t\t   A B C D E F G H I J " << std::endl;

	for (int i = 0; i < 10; i++) {

		i != 9 ? std::cout << "\t\t\t " << i + 1 << " " : std::cout << "\t\t\t" << i + 1 << " ";


		for (int c = 0; c < 10; c++) {
			if (field1->field[i][c] == '~' || field1->field[i][c] == '0') {
				std::cout << sea << " ";
			}
			else if (field1->field[i][c] == '#') {
				std::cout << ship << " ";
			}
			else {
				std::cout << field1->field[i][c] << " ";
			}

		}

		std::cout << "\t\t";
		i != 9 ? std::cout << " " << i + 1 << " " : std::cout << i + 1 << " ";

		for (int c = 0; c < 10; c++) {

			if (field2->field[i][c] == '~' || field2->field[i][c] == '#') {
				std::cout << mist << " ";
			}
			else if (field2->field[i][c] == '0') {
				std::cout << miss << " ";
			}
			else {
				std::cout << field2->field[i][c] << " ";
			}
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;
	std::cout << "   Remaining ships: \t\t   Remaining ships: " << std::endl;
	std::cout << " Single-deck - " << field1->ship1[0].genStat + field1->ship1[1].genStat + field1->ship1[2].genStat + field1->ship1[3].genStat;
	std::cout << "\t\t Single-deck - " << field2->ship1[0].genStat + field2->ship1[1].genStat + field2->ship1[2].genStat + field2->ship1[3].genStat << std::endl;
	std::cout << " Double-deck - " << field1->ship2[0].genStat + field1->ship2[1].genStat + field1->ship2[2].genStat;
	std::cout << "\t\t Double-deck - " << field2->ship2[0].genStat + field2->ship2[1].genStat + field2->ship2[2].genStat << std::endl;
	std::cout << " Three-deck - " << field1->ship3[0].genStat + field1->ship3[1].genStat;
	std::cout << "\t\t\t Three-deck - " << field2->ship3[0].genStat + field2->ship3[1].genStat << std::endl;
	std::cout << " Four-deck - " << field1->ship4[0].genStat;
	std::cout << "\t\t\t Four-deck - " << field2->ship4[0].genStat << std::endl;

}

void makeShoot(playerField* field1, playerField* field2)
{
	int modX = 1, modY = 35;
	int coordX = modX, coordY = modY;

	char entering;
	enum { UP = 72, DOWN = 80, RIGHT = 77, LEFT = 75, ENTER = '\r', ROTATION = 'r' };

	//перемещаем курсор по полю в поискахъ позиции для выстрела
	//после выбора позиции, нажимаем на выстрел
	//если там есть корабль, то начинаем разбираться
		//если полностью потоплен, то окружаем место корабля на одну клетку помоченным
		// если не потоплен, то ппомечаем чать корабля подбитой
		// записываем информацию на поле
		// записываем информацию в сам корабль
	//если корабля нет
		//сообщение о промахе
		//помечаем место как разведаное



	showFields(field1, field2);
	setCursorPos(coordX, coordY);
	std::cout << '*';
	while (true) {

		entering = _getch();

		if (entering == UP) {
			system("CLS");
			if (coordX > modX) {
				coordX -= 1;
			}
			showFields(field1, field2);
			setCursorPos(coordX, coordY);
			std::cout << '*';
			setCursorPos(20, 0);
			std::cout << coordX - modX << "  " << coordY - modY - ((coordY - modY) / 2) << std::endl;
		}
		else if (entering == DOWN) {
			system("CLS");
			if (coordX < modX + 9) {
				coordX += 1;
			}
			showFields(field1, field2);
			setCursorPos(coordX, coordY);
			std::cout << '*';
			setCursorPos(20, 0);
			std::cout << coordX - modX << "  " << coordY - modY - ((coordY - modY) / 2) << std::endl;
		}
		else if (entering == RIGHT) {
			system("CLS");
			if (coordY < modY + 18) {
				coordY += 2;
			}
			showFields(field1, field2);
			setCursorPos(coordX, coordY);
			std::cout << '*';
			setCursorPos(20, 0);
			std::cout << coordX - modX << "  " << coordY - modY - ((coordY - modY) / 2) << std::endl;
		}
		else if (entering == LEFT) {
			system("CLS");
			if (coordY > modY) {
				coordY -= 2;
			}
			showFields(field1, field2);
			setCursorPos(coordX, coordY);
			std::cout << '*';
			setCursorPos(20, 0);
			std::cout << coordX - modX << "  " << coordY - modY - ((coordY - modY) / 2) << std::endl;
		}
		else if (entering == ENTER) {
			shootChecker(field2, coordX - modX, coordY - modY - ((coordY - modY) / 2));
		}
	}
}

//возвращать будет true при результативном попадании
bool shootChecker(playerField* field, int x, int y)
{

	std::cout << field->ship1[0].decCoord[0][0] << " " << field->ship1[0].decCoord[0][1] << std::endl;
	std::cout << field->ship1[1].decCoord[0][0] << " " << field->ship1[1].decCoord[0][1] << std::endl;
	std::cout << field->ship1[2].decCoord[0][0] << " " << field->ship1[2].decCoord[0][1] << std::endl;
	std::cout << field->ship1[3].decCoord[0][0] << " " << field->ship1[3].decCoord[0][1] << std::endl;

	if (field->field[x][y] == '#') {

		//поиск грёбаного корабля
		for (int i = 0; i < 4; i++) {

			if (field->ship1[i].decCoord[0][0] == x && field->ship1[i].decCoord[0][1] == y) {
				(*field->ship1[i].decStat) = false;
				field->ship1[i].genStat = 0;
				field->field[x][y] = 'X';

				if (field->ship1[i].genStat == 0) {
					//работает функция для границ
				}

				return true;
			}
		}

	}
	else if (field->field[x][y] == '~') {
		//звук всплеска, например
		field->field[x][y] = '0';
		std::cout << "Miss!" << std::endl;
		return false;
	}
	else if (field->field[x][y] == 'X' || field->field[x][y] == '0') {
		std::cout << "Miss! You're blind bastard!" << std::endl;
		return false;
	}
	else {
		return false;
	}



}

void mainMenu(playerField* field_1, playerField* field_2)
{
	field_1->playerName = "Player1";
	field_2->playerName = "Player2";


	char choose;
	bool exit = true;

	while (exit) {

		system("CLS");

		std::cout << "\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t1. New game." << std::endl;
		std::cout << "\t\t\t\t\t0. Exit." << std::endl;

		choose = _getch();


			switch (choose) {
				case '1':

					system("CLS");

					std::cout << "\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t1. Player vs player." << std::endl;
					std::cout << "\t\t\t\t\t2. Player vs PC." << std::endl;
					std::cout << "\t\t\t\t\t3. PC vs PC." << std::endl;
					std::cout << "\t\t\t\t\t0. Exit." << std::endl;

					choose = _getch();

						switch (choose) {
							case '1':	//игрок против игрока

								system("CLS");

								std::cout << "\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t1. Arrange ships manually." << std::endl;
								std::cout << "\t\t\t\t\t2. Arrange ships automatically." << std::endl;
								std::cout << "\t\t\t\t\t3. Enter the names." << std::endl;  //ручной ввод имён игроков, в противном случае будут просто player1 и player2

								std::cout << "\t\t\t\t\t0. Exit." << std::endl;

								choose = _getch();

									switch (choose) {
									case '1':
										//функция для ручного расставления кораблей для игрока 1
										fillFieldManual(field_1);
										//функция для ручного расставления кораблей для игрока 2
										fillFieldManual(field_2);
										startGame(field_1, field_2);
										break;
									case '2':
										//функция для автоматического расставления кораблей для игрока 1
										fillFieldAutomatic(field_1);
										//функция для автоматического расставления кораблей для игрока 2
										fillFieldAutomatic(field_2);
										startGame(field_1, field_2);
										break;
									case '0':
										exit = false;
										break;
									default:
										break;
									}
								
							break;
							case '2':	//игрок против PC
								std::cout << "\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t1. Arrange ships manually." << std::endl;
								std::cout << "\t\t\t\t\t2. Arrange ships automatically." << std::endl;
								std::cout << "\t\t\t\t\t0. Exit." << std::endl;

								choose = _getch();

								switch (choose) {
									case '1':
										//функция для ручного расставления кораблей для игрока 1
										//функция для автоматического расставления кораблей для PC
										//запуск игры с переданными полями
										break;
									case '2':
										//функция для автоматического расставления кораблей для игрока 1
										//функция для автоматического расставления кораблей для PC
										//запуск игры с переданными полями
										break;
									case '0':
										exit = false;
										break;
									default:
										break;
									}
								break;
							case '3':	//PC vs PC
								//запуск функций на автоматаческое заполнение полей
								//запуск функции на начало игры PC vs PC
								break;
							case '0':
								exit = false;
								break;
							default:
								break;
							}
						
					break;
				case '0':
					exit = false;
					break;
				default:
					break;
			}
		
	}
}

void consoleSize()
{
	CONSOLE_SCREEN_BUFFER_INFOEX consolesize;

	consolesize.cbSize = sizeof(consolesize);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleScreenBufferInfoEx(hConsole, &consolesize);

	COORD c;
	c.X = 100;
	c.Y = 30;
	consolesize.dwSize = c;

	consolesize.srWindow.Left = 0;
	consolesize.srWindow.Right = 100;
	consolesize.srWindow.Top = 0;
	consolesize.srWindow.Bottom = 30;

	SetConsoleScreenBufferInfoEx(hConsole, &consolesize);
}
//рандомно ищёт корабли на поле, возвращает true в случае удачного выстрела
bool terminator_1(playerField* field){

	while (true) {
		int x = rand() % 10;
		int y = rand() % 10;

		if (field->field[x][y] == '~') {
			field->field[x][y] = '0';
			std::cout << "Miss!";
			return false;
		}
		else if (field->field[x][y] == '#') {
			field->field[x][y] = 'X';
			std::cout << "Hit!";
			return true;
		}
	}
}
//"умный" бот, , возвращает true в случае удачного выстрела
bool terminator_2(playerField*) { return true; }

void startGame(playerField* field1, playerField* field2)
{
	//пока есть корабли или не нажата кнопка выхода 
		//игроку1 предлагается сделать выстрел (если это человек, то выхывается функция нацеливания)
			//если попал, выводится сообщение о попадании
				//если у противника не осталось кораблей, то игра заканчивается, и выводится сообщение о победе
			//если промазал, то ход передаётся игроку2

	system("CLS");

	while (true) {  //функция, возвращающая количество оставшихся кораблей, если у одного из игроков кораблей не осталось, то возвращается false
		while (shooter(field1, field2)) {
			std::cout << "MIIIIIIIS";
			system("PAUSE");
		}

		while (shooter(field2, field1)) {
			std::cout << "MIIIIIIIS";
			system("PAUSE");
		}

	}
}
//предоставляет возможность игроку сделать выстрел
bool shooter(playerField* field1, playerField* field2)
{
	
	int coordX, coordY;
	char entering;
	enum { UP = 72, DOWN = 80, RIGHT = 77, LEFT = 75, ENTER = '\r'};

	coordX = 5;
	coordY = 59;

	while (true) {
		system("CLS");

		showFields(field1, field2);

		setCursorPos(coordX, coordY);

		std::cout << '+';


		entering = _getch();

		if (entering == UP) {
			system("CLS");
			if (coordX > 5) {
				coordX -= 1;
			}
		}
		else if (entering == DOWN) {
			system("CLS");
			if (coordX < 14) {
				coordX += 1;
			}
		}
		else if (entering == RIGHT) {
			system("CLS");
			if (coordY < 77) {
				coordY += 2;
			}
		}
		else if (entering == LEFT) {
			system("CLS");
			if (coordY > 59) {
				coordY -= 2;
			}
		}
		else if (entering == ENTER) {

			if (field2->field[coordX - 5][(coordY - 59) / 2] == '#') {
				//функция, для поиска и записи состояния о корабле
				field2->field[coordX - 5][(coordY - 59) / 2] = 'X';
				std::cout << "Nice shoot!" << std::endl;
			}
			else if(field2->field[coordX - 5][(coordY - 59) / 2] == 'X' || field2->field[coordX - 5][(coordY - 59) / 2] == '0') {
				std::cout << "Try again." << std::endl;
			}
			else {
				field2->field[coordX - 5][(coordY - 59) / 2] = '0';
				std::cout << "Miss." << std::endl;
				return false;
			}


		}
	}





}

bool winChecker(playerField* field1, playerField* field2)
{

}

void accountant(playerField field, int x, int y)
{

	//однопалубники

	for (int i = 0; i < 4; i++) {
		if (field.ship1[i].decCoord[0][0] == x && field.ship1[i].decCoord[0][0] == y) {
			field.ship1[i].decStat = false;
			field.ship1[i].genStat = 0;
		}
	}

	//двухпалубники

	for (int i = 0; i < 3; i++) {
		for (int c = 0; c < 2; c++) {
			field.ship2[i].decCoord[c][0] == x && field.ship2[i].decCoord[c][1] == y;
			field.ship1[i].decStat = false;
		}

	}

	//трёхпалубники

	for (int i = 0; i < 2; i++) {
		if (field.ship1[i].decCoord[0][0] == x || field.ship1[i].decCoord[0][0] == y) {
			field.ship1[i].decStat = false;
			field.ship1[i].genStat = 0;
		}
	}

	//четырехпалубники

	for (int i = 0; i < 4; i++) {
		if (field.ship1[i].decCoord[0][0] == x || field.ship1[i].decCoord[0][0] == y) {
			field.ship1[i].decStat = false;
			field.ship1[i].genStat = 0;
		}
	}



}


//заполянет поля вокруг корабля, просто принимая его (там уже содержатся координаты и прочее
//функция, проверяющая потоплен ли корабль в который попали, или нет
