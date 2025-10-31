
/******************************************************************************
~КУРСОВА РОБОТА~
НА ТЕМУ:"Розробка програми ведення ділового щоденника "
-ОСНОВНІ КЛАСИ:
подія (назва, тип події, дата, час проведення, посилання на файл з описом події),
список подій
-ОСНОВНІ ФУНКУІЇ:
ведення списку подій, перевірка, чи не збігаються декілька подій в часі,
пошук подій за різними ознаками, ведення довідника посад типів подій.
*****************************************************************/
#include <windows.h>
#include <iostream>
#include <locale>
#include <codecvt>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm> // для std::find

using namespace std;
class Event                          //Клас подія
{
private:
	string titleType;                 // назва події
	string eventType;                 // тип події
	string data;                      // дата події
	string time_event;                // час проведення події
	string file_name=" ";
public:
	//Конструктор зазамовчуванням
	Event() {}
	//Конструктор параметризований 
	Event(string t, string e, string d, string tim, const string& file) :
		titleType(t), eventType(e), data(d), time_event(tim), file_name(file) {}
	// Сетeер
	void setTitleType(const string& t) { titleType = t; }
	void setEventType(const string& e) { eventType = e; }
	void setData(const string& d) { data = d; }
	void setTime_event(const string& tim) { time_event = tim; }
	void setFile_name(const string& f) { file_name = f; }
	// Геттер
	string getTitleType() const { return titleType; }
	string getEventType() const { return eventType; }
	string getDate() const { return data; }
	string getTimeEvent() const { return time_event; }
	string getFileName() const { return file_name; }
	//Cтворення файлу    
	void createFile()
	{
		ofstream file(file_name);

		if (!file) {
			cerr << "\xE2\x9C\x96  Помилка створення файлу!" << endl;
			exit(1);
		}
		else {
			cout << "\xE2\x9C\x94 Файл: " << file_name << " успішно створене!" << endl;
		}

	}
	// Запис в файл
	void  writeFile(const string& text)
	{
		ofstream     file(file_name, ios::app);  // Відкриваємо файл у режимі допису

		if (file) {
			file << text << endl;
			cout << " Дані записані у файл '" << file_name << "'" << endl;
		}
		else {
			cerr << " Помилка запису у файл!" << endl;
		}
		file.close();
	}
	// Зчитування файлу  в консоль
	void readFile() const
	{
		ifstream file(file_name);
		if (!file.is_open())
		{
			cout << file_name << "не відкрився для зчитування" << endl;
			return;
			
		}
		while (file)
		{
			string line;
			getline(file, line);
			cout << line << endl;
		}
		file.close();
	}
	// Очищення вмісту файла    
	void clearFile()
	{
		ofstream file(file_name, ios::trunc);
		if (file.is_open()) {
			std::cout << "Файл '" << file_name << "' успішно очищено!" << std::endl;
			// Файл автоматично закриється при виході з області видимості
		}
		else {
			std::cerr << "Не вдалося відкрити файл '" << file_name << "' для очищення." << std::endl;
		}
		file.close();
	}
	//Прототип  перевантаження оператора виводу об'єктів    
	friend std::ostream& operator<<(std::ostream& out, const Event& e1);
	//Прототип  перевантаження оператора вводу об'єктів 
	friend std::istream& operator>>(std::istream& in, Event& e1);
	// Прототип перевантаження операторів порівняння
	friend bool operator== (const Event& e1, const Event& e2);
	friend bool operator!= (const Event& e1, const Event& e2);
};
//Визначення оператора виводу
std::ostream& operator<<(std::ostream& out, const Event& e1)
{
	out << "Назва події: " << e1.titleType << "\n"
		<< "Тип події: " << e1.eventType << "\n"
		<< "Дата: " << e1.data << "\n"
		<< "Час: " << e1.time_event << "\n"
		<< "Ім'я файлу: " << e1.file_name << "\n";
	return out;
}
//Визначення операторів порівняння
bool operator== (const Event& e1, const Event& e2)
{
	return (e1.time_event == e2.time_event);
}
bool operator!= (const Event& e1, const Event& e2)
{
	return !(e1.time_event == e2.time_event);
}
istream& operator>>(istream& in, Event& e1) {
	cout << "Назва події" << endl;
	getline(in, e1.titleType);
	if (e1.titleType.empty()) {
		cerr << "Error! Рядок назва події не може бути порожнім" << endl;
		return in;
	}
	cout << "Тип події" << endl;
	getline(in, e1.eventType);
	if (e1.eventType.empty()) {
		cerr << "Error! Рядок тип події не може бути порожнім" << endl;
		return in;
	}
	cout << "Дата в форматі дд:мм:рр : " << endl;
	getline(in, e1.data);
	if (e1.data.empty()) {
		cerr << "Error! Рядок дата не може бути порожнім" << endl;
		return in;
	}
	cout << "Час   в форматі 00:00 :" << endl;
	getline(in, e1.time_event);
	if (e1.time_event.empty()) {
		cerr << "Error! Рядок час не може бути порожнім" << endl;
		return in;
	}
	cout << "Назва файла з описом події:" << endl;
	getline(in, e1.file_name);
	if (e1.file_name.empty()) {
		cerr << "Error! Рядок з назвою файла не може бути порожнім" << endl;
		return in;
	}
	return in;
}
class List:public Event                  // Клас список подій 
{
public:
	vector<Event>elist;
	vector<string> p;	
	List(): p({
	 "Генеральний директор 	Управління	Визначає стратегію компанії, приймає ключові рішення	director@example.com",
	 "Фінансовий директор	Фінансовий відділ	Контролює фінансові операції, бюджетування	finance@example.com",
	  "HR-директор	Відділ кадрів	Відповідає за підбір, навчання та розвиток персоналу	hr@example.com",
	  "Керівник відділу продажів	Відділ продажів	Організовує роботу менеджерів з продажу	sales.head@example.com",
	 "Менеджер з продажу	Відділ продажів	Пошук клієнтів, укладання угод	sales@example.com",
	 "Маркетинг-директор	Відділ маркетингу	Визначає маркетингову стратегію	marketing@example.com",
	  "SMM-менеджер	Відділ маркетингу	Веде соцмережі, створює контент	smm@example.com",
	  "Керівник IT-відділу	IT-відділ	Контролює роботу ІТ-інфраструктури	it.head@example.com",
	  "Системний адміністратор	IT-відділ	Забезпечує роботу серверів та мереж	sysadmin@example.com",
	 "Керівник проєктів	Відділ управління проєктами	Координує реалізацію проєктів, контролює терміни	projects@example.com",
	  "Юрист	Юридичний відділ	Контролює договори, юридичні питання	legal@example.com",
	 "Бухгалтер	Фінансовий відділ	Веде бухгалтерський облік, фінансову звітність	accountant@example.com",
	  "Секретар	Адміністрація	Веде документообіг, організовує зустрічі	secretary@example.com"
		}) {}
		void print1()
	{
		for (const auto& event : elist) {
			cout
				<< "Назва: " << event.getTitleType() << ", "
				<< "Тип: " << event.getEventType() << ", "
				<< "Дата: " << event.getDate() << ", "
				<< "Час: " << event.getTimeEvent() << ", "
				<< "Файл: " << event.getFileName() << endl;
		}
	}
	//Метод для перевірки збігу  подій  в часі
	void timeCheck()
	{
		string time;
		cout << "Введіть час для пошуку" << endl;
		cin >> time;
		for (const auto& event : elist)
		{
			if (event.getTimeEvent() == time)
			{
				cout << "Error!збіг в часі  виявленно" << endl;
				return;
			}				
		}
		cout << "Ok!час вільний для створення нової події" << endl;
	}
	//Метод для пошуку подій за різними ознаками 
	 void  search()
	{
		if (elist.empty()) {
			cout << " У списку подій нічого немає! Додайте хоча б одну подію перед пошуком.\n";
			return;
		}		
		if (std::cin.fail()) // якщо попереднє вилучення не виконалося чи відбулося переповнення
		{
			cin.clear();
			cin.ignore(); // Очищаємо буфер
		}
		string t, e, d1, t1, f;
		cout << "Введіть назву (або Enter для пропуску): " << endl;
		cin.ignore();
		getline(cin, t);
		cout << "Введіть тип події (або Enter для пропуску): " << endl;
		getline(cin, e);

		cout << "Введіть дату події (або Enter для пропуску): " << endl;
		getline(cin, d1);

		cout << "Введіть час події (або Enter для пропуску): " << endl;
		getline(cin, t1);

		cout << "Введіть ім'я файлу (або Enter для пропуску): " << endl;
		//cin.ignore();
getline(cin, f);
		
		bool found = false; // Флаг для перевірки, чи знайдена хоча б одна подія

		for (const auto& event : elist)
		{
			// Перевірка умов для точного співпадіння
			if ((t.empty() || event.getTitleType() == t) &&
				(e.empty() || event.getEventType() == e) &&
				(d1.empty() || event.getDate() == d1) &&
				(t1.empty() || event.getTimeEvent() == t1) &&
				(f.empty() || event.getFileName() == f))
			{
				cout << "Співпадіння події знайдено:\n" << event << endl;
				found = true;
			}
		}
		if (!found)
		{
			cout << "Подію не знайдено" << endl;
		}
	}	
	// вивід списку посад
	void print() 
	{
		cout << "                               ДОВІДНИК ПОСАД " << endl;
		for (size_t i = 0; i < p.size(); ++i)
		{
			cout << i << "  " << p[i] << endl;
		}
	}
	// Додавання інформації до списку посад
	void add(const string& index)
	{
		p.push_back(index);
		cout << "Інформацію успішно додано" << endl;
	}
	// пошук конкретного рядку по посаді учасника в векторі
	void faind(const string& s)
	{
		auto it = std::find_if(p.begin(), p.end(), [&](const string& item)
			{ return item.find(s) != string::npos; }); // Шукаємо частковий збіг
		if (it != p.end())
		{
			std::cout << "Учасника додано до події: \n" << *it << std::endl;
		}
		else
		{
			std::cout << "Вказаної посади не знайдено!" << std::endl;
		}
	}
	//Метод для видалення запису в довіднику типів посад
	void deleteList(string s)
	{
		auto it = std::find_if(p.begin(), p.end(), [&](const string& item)
			{ return item.find(s) != string::npos; }); // Шукаємо частковий збіг
		if (it != p.end()) {
			p.erase(it);
			cout << "Запис успішно видалено з довіднику типів посад події" << endl;
		}
	}
	//метод для додавання події
	void addEvent()
	{
		Event p2;
		cin >> p2;
		elist.push_back(p2);//додається подія р2 в vector elist
		p2.createFile();
		string description; //опис до події
		cout << "Натисніть клавішу Enter! Опишіть пишіть подію в файлі :" << endl;
		cin.ignore();
		getline(cin, description);
		p2.writeFile(description);
	}
	//Метод для зчитування інформації з файла в консоль
	void read() {
		string file = ""; // файл який шукає користувач для виводу в консоль
		cout << "Введіть ім'я файла яке  потрібно зчитати в консоль:" << endl;
		cin.ignore();
		getline(cin,file);
		for (auto& event : elist)
		{
			if (file == event.getFileName()) {
				//cout << "Подія з файлом: " << event.getFileName() << std::endl;
				event.readFile(); // Викликаємо readFile для кожної події
			}
			else if(file != event.getFileName()) {
				cout << "Імя файла відсутнє :" << endl;
			}
		}
	}
	//Метод для очищення вмісту файла
	void clear()
	{
		string file1; // файл який шукає користувач для виводу в консоль
		cout << "Введіть ім'я файла яке  потрібно зчитати в консоль:" << endl;
		cin.ignore();
		getline(cin, file1);
		for (auto& event : elist)
		{
			if (file1 == event.getFileName()) {
				
				event.clearFile(); // Викликаємо readFile для файла вказаним користувачем назвою
			}
			else if (file1 != event.getFileName()) {
				cout << "Імя файла відсутнє :" << endl;
			}
		}
	}
	//Метод для запису інформації до файла
	void writeFile(const string& text, const string& text1)
	{
		
			ofstream     file(text1, ios::in);  // Відкриваємо файл у режимі допису

			if (file) {
				file << text << endl;
				cout << "Дані записані у файл '" << text1 << "'" << endl;
			}
			else {
				cerr << " Помилка запису у файл!" << endl;
			}
			file.close();	
	}
};
int main()
{
	
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	
	int choice1=0;  //використовуєтьс для вибору пункта меню
	List lis;
	Event event{" "," "," "," "," "};
	do
	{
		cout << "                          ~ ВІТАЄМО В МЕНЮ ДІЛОВОГО ЩОДЕННИКА: ~" << endl;
		cout << "1) Вибір посади з довіднику посад  та  створення події\n";
		cout << "2) Видалення запису з довіднику типів посад \n";
		cout << "3) Перевірити подію на співпадіння  в часі \n";
		cout << "4) Пошук подій за наступними ознаками: назва, тип події, дата, час проведення \n";
		cout << "5) Зчитати файл для виводу в консоль \n";
		cout << "6) Очистити  вмісту файла  \n";
		cout << "7) Зробити запис до файла \n" ;
		cout << "8) Додавання інформації в довідник списку посад \n";
		cout << "9) Вихід з програми\n";
		cout << "Будьласка зробіть вибір пункту меню користуючись цифрами 1...9 _";
		cin >> choice1;
		if (cin.fail()) {  // Перевірка на некоректний ввід (наприклад, літери)
			cin.clear();
			cout << "ПОМИЛКА: Введено некоректне значення! Спробуйте ще раз.\n";
			continue;
		}
		string s; // Дані учсника для видаленняз списку посад
		string member; // Дані учсника для додавання до списку посад
		string addText ;// зміна для додавання запису до файла пункт меню 7
		string chooseAFileName; //Обрати потрібне ім'я файлу пункт меню 7
		switch (choice1)
		{
		case 1:
			// 1)Створення довіднику посад типів подій                     
			lis.print();  //вивід списку посаді            
			char symbol;
			for (int i = 0; i < 20; i++) {
				string participant; // можливий учасник обирає користувач
				cin.ignore();
				cout << "Введіть посаду учасника:  ";
				getline(cin, participant);
				lis.faind(participant); // Шукаємо учасника в списку
				// Запитуємо, чи хоче користувач продовжити
				cout << "Продовжити вибір? (y/n): ";
				cin >> symbol;
				cin.ignore(); // Очищаємо буфер вводу			
				if (symbol == 'n') {
					break; // Вихід із циклу
				}
				else if (symbol != 'y') {
					cout << "Невідомий вибір! Введіть 'y' або 'n'." << endl;
					i--; // Повторний запит, якщо введено неправильний символ
				}
			}
			// 2) Створення події в довіднику  події
			lis.addEvent();
			lis.print1();
			char choice;
			while (true)
			{
				cout << "бажаєте додати ще подію? (так = y/ні = n): ";
				cin >> choice;
				cin.ignore();
				if (choice == 'n')
				{
					break;
				}
				else if (choice == 'y')
				{
					lis.addEvent();
				}
				else if (choice != 'y' && choice != 'n')
				{
					cout << "невідомий вибір! введіть 'y' або 'n'." << endl;
				}
			}		
			break;
		case 2:
			cout << "Введіть рядок який видалити" << endl;
			cin.ignore(); // Очищаємо буфер, якщо був попередній ввід
			getline(cin, s);
			lis.deleteList(s);
			lis.print1();  //вивід списку посаді
			break;
		case 3:
			lis.timeCheck();
			break;
		case 4:
			lis.search();
			break;
		case 5:
			lis.read();
			break;
		case 6:
			lis.clear();
			break;
		case 7:
		
			cin.ignore();
			cout << "Введіть текст файла_  ";
			getline(cin, addText);
			cout << "Введіть назву файла:  ";
			getline(cin, chooseAFileName);
			
				lis.writeFile(addText, chooseAFileName);
				
			break;
		
		case 8:
			cin.ignore(); // Очищаємо буфер, якщо був попередній ввід
			cout << "Введіть інформацію до довідника посад компанії: Посада; Відділ;Обов'язки;Електрона почта " << endl;
			getline(cin, member);
			lis.add(member);
			lis.print();
			break;
		case 9:
				cout << "ДЯКУЮ ЩО КОРИСТУЄТЕСЯ НАШИМ ДОВІДНИКОМ" << "\n";
				return 0;
			break;
		}
	} 
	while (choice1 != 9);
return 0;
}


// вирішити якщо назви файла не існує то 
