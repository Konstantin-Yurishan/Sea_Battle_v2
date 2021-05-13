/*Первостепенное*/
//заполнение поля двумя игроками
//для каждого игрока в его очередь отображается его поле, и поле противника с туманом войны + специальные кнопки для досрочного завершения игры
//красивое оформление полей 
//навигация по чужому полю для выстрела
//произведение выстрела и его последствия, вывод сообщений о попаданиях и промахах
//произведение выстрела противником с лёгким ии
//кондиции завершения игры
//произведение выстрела противником с сложным ии
//разборка с освобождением памяти

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
//
//	сохранение и загрузка
// реализация возможности сохранения и загрузки  
//



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
void showFields(playerField, playerField);
void makeShoot(playerField*, playerField*);
bool shootChecker(playerField*, int, int);
void game(); //входят функции intro, mainMenu
void mainMenu();


int main()
{

	srand(time(NULL));

	playerField field_1 = createPlayerField();
	playerField* field_1_ptr = &field_1;
	playerField field_2 = createPlayerField();
	playerField* field_2_ptr = &field_2;


	mainMenu();



	//fillFieldAutomatic(field_1_ptr);
	//fillFieldAutomatic(field_2_ptr);
	//showField(field);
	//fillFieldManual(field_1_ptr);
	//showFields(field_1, field_2);
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

void fillFieldAutomatic(playerField* field)
{

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

void showFields(playerField field1, playerField field2)
{

	//сами поля
	//очерёдность хода
	//статистика по кораблям внизу
	//имена игроков над полями
	//"вызов" прицела попробовать вызвать изменение позиции курсора отдельной функцией и тд и тп

	std::cout << "   A B C D E F G H I J \t\t   A B C D E F G H I J " << std::endl;

	for (int i = 0; i < 10; i++) {

		i != 9 ? std::cout << " " << i + 1 << " " : std::cout << i + 1 << " ";


		for (int c = 0; c < 10; c++) {
			std::cout << field1.field[i][c] << " ";
		}

		std::cout << "\t\t";

		i != 9 ? std::cout << " " << i + 1 << " " : std::cout << i + 1 << " ";

		for (int c = 0; c < 10; c++) {
			std::cout << field2.field[i][c] << " ";
		}

		std::cout << std::endl;
	}


	std::cout << std::endl;
	std::cout << "   Remaining ships: \t\t   Remaining ships: " << std::endl;
	std::cout << " Single-deck - " << field1.ship1[0].genStat + field1.ship1[1].genStat + field1.ship1[2].genStat + field1.ship1[3].genStat;
	std::cout << "\t\t Single-deck - " << field2.ship1[0].genStat + field2.ship1[1].genStat + field2.ship1[2].genStat + field2.ship1[3].genStat << std::endl;
	std::cout << " Double-deck - " << field1.ship2[0].genStat + field1.ship2[1].genStat + field1.ship2[2].genStat;
	std::cout << "\t\t Double-deck - " << field2.ship2[0].genStat + field2.ship2[1].genStat + field2.ship2[2].genStat << std::endl;
	std::cout << " Three-deck - " << field1.ship3[0].genStat + field1.ship3[1].genStat;
	std::cout << "\t\t\t Three-deck - " << field2.ship3[0].genStat + field2.ship3[1].genStat << std::endl;
	std::cout << " Four-deck - " << field1.ship4[0].genStat;
	std::cout << "\t\t\t Four-deck - " << field2.ship4[0].genStat << std::endl;





}

void statistic() {}

void fillFieldManual(playerField* field)
{

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
	std::cout << "*";

	while (shipNum < 4) {

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



	showFields(*field1, *field2);
	setCursorPos(coordX, coordY);
	std::cout << '*';
	while (true) {

		entering = _getch();

		if (entering == UP) {
			system("CLS");
			if (coordX > modX) {
				coordX -= 1;
			}
			showFields(*field1, *field2);
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
			showFields(*field1, *field2);
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
			showFields(*field1, *field2);
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
			showFields(*field1, *field2);
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

void mainMenu()
{

	char choose;
	bool exit = true;
	bool bback = true;

	while (exit) {

		system("CLS");

		std::cout << "1. New game." << std::endl;
		std::cout << "0. Exit." << std::endl;

		choose = _getch();
		//тестовый возврат
		bback = true;
		while (exit && bback) {
			switch (choose) {
			case '1':

				system("CLS");

				std::cout << "1. Player vs player." << std::endl;
				std::cout << "2. Player vs PC." << std::endl;
				std::cout << "3. PC vs PC." << std::endl;
				std::cout << "9. Back." << std::endl;
				std::cout << "0. Exit." << std::endl;

				choose = _getch();

				switch (choose) {
				case '1':	//игрок против игрока
					std::cout << "1. Arrange ships manually." << std::endl;
					std::cout << "2. Arrange ships automatically." << std::endl;
					std::cout << "0. Exit." << std::endl;

					choose = _getch();

					switch (choose) {
					case '1':
						//функция для ручного расставления кораблей для игрока 1
						//функция для ручного расставления кораблей для игрока 2
						//запуск игры с переданными полями
						break;
					case '2':
						//функция для автоматического расставления кораблей для игрока 1
						//функция для автоматического расставления кораблей для игрока 2
						//запуск игры с переданными полями
						break;
					case '0':
						exit = false;
						break;
					default:
						break;
					}

					break;
				case '2':	//игрок против PC
					std::cout << "1. Arrange ships manually." << std::endl;
					std::cout << "2. Arrange ships automatically." << std::endl;
					std::cout << "0. Exit." << std::endl;

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
				case '9':
					bback = false;
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
				std::cout << "Something wrong." << std::endl; //в принципе будет бесполезен, если программа будет реагировать только на приавильные нажатия, игнорируя неправильные, но пока пусть будет
				break;
			}
		}

	}

}
