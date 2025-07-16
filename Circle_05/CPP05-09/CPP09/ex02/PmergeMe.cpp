
#include "PmergeMe.hpp"

//	[f] OCF: 4ea
PmergeMe::PmergeMe() {}
PmergeMe::~PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe &other)
{
    input = other.input;

    originalVector = other.originalVector;
	sortedVector = other.sortedVector;
	vectorSortTime = other.vectorSortTime;

    originalDeque = other.originalDeque;
    sortedDeque = other.sortedDeque;
    dequeSortTime = other.dequeSortTime;
}

PmergeMe& PmergeMe::operator=(const PmergeMe &other)
{
    if(this != &other)
	{
        input = other.input;

        originalVector = other.originalVector;
		sortedVector = other.sortedVector;
		vectorSortTime = other.vectorSortTime;

        originalDeque = other.originalDeque;
        sortedDeque = other.sortedDeque;
        dequeSortTime = other.dequeSortTime;
    }
    return *this;
}

//	[f] Input: 3ea
bool PmergeMe::isOnlyWhitespace(const std::string& input)
{
    if (input.empty())
	{
        std::cerr << "[Error]: Input is empty" << std::endl;
        return false;
    }

    bool hasOnlyWhitespace = true;
    for (size_t i = 0; i < input.length(); i++)
	{
		if (!std::isspace(input[i]))
		{
            hasOnlyWhitespace = false;
            break;
        }
    }

    if (hasOnlyWhitespace)
	{
        std::cerr << "[Error]: Input has only whitespace" << std::endl;
        return false;
    }

    return true;
}

/*	[f]
    1. Skip whitespace and parse integers from the input string.
    2. Check for invalid characters, leading zeros, and standalone zero.
    3. Check for duplicate numbers in the parsed result.
*/
bool PmergeMe::isValidInputSyntax(std::string input)
{
    std::vector<int> numbers;
    std::string::iterator current = input.begin();

    while(current != input.end())
	{
        while (current != input.end() && std::isspace(*current))
            ++current;

        if (current == input.end())
            break;

        if (!std::isdigit(*current))
		{
            std::cerr << "[Error]: Invalid input (No digit)" << std::endl;
            return false;
        }

        int num = 0;
        while (current != input.end() && std::isdigit(*current))
		{
            if (*current == '0' && num == 0)
			{
                std::cerr << "[Error]: Invalid input (leading zero or 0)" << std::endl;
                return false;
            }
            num = num * 10 + (*current - '0');
            ++current;
        }

        numbers.push_back(num);
    }

    for (size_t i = 0; i < numbers.size(); ++i)
	{
        for (size_t j = i + 1; j < numbers.size(); ++j)
		{
            if (numbers[i] == numbers[j])
			{
                std::cerr << "[Error]: Duplicate number (" << numbers[i] << ")" << std::endl;
                return false;
            }
        }
    }

    return true;
}

bool PmergeMe::readInput(int ac, char** av)
{
    std::string rawInput;

    for(int i = 1; i < ac; i++)
	{
        rawInput += av[i];
        rawInput += " ";
    }

    if(!isOnlyWhitespace(rawInput))
        return false;

    if(!isValidInputSyntax(rawInput))
        return false;

    input = rawInput;
	return true;
}

/*	[f] Vector: 2ea
	Delegating function: just forwarding to delegate data.
*/
void PmergeMe::parseInputToVector()
{
	parseInputToContainer(originalVector, input);
}

/*	[f]
	Data type
	1. clock()		  : long
	2. end - start	  : integer
	3. CLOCKS_PER_SEC : integer
	---------------------------
	4. vectorSortTime : double
	= Use 'static_cast<double>'
*/
void PmergeMe::runVectorSort()
{
    clock_t start = clock();
    parseInputToVector();
    sortedVector = fordJohnsonSort(originalVector);
    clock_t end = clock();
    vectorSortTime  = static_cast<double>(end - start) / CLOCKS_PER_SEC;
}

//	[f] Deque: 2ea
void PmergeMe::parseInputToDeque()
{
	parseInputToContainer(originalDeque, input);
}

void PmergeMe::runDequeSort()
{
    clock_t start = clock();
    parseInputToDeque();
    sortedDeque = fordJohnsonSort(originalDeque);
    clock_t end = clock();
    dequeSortTime  = static_cast<double>(end - start) / CLOCKS_PER_SEC;
}

//	[f] ETC: 2ea
/*	[f]
	if(originalVector.size() == 0) return;
	= Already validate in main(), so don't need here.

	const memeber function: Be sure to not to change any member variables.
*/
void PmergeMe::printResult() const
{
    std::cout << "Before: ";
    for(std::vector<node>::const_iterator it = originalVector.begin(); it != originalVector.end(); ++it)
	{
        std::cout << it->value << " ";
    }
    std::cout << std::endl;

    std::cout << "After: ";
    for(std::vector<node>::const_iterator it = sortedVector.begin(); it != sortedVector.end(); ++it)
	{
        std::cout << it->value << " ";
    }
    std::cout << std::endl;

    std::cout << "Time to process a range of " << originalVector.size() << " elements with std::vector : " 
        << std::fixed << std::setprecision(5) << vectorSortTime * 1000 << " ms" << std::endl;

    std::cout << "Time to process a range of " << originalDeque.size() << " elements with std::deque : "
        << std::fixed << std::setprecision(5) << dequeSortTime* 1000 << " ms" << std::endl;
}

/*	[f]
	clear()
	- std::string::clear()
	- std::vector<T>::clear()
	- std::deque<T>::clear()
	-> Those 3 data types is available to use [f]clear().
*/
void PmergeMe::resetData()
{
	input.clear();

    originalVector.clear();
	originalDeque.clear();

	sortedVector.clear();
    sortedDeque.clear();

	vectorSortTime = 0.0;
	dequeSortTime = 0.0;
}
