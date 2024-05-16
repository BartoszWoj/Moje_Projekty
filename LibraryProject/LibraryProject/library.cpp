#define _CRT_SECURE_NO_WARNINGS
#include "classes.h"
#include <fstream>
#include <sstream>

using namespace std;

void Library::loadBooksData()
{
	ifstream booksFile("books.txt");
	if (booksFile.good())
	{
		int bID;
		string title, author, bIDstring, rentedString;
		bool rented;
		while (getline(booksFile, bIDstring, '\t'))
		{
			getline(booksFile, title, '\t');
			getline(booksFile, author, '\t');
			getline(booksFile, rentedString, '\n');
			stringstream SSbID(bIDstring);
			stringstream SSrented(rentedString);
			if ((SSbID >> bID) && (SSrented >> rented))
				booksData.push_back({ bID, title, author, rented });
			else
				throw 2;
		}
		booksFile.close();
	}
	else
		throw 1;
}
void Library::loadCustomersData()
{
	ifstream customersFile("customers.txt");
	if (customersFile.good())
	{
		int cID;
		string name, surname, toConvert;
		while (getline(customersFile, toConvert, '\t'))
		{
			stringstream ss(toConvert);
			if (ss >> cID)
			{
				getline(customersFile, name, '\t');
				getline(customersFile, surname, '\n');
				customersData.push_back({ cID, name, surname });
			}
			else
				throw 4;
		}
		customersFile.close();
	}
	else
		throw 3;
}
void Library::loadEventsData()
{
	ifstream eventsFile("events.txt");
	if (eventsFile.good())
	{
		int bID, cID;
		Date rentDate, returnDate;
		string bIDstring, cIDstring, date1String, date2String;
		while (getline(eventsFile, bIDstring, '\t'))
		{
			getline(eventsFile, cIDstring, '\t');
			getline(eventsFile, date1String, '\t');
			getline(eventsFile, date2String, '\n');
			stringstream SSbID(bIDstring);
			stringstream SScID(cIDstring);
			stringstream SSdate1(date1String);
			stringstream SSdate2(date2String);
			if ((SSbID >> bID) && (SScID >> cID) && (SSdate1 >> rentDate) && (SSdate2 >> returnDate))
			{
				Book* bookptr = nullptr;
				Customer* customerptr = nullptr;
				for (int i = 0; i < booksData.size(); i++)
				{
					if (bID == booksData[i].bookID)
						bookptr = &booksData[i];
				}
				for (int i = 0; i < customersData.size(); i++)
				{
					if (cID == customersData[i].customerID)
						customerptr = &customersData[i];
				}
				eventsData.push_back({ bookptr, customerptr, rentDate, returnDate });
			}
			else
				throw 6;
		}
		eventsFile.close();
	}
	else
		throw 5;
}
void Library::addBook(string title, string author)
{
	ofstream booksFile("books.txt", ofstream::app);
	if (booksFile.good())
	{
		int bID = booksData.size() + 1;
		booksData.push_back({ bID, title, author });
		booksFile << booksData.back() << '\n';

		booksFile.close();
	}
	else
		cout << "Error. Can't open books.txt file. Adding book canceled" << endl;
}
void Library::addCustomer(string name, string surname)
{
	ofstream customersFile("customers.txt", ofstream::app);
	if (customersFile.good())
	{
		int cID = customersData.size() + 1;
		customersData.push_back({ cID, name, surname });
		customersFile << customersData.back() << '\n';

		customersFile.close();
	}
	else
		cout << "Error. Can't open customers.txt file. Adding a customer canceled" << endl;
}
Book* Library::searchForBook(string title)
{
	vector<Book*> temp;
	for (int i = 0; i < booksData.size(); i++)
	{
		if (title == booksData[i].title)
			temp.push_back(&booksData[i]);
	}
	if (temp.size() == 0)
	{
		cout << "No book with such title was found.\n";
		return nullptr;
	}
	else if (temp.size() == 1)
		return temp[0];
	else
	{
		cout << "Found " << temp.size() << " books with such title. What is the book's ID?\n";
		while (1)
		{
			for (int i = 0; i < temp.size(); i++)
			{
				cout << "[" << i + 1 << "] ID=" << temp[i]->bookID;
				if (temp[i]->currentlyRented)
					cout << " (rented)\n";
				else
					cout << " (AVAILABLE)\n";
			}
			cout << "[B] - Go back.\n";
			string choice;
			int n;
			getline(cin, choice);
			stringstream ss(choice);
			if (ss >> n)
			{
				if (n <= temp.size())
					return temp[n - 1];
				else
					cout << "INVALID NUMBER. TRY AGAIN.\n";
			}
			else if (choice == "b" || choice == "B")
				return nullptr;
			else
				cout << "INVALID CHARACTER. TRY AGAIN.\n";
		}
	}
}
Customer* Library::searchForCustomer(string name, string surname)
{
	vector<Customer*> temp;
	for (int i = 0; i < customersData.size(); i++)
	{
		if ((name == customersData[i].name) && (surname == customersData[i].surname))
			temp.push_back(&customersData[i]);
	}
	if (temp.size() == 0)
	{
		cout << "No customer with such name was found.\n";
		return nullptr;
	}
	else if (temp.size() == 1)
		return temp[0];
	else
	{
		cout << "Found " << temp.size() << " customers with such name. What is their library ID?\n";
		while (1)
		{
			for (int i = 0; i < temp.size(); i++)
			{
				cout << "[" << i + 1 << "] ID=" << temp[i]->customerID << '\n';
			}
			cout << "[B] - Go back.\n";
			string choice;
			int n;
			getline(cin, choice);
			stringstream ss(choice);
			if (ss >> n)
			{
				if (n <= temp.size())
					return temp[n - 1];
				else
					cout << "INVALID NUMBER. TRY AGAIN.\n";
			}
			else if (choice == "b" || choice == "B")
				return nullptr;
			else
				cout << "INVALID CHARACTER. TRY AGAIN.\n";
		}

	}
}
Event* Library::searchForEvent(Book* bookptr, Customer* customerptr)
{

	if (bookptr != nullptr)
	{
		Event* eventptr = nullptr;
		for (int i = 0; i < eventsData.size(); i++)
		{
			if ((eventsData[i].book == bookptr) && (eventsData[i].returningDate.day == 0))
			{
				eventptr = &eventsData[i];
			}
		}
		return eventptr;
	}
	else if (customerptr != nullptr)
	{
		vector<Event*> temp;
		for (int i = 0; i < eventsData.size(); i++)
		{
			if ((eventsData[i].customer == customerptr) && (eventsData[i].returningDate.day == 0))
			{
				temp.push_back(&eventsData[i]);
			}
		}
		if (temp.size() == 0)
		{
			cout << "This customer isn't currently renting any books.\n";
			return nullptr;
		}
		else if (temp.size() == 1)
			return temp[0];
		else
		{
			cout << customerptr->name << ' ' << customerptr->surname << " is currently renting " << temp.size() << " books. Which one should be returned?\n";
			while (1)
			{
				for (int i = 0; i < temp.size(); i++)
				{
					cout << "[" << i + 1 << "] - " << temp[i]->book->title << " by " << temp[i]->book->author << " (ID = " << temp[i]->book->bookID << ")" << '\n';
				}
				cout << "[B] - Go back.\n";
				string choice;
				int n;
				getline(cin, choice);
				stringstream ss(choice);
				if (ss >> n)
				{
					if (n <= temp.size())
						return temp[n - 1];
					else
						cout << "INVALID NUMBER. TRY AGAIN.\n";
				}
				else if (choice == "b" || choice == "B")
					return nullptr;
				else
					cout << "INVALID CHARACTER. TRY AGAIN.\n";
			}
		}
	}
}
Date Library::getTime()
{
	struct tm* t;
	time_t tt;
	time(&tt);
	t = localtime(&tt);
	int day = t->tm_mday;
	int month = t->tm_mon + 1;
	int year = t->tm_year + 1900;
	int hour = t->tm_hour;
	int minute = t->tm_min;
	Date currentDate(day, month, year, hour, minute);
	return currentDate;
}
void Library::rentBook(Book* bookptr, Customer* customerptr)
{
	Date rentingDate = getTime();
	Date returningDate(0, 0, 0, 0, 0);
	bookptr->currentlyRented = 1;
	eventsData.push_back({ bookptr, customerptr, rentingDate, returningDate });
	ofstream booksFile("books.txt");
	if (booksFile.good())
	{
		for (int i = 0; i < booksData.size(); i++)
			booksFile << booksData[i] << '\n';
	}
	else
		cout << "Error. Can't open books.txt file." << endl;
	ofstream eventsFile("events.txt", ofstream::app);
	if (eventsFile.good())
	{

		eventsFile << bookptr->bookID << '\t' << customerptr->customerID << '\t' << rentingDate << '\t' << returningDate << '\n';

		eventsFile.close();
	}
	else
		cout << "Error. Can't open events.txt file." << endl;
}
void Library::returnBook(Event* eventptr)
{
	Date returnDate = getTime();
	eventptr->book->currentlyRented = 0;
	eventptr->returningDate = returnDate;
	ofstream booksFile("books.txt");
	if (booksFile.good())
	{
		for (int i = 0; i < booksData.size(); i++)
			booksFile << booksData[i] << '\n';
	}
	else
		cout << "Error. Can't open books.txt file." << endl;
	ofstream eventsFile("events.txt");
	if (eventsFile.good())
	{
		for (int i = 0; i < eventsData.size(); i++)
			eventsFile << eventsData[i].book->bookID << '\t' << eventsData[i].customer->customerID << '\t' << eventsData[i].rentingDate << '\t' << eventsData[i].returningDate << '\n';
		cout << "Book " << eventptr->book->title << " by " << eventptr->book->author << " has been returned succesfully.\n";
	}
	else
		cout << "Error. Can't open events.txt file." << endl;
}
void Library::printBookHistory(Book* bookptr)
{
	for (int i = 0; i < eventsData.size(); i++)
	{
		if (eventsData[i].book == bookptr)
		{
			cout << "Rented by " << eventsData[i].customer->name << ' ' << eventsData[i].customer->surname << " on " << eventsData[i].rentingDate;
			if (eventsData[i].returningDate.day != 0)
			{
				cout << " , returned on " << eventsData[i].returningDate << '\n';
			}
			else
			{
				cout << '\n';
			}
		}
	}
}
void Library::printCustomerHistory(Customer* customerptr)
{
	for (int i = 0; i < eventsData.size(); i++)
	{
		if (eventsData[i].customer == customerptr)
		{
			cout << "Rented \"" << eventsData[i].book->title << "\" by " << eventsData[i].book->author << " on " << eventsData[i].rentingDate;
			if (eventsData[i].returningDate.day != 0)
				cout << " , returned on " << eventsData[i].returningDate << '\n';
			else
				cout << '\n';
		}
	}
}