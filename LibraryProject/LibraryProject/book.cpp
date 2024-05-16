#include "classes.h"
#include <iostream>

using namespace std;

Book::Book(int _bID, string _title, string _author, bool _rented) : bookID(_bID), title(_title), author(_author), currentlyRented(_rented) {}
void Book::printInfo()
{
	cout << "Title: \"" << title << "\"\n";
	cout << "Author: " << author << '\n';
	cout << "Currently available: ";
	if (currentlyRented)
		cout << "NO\n";
	else
		cout << "YES\n";
}
bool Book::isRented()
{
	return currentlyRented;
}
ostream& operator <<(ostream& s, const Book& b)
{
	s << b.bookID << '\t' << b.title << '\t' << b.author << '\t' << b.currentlyRented;

	return s;
}