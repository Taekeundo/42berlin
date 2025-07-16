#include "BitcoinExchange.hpp"

/* [ OCCF ]: 4ea */
// 1. Constructor
/*
	[ Background ]
	1. std::ifstream here("data.csv")
		= Read only stream
		
		[ Role ]
		Open the file(= Param(1):"data.csv")
		and read data from that file and store it in here(name).

	2. std::getline(file, line);
		Param(1): Target
		Param(2): Save place.
		
		[ Role ]
		Reads first one line from the target(= Param(1):"file")
		and stores it in the place(= param(2):"line").

		[ ETC ]
		It stops at '\n' but doesn't store '\n'.

		[ When ]
		Used for reading files line by line.
	
	3. "data.csv"
		Exchange rate reference table of Bitcoin, given by the exercise.
		
	[ Role ]
	Open the "data.scv"file. While reading each line, the values
	​​before and after the comma are stored in the map container.
*/
BitcoinExchange::BitcoinExchange()
{
	// 1. File open, not open? -> Error
	std::ifstream file("data.csv");
	if (!file.is_open())
		throw OpenDataException();
	
	// 2. Skip the line of titles in file (= "date,exchange_rate")
	std::string line;
	std::getline(file, line);

	// 3. Read one line by one line and divide it by the , and store it in each map struct's value.
	while (std::getline(file, line))
	{
		// Type convert (string to stringstream)
		std::stringstream ss(line);
		std::string date;
		double exchangeRate;

		/*
		Extract the data from the Param(1):"ss"
		until meet with the Param(3):","
		and save it to the Param(2):"date"
		= Now in the Param(1):"ss", value after "," is only stored.
		= Finished to extract.
		*/
		std::getline(ss, date, ',');
		ss >> exchangeRate;

		// Save it to the "mapData" container.
		this->mapData[date] = exchangeRate;
	}
}

// 2. Destructor
BitcoinExchange::~BitcoinExchange() {}

// 3. Copy Constructor
BitcoinExchange::BitcoinExchange(const BitcoinExchange& other)
{
	*this = other;
}

// 4. Overloaded assignment operator
BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& rhs)
{
	if (this != &rhs)
		this->mapData = rhs.mapData;
	return (*this);
}

// ----------------------------------------------------------------------------------------- //

/* [ Method ]: 7ea */
/*	[F]
	[ Role ]
	1. Convert the type of Param(1):"line" from string to stringstream for dividing by the Param(2):"delimiter".
	2. Store the values before and after the Param(2):"delimiter"
		in the Param(3):"date" & in the Param(4):"value" in "mapData"container.
	= 라인에서 구분자로 나누어 날짜와 값을 반환하는 함수
*/
bool	BitcoinExchange::splitLineByDelimiter(const std::string& line, char delimiter, std::string& date, std::string& value)
{
	std::stringstream ss(line);

	// 구분자 없으면 false 반환
	if (!std::getline(ss, date, delimiter))
        return false;

	// 두 번째 값이 없으면 false 반환
    if (!std::getline(ss, value))
        return false;

    return (true);
}

/*	[F]
	[ Background ]
	1.	[f] remove_if(date.begin(), date.end(), isspace)
		Traverse from 'begin' to 'end' of the container,
		remove whitespace, if finds it.

		ex) std::string date = "  2025 - 04 - 02  ";
			remove_if(date.begin(), date.end(), isspace);

			Before: [  2025 - 04 - 02  ]
			After : [2025-04-02 - - - - -]

			= 공백이 제거된 문자들은 앞으로 이동되지만, 문자열의 길이는 그대로 남는다.
			= 그래서 std::string::erase()를 같이 사용하는거다.
	
	2. std::string::erase(position, quantity)
		Param(1): position = Position to start deleting.
		Param(2): quantity = The number of characters to delete.
		= Erase from the Param(1):"position" as many as Param(2):"quantity".

		ex) std::string str = "Hello, World";
			str.erase(5, 2);

			Before: "Hello, World";
			After : "HelloWorld";
		
		date.erase(remove_if(date.begin(), date.end(), isspace), date.end());
			Before: [2025-04-02 - - - - -]
			After : [2025-04-02]

	[ Role ]
	Remove the whitespace from the Parma(1): "str".
	= 문자열에서 공백 제거 함수

	Before: [2011-01-03 | 3]
	After : [2011-01-03]
*/
void	BitcoinExchange::trimWhitespace(std::string& str)
{
	str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
}

/*	[F]
	[ Role ]
	주어진 파일을 읽어 각 줄을 처리하고 날짜와 값 정보를 파싱하여 검증 후 결과를 출력.
*/
void	BitcoinExchange::processInputFile(const std::string& filename)
{
	//	1. Open the file (= Param(1):"filename").
	// c_str: std::ifstream needs (const char *).
	std::ifstream file(filename.c_str());
	if (!file.is_open())
		throw OpenFileException();

	//	2. Skip the title line from the Param(1):"file".
	std::string line;
	std::getline(file, line);

	/*	3. Process each line of a file
			Traverse line by line from the Param(1):"file".
				Save before "|"  -> map.date
				Save before "\n" -> map.value
	*/
	while (std::getline(file, line))
	{
		std::string date;
		std::string value;
		// 3-1. Separate dates and values ​​in lines.
		if (!splitLineByDelimiter(line, '|', date, value))
		{
			std::cerr << "Error: invalid date." << std::endl;
			continue;
		}

		// 3-2. Remove whitespace from the 'date' & 'value' string.
		trimWhitespace(date);
		trimWhitespace(value);

		// 4. Print output after validate 'date' & 'value' & Error handling.
		try
		{
			validateDateFormat(date);
			displayExchangeResult(date, validateAndConvertValue(value));
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	return ;
}

/*	[F]
	[ Role ]
	Check the leap year.
*/
bool	BitcoinExchange::isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

/*	[F]
	[ Goal ]
	date format validation

	[ Background ]
	1. std::istringstream
		= input stream class
		= Handle(Read) the string type data as if it's input stream type.
		= 문자열을 입력 스트림처럼 처리하여 데이터를 읽을 수 있다.
		= 주로 문자열에서 데이터를 추출할 때 사용.

		ex) date.substr(0, 4);
			"2011-01-03"

			subtrim the string from index[0] as many as Param(2): 4 charactors.
			= "2011" -> Store it to the "year".

	[ Logic ]
	1. Check the date format. ("YYYY-MM-DD")
		Check the length by using [f]size() & whether '-' is located in right location.
		ex) 2020-09-12

	2. Check, if the range of year, month, and day is within valid range.
		ex) Year	: 1 -
			Month	: 1 - 12
			Day		: 1 - 31

			2020-13-01	// False

	3. Check monthly days.
		ex) Jan: 31, Feb: 28 || 29, March: 31, ...
	
		1) 31days: 7ea
			Jan, March, May, July, August, October, December
			1	3	5	7	8	10	12
	
		2) 30days: 4ea
			4	6	9	11
	
		3) Special case (Feb): 1ea
			Leap year  : Feb. 29th	// Once every four years.
			Normal year: Feb. 28th

	4. Check leap year with Feb.
		ex) 29th || 28th
*/
void	BitcoinExchange::validateDateFormat(std::string date)
{
	// 1. Check the date format.
	if(date.size() != 10 || date[4] != '-' || date[7] != '-')
		throw InvalidDateException();

	// 2. Extract year, month, day from the Param(1):"date" string.
	int year, month, day;
	std::istringstream(date.substr(0, 4)) >> year;
	std::istringstream(date.substr(5, 2)) >> month;
	std::istringstream(date.substr(8, 2)) >> day;

	// Test
	// std::cout << GRE << "year: " << year << EOC << std::endl;
	// std::cout << GRE << "month: " << month << EOC << std::endl;
	// std::cout << GRE << "day: " << day << EOC << std::endl;
	// Test

	// 3. Check, if the range of year, month, and day is within valid range.
	if(year < 1 || month < 1 || month > 12 || day < 1 || day > 31)
		throw InvalidDateException();

	// 4. Matching the Max.day with month.
	const int dayInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int maxDays = dayInMonth[month - 1];

	// 5. Handling 'Leap year' case.
	if (month == 2 && isLeapYear(year))
		maxDays = 29;

	// 6. Check days
	if (day > maxDays)
		throw InvalidDateException();

	return ;
}

/*	[F]
	[ Goal ]
	1. Validates the Param(1):"value"	
	2. Converts it to a valid real number("double" type) -> res
	3. Check the valid range of the 'res'.
*/
double	BitcoinExchange::validateAndConvertValue(std::string value)
{
	// 1. Throws an exception if there are char other than nums, periods, or hyphens.
	if (value.find_first_not_of("0123456789.-") != std::string::npos)
		throw InvalidValueException();

	/*	2. Check 4 exceptional cases.
			2-1. Empty				// 비어 있거나		ex) " "
			2-2. Starts with dot	// 점으로 시작		ex) .5
			2-3. Ends with dot		// 점으로 끝나		ex) 3.
			2-4. Wrong 0 include	// 잘못된 0포함		ex) 05
	*/
	bool isValueEmpty = value.empty();
	bool valueStartsWithDot = !value.empty() && value[0] == '.';
	bool valueEndsWithDot = !value.empty() && value[value.size() - 1] == '.';
	bool startsWithZeroWithoutDot = value.size() > 1 && value[0] == '0' && value[1] != '.';
	if (isValueEmpty || valueStartsWithDot || valueEndsWithDot || startsWithZeroWithoutDot)
		throw InvalidValueException();

	/*	3. Convert the Param(1):"value", which is string type, to double type by using [f]istringstream

		Why two "catch" blocks?
		= To improve the granularity(세분화) and safety of exception handling.
	*/
	double res;
	try
	{
		std::istringstream(value) >> res;
	}
	catch (const std::invalid_argument& e)
	{
		throw InvalidValueException();
	}
	catch (const std::exception& e)
	{
		throw InvalidValueException();
	}

	/*	4. Check the range of res.
		Available range: 0 <= res <= 1000
	*/
	if (res > 1000.0)
		throw OutOfRangeValueException();
	if (res < 0.0)
		throw NegativeValueException();
	return (res);
}

/*	[F]
	[ Goal ]
	1. Finds the Bitcoin exchange rate for the given Param(1):"date" and Param(2):"value" in mapData.
	2. Print it to the terminal as converted type(= "double").
	3. If ther is no exchange rate info for the given date
		it finds the closest previous date and prints it using that value.
	
	[ Background ]
	1. std::map::count(key)
	 	Checks how many times a given 'key' appears in a map container.
	
	2. std::map::lower_bound(key)
		Returns an iterator for the first element greater than or equal to the given key.

		ex) dataMap["2025-04-01"] = 500;
			dataMap["2025-04-03"] = 600;
			dataMap["2025-04-05"] = 700;

			std::string searchDate = "2024-04-02";
			std::map<std::string, double>::iterator it = dataMap.lower_bound(searchDate);

			it = "[2025-04-03"] = 600;"
			-> closest greater key.
	
	3. it->first, it->second

		first : key		("date")
		second: value	("value")
*/
void	BitcoinExchange::displayExchangeResult(std::string date, double value)
{
	double exchangeRate;

	/*	1. Check, if date exists in mapData.
			map::count(key) > 0 == key exists.
			'date'라는 키가 맵에 존재하면 True, 존재하지 않으면 False.

		Return format example from PDF
		= "2011-01-03 => 3 = 0.9"
	*/
	if (this->mapData.count(date) > 0)
	{
		exchangeRate = this->mapData[date];
		std::cout << date << " => " << value << " = " << exchangeRate * value << std::endl;
	}
	else
	{
		// 2. If no date is found, find the closest previous date, which is not less than given date.
		std::map<std::string, double>::iterator it = mapData.lower_bound(date);

		/* 3. Throws an exception, if 'iterator' is earlier than the given date.
			ex) dataMap["2025-04-01"] = 500;
				dataMap["2025-04-03"] = 600;
				dataMap["2025-04-05"] = 700;

				inputDate: "2025-03-01"
				closest Date: "2025-04-01"
				-> first date in the given date
				-> dataMap.begin()

			= Exchange rate calculations require the use of past data.
			  Calculations are not possible without past data.
			= 환율 계산은 과거 데이터를 사용해야 하는데, 과거 데이터가 없으면 계산 불가능
		*/
		if (it == this->mapData.begin())
			throw EarlyDateException();
		// current it: "2025-04-03 = 600"
		else
		{
			--it;						// Iterate to earlier date -> "2025-04-01 = 500"
			exchangeRate = it->second;	// it->second: dataMap's value ("500").
			std::cout << date << " => " << value << " = " << exchangeRate * value << std::endl;
		}
	}
	return ;
}

// ----------------------------------------------------------------------------------------- //

/* [ Exception ]: 7ea */
// Open: 2ea
const char *BitcoinExchange::OpenFileException::what() const throw()
{
	return ("Error: could not open file.");
}

const char *BitcoinExchange::OpenDataException::what() const throw()
{
	return ("Error: could not open data.");
}

// Date: 2ea
const char *BitcoinExchange::InvalidDateException::what() const throw()
{
	return ("Error: invalid date.");
}

const char *BitcoinExchange::EarlyDateException::what() const throw()
{
	return ("Error: too early date.");
}

// Value: 3ea
const char *BitcoinExchange::InvalidValueException::what() const throw()
{
	return ("Error: invalid value.");
}

const char *BitcoinExchange::OutOfRangeValueException::what() const throw()
{
	return ("Error: too large a number.");
}

const char *BitcoinExchange::NegativeValueException::what() const throw()
{
	return ("Error: not a positive number.");
}
