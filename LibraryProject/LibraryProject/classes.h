#ifndef CLASSES_H
#define CLASSES_H

#include <iostream>
#include <vector>

using namespace std;

class Book
{
	friend class Library;

	unsigned bookID;
	string title, author;
	bool currentlyRented;

public:
	Book(int _bID, string _title, string _author, bool _rented = 0);
	void printInfo();
	bool isRented();
	friend ostream& operator <<(ostream& s, const Book& b);
};

class Customer
{
	friend class Library;

	unsigned customerID;
	string name, surname;

public:
	Customer(int _cID, string _name, string _surname);
	void printInfo();
	friend ostream& operator <<(ostream& s, const Customer& c);
};

class Date
{
	friend class Event;
	friend class Library;

	unsigned day, month, year, hour, minute;

public:
	Date(int _day, int _month, int _year, int _hour, int _minute);
	Date();
	friend ostream& operator <<(ostream& s, const Date& d);
	friend istream& operator >> (istream& s, Date& d);
};

class Event
{
	friend class Library;

	Book* book;
	Customer* customer;
	Date rentingDate, returningDate;

public:
	Event(Book* _book, Customer* _customer, Date _rentDate, Date _returnDate);
	Event();
};

class Library
{
	vector<Book> booksData;
	vector<Customer> customersData;
	vector<Event> eventsData;

public:
	void loadBooksData();
	void loadCustomersData();
	void loadEventsData();
	void addBook(string title, string author);
	void addCustomer(string name, string surname);
	Book* searchForBook(string title);
	Customer* searchForCustomer(string name, string surname);
	Event* searchForEvent(Book* bookptr, Customer* customerptr);
	Date getTime();
	void rentBook(Book* bookptr, Customer* customerptr);
	void returnBook(Event* eventptr);
	void printBookHistory(Book* bookptr);
	void printCustomerHistory(Customer* customerptr);
};

#endif