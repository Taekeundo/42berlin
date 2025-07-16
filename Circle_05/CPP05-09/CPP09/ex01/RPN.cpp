#include "RPN.hpp"

/* [ OCCF ]: 5ea */
// 1. Constructor
RPN::RPN() {}

// 2. Destructor
RPN::~RPN() {}

// 3. Copy Constructor
RPN::RPN(const RPN& other)
{
	*this = other;
}

// 4. Parmeterized constructor
RPN::RPN(const std::string& rpnExpression)
{
	try
	{
		calculateRpnExpression(rpnExpression);
		std::cout << GRE << "Result: " << this->operandStack.top() << EOC << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << RED << e.what() << EOC << std::endl;
	}
}

// 5. Overloaded assignment operator
RPN& RPN::operator=(const RPN& rhs)
{
	if (this != &rhs)
		this->operandStack = rhs.operandStack;
	return (*this);
}

// ----------------------------------------------------------------------------------------- //

/* [ Method ]: 3ea */
void RPN::processOperand(char digit)
{
	this->operandStack.push(digit - '0');
}

void RPN::processOperator(char operation)
{
	if (this->operandStack.size() < 2)
		throw RPN::InvalidRpnExpressionException();

	// Top & Pop for "right operand"
	double right = this->operandStack.top();
	this->operandStack.pop();

	// Top & Pop for "left operand"
	double left = this->operandStack.top();
	this->operandStack.pop();

	// Calculate
	if (operation == '+')
		this->operandStack.push(left + right);
	else if (operation == '-')
		this->operandStack.push(left - right);
	else if (operation == '*')
		this->operandStack.push(left * right);
	else if (operation == '/')
	{
		if (right == 0)
			throw RPN::InvalidRpnExpressionException();
		this->operandStack.push(left / right);
	}
}

void RPN::calculateRpnExpression(const std::string& rpnExpression)
{
	char currentChar;
	int expressionLength = rpnExpression.size();

	for (int i = 0; i < expressionLength; ++i)
	{
		currentChar = rpnExpression[i];
		// 1. Check (operand) // digit
		if (isdigit(currentChar))
			processOperand(currentChar);

		// 2. Check (operator) // token
		else if (currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/')
			processOperator(currentChar);

		// 3. Jump blank.
		else if (currentChar == ' ')
			continue;

		// 4. Error handling
		else
			throw RPN::InvalidRpnExpressionException();
	}
	if (this->operandStack.size() != 1)
		throw RPN::InvalidRpnExpressionException();
}

/* [ Exception ]: 1ea */
const char *RPN::InvalidRpnExpressionException::what() const throw()
{
	return ("Error: Invalid RPN-Expression");
}
