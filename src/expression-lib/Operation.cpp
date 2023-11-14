#include "Operation.h"

Operation::Operation()
{
	symb = '0';
	oper = '0';
	prior = 0;
	assoc = 0;
}

Operation::Operation(char symb, char oper, int prior, char assoc)
{
	if (symb >= 'A' && symb <= 'Z')
		this->symb = symb + 32;
	else
		this->symb = symb;

	this->oper = oper;
	this->prior = prior;
	if (assoc == 'L')
		this->assoc = false;
	else
		this->assoc = true;
}

bool Operation::operator==(const Operation& other) const
{
	return (this->prior == other.prior);
}

bool Operation::operator<(const Operation& other) const
{
	return (this->prior < other.prior);
}

bool Operation::operator>(const Operation& other) const
{
	return (this->prior > other.prior);
}

bool Operation::operator<=(const Operation& other) const
{
	return !(*this > other);
}

bool Operation::operator>=(const Operation& other) const
{
	return !(*this < other);
}

char Operation::Symbol() const
{
	return symb;
}

char Operation::Operator() const
{
	return oper;
}

bool Operation::Association() const
{
	return assoc;
}
