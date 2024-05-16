#include "classes.h"
#include <iostream>

using namespace std;

Event::Event(Book* _book, Customer* _customer, Date _rentDate, Date _returnDate) :
	book(_book), customer(_customer), rentingDate(_rentDate), returningDate(_returnDate) {}
Event::Event() {}