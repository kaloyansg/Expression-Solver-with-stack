#include "expression.h"
#include "Operation.h"
#include "OperationSet.h"
#include <stack>
#include <vector>

///
/// @brief Evaluates an expression.
///
/// @param expression
///   A null-terminated string that contains the expression.
/// @param ops
///   An input stream which contains the descriptions of all operations used in the expression.
///
/// @return The calculated value of the expression
///

bool emptyExpression(const char* expression)
{
	bool spacesOnly = true;
	unsigned count = 0;
	for (count; expression[count] != '\0'; ++count)
	{
		if (expression[count] != ' ')
			spacesOnly = false;
	}
	if (count == 0) return true;
	if (spacesOnly) return true;
	return false;
}

bool validExpression(const char* expression, const OperationSet& set)
{
	bool noNum = true;
	bool number = false;

	for (unsigned i = 0; expression[i] != '\0'; ++i)
	{
		while (expression[i] == ' ')
			++i;

		if (expression[i] == '-')
		{
			if (!isdigit(expression[i + 1]))
				throw "Minus as an operation";
			++i;
		}
		if (isdigit(expression[i]))
		{
			if (number)
				throw "Numbers without an expression between them";
			number = true;
			noNum = false;
			while (isdigit(expression[i]))
				++i;
			if (expression[i] != ' ' && expression[i] != '\0')
				return false;
			if (expression[i] == '\0')
				break;
		}
		else if (expression[i] == '(' || expression[i] == ')')
		{
			if (expression[i + 1] != ' ' && expression[i + 1] != '\0')
				throw "No space after the bracket";

			if (expression[i] == '(' && number)
				return false;
			else if (expression[i] == ')' && !number)
				return false;
		}
		else
		{
			if (!number)
				throw "Incomplete expression";
			number = false;
			if (expression[i + 1] != ' ')
				return false;

			if (!set.contains(expression[i]))
				throw "Operations that are not defined in the set";
		}
	}

	if (noNum)
		throw "There are no numbers";
	return true;
}

bool validBrackets(const char* expression)
{
	int check = 0;
	for (unsigned i = 0; expression[i] != '\0'; ++i)
	{
		if (expression[i] == '(')
			++check;
		else if (expression[i] == ')')
			if (check == 0)
				return false;
			else
				--check;
	}
	if (check == 0)
		return true;
	else
		return false;
}

void extractExpression(const char* expression, std::vector<std::string>& expr)
{
	std::string element;
	while (*expression != '\0')
	{
		element = "";
		while (*expression == ' ')
			++expression;
		while (*expression != ' ' && *expression != '\0' && *expression != '(' && *expression != ')')
		{
			element += *expression;
			++expression;
		}
		if (element != "")
			expr.push_back(element);

		if (*expression == '(' || *expression == ')')
		{
			element = "";
			element += *expression;
			expr.push_back(element);
			++expression;
		}
	}

}

int getNumber(std::string& str)
{
	const char* item(str.c_str());

	int length = strlen(item);
	int beg = 0;

	bool negative = false;
	if (item[beg] == '-')
	{
		negative = true;
		++beg;
	}

	int result = 0;
	for (int i = beg; i < length; ++i)
	{
		if (!std::isdigit(item[i]))
			break;
		result = (result * 10) + (item[i] - '0');
	}

	return (negative ? -result : result);
}

double solve(double lhs, char oper, double rhs)
{
	switch (oper)
	{
	case '+': return lhs + rhs;
	case '-': return lhs - rhs;
	case '*': return lhs * rhs;
	case '/': return lhs / rhs;
	}
}

double evaluate(const char* expression, std::istream& ops)
{
	OperationSet set(ops);

	try
	{
		if (expression == nullptr)
			throw incorrect_expression("No expression submitted");
		if (emptyExpression(expression))
			return 0;
		if (!validBrackets(expression))
			throw incorrect_expression("Invalid brackets");
		if (!validExpression(expression, set))
			throw incorrect_expression("Invalid expression");
	}
	catch (const char* err)
	{
		throw incorrect_expression(err);
	}

	std::vector<std::string> exp;
	extractExpression(expression, exp);
	std::stack<double> numbers;
	std::stack<char> operations;

	size_t i = 0;
	while (numbers.empty())
	{
		if (exp[i][0] == '(')
		{
			operations.push('(');
			++i;
		}
		else
		{
			numbers.push(getNumber(exp[i]));
			++i;
		}
	}

	double num;
	Operation currentOper;
	for (i; i < exp.size(); ++i)
	{
		if (exp[i][0] == ')')
		{
			while (operations.top() != '(')
			{
				num = numbers.top();
				numbers.pop();
				numbers.top() = solve(numbers.top(),
					set.getOperation(operations.top()).Operator(), num);
				operations.pop();
			}
			operations.pop();
			++i;
		}

		if (i >= exp.size())
			break;

		if (exp[i][0] == ')')
		{
			--i;
			continue;
		}

		if (!operations.empty() && operations.top() == '(')
		{
			operations.push(exp[i][0]);
			++i;
			if (exp[i][0] != '(')
				numbers.push(getNumber(exp[i]));
			else
			{
				operations.push(exp[i][0]);
				++i;
				numbers.push(getNumber(exp[i]));
			}
		}
		else
		{
			currentOper = set.getOperation(exp[i][0]);
			if ((currentOper.Association() && (operations.empty() || currentOper >= set.getOperation(operations.top())))
				|| (!currentOper.Association() && (operations.empty() || currentOper > set.getOperation(operations.top()))))
			{
				operations.push(currentOper.Symbol());
				++i;
				if (exp[i][0] == '(')
				{
					operations.push('(');
					++i;
				}
				numbers.push(getNumber(exp[i]));
			}
			else
			{
				num = numbers.top();
				numbers.pop();
				numbers.top() = solve(numbers.top(),
					set.getOperation(operations.top()).Operator(), num);
				operations.pop();
				--i;
			}
		}
	}

	while (!operations.empty())
	{
		num = numbers.top();
		numbers.pop();
		numbers.top() = solve(numbers.top(),
			set.getOperation(operations.top()).Operator(), num);
		operations.pop();
	}

	return numbers.top();
}