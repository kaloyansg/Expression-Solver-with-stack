#pragma once
#include "Operation.h"
#include <istream>
#include <vector>

class OperationSet
{
	std::vector<Operation> set;

public:
	OperationSet(std::istream&);
	OperationSet(const OperationSet&) = delete;
	OperationSet& operator=(const OperationSet&) = delete;

	bool contains(char) const;
	const Operation& getOperation(char) const;
};