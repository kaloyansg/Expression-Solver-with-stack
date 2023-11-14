#pragma once
class Operation
{
	char symb;
	char oper;
	int prior;
	bool assoc;

public:
	Operation();
	Operation(char, char, int, char);

	bool operator==(const Operation&) const;
	bool operator<(const Operation&) const;
	bool operator>(const Operation&) const;
	bool operator<=(const Operation&) const;
	bool operator>=(const Operation&) const;

	char Symbol() const;
	char Operator() const;
	bool Association() const;
};
