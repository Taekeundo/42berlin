#include "BitcoinExchange.hpp"

/*
	[ Flow summary ]
	main()
	├─ BitcoinExchange()               → Loads "data.csv" into mapData
	└─ processInputFile(argv[1])
		├─ splitLineByDelimiter()     → Split date | value
		├─ trimWhitespace()           → Remove spaces
		├─ validateDateFormat()       → Check date format/range
		├─ validateAndConvertValue()  → Validate and convert value
		└─ displayExchangeResult()    → Lookup rate, calculate, print

	[ Flow detail ]
	main()
	1. Make the object by using constructor.
		-> BitcoinExchange bitcoinExchange;
			-> Opens "data.csv"
			-> Reads line by line
			-> Parses (data, exchange rate)
			-> Stores into mapData(map container)

			ex) 2009-01-02, 0 -> map["2009-01-02"] = 0.0
		
	2. Handle user input
		-> bitcoinExchange.processInputFile(argv[1]);
			-> Opens given input file (= "input.txt")
			-> Reads and parse each line
			-> Validates (date) and (value)
			-> Calculates and prints exchange result

			a. splitLineByDelimiter()
				-> Splits line into "data" and "value" by '|'

			b. trimWhitespace()
				-> Removes extra spaces from "data" and "value"

			c. validateDataFormat()
				-> Checks format (YYYY-MM-DD)
				-> Validates data range and leap year
			
			d. validateAndCovertValue()
				-> Checks value format
				-> Converts to double
				-> Validates range (0 ~ 1000)

			e. displayExchangeResult()
				-> Looks up exchange rate from mapData
				-> Calculates value * rate
				-> Prints result
*/
int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cout << RED << "Error: could not open file." << EOC << std::endl;
		return (1);
	}
	try
	{
		BitcoinExchange bitcoinExchange;
		bitcoinExchange.processInputFile(argv[1]);
	}
	catch(const std::exception& e)
	{
		std::cerr << RED << "Error: " << EOC << e.what() << std::endl;
	}
	return (0);
}
