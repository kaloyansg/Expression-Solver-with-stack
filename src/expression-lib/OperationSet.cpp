#include "OperationSet.h"

OperationSet::OperationSet(std::istream& file)
{
	char c;
	char symb;
	char oper;
	char assoc;

	bool negative = false;
	int prior = 0;

	file.get(c);
	if (file.eof())
		return;
	file.seekg(0, std::ios::beg);

	while (!file.eof())
	{
		prior = 0;

		file.get(c);
		while (c == ' ')
			file.get(c);
		symb = c;

		file.get(c);
		while (c == ' ')
			file.get(c);
		oper = c;

		file.get(c);
		while (c == ' ')
			file.get(c);

		if (c == '-')
			negative = true;
		else
			file.seekg(-1, std::ios::cur);

		file.get(c);
		while (c != ' ')
		{
			prior = prior * 10 + (c - '0');
			file.get(c);
		}
		if (negative)
			prior *= -1;

		file.get(c);
		while (c == ' ')
			file.get(c);
		assoc = c;

		while (c != '\n' && !file.eof())
			file.get(c);

		set.push_back(Operation(symb, oper, prior, assoc));
	}
}

bool OperationSet::contains(char symb) const
{
	for (unsigned i = 0; i < set.size(); ++i)
	{
		if (set[i].Symbol() == symb)
			return true;
	}
	return false;
}

const Operation& OperationSet::getOperation(char symb) const
{
	for (unsigned i = 0; i < set.size(); ++i)
	{
		if (set[i].Symbol() == symb)
			return set[i];
	}
}
