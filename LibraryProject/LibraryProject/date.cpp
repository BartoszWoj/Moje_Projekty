#include "classes.h"
#include <iostream>

using namespace std;

Date::Date(int _day, int _month, int _year, int _hour, int _minute) :
	day(_day), month(_month), year(_year), hour(_hour), minute(_minute) {}
Date::Date() {}
ostream& operator <<(ostream& s, const Date& d)
{
	s << d.day << ' ' << d.month << ' ' << d.year << ' ' << d.hour << ' ' << d.minute;

	return s;
}
istream& operator >> (istream& s, Date& d)
{
	s >> d.day >> d.month >> d.year >> d.hour >> d.minute;

	return s;
}