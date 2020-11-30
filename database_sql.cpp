#include <iostream>
#include "sqlite-amalgamation-3320300/sqlite3.h"
#include <string>
#include <cstring>
#include <iomanip>

void PrintMenu();
void PrintHeader();
void PrintEnding();
void CreateTable(sqlite3* db_connection, char* err);
void InsertStudent(sqlite3* db_connection, char* err);
void DeleteStudent(sqlite3* db_connection, char* err);
void UpdateStudent(sqlite3* db_connection, char* err, int SelectedValue);
void SearchStudent(sqlite3* db_connection, char* err, int SelectedValue);
void Sort(sqlite3* db_connection, char* err, int SelectedValue);
void TableOutput(sqlite3* db_connection, char* err);
int callback(void* NotUsed, int argc, char** argv, char** azColName);


int main()
{
	setlocale(LC_ALL, "Russian");

	sqlite3* db_connection = 0;
	char* err = 0;
	const char* db_file = "students.sqlite";
	int choice;
	int SelectedValue = 0;

	int open_result = sqlite3_open(db_file, &db_connection);

	if (open_result == 0)
	{
		std::cout << "Соединение к " << db_file << " установлено. \n" << std::endl;

		CreateTable(db_connection, err);

		do 
		{ 
			PrintMenu();
			std::cout << "Ваш выбор: ";
			std::cin >> choice;
			std::cout << std::endl;

			switch (choice)
			{
				case 1: 
					InsertStudent(db_connection, err);
					break;

				case 2:
					TableOutput(db_connection, err);
					break;

				case 3: 
					DeleteStudent(db_connection, err);
					break;

				case 4:
					UpdateStudent(db_connection, err, SelectedValue);
					break;

				case 5:
					SearchStudent(db_connection, err, SelectedValue);
					break;

				case 6:
					Sort(db_connection, err, SelectedValue);
					break;

				case 7:
					sqlite3_close(db_connection);
					break;

				default:
					std::cout << "Несуществующий пункт меню. Повторите ввод." << std::endl;
			}

			if (choice != 7)
				system("pause");
		}

		while (choice != 7);
	}
	else
	{
		std::cout << "Ошибка соединения к " << db_file << "." << std::endl;
	}

	return 0;
}

void PrintMenu()
{
	std::cout << "1 - Добавить новую запись о студенте в таблицу;" << std::endl;
	std::cout << "2 - Вывести таблицу с информацией о студентах;" << std::endl;
	std::cout << "3 - Удалить запись о студенте;" << std::endl;
	std::cout << "4 - Внести изменения в информацию о студенте;" << std::endl;
	std::cout << "5 - Поиск определенного студента;" << std::endl;
	std::cout << "6 - Сортировка таблицы с информацией о студентах;" << std::endl;
	std::cout << "7 - Закрыть соединение; \n" << std::endl;
}

void PrintHeader() 
{
	std::cout << "|| № || Имя и фамилия студента || Дата рождения студента || Группа, в которой учится студент ||" << std::endl;
	std::cout << "||---||------------------------||------------------------||----------------------------------||" << std::endl;
}

void PrintEnding()
{
	std::cout << "||---||------------------------||------------------------||----------------------------------||" << std::endl;
}

void CreateTable(sqlite3* db_connection, char* err)
{
	const char* query = "CREATE TABLE IF NOT EXISTS students (id INTEGER PRIMARY KEY AUTOINCREMENT, Name VARCHAR(50), BirthdayDate DATE, StudentGroup VARCHAR(20));";

	int exec_result = sqlite3_exec(db_connection, query, 0, 0, &err);

	if (exec_result == 0)
	{
		std::cout << "Запрос выполнен успешно. \n" << std::endl;
	}
	else
	{
		std::cout << "Ошибка запроса. Пояснение ошибки: " << err <<  std::endl;
	}
}

void InsertStudent(sqlite3* db_connection, char* err)
 {
	std::string StudentName;
	std::string StudentBirthdayDate;
	std::string StudentGroup;

	std::cin.get();
	std::cout << "Введите имя студента: ";
	std::getline(std::cin, StudentName);
	std::cout << "Введите дату рождения студента: ";
	std::getline(std::cin, StudentBirthdayDate);
	std::cout << "Введите имя группы, в которой учится студент: ";
	std::getline(std::cin, StudentGroup);

	std::string query = "INSERT INTO students (Name, BirthdayDate, StudentGroup) VALUES ('" + StudentName + "','" + StudentBirthdayDate + "', '" + StudentGroup + "');";

	int exec_result = sqlite3_exec(db_connection, query.c_str(), 0, 0, &err);

	if (exec_result == 0)
	{
		std::cout << "Запрос выполнен успешно. \n" << std::endl;
	}
	else
	{
		std::cout << "Ошибка запроса. Пояснение ошибки: " << err << std::endl;
	}
}

void TableOutput(sqlite3* db_connection, char* err)
{
	PrintHeader();
	const char* query = "SELECT * FROM students;";
	int exec_result = sqlite3_exec(db_connection, query, callback, 0, &err);
	PrintEnding();

	if (exec_result == 0)
	{
		std::cout << "Запрос выполнен успешно. \n" << std::endl;
	}
	else
	{
		std::cout << "Ошибка запроса. Пояснение ошибки: " << err << std::endl;
	}
}

int callback(void* NotUsed, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(azColName[i], "id") == 0)
		{
			std::cout << "||" << std::setw(3) << argv[i] << "||";
		}
		else
			if (strcmp(azColName[i], "Name") == 0)
			{
				std::cout << std::setw(24) << argv[i] << "||";
			}
			else
				if (strcmp(azColName[i], "BirthdayDate") == 0)
				{
					std::cout << std::setw(24) << argv[i] << "||";
				}
				else
					if (strcmp(azColName[i], "StudentGroup") == 0)
					{
						std::cout << std::setw(34) << argv[i] << "||" << std::endl;
					}
	}

	return 0;
}

void DeleteStudent(sqlite3* db_connection, char* err)
{
	std::string NumberOfString;

	std::cout << "Введите номер строки, которую хотите удалить - ";
	
	std::cin >> NumberOfString;

	std::string query = "DELETE FROM students WHERE id = " + NumberOfString + ";";

	int exec_result = sqlite3_exec(db_connection, query.c_str(), 0, 0, &err);

	if (exec_result == 0)
	{
		std::cout << "Запрос выполнен успешно. \n" << std::endl;
	}
	else
	{
		std::cout << "Ошибка запроса. Пояснение ошибки: " << err << std::endl;
	}
}

void UpdateStudent(sqlite3* db_connection, char* err, int SelectedValue)
{
	std::string NameOfColumn;
	std::string UpdValue;
	std::string NumberOfString;

	std::cout << "Введите номер строки, которую хотите изменить - ";
	std::cin >> NumberOfString;

	std::cout << "Информацию из какого столбца вы хотите поменять? \n 1 - Имя и фамилия студента \n 2 - Дата рождения студента \n 3 - Группа, в которой учится студент \n" << std::endl;
	
	std::cout << "Ваш выбор: ";
	std::cin >> SelectedValue;
	std::cout << std::endl;

	do
	{
		switch (SelectedValue)
		{
		case 1:
			NameOfColumn = "Name";
			std::cout << "Введите новые фамилию и имя студента: ";
			std::cin.get();
			std::getline(std::cin, UpdValue);
			SelectedValue = 0;
			break;

		case 2:
			NameOfColumn = "BirthdayDate";
			std::cout << "Введите новую дату рождения студента: ";
			std::cin >> UpdValue;
			SelectedValue = 0;
			break;

		case 3:
			NameOfColumn = "StudentGroup";
			std::cout << "Введите новую группу в которой учится студент: ";
			std::cin >> UpdValue;
			SelectedValue = 0;
			break;

		default:
			std::cout << "Несуществующий пункт меню. Повторите ввод." << std::endl;
		}

		if (SelectedValue > 0)
			system("pause");
	}
	
	while (SelectedValue > 0);

	std::string query = "UPDATE students SET " + NameOfColumn + " = '" + UpdValue + "' WHERE id = " + NumberOfString + ";";

	int exec_result = sqlite3_exec(db_connection, query.c_str(), 0, 0, &err);

	if (exec_result == 0)
	{
		std::cout << "Запрос выполнен успешно. \n" << std::endl;
	}
	else
	{
		std::cout << "Ошибка запроса. Пояснение ошибки: " << err << std::endl;
	}
}

void SearchStudent(sqlite3* db_connection, char* err, int SelectedValue)
{
	std::string NameOfColumn;
	std::string ReqValue;

	std::cout << "Выберите фильтр для поиска: " << std::endl;
	std::cout << "1 - Поиск по фамилии и имени студента;" << std::endl;
	std::cout << "2 - Поиск по дате рождения студента;" << std::endl;
	std::cout << "3 - Поиск по названию группы, в которой учится студент;" << std::endl;

	std::cout << "Ваш выбор: ";
	std::cin >> SelectedValue;
	std::cout << std::endl;

	switch (SelectedValue)
	{
		case 1:
			NameOfColumn = "Name";
			std::cout << "Введите фамилию и имя студента: ";
			std::cin.get();
			std::getline(std::cin, ReqValue);
			break;

		case 2:
			NameOfColumn = "BirthdayDate";
			std::cin.get();
			std::cout << "Введите дату рождения студента: ";
			std::getline(std::cin, ReqValue);
			break;

		case 3:
			NameOfColumn = "StudentGroup";
			std::cin.get();
			std::cout << "Введите название группы, в которой учится студент: ";
			std::getline(std::cin, ReqValue);
			break;

		default:
			std::cout << "Несуществующий пункт меню. Повторите ввод." << std::endl;
	}

	PrintHeader();
	std::string query = "SELECT * FROM students WHERE " + NameOfColumn + " = '" + ReqValue + "';";
	int exec_result = sqlite3_exec(db_connection, query.c_str(), callback, 0, &err);
	PrintEnding();

	if (exec_result == 0)
	{
		std::cout << "Запрос выполнен успешно. \n" << std::endl;
	}
	else
	{
		std::cout << "Ошибка запроса. Пояснение ошибки: " << err << std::endl;
	}
}

void Sort(sqlite3* db_connection, char* err, int SelectedValue)
{
	std::string NameOfColumn;
	std::string SortType;

	std::cout << "Выберите фильтр сортировки базы данных с информацией о студентах: \n" << std::endl;
	std::cout << "1 - Сортировка по фамилии и имени по алфавиту;" << std::endl;
	std::cout << "2 - Сортировка по фамилии и имени в обратном алфавиту порядке;" << std::endl;
	std::cout << "3 - Сортировка по дате рождения по возрастанию (от старшего к младшему);" << std::endl;
	std::cout << "4 - Сортировка по дате рождения по убыванию (от младшего к старшему);" << std::endl;
	std::cout << "5 - Сортировка по имени группы по алфавиту;" << std::endl;
	std::cout << "6 - Сортировка по имени группы в обратном алфавиту порядке;" << std::endl;

	std::cout << "Ваш выбор: ";
	std::cin >> SelectedValue;
	std::cout << std::endl;

	switch (SelectedValue)
	{
	case 1:
		NameOfColumn = "Name";
		SortType = "ASC";

		std::cout << "Сортировка по фамилии и имени по алфавиту: \n" << std::endl;
		break;

	case 2:
		NameOfColumn = "Name";
		SortType = "DESC";

		std::cout << "Сортировка по фамилии и имени в обратном алфавиту порядке: \n" << std::endl;
		break;

	case 3:
		NameOfColumn = "BirthdayDate";
		SortType = "ASC";

		std::cout << "Сортировка по дате рождения по возрастанию (от старшего к младшему): \n" << std::endl;
		break;

	case 4:
		NameOfColumn = "BirthdayDate";
		SortType = "DESC";

		std::cout << "Сортировка по дате рождения по возрастанию (от младшего к старшему): \n" << std::endl;
		break;

	case 5:
		NameOfColumn = "StudentGroup";
		SortType = "ASC";

		std::cout << "Сортировка по имени группы по алфавиту: \n" << std::endl;
		break;

	case 6:
		NameOfColumn = "StudentGroup";
		SortType = "DESC";

		std::cout << "Сортировка по имени группы в обратном алфавиту порядке: \n" << std::endl;
		break;
	}

	PrintHeader();
	std::string query = "SELECT * FROM students ORDER BY " + NameOfColumn + " " + SortType +";";
	int exec_result = sqlite3_exec(db_connection, query.c_str(), callback, 0, &err);
	PrintEnding();

	if (exec_result == 0)
	{
		std::cout << "Запрос выполнен успешно. \n" << std::endl;
	}
	else
	{
		std::cout << "Ошибка запроса. Пояснение ошибки: " << err << std::endl;
	}
}