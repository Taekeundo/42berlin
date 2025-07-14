#include "RPN.hpp"

/*
	[ Background ]
	1. What is RPN?
		RPN (Reverse Polish Notation)
		= A mathematical notation in which operators follow their operands.
		= Unlike standard infix notation (e.g., 3 + 4),
		  RPN writes the same expression as: 3 4 +.

		→ No need for parentheses or operator precedence.
		→ Evaluated using a stack:
	   		- Push numbers to the stack.
	   		- When an operator is encountered,
				pop two numbers, apply the operator, and push the result back.

		Example:
			Input : "3 4 + 2 *"
			Steps : (3 + 4) = 7 → 7 * 2 = 14
			Result: 14

			process:
			1) '3' -> push
				operandStack: [3]	// bottom

			2) '4' -> push
				operandStack: [4]	// top
							  [3]	// bottom

			3) '+' -> processOperator
				-> right: 4			// stack.top(), then pop
				-> left: 3			// stack.top(), then pop
				-> operandStack.push(left + right);
				-> operandStack: [7]

			4) '2' -> push
				operandStack: [2]	// top
							  [7]	// bottom

			5) '*' -> processOperator
				-> right: 2			// stack.top(), then pop
				-> left: 7			// stack.top(), then pop
				-> operandStack.push(left * right);
				-> operandStack: [14]

			6) Final result = operandStack.top() = 14

	[ Flow summary ]
	main()
	└─ RPN(argv[1])                      -> Constructor (Param)
	     └─ calculateRpnExpression()     -> Parse and evaluate the RPN expression
	         ├─ processOperand()         -> Push digit to stack
	         ├─ processOperator()        -> Pop 2 values, apply operator, push result
	         └─ (final check)            -> Ensure only 1 result remains on stack

	[ Flow detail ]
	main()
	1. Check input validation
		-> If argc != 2, print usage error.

	2. Construct RPN object with input expression
		-> RPN rpn(argv[1]);
			-> RPN(const std::string& rpnExpression)
			-> ex) argv[1]: "3 4 + 2 *"

		-> [f] calculateRpnExpression(rpnExpression)
			a. For each character:
				- If Operand(3, 4, 2, ..) -> processOperand()
					-> Converts 'char' to 'int' and pushes to operandStack
					-> operandStack.push(digit - '0');
				
				- If operator (+, -, *, /) -> processOperator()
					→ Pops 2 values from operandStack
					→ Applies operator with them
					→ Pushes result back to operandStack

				- If space(' ') -> skip // continue

				- Else -> throw InvalidRpnExpressionException()

			b. At the end:
				- If stack has more than one element -> throw exception

		-> If successful
			-> prints the top of the stack as result
			
		-> If any error occurs
			-> catch and print error message
*/
int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cout << RED << "Error: Incorrect arguments.\n" << EOC
					<< "Usage: ./RPN \"RPN-Expression\"" << std::endl;
		return (1);
	}
	RPN rpn(argv[1]);
	return (0);
}
