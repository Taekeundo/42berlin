#ifndef RPN_HPP
#define RPN_HPP

/* MACRO */
// Bold color definitions using ANSI
#define EOC "\033[0;0m"
#define RED "\033[1;31m"
#define GRE "\033[1;32m"
#define YEL "\033[1;33m"
#define BLU "\033[1;34m"
#define PUR "\033[1;35m"
#define CYN "\033[1;36m"
#define WHI "\033[1;37m"

/* HEADER FILE */
#include <iostream>         // std::cout, std::cerr
#include <stack>            // Container: "stack"
#include <string>			// std::string
#include <cctype>			// [f]isdigit();

/* CLASS */
class RPN
{
	private:
		/* [ Member variable ]: 1ea */
		std::stack<int> operandStack;

	public:
        /* [ OCCF ]: 5ea */
		RPN();                          		// Default constructor
        ~RPN();                         		// Destructor
        RPN(const RPN& other);          		// Copy constructor
        RPN(const std::string& rpnExpression);  // Parmeterized constructor
		RPN& operator=(const RPN& rhs);			// Overloaded assignment operator

		/* [ Method ]: 3ea */
		void processOperand(char currentChar);
		void processOperator(char currentChar);
        void calculateRpnExpression(const std::string& rpnExpression);

		/* [ Exception ]: 1ea */
		class InvalidRpnExpressionException : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};
};

#endif
