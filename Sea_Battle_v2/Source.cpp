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
//подсветка пунктов меню при выборе + при выборе появление стрелки влево "->"
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
#include <conio.h>
#pragma comment(lib, "winmm.lib")  //для вывода звука


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
	short amount = 20;
	char** field;

};

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void intro();
playerField createPlayerField(); //создаёт экземпляр структуры, и заполняет в ней поле field
bool checkArea(playerField*, int, int); //проверяет окресности переданной точки на переданном поле
void fillFieldAutomatic(playerField*);
void fillFieldManual(playerField*);
void showField(playerField*);
void setCursorPos(int, int);
void showFields(playerField*, playerField*);
//void makeShoot(playerField*, playerField*);
//bool shootChecker(playerField*, int, int);
//void game(); //входят функции intro, mainMenu все функции для игры
void mainMenu(playerField*, playerField*);
void consoleSize(); //задаёт размер окна
bool terminator_1(playerField*);
bool terminator_2(playerField*);
void startGame(playerField*, playerField*, int);
bool shooter(playerField*, playerField*); //представляем навигацию по вражескому полю
bool winChecker(playerField*, playerField*);
void accountant(playerField*, int, int); // запись информации по выстрелу
void arealMarker(playerField*, ship);


int main()
{

	srand(time(NULL));
	consoleSize();

	//скрывает мигающий курсор - ????????????
	void* handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(handle, &structCursorInfo);
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &structCursorInfo);


	playerField field_1 = createPlayerField();
	playerField* field_1_ptr = &field_1;
	playerField field_2 = createPlayerField();
	playerField* field_2_ptr = &field_2;

	//intro();
	while (_kbhit()) _getch();  //для игнорирования "случайного" воода во время заставки, чтобы он не отразился на вводе в меню
	mainMenu(field_1_ptr, field_2_ptr);

	return 0;
}

//+создание изначальных полей
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

//+проверка окресности поля
bool checkArea(playerField* field, int X, int Y)
{
	for (int i = X - 1; i <= X + 1; i++) {
		for (int c = Y - 1; c <= Y + 1; c++) {
			if (i < 0 || c < 0 || i > 9 || c > 9 || field->field[i][c] == '~' || field->field[i][c] != '#') {
				continue;
			} else {
				return false;
			}
		}
	}
	return true;
}

//+заполнение поля кораблями вручную 
void fillFieldManual(playerField* field)
{
	system("CLS");

	int coordX, coordY, shipNum, coordX2, coordY2, coordX3, coordY3, coordX4, coordY4;
	char entering;
	bool rota = true;
	enum { UP = 72, DOWN = 80, RIGHT = 77, LEFT = 75, ENTER = '\r', ROTATION = 'r' };

	showField(field);

	//четырёхпалубные
	shipNum = 0;
	coordX = 8;
	coordY = 35;

	setCursorPos(8, 35);
	std::cout << "* * * *";

	while (shipNum < 1) {
		entering = _getch();

		if (entering == UP) {
			PlaySound(TEXT("musica\\button.wav"), NULL, SND_FILENAME | SND_ASYNC);
			system("CLS");

			if (coordX > 8) coordX -= 1;
			
			showField(field);

			if (rota) {
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX, coordY + 2);
				std::cout << "*";
				setCursorPos(coordX, coordY + 4);
				std::cout << "*";
				setCursorPos(coordX, coordY + 6);
				std::cout << "*";
			} else {
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY);
				std::cout << "*";
				setCursorPos(coordX + 2, coordY);
				std::cout << "*";
				setCursorPos(coordX + 3, coordY);
				std::cout << "*";
			}
		} else if (entering == DOWN) {
			PlaySound(TEXT("musica\\button.wav"), NULL, SND_FILENAME | SND_ASYNC);
			system("CLS");

			showField(field);

			if (rota) {
				if (coordX < 17) coordX += 1;
				
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX, coordY + 2);
				std::cout << "*";
				setCursorPos(coordX, coordY + 4);
				std::cout << "*";
				setCursorPos(coordX, coordY + 6);
				std::cout << "*";
			} else {
				if (coordX < 14) coordX += 1;
				
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY);
				std::cout << "*";
				setCursorPos(coordX + 2, coordY);
				std::cout << "*";
				setCursorPos(coordX + 3, coordY);
				std::cout << "*";
			}
		} else if (entering == RIGHT) {
			PlaySound(TEXT("musica\\button.wav"), NULL, SND_FILENAME | SND_ASYNC);
			system("CLS");

			showField(field);

			if (rota) {
				if (coordY < 46) coordY += 2;
				
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX, coordY + 2);
				std::cout << "*";
				setCursorPos(coordX, coordY + 4);
				std::cout << "*";
				setCursorPos(coordX, coordY + 6);
				std::cout << "*";
			} else {
				if (coordY < 53) coordY += 2;
				
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY);
				std::cout << "*";
				setCursorPos(coordX + 2, coordY);
				std::cout << "*";
				setCursorPos(coordX + 3, coordY);
				std::cout << "*";
			}
		} else if (entering == LEFT) {
			PlaySound(TEXT("musica\\button.wav"), NULL, SND_FILENAME | SND_ASYNC);
			system("CLS");

			if (coordY > 35) coordY -= 2;
			
			showField(field);

			if (rota) {
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX, coordY + 2);
				std::cout << "*";
				setCursorPos(coordX, coordY + 4);
				std::cout << "*";
				setCursorPos(coordX, coordY + 6);
				std::cout << "*";
			} else {
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY);
				std::cout << "*";
				setCursorPos(coordX + 2, coordY);
				std::cout << "*";
				setCursorPos(coordX + 3, coordY);
				std::cout << "*";
			}
		}
		else if (entering == ENTER) {
			PlaySound(TEXT("musica\\button.wav"), NULL, SND_FILENAME | SND_ASYNC);
			system("CLS");

			if (rota) {
				coordX = coordX - 8;
				coordY = (coordY - 35) / 2;
				coordX2 = coordX;
				coordY2 = coordY + 1;
				coordX3 = coordX;
				coordY3 = coordY + 2;
				coordX4 = coordX;
				coordY4 = coordY + 3;
			}
			else {
				coordX = coordX - 8;
				coordY = (coordY - 35) / 2;
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
				showField(field);
			} else {
				showField(field);
				PlaySound(TEXT("musica\\error.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
		} else if (entering == ROTATION) {
			PlaySound(TEXT("musica\\button.wav"), NULL, SND_FILENAME | SND_ASYNC);

			if (coordX + 3 > 17 || coordY + 4 > 52) continue;
			
			system("CLS");

			showField(field);

			rota = !rota;

			if (rota) {
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX, coordY + 2);
				std::cout << "*";
				setCursorPos(coordX, coordY + 4);
				std::cout << "*";
				setCursorPos(coordX, coordY + 6);
				std::cout << "*";
			}
			else {
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY);
				std::cout << "*";
				setCursorPos(coordX + 2, coordY);
				std::cout << "*";
				setCursorPos(coordX + 3, coordY);
				std::cout << "*";
			}
		}
	}


	//трёхпалубные
	shipNum = 0;
	coordX = 8;
	coordY = 35;
	rota = true;

	setCursorPos(8, 35);
	std::cout << "* * *";

	while (shipNum < 2) {

		entering = _getch();

		if (entering == UP) {
			PlaySound(TEXT("musica\\button.wav"), NULL, SND_FILENAME | SND_ASYNC);
			system("CLS");

			if (coordX > 8) coordX -= 1;
			
			showField(field);

			if (rota) {
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX, coordY + 2);
				std::cout << "*";
				setCursorPos(coordX, coordY + 4);
				std::cout << "*";
			} else {
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY);
				std::cout << "*";
				setCursorPos(coordX + 2, coordY);
				std::cout << "*";
			}
		} else if (entering == DOWN) {
			PlaySound(TEXT("musica\\button.wav"), NULL, SND_FILENAME | SND_ASYNC);
			system("CLS");

			showField(field);

			if (rota) {
				if (coordX < 17) coordX += 1;
				
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX, coordY + 2);
				std::cout << "*";
				setCursorPos(coordX, coordY + 4);
				std::cout << "*";
			} else {
				if (coordX < 15) coordX += 1;
				
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY);
				std::cout << "*";
				setCursorPos(coordX + 2, coordY);
				std::cout << "*";
			}
		} else if (entering == RIGHT) {
			PlaySound(TEXT("musica\\button.wav"), NULL, SND_FILENAME | SND_ASYNC);
			system("CLS");
			showField(field);

			if (rota) {
				if (coordY < 48) coordY += 2;
				
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX, coordY + 2);
				std::cout << "*";
				setCursorPos(coordX, coordY + 4);
				std::cout << "*";
			} else {
				if (coordY < 53) coordY += 2;
				
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY);
				std::cout << "*";
				setCursorPos(coordX + 2, coordY);
				std::cout << "*";
			}
		} else if (entering == LEFT) {
			PlaySound(TEXT("musica\\button.wav"), NULL, SND_FILENAME | SND_ASYNC);
			system("CLS");

			if (coordY > 35) coordY -= 2;
			
			showField(field);

			if (rota) {
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX, coordY + 2);
				std::cout << "*";
				setCursorPos(coordX, coordY + 4);
				std::cout << "*";
			} else {
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY);
				std::cout << "*";
				setCursorPos(coordX + 2, coordY);
				std::cout << "*";
			}
		} else if (entering == ENTER) {
			PlaySound(TEXT("musica\\button.wav"), NULL, SND_FILENAME | SND_ASYNC);
			system("CLS");

			if (rota) {
				coordX2 = coordX;
				coordY2 = coordY + 2;
				coordX3 = coordX;
				coordY3 = coordY + 4;
			}
			else {
				coordX2 = coordX + 1;
				coordY2 = coordY;
				coordX3 = coordX + 2;
				coordY3 = coordY;
			}

			if (checkArea(field, coordX - 8, (coordY - 35) / 2) && checkArea(field, coordX2 - 8, (coordY2 - 35) / 2) && checkArea(field, coordX3 - 8, (coordY3 - 35) / 2)) {
				field->field[coordX - 8][(coordY - 35) / 2] = '#';
				field->field[coordX2 - 8][(coordY2 - 35) / 2] = '#';
				field->field[coordX3 - 8][(coordY3 - 35) / 2] = '#';

				field->ship3[shipNum].size = 3;
				field->ship3[shipNum].decCoord = new int* [3];
				field->ship3[shipNum].decCoord[0] = new int[2];
				field->ship3[shipNum].decCoord[1] = new int[2];
				field->ship3[shipNum].decCoord[2] = new int[2];
				field->ship3[shipNum].decCoord[0][0] = coordX - 8;
				field->ship3[shipNum].decCoord[0][1] = (coordY - 35) / 2;
				field->ship3[shipNum].decCoord[1][0] = coordX2 - 8;
				field->ship3[shipNum].decCoord[1][1] = (coordY2 - 35) / 2;
				field->ship3[shipNum].decCoord[2][0] = coordX3 - 8;
				field->ship3[shipNum].decCoord[2][1] = (coordY3 - 35) / 2;
				field->ship3[shipNum].decStat = new bool[3];
				field->ship3[shipNum].decStat[0] = true;
				field->ship3[shipNum].decStat[1] = true;
				field->ship3[shipNum].decStat[2] = true;
				field->ship3[shipNum].genStat = 1;
				shipNum++;
				showField(field);
			} else {
				showField(field);
				PlaySound(TEXT("musica\\error.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
		} else if (entering == ROTATION) {
			PlaySound(TEXT("musica\\button.wav"), NULL, SND_FILENAME | SND_ASYNC);

			if (coordX + 2 > 17 || coordY + 4 > 53) continue;
			
			system("CLS");
			showField(field);

			rota = !rota;

			if (rota) {
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX, coordY + 2);
				std::cout << "*";
				setCursorPos(coordX, coordY + 4);
				std::cout << "*";
			} else {
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY);
				std::cout << "*";
				setCursorPos(coordX + 2, coordY);
				std::cout << "*";
			}
		}
	}


	//двухпалубные
	shipNum = 0;
	coordX = 8;
	coordY = 35;
	rota = true;

	setCursorPos(8, 35);
	std::cout << "* *";

	while (shipNum < 3) {

		entering = _getch();

		if (entering == UP) {
			PlaySound(TEXT("musica\\button.wav"), NULL, SND_FILENAME | SND_ASYNC);
			system("CLS");

			if (coordX > 8) coordX -= 1;
			
			showField(field);

			if (rota) {
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX, coordY + 2);
				std::cout << "*";
			} else {
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY);
				std::cout << "*";
			}
		} else if (entering == DOWN) {
			PlaySound(TEXT("musica\\button.wav"), NULL, SND_FILENAME | SND_ASYNC);
			system("CLS");

			showField(field);

			if (rota) {
				if (coordX < 17) coordX += 1;
				
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX, coordY + 2);
				std::cout << "*";
			} else {
				if (coordX < 16) coordX += 1;
				
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY);
				std::cout << "*";
			}
		} else if (entering == RIGHT) {
			PlaySound(TEXT("musica\\button.wav"), NULL, SND_FILENAME | SND_ASYNC);
			system("CLS");

			showField(field);

			if (rota) {
				if (coordY < 50) coordY += 2;
				
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX, coordY + 2);
				std::cout << "*";
			} else {
				if (coordY < 53) coordY += 2;
				
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY);
				std::cout << "*";
			}
		} else if (entering == LEFT) {
			PlaySound(TEXT("musica\\button.wav"), NULL, SND_FILENAME | SND_ASYNC);
			system("CLS");

			if (coordY > 35) coordY -= 2;
			
			showField(field);

			if (rota) {
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX, coordY + 2);
				std::cout << "*";
			} else {
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY);
				std::cout << "*";
			}
		} else if (entering == ENTER) {
			PlaySound(TEXT("musica\\button.wav"), NULL, SND_FILENAME | SND_ASYNC);
			system("CLS");

			if (rota) {
				coordX2 = coordX;
				coordY2 = coordY + 2;
			}
			else {
				coordX2 = coordX + 1;
				coordY2 = coordY;
			}

			if (checkArea(field, coordX - 8, (coordY - 35) / 2) && checkArea(field, coordX2 - 8, (coordY2 - 35) / 2)) {
				field->field[coordX - 8][(coordY - 35) / 2] = '#';
				field->field[coordX2 - 8][(coordY2 - 35) / 2] = '#';
				
				field->ship2[shipNum].size = 2;
				field->ship2[shipNum].decCoord = new int* [2];
				field->ship2[shipNum].decCoord[0] = new int[2];
				field->ship2[shipNum].decCoord[1] = new int[2];
				field->ship2[shipNum].decCoord[0][0] = coordX - 8;
				field->ship2[shipNum].decCoord[0][1] = (coordY - 35) / 2;
				field->ship2[shipNum].decCoord[1][0] = coordX2 - 8;
				field->ship2[shipNum].decCoord[1][1] = (coordY2 - 35) / 2;
				field->ship2[shipNum].decStat = new bool[2];
				field->ship2[shipNum].decStat[0] = true;
				field->ship2[shipNum].decStat[1] = true;
				field->ship2[shipNum].genStat = 1;
				shipNum++;
				showField(field);
			} else {
				showField(field);
				PlaySound(TEXT("musica\\error.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
		} else if (entering == ROTATION) {
			PlaySound(TEXT("musica\\button.wav"), NULL, SND_FILENAME | SND_ASYNC);
			if (coordX + 1 > 17 || coordY + 2 > 53) continue;
			
			system("CLS");

			showField(field);

			rota = !rota;

			if (rota) {
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX, coordY + 2);
				std::cout << "*";
			} else {
				setCursorPos(coordX, coordY);
				std::cout << "*";
				setCursorPos(coordX + 1, coordY);
				std::cout << "*";
			}
		}
	}

	//однопалубные
	shipNum = 0;
	coordX = 8;
	coordY = 35;

	setCursorPos(8, 35);
	std::cout << "*";

	while (shipNum < 4) {

		entering = _getch();

		if (entering == UP) {
			PlaySound(TEXT("musica\\button.wav"), NULL, SND_FILENAME | SND_ASYNC);
			system("CLS");

			if (coordX > 8) coordX -= 1;
			
			showField(field);

			setCursorPos(coordX, coordY);
			std::cout << '*';
		} else if (entering == DOWN) {
			PlaySound(TEXT("musica\\button.wav"), NULL, SND_FILENAME | SND_ASYNC);
			system("CLS");

			if (coordX < 17) coordX += 1;
			
			showField(field);

			setCursorPos(coordX, coordY);
			std::cout << '*';
		} else if (entering == RIGHT) {
			PlaySound(TEXT("musica\\button.wav"), NULL, SND_FILENAME | SND_ASYNC);
			system("CLS");

			if (coordY < 53) coordY += 2;
			
			showField(field);

			setCursorPos(coordX, coordY);
			std::cout << '*';
		} else if (entering == LEFT) {
			PlaySound(TEXT("musica\\button.wav"), NULL, SND_FILENAME | SND_ASYNC);
			system("CLS");
			if (coordY > 35) coordY -= 2;
			
			showField(field);

			setCursorPos(coordX, coordY);
			std::cout << '*';
		} else if (entering == ENTER) {
			PlaySound(TEXT("musica\\button.wav"), NULL, SND_FILENAME | SND_ASYNC);
			system("CLS");

			if (checkArea(field, coordX - 8, (coordY - 35) / 2)) {
				field->field[coordX - 8][(coordY - 35) / 2] = '#';

				field->ship1[shipNum].size = 1;
				field->ship1[shipNum].decCoord = new int* [1];
				field->ship1[shipNum].decCoord[0] = new int[2];
				field->ship1[shipNum].decCoord[0][0] = coordX - 8;
				field->ship1[shipNum].decCoord[0][1] = (coordY - 35) / 2;
				field->ship1[shipNum].decStat = new bool[1];
				field->ship1[shipNum].decStat[0] = true;
				field->ship1[shipNum].genStat = 1;
				shipNum++;
				showField(field);
			} else {
				showField(field);
				PlaySound(TEXT("musica\\error.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
		}
	}
}

//+заполнение полей автоматически
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
			} else if (dir == 1) {
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
			} else if (dir == 2) {
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
			} else {
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
			} else if (dir == 1) {
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
			} else if (dir == 2) {
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
			} else {
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
			} else if (dir == 1) {
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
			} else if (dir == 2) {
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
			} else {
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

//-вывод поля для заполнения кораблями вручную
void showField(playerField* field)
{
	char sea = 176;
	char ship = 254;

	std::cout << "\n\n\n\n\n\t\t\t\t    Arrange the ships." << std::endl;
	std::cout << "\t\t\t\t\t" << field->playerName << std::endl;

	std::cout << "\t\t\t\t   A B C D E F G H I J" << std::endl;

	for (int i = 0; i < 10; i++) {
		i != 9 ? std::cout << "\t\t\t\t " << i + 1 << " " : std::cout << "\t\t\t\t" << i + 1 << " ";
		for (int c = 0; c < 10; c++){
			if (field->field[i][c] == '~') {
				SetConsoleTextAttribute(hConsole, 9);
				std::cout << sea << " ";
			} else if (field->field[i][c] == '#') {
				SetConsoleTextAttribute(hConsole, 10);
				std::cout << ship << " ";
			} else {
				std::cout << field->field[i][c] << " ";
			}
			SetConsoleTextAttribute(hConsole, 7);
		}
		std::cout << std::endl;
	}

	std::cout << "\n\t\t\tr - for rotation, arrow keys for navigation" << std::endl;
}

//-показывает оба игровых поля
void showFields(playerField* field1, playerField* field2)
{

	char sea = 176;
	char ship = 254;
	char destr = 206;
	char miss = 176;
	char mist = 178;

	std::cout << "\n\n\n";
	std::cout << "\t\t\t\t" << field1->playerName << "\t\t\t\t" << field2->playerName << std::endl;
	std::cout << "\t\t\t   A B C D E F G H I J \t\t   A B C D E F G H I J " << std::endl;

	for (int i = 0; i < 10; i++) {

		i != 9 ? std::cout << "\t\t\t " << i + 1 << " " : std::cout << "\t\t\t" << i + 1 << " ";

		for (int c = 0; c < 10; c++) {
			if (field1->field[i][c] == '~' || field1->field[i][c] == '0') {
				SetConsoleTextAttribute(hConsole, 9);
				std::cout << sea << " ";
			}
			else if (field1->field[i][c] == '#') {
				SetConsoleTextAttribute(hConsole, 10);
				std::cout << ship << " ";
			}
			else if (field1->field[i][c] == 'X') {
				SetConsoleTextAttribute(hConsole, 4);
				std::cout << field1->field[i][c] << " ";
			}
			else {
				std::cout << field1->field[i][c] << " ";
			}
		}

		SetConsoleTextAttribute(hConsole, 7);

		std::cout << "\t\t";
		i != 9 ? std::cout << " " << i + 1 << " " : std::cout << i + 1 << " ";

		for (int c = 0; c < 10; c++) {
			if (field2->field[i][c] == '~' || field2->field[i][c] == '#') {
				SetConsoleTextAttribute(hConsole, 9);
				std::cout << mist << " ";  //заменить на mist - туман войны
			}
			else if (field2->field[i][c] == '0') {
				SetConsoleTextAttribute(hConsole, 9);
				std::cout << miss << " ";
			}
			else if (field2->field[i][c] == 'X') {
				SetConsoleTextAttribute(hConsole, 4);
				std::cout << field2->field[i][c] << " ";
			}
			else {
				std::cout << field2->field[i][c] << " ";
			}
		}

		SetConsoleTextAttribute(hConsole, 7);

		std::cout << std::endl;
	}

	std::cout << std::endl;
	std::cout << "\t\t\t   Remaining ships: \t\t   Remaining ships: " << std::endl;
	std::cout << "\t\t\t    Single-deck - " << field1->ship1[0].genStat + field1->ship1[1].genStat + field1->ship1[2].genStat + field1->ship1[3].genStat;
	std::cout << "\t\t Single-deck - " << field2->ship1[0].genStat + field2->ship1[1].genStat + field2->ship1[2].genStat + field2->ship1[3].genStat << std::endl;
	std::cout << "\t\t\t    Double-deck - " << field1->ship2[0].genStat + field1->ship2[1].genStat + field1->ship2[2].genStat;
	std::cout << "\t\t Double-deck - " << field2->ship2[0].genStat + field2->ship2[1].genStat + field2->ship2[2].genStat << std::endl;
	std::cout << "\t\t\t    Three-deck - " << field1->ship3[0].genStat + field1->ship3[1].genStat;
	std::cout << "\t\t\t Three-deck - " << field2->ship3[0].genStat + field2->ship3[1].genStat << std::endl;
	std::cout << "\t\t\t    Four-deck - " << field1->ship4[0].genStat;
	std::cout << "\t\t\t Four-deck - " << field2->ship4[0].genStat << std::endl;

}

//+установка курсора в переданную позицию по координатам
void setCursorPos(int y, int x)
{
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x, y };
	SetConsoleCursorPosition(output, pos);
}

//пока не решил что делать с этим
/* 
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
*/

//-главное меню игры
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

				PlaySound(TEXT("musica\\menu_beep.wav"), NULL, SND_FILENAME | SND_ASYNC);
				system("CLS");

				std::cout << "\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t1. Player vs player." << std::endl;
				std::cout << "\t\t\t\t\t2. Player vs PC (easy)." << std::endl;
				std::cout << "\t\t\t\t\t3. Player vs PC (hard)." << std::endl;
				std::cout << "\t\t\t\t\t3. PC vs PC." << std::endl;
				std::cout << "\t\t\t\t\t0. Exit." << std::endl;
				
				choose = _getch();

				switch (choose) {
					case '1':	//игрок против игрока

						PlaySound(TEXT("musica\\menu_beep.wav"), NULL, SND_FILENAME | SND_ASYNC);
						system("CLS");

						std::cout << "\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t1. Arrange ships manually Player1." << std::endl;
						std::cout << "\t\t\t\t\t2. Arrange ships automatically Player1." << std::endl;
						std::cout << "\t\t\t\t\t0. Exit." << std::endl;

						choose = _getch();

						switch (choose) {
							case '1':
								PlaySound(TEXT("musica\\menu_beep.wav"), NULL, SND_FILENAME | SND_ASYNC);
								//функция для ручного расставления кораблей для игрока 1
								fillFieldManual(field_1);
							break;
							case '2':
								PlaySound(TEXT("musica\\menu_beep.wav"), NULL, SND_FILENAME | SND_ASYNC);
								//функция для автоматического расставления кораблей для игрока 1
								fillFieldAutomatic(field_1);
							break;
							case '0':
								exit = false;
								break;
							default:
							break;
						}

						system("CLS");

						std::cout << "\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t1. Arrange ships manually Player2." << std::endl;
						std::cout << "\t\t\t\t\t2. Arrange ships automatically Player2." << std::endl;
						std::cout << "\t\t\t\t\t0. Exit." << std::endl;

						choose = _getch();

						switch (choose) {
							case '1':
								PlaySound(TEXT("musica\\menu_beep.wav"), NULL, SND_FILENAME | SND_ASYNC);
								//функция для ручного расставления кораблей для игрока 2
								fillFieldManual(field_2);
							break;
						case '2':
								PlaySound(TEXT("musica\\menu_beep.wav"), NULL, SND_FILENAME | SND_ASYNC);
								//функция для автоматического расставления кораблей для игрока 2
								fillFieldAutomatic(field_2);
							break;
						case '0':
							exit = false;
							break;
						default:
							break;
						}
						//начало игры игрок против игрока
						startGame(field_1, field_2, 0);
								
					break;
					case '2':	//игрок против PC (лёгкий)
						system("CLS");
						PlaySound(TEXT("musica\\menu_beep.wav"), NULL, SND_FILENAME | SND_ASYNC);

						std::cout << "\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t1. Arrange ships manually." << std::endl;
						std::cout << "\t\t\t\t\t2. Arrange ships automatically." << std::endl;
						std::cout << "\t\t\t\t\t0. Exit." << std::endl;

						choose = _getch();

						switch (choose) {
							case '1':
								PlaySound(TEXT("musica\\menu_beep.wav"), NULL, SND_FILENAME | SND_ASYNC);
								//функция для ручного расставления кораблей для игрока 1
								fillFieldManual(field_1);
								//функция для автоматического расставления кораблей для PC
								fillFieldAutomatic(field_2);
								//запуск игры с переданными полями
								startGame(field_1, field_2, 1);
							break;
							case '2':
								PlaySound(TEXT("musica\\menu_beep.wav"), NULL, SND_FILENAME | SND_ASYNC);
								//функция для автоматического расставления кораблей для игрока 1
								fillFieldAutomatic(field_1);
								//функция для автоматического расставления кораблей для PC
								fillFieldAutomatic(field_2);
								//запуск игры с переданными полями
								startGame(field_1, field_2, 1);
							break;
							case '0':
								PlaySound(TEXT("musica\\menu_beep.wav"), NULL, SND_FILENAME | SND_ASYNC);
								exit = false;
							break;
							default:
							break;
						}
					break;
					case '3':	//игрок против PC (сложный)
						system("CLS");
						PlaySound(TEXT("musica\\menu_beep.wav"), NULL, SND_FILENAME | SND_ASYNC);

						std::cout << "\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t1. Arrange ships manually." << std::endl;
						std::cout << "\t\t\t\t\t2. Arrange ships automatically." << std::endl;
						std::cout << "\t\t\t\t\t0. Exit." << std::endl;

						choose = _getch();

						switch (choose) {
						case '1':
							PlaySound(TEXT("musica\\menu_beep.wav"), NULL, SND_FILENAME | SND_ASYNC);
							//функция для ручного расставления кораблей для игрока 1
							fillFieldManual(field_1);
							//функция для автоматического расставления кораблей для PC
							fillFieldAutomatic(field_2);
							//запуск игры с переданными полями
							startGame(field_1, field_2, 2);
							break;
						case '2':
							PlaySound(TEXT("musica\\menu_beep.wav"), NULL, SND_FILENAME | SND_ASYNC);
							//функция для автоматического расставления кораблей для игрока 1
							fillFieldAutomatic(field_1);
							//функция для автоматического расставления кораблей для PC
							fillFieldAutomatic(field_2);
							//запуск игры с переданными полями
							startGame(field_1, field_2, 2);
							break;
						case '0':
							PlaySound(TEXT("musica\\menu_beep.wav"), NULL, SND_FILENAME | SND_ASYNC);
							exit = false;
							break;
						default:
							break;
						}
					break;
					case '4':  //PC vs PC
						system("CLS");
						PlaySound(TEXT("musica\\menu_beep.wav"), NULL, SND_FILENAME | SND_ASYNC);

						std::cout << "\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t1. Hard PC vs Easy PC" << std::endl;
						std::cout << "\t\t\t\t\t2. Easy PC vs Easy PC" << std::endl;
						std::cout << "\t\t\t\t\t3. Hard PC vs Hard PC" << std::endl;
						std::cout << "\t\t\t\t\t0. Exit." << std::endl;

						choose = _getch();

						fillFieldAutomatic(field_1);
						fillFieldAutomatic(field_2);

						switch (choose) {
						case '1':
							PlaySound(TEXT("musica\\menu_beep.wav"), NULL, SND_FILENAME | SND_ASYNC);
							startGame(field_1, field_2, 3);
							break;
						case '2':
							PlaySound(TEXT("musica\\menu_beep.wav"), NULL, SND_FILENAME | SND_ASYNC);
							startGame(field_1, field_2, 4);
							break;
						case '3':
							PlaySound(TEXT("musica\\menu_beep.wav"), NULL, SND_FILENAME | SND_ASYNC);
							startGame(field_1, field_2, 5);
							break;
						case '0':
							PlaySound(TEXT("musica\\menu_beep.wav"), NULL, SND_FILENAME | SND_ASYNC);
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
						
			break;
			case '0':
				exit = false;
			break;
			default:
			break;
			}
	}
}

//+задаёт размер окна консоли
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

//-рандомно ищёт корабли на поле, возвращает true в случае удачного выстрела, выводит информацию о попадании или промахе
bool terminator_1(playerField* field){

	char cont;
	while (true) {
		int x = rand() % 10;
		int y = rand() % 10;

		if (field->field[x][y] == '~') {
			field->field[x][y] = '0';
			system("CLS");
			while (true) {
				setCursorPos(8, 35);
				std::cout << "PC  is miss!" << std::endl;
				std::cout << "\t\t\t\tPress enter";
				cont = _getch();
				if (cont == '\r') {
					return false;
				}
			}
		} else if (field->field[x][y] == '#') {
			field->field[x][y] = 'X';
			accountant(field, x, y);
			system("CLS");
			while (true) {
				setCursorPos(8, 35);
				std::cout << "PC hit you!" << std::endl;
				std::cout << "\t\t\t\tPress enter";
				cont = _getch();
				if (cont == '\r') {
					return true;
				}
			}
		}
	}
}

//-"умный" бот, , возвращает true в случае удачного выстрела
bool terminator_2(playerField* field) { 
	

	
	
	
	return true; 
}

//-начинает игру с переданными параметрами
void startGame(playerField* field1, playerField* field2, int mode)
{

	//modes - 0 - игрок против игрока, 1 - игрок против PC, 2 - PC против PC
	system("CLS");
	if (mode == 0) {
		while (winChecker(field1, field2)) {
			while (shooter(field1, field2)) {
			}
			while (shooter(field2, field1)) {
			}
		}
	}
	else if (mode == 1) {
		while (winChecker(field1, field2)) {
			while (shooter(field1, field2)) {
			}
			while (terminator_1(field1)) {
			}
		}
	}
	else if (mode == 2) {

	}
	else {
		std::cout << "Error in block \"startGame\"";
	}
}

//-предоставляет возможность игроку сделать выстрел
bool shooter(playerField* field1, playerField* field2)
{
	
	int coordX, coordY;
	char entering;
	enum { UP = 72, DOWN = 80, RIGHT = 77, LEFT = 75, ENTER = '\r'};

	coordX = 5;
	coordY = 59;

	while (winChecker(field1, field2)) {
		system("CLS");

		showFields(field1, field2);

		setCursorPos(coordX, coordY);

		std::cout << '+';


		entering = _getch();

		if (entering == UP) {
			if (coordX > 5) {
				coordX -= 1;
			}
		} else if (entering == DOWN) {
			if (coordX < 14) {
				coordX += 1;
			}
		} else if (entering == RIGHT) {
			if (coordY < 77) {
				coordY += 2;
			}
		} else if (entering == LEFT) {
			if (coordY > 59) {
				coordY -= 2;
			}
		} else if (entering == ENTER) {
			if (field2->field[coordX - 5][(coordY - 59) / 2] == '#') {
				field2->amount--;
				field2->field[coordX - 5][(coordY - 59) / 2] = 'X';
				accountant(field2, coordX - 5, (coordY - 59) / 2);
				system("CLS");
				std::cout << "Nice shoot!" << std::endl;
				Sleep(2000);
			} else if(field2->field[coordX - 5][(coordY - 59) / 2] == '~') {
				field2->field[coordX - 5][(coordY - 59) / 2] = '0';
				system("CLS");
				std::cout << "You miss!" << std::endl;
				Sleep(2000);
				return false;
			}
			else {
				PlaySound(TEXT("musica\\error.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
		}
	}
}

//-производит проверку выйгрыша в игре (окончания)
bool winChecker(playerField* field1, playerField* field2)
{
	if (field1->amount == 0) {
		std::cout << field2->playerName << " is wictory!!!";
		return false;
	}

	if (field2->amount == 0) {
		std::cout << field1->playerName << " is wictory!!!";
		return false;
	}

	return true;
}

//+заносит информацию о поражении корабля по координатам, записывает информацию, если корабль уничтожен, и вызывает функцию для заполнения полей вокруг уничтоженнго корабля
void accountant(playerField* field, int x, int y)
{
	//однопалубники
		for (int i = 0; i < 4; i++) {
			if (field->ship1[i].genStat == 0) continue;  // пропуск, если корабль уже уничтожен
			if (field->ship1[i].decCoord[0][0] == x && field->ship1[i].decCoord[0][1] == y) {
				field->ship1[i].decStat[0] = false;
				field->ship1[i].genStat = 0;
				arealMarker(field, field->ship1[i]);
				return;
			}
		}

		//двухпалубники
		for (int i = 0; i < 3; i++) {
			if (field->ship2[i].genStat == 0) continue;  // пропуск, если корабль уже уничтожен
			for (int c = 0; c < 2; c++) {
				if (field->ship2[i].decCoord[c][0] == x && field->ship2[i].decCoord[c][1] == y) {
					std::cout << field->ship2[i].decCoord[c][0] << "  " << field->ship2[i].decCoord[c][1] << std::endl;
					field->ship2[i].decStat[c] = false;
				}
			}
			if (field->ship2[i].decStat[0] == false && field->ship2[i].decStat[1] == false) {
				field->ship2[i].genStat = 0;
				arealMarker(field, field->ship2[i]);
				return;
			}
		}
		
		//трёхпалубники
		for (int i = 0; i < 2; i++) {
			if (field->ship3[i].genStat == 0) continue;  // пропуск, если корабль уже уничтожен
			for (int c = 0; c < 3; c++) {
				if (field->ship3[i].decCoord[c][0] == x && field->ship3[i].decCoord[c][1] == y) {
					field->ship3[i].decStat[c] = false;
				}
			}
			if (field->ship3[i].decStat[0] == false && field->ship3[i].decStat[1] == false && field->ship3[i].decStat[2] == false) {
				field->ship3[i].genStat = 0;
				arealMarker(field, field->ship3[i]);
				return;
			}
		}

		//четырехпалубник
		if (!(field->ship4[0].genStat == 0)) {
			for (int c = 0; c < 4; c++) {
				if (field->ship4[0].decCoord[c][0] == x && field->ship4[0].decCoord[c][1] == y) {
					field->ship4[0].decStat[c] = false;
				}
			}
			if (field->ship4[0].decStat[0] == false && field->ship4[0].decStat[1] == false && field->ship4[0].decStat[2] == false && field->ship4[0].decStat[3] == false) {
				field->ship4[0].genStat = 0;
				arealMarker(field, field->ship4[0]);
				return;
			}
		}
}

//+заполянет поля вокруг уничтоженного корабля
void arealMarker(playerField* field, ship ship)
{
	for (int i = 0; i < ship.size; i++) {
		for (int g = ship.decCoord[i][0] - 1; g <= ship.decCoord[i][0] + 1; g++) {
			for (int c = ship.decCoord[i][1] - 1; c <= ship.decCoord[i][1] + 1; c++) {
				if (g < 0 || c < 0 || g > 9 || c > 9 || field->field[g][c] == 'X') {
					continue;
				}
				else {
					field->field[g][c] = '0';
				}
			}
		}
	}
}

//-начальная заставка
void intro()
{

	PlaySound(TEXT("musica\\intro.wav"), NULL, SND_FILENAME | SND_ASYNC);

	std::string logo[19] = { "\n\n                               $$$$$$$$\\ $$$$$$$\\ $$$$$$\\  $$$$$$\\                              ",
							"                               $$  _____|$$  __$$\\\\_$$  _|$$  __$$\\                             ",
							"                               $$ |      $$ |  $$ | $$ |  $$ /  \\__|                            ",
							"                               $$$$$\\    $$$$$$$  | $$ |  $$ |                                  ",
							"                               $$  __|   $$  ____/  $$ |  $$ |                                  ",
							"                               $$ |      $$ |       $$ |  $$ |  $$\\                             ",
							"                               $$$$$$$$\\ $$ |     $$$$$$\\ \\$$$$$$  |                            ",
							"                               \\________|\\__|     \\______| \\______/                             ",
							"",
							"",
							"",
							"  $$$$$$\\  $$$$$$$$\\  $$$$$$\\        $$$$$$$\\   $$$$$$\\ $$$$$$$$\\ $$$$$$$$\\ $$\\       $$$$$$$$\\ ",
							" $$  __$$\\ $$  _____|$$  __$$\\       $$  __$$\\ $$  __$$\\\\__$$  __|\\__$$  __|$$ |      $$  _____|",
							" $$ /  \\__|$$ |      $$ /  $$ |      $$ |  $$ |$$ /  $$ |  $$ |      $$ |   $$ |      $$ |      ",
							" \\$$$$$$\\  $$$$$\\    $$$$$$$$ |      $$$$$$$\\ |$$$$$$$$ |  $$ |      $$ |   $$ |      $$$$$\\    ",
							"  \\____$$\\ $$  __|   $$  __$$ |      $$  __$$\\ $$  __$$ |  $$ |      $$ |   $$ |      $$  __|   ",
							" $$\\   $$ |$$ |      $$ |  $$ |      $$ |  $$ |$$ |  $$ |  $$ |      $$ |   $$ |      $$ |      ",
							" \\$$$$$$  |$$$$$$$$\\ $$ |  $$ |      $$$$$$$  |$$ |  $$ |  $$ |      $$ |   $$$$$$$$\\ $$$$$$$$\\ ",
							"  \\______/ \\________|\\__|  \\__|      \\_______/ \\__|  \\__|  \\__|      \\__|   \\________|\\________|" };

	for (int i = 0; i < 15; i++) {
		for (auto str : logo) {
			SetConsoleTextAttribute(hConsole, rand() % 15 + 1);

			std::cout << str << std::endl;
		}

		SetConsoleTextAttribute(hConsole, 7);

		std::cout << "\n\n\t\t\t\tGame will start untill " << 15 - i << " sec.";

		Sleep(1000);

		system("CLS");
	}
}
