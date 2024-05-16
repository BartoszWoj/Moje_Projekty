#include "classes.h"
#include <iostream>

using namespace std;

Customer::Customer(int _cID, string _name, string _surname) : customerID(_cID), name(_name), surname(_surname) {}
void Customer::printInfo()
{
	cout << "Customer's name: " << name << ' ' << surname << '\n';

}
ostream& operator <<(ostream& s, const Customer& c)
{
	s << c.customerID << '\t' << c.name << '\t' << c.surname;

	return s;
}