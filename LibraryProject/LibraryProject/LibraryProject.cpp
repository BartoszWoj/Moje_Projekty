#include "classes.h"
#include <fstream>
#include <sstream>

using namespace std;

int main()
{
	Library lib;
	string choice;
	bool loop0;
	try
	{
		lib.loadBooksData();
	}
	catch (int exception)
	{
		switch (exception)
		{
		case 1:
			cout << "Can't open books.txt file. ";
			loop0 = 1;
			while (loop0)
			{
				cout << "Do you want to create new file with books data?\n";
				cout << "[Y]es		[N]o\n";
				getline(cin, choice);
				cout << "\n";
				if ((choice == "y") || (choice == "Y"))
				{
					std::ofstream{ "books.txt" };
					loop0 = 0;
				}
				else if ((choice == "n") || (choice == "N"))
				{
					cout << "Closing the application.\n";
					return 0;
				}
				else
				{
					loop0 = 1;
					cout << "INVALID CHARACTER. TRY AGAIN.\n";
				}
			}
			break;
		case 2:
			cout << "Error! Can't read a line from books.txt file. Closing the application.\n";
			return 0;
		}

	}
	try
	{
		lib.loadCustomersData();
	}
	catch (int exception)
	{
		switch (exception)
		{
		case 3:
			cout << "Can't open customers.txt file. ";
			loop0 = 1;
			while (loop0)
			{
				cout << "Do you want to create new file with customers data?\n";
				cout << "[Y]es		[N]o\n";
				getline(cin, choice);
				cout << "\n";
				if ((choice == "y") || (choice == "Y"))
				{
					std::ofstream{ "customers.txt" };
					loop0 = 0;
				}
				else if ((choice == "n") || (choice == "N"))
				{
					cout << "Closing the application.\n";
					return 0;
				}
				else
				{
					loop0 = 1;
					cout << "INVALID CHARACTER. TRY AGAIN.\n";
				}
			}
			break;
		case 4:
			cout << "Error! Can't read a line from customers.txt file. Closing the application.\n";
			return 0;
		}

	}
	try
	{
		lib.loadEventsData();
	}
	catch (int exception)
	{
		switch (exception)
		{
		case 5:
			cout << "Can't open events.txt file. ";
			loop0 = 1;
			while (loop0)
			{
				cout << "Do you want to create new file with events data?\n";
				cout << "[Y]es		[N]o\n";
				getline(cin, choice);
				cout << "\n";
				if ((choice == "y") || (choice == "Y"))
				{
					std::ofstream{ "events.txt" };
					loop0 = 0;
				}
				else if ((choice == "n") || (choice == "N"))
				{
					cout << "Closing the application.\n";
					return 0;
				}
				else
				{
					loop0 = 1;
					cout << "INVALID CHARACTER. TRY AGAIN.\n";
				}
			}
			break;
		case 6:
			cout << "Error! Can't read a line from events.txt file. Closing the application.\n";
			return 0;
		}

	}
	cout << "Welcome to the library system. What do you want to do?\n";
	loop0 = 1;
	while (loop0)
	{
		cout << "\nMAIN MENU\n";
		cout << "[B] - Enter books database.\n";
		cout << "[C] - Enter customers database.\n";
		cout << "[E] - Exit the program.\n";
		getline(cin, choice);
		bool loop1 = 1;
		if ((choice == "b") || (choice == "B"))
		{
			while (loop1)
			{
				cout << '\n';
				cout << "[A] - Add new book to the database.\n";
				cout << "[S] - Search for a book.\n";
				cout << "[M] - Go back to main menu.\n";
				getline(cin, choice);
				cout << '\n';
				if ((choice == "a") || (choice == "A"))
				{
					loop1 = 0;
					cout << "Write the title of the book you want to add.\n";
					string title, author;
					getline(cin, title);
					cout << "\n";
					cout << "Write the name of the author of this book.\n";
					getline(cin, author);
					bool loop2 = 1;
					while (loop2) {
						cout << '\n';
						cout << "Are you sure you want to add book \"" << title << "\" by " << author << " to the database?\n";
						cout << "[Y]es		[N]o\n";
						getline(cin, choice);
						cout << "\n";
						if ((choice == "y") || (choice == "Y"))
						{
							lib.addBook(title, author);
							cout << "Book has been added to the database.\n";
							loop2 = 0;
						}
						else if ((choice == "n") || (choice == "N"))
						{
							cout << "Adding book canceled.\n";
							loop2 = 0;
						}
						else
						{
							loop2 = 1;
							cout << "INVALID CHARACTER. TRY AGAIN.\n";
						}
					}
				}
				else if ((choice == "s") || (choice == "S"))
				{
					cout << "Write the title of the book you are looking for.\n";
					string title;
					getline(cin, title);
					cout << '\n';
					Book* bookptr = lib.searchForBook(title);
					if (bookptr != nullptr)
					{
						bool loop2 = 1;
						while (loop2)
						{
							cout << '\n';
							bookptr->printInfo();
							if (bookptr->isRented())
								cout << "[T] - Take back the book.\n";
							else
								cout << "[R] - Rent the book.\n";
							cout << "[H] - display history of events of this book.\n";
							cout << "[M] - Go back to main menu.\n";
							getline(cin, choice);
							cout << "\n";
							if (!(bookptr->isRented()) && (choice == "r" || choice == "R"))
							{
								cout << "Write the first name of customer who wants to rent this book.\n";
								string name, surname;
								getline(cin, name);
								cout << "\n";
								cout << "Write the last name of customer who wants to rent this book.\n";
								getline(cin, surname);
								cout << "\n";
								Customer* customerptr = lib.searchForCustomer(name, surname);
								if (customerptr != nullptr)
								{
									lib.rentBook(bookptr, customerptr);
									cout << "Book has been rented.\n";
								}
							}
							else if ((bookptr->isRented()) && (choice == "t" || choice == "T"))
							{
								Event* eventptr = lib.searchForEvent(bookptr, nullptr);
								lib.returnBook(eventptr);
							}
							else if (choice == "h" || choice == "H")
							{
								lib.printBookHistory(bookptr);
							}
							else if (choice == "m" || choice == "M")
							{
								loop1 = 0;
								loop2 = 0;
							}
							else
							{
								loop2 = 1;
								cout << "INVALID CHARACTER. TRY AGAIN.\n";
							}
						}
					}
				}

				else if ((choice == "m") || (choice == "M"))
				{
					loop1 = 0;
				}
				else
				{
					loop1 = 1;
					cout << "INVALID CHARACTER. TRY AGAIN.\n";
				}
			}
		}
		else if ((choice == "c") || (choice == "C"))
		{
			while (loop1)
			{
				cout << '\n';
				cout << "[A] - Add new customer to the database.\n";
				cout << "[S] - Search for a customer.\n";
				cout << "[M] - Go back to main menu.\n";
				getline(cin, choice);
				cout << '\n';
				if ((choice == "a") || (choice == "A"))
				{
					loop1 = 0;
					cout << "Write the first name of the new customer.\n";
					string name, surname;
					getline(cin, name);
					cout << "\n";
					cout << "Write the last name of the new customer.\n";
					getline(cin, surname);
					bool loop2 = 1;
					while (loop2) {
						cout << '\n';
						cout << "Are you sure you want to add customer " << name << " " << surname << " to the database?\n";
						cout << "[Y]es		[N]o\n";
						getline(cin, choice);
						cout << "\n";
						if ((choice == "y") || (choice == "Y"))
						{
							lib.addCustomer(name, surname);
							cout << name << ' ' << surname << " has been added to the database.\n";
							loop2 = 0;
						}
						else if ((choice == "n") || (choice == "N"))
						{
							cout << "Adding the customer canceled.\n";
							loop2 = 0;
						}
						else
						{
							loop2 = 1;
							cout << "INVALID CHARACTER. TRY AGAIN.\n";
						}
					}
				}
				else if ((choice == "s") || (choice == "S"))
				{
					cout << "Write the first name of the customer you are looking for.\n";
					string name, surname;
					getline(cin, name);
					cout << '\n';
					cout << "Write the last name of the customer you are looking for.\n";
					getline(cin, surname);
					cout << '\n';
					Customer* customerptr = lib.searchForCustomer(name, surname);
					if (customerptr != nullptr)
					{
						bool loop2 = 1;
						while (loop2)
						{
							cout << '\n';
							customerptr->printInfo();
							cout << "[R] - Rent a book.\n";
							cout << "[T] - Take back a book.\n";
							cout << "[H] - display history of events of this customer.\n";
							cout << "[M] - Go back to main menu.\n";
							getline(cin, choice);
							cout << "\n";
							if ((choice == "r") || (choice == "R"))
							{
								string title;
								cout << "Write the title of book you want to rent.\n";
								getline(cin, title);
								cout << "\n";
								Book* bookptr = lib.searchForBook(title);
								if (bookptr != nullptr)
								{
									if (!(bookptr->isRented()))
									{
										lib.rentBook(bookptr, customerptr);
										cout << "Book has been rented.\n";
									}
									else
										cout << "This book is currently rented\.";
								}
							}
							else if ((choice == "t") || (choice == "T"))
							{
								Event* eventptr = lib.searchForEvent(nullptr, customerptr);
								if (eventptr != nullptr)
									lib.returnBook(eventptr);
							}
							else if (choice == "h" || choice == "H")
							{
								lib.printCustomerHistory(customerptr);
							}
							else if (choice == "m" || choice == "M")
							{
								loop1 = 0;
								loop2 = 0;
							}
							else
							{
								loop2 = 1;
								cout << "INVALID CHARACTER. TRY AGAIN.\n";
							}
						}
					}
				}
				else if ((choice == "m") || (choice == "M"))
				{
					loop1 = 0;
				}
				else
				{
					loop1 = 1;
					cout << "INVALID CHARACTER. TRY AGAIN.\n";
				}
			}
		}
		else if ((choice == "e") || (choice == "E"))
		{
			loop0 = 0;
		}
		else
		{
			cout << "INVALID CHARACTER. TRY AGAIN.\n";
		}
	}

	return 0;

}