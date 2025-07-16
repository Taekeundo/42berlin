#pragma once

#include <iostream>		// std::cout, std::cerr
#include <iomanip>		// std::fixed, std::setprecision
#include <string>		// std::string, string functions.
#include <cctype>		// std::isspace, std::digit
#include <vector>		// std::vector
#include <deque>		// std::deque
#include <sstream>		// std::istringstream
#include <ctime>		// clock_t, clock(), CLOCKS_PER_SEC

/*	[f]
	1. Input: 10 3 5 9 8 1

	2. Pair: (10, 3), (5, 9), (8, 1)
	
	3. Current node
		= consist of bigger elements from each pair.
		= 10, 9, 8

	4. Small node
		= consist of smaller elements from each pair.
		= 3, 5, 1

	5. Each current node(= "value")'s element -> points -> Each small node(= "value: *small")'s element
		= current -> small
		= 10 -> 3
		= 9 -> 5
		= 8 -> 1
	
	6. Overview
		node n1 = {10, pointer_to_node_3};
		node n2 = {9, pointer_to_node_5};
		node n3 = {8, pointer_to_node_1};

	[ Member ]
	 value		The number to sort.
	 small		list for saving smaller element from the original pair <-> Winner.
*/
struct node
{
    int	value;
	node *small;

	node(int v = 0, node* s = NULL)
	{
		value = v;
		small = s;
	}
};

class PmergeMe
{
	private:
		// 1. Input
		std::string input;

		// 2. Original data
		std::vector<node> originalVector;
		std::deque<node> originalDeque;

		// 3. Sorted data
		std::vector<node> sortedVector;
		std::deque<node> sortedDeque;

		// 4. Duration time
		double vectorSortTime;
		double dequeSortTime;

	public:
		// OCF: 4ea
		PmergeMe();
		~PmergeMe();
		PmergeMe(const PmergeMe &other);				// Copy(Create new object)
		PmergeMe& operator=(const PmergeMe &other);		// Reference(Using original object)

		// Input: 3ea
		bool readInput(int ac, char **av);
		bool isOnlyWhitespace(const std::string& input);
		bool isValidInputSyntax(std::string input);

		// Vector: 2ea
		void runVectorSort();
		void parseInputToVector();

		// Deque: 2ea
		void runDequeSort();
		void parseInputToDeque();

		// ETC: 2ea
		void printResult() const;
		void resetData();

		// Common (vector & deque): 7ea
		/*	[f]
			[ Backgroud ]
			1. 
			std::istringstream	// Input only stringstream.		// Read only
			std::ostringstream	// Output only stringstream.	// Write only
			std::stringstream	// I/O stringstream.			// R + W

			2. 
			int number;
			iss(string) >> number(int)
			= 공백 단위로 문자열을 잘라서 int로 자동 변환해주는 operator.
			= number에 기존에 어떤 값이 들어있던 덮어쓰기 때문에 값이 들어있어도 상관없다.

			[ "Input" to "Vector" ]
			Input: "1 3 7 4"

			while('1')
			-> node current.value = '1'
			-> node current.small = NULL
			-> vector.push_back(current(1, NULL))

			while ('2')
			-> node current.value = '2'
			-> node current.small = NULL
			-> vector.push_back(current(2, NULL))

			...

			vector
			{
				node{value: 1, small: NULL},
				node{value: 3, small: NULL},
				node{value: 7, small: NULL},
				node{value: 4, small: NULL}
			}
		
			deque
			{
				node{value: 1, small: NULL},
				node{value: 3, small: NULL},
				node{value: 7, small: NULL},
				node{value: 4, small: NULL}
			}
			
			[ Logic ]
			1. Parse input string to 'int' type.
			2. Create node object for storing data to object.
			3. Store each node in the container(vector || deque) using push_back().
		*/
		template<typename Container>
		void parseInputToContainer(Container& container, const std::string& input)
		{
			std::istringstream iss(input);
			int number;
			while(iss >> number)
			{
				node current(number, NULL);
				container.push_back(current);
			}
		}

		/*	[f]
			[ Example ]
			Input(data): 1 3 7 4 9 6 8 2
			Index      : 0 1 2 3 4 5 6 7 == data.size() - 1 == 8 - 1 == 7

			[ Loop: i += 2 ]
			= Jump one element -> compare -> save
			Step 1: Compare '1' vs '3' -> bigger: 3, smaller: 1
			Step 2: Compare '7' vs '4' -> bigger: 7, smaller: 4
			Step 3: Compare '9' vs '6' -> bigger: 9, smaller: 6
			Step 4: Compare '8' vs '2' -> bigger: 8, smaller: 2

			[ Logic ]
			1. Compare each pair of two consecutive nodes. (e.g.. (1,3), (7,4))
			2. Save the bigger value in '.value', and set '.small' to point to the smaller one.
			   = In other words, link the smaller element to the bigger one using a pointer.
			3. Store the resulting node into parma(2):the 'winners' container, using push_back().

			[ Result: winners ]
			[
				node { value: 3, small -> point -> node{1} },
				node { value: 7, small -> point -> node{4} },
				node { value: 9, small -> point -> node{6} },
				node { value: 8, small -> point -> node{2} }
			]
		*/
		template<typename Container>
		void splitContainerIntoPairs(Container& data, Container& winners)
		{
			for(size_t i = 0; i < data.size() - 1; i += 2)
			{
				node current;
				if(data[i].value > data[i + 1].value)	
					current = node(data[i].value, &data[i + 1]);
				else
					current = node(data[i + 1].value, &data[i]);
				winners.push_back(current);
			}
		}

		/*	[f]
			The title says it all
		*/
		template<typename Container>
		void extractSmallElementsFromPairs(const Container& data, const Container& winners, Container& smalls)
		{
			for (size_t i = 0; i < winners.size(); ++i)
			{
				if (winners[i].small != NULL)
				{
					node smallNode(winners[i].small->value, NULL);
					smalls.push_back(smallNode);
				}
			}

			/*
				If the number of elements is odd,
				the last element doesn't have a pair,
				so it must also be added to the 'smalls' container.
			*/
			if (data.size() % 2 != 0)
			{
				node leftoverElement(data.back().value, NULL);
				smalls.push_back(leftoverElement);
			}
		}

		/*	[f]
			[ Background ]
			1. Jacobsthal sequence: Check README.md file.
				= [0, 1, 1, 3, 5, 11, 21, ...] // if (n >= 2)

			[ Overview ]
			Generate an insertion order of indices for the 'smalls' elements,
			based on a customized Jacobsthal sequence,
			which determines the order in which small elements
			are inserted into the sorted winner list
			during the Ford-Johnson merge-insert sort."
			= smalls의 요소들을 어떤 순서로 삽입할지 결정하기 위해,
			  커스텀 Jacobsthal 수열을 기반으로 한 삽입 인덱스 순서를 생성하는 함수.

			[ Custom jacobsthal sequence ]
			1. J(0)을 제외하고 J(1)부터 시작
				일반 Jacobsthal은 J(0) = 0, J(1) = 1이지만,
				이 구현에서는 0을 직접 order.push_back(0) 해주고 수열에는 포함하지 않음.

			2. 필요한 만큼만 수열을 생성 (n보다 큰 수가 나올 때까지)
				무한 Jacobsthal 수열 전체가 아닌,
				smalls.size()에 맞게 수열을 조기 종료하여 동적 생성.

			3. 생성한 수열을 그대로 쓰지 않고 ‘인덱스 삽입 순서’로 재가공함
				일반 수열처럼 단순히 J(1), J(2)...를 나열하는 것이 아니라,
				각 수열 항 사이의 범위를 역방향(큰→작은) 으로 순회하면서 order를 구성함.
			
				jacob = [1, 3, 5, 11] -> order = [0, 2, 1, 4, 3, 5]
				= 일반적인 수열 순서와 다른 흐름의 삽입 순서가 생성된다.
				= 중간값을 먼저 넣고, 양 옆으로 확장하는 균형 잡힌 삽입 순서.
				  결과적으로 이진 탐색 시, 비교 횟수를 최소화 할 수 있다.
				  Ford_Johnson알고리즘의 핵심 아이디어. 그래서 굳이 복잡해도 구현한거다.

			[ Logic ]
			1. Create Jacobsthal sequence: 1, 3, 5, 11, 21, ... until it exceeds parma:'n'.
			2. Always add 0 first to the final order. (= Rule for Jacobsthal sequence).
			3. For each Jacobsthal number, iterate backwards from (jacob[i] - 1)
				down to jacob[i-1], and collect valid indices (excluding 0 and >= n).
			4. Return the insertion order container(= vector || deque).
		*/
		template<typename Container>		
		Container generateInsertionOrderByJacobsthal(size_t n)
		{
			Container order;
			std::vector<size_t> jacob;	// 내부 계산용이라서 vector로 사용해도 deque도 받을수 있고 전혀 지장없음.

			/*
				왜 초기값 2개가 필요한가?
					Jacobsthal sequence가 while루프를 통해 만들어지기 위해선
					공식에 필요한 2개의 수열 초기 값이 필요하다.
				
					ex) j(n) = j(n-1) + 2 * j(n - 2)
					= j(n)을 while루프에서 계산하기 위해선 최소한 j(n-1), j(n-2)값이 이미 존재해야한다.
					= 초기값 두개는 직접 넣어줘야 루프가 돌아간다.  
					= 초기값: n-2: J(1), n-1: J(3)

				J(2)가 아니라 J(3)을 해준 이유:
					Jacobsthal sequence은 일반적으로 J(0) = 0, J(1) = 1부터 시작하지만,
					이 구현에서는 알고리즘에 필요한 삽입 순서를 만들기 위해
					"1 3 5 11 21 .."처럼 J(1), J(3), J(4), J(5) .. 순으로 사용한다.

					J(0) = 0	// 따로 직접 order.push_back(0) 해줄꺼라 생략.
					J(2) = 1	// J(1)과 중복되어서 생략.
					J(1), J(3)	// 순서 역산 로직에 베이스가 되는 최소 2가지 요소.

					jacob = [1, 3]
			*/
			jacob.push_back(1);
			jacob.push_back(3);

			/*
				Build Jacobsthal sequence until exceed 'n'
			
				example:
				jacob = [1, 3] & n = 6

				loop(1): 3 < 6
					-> next = jacob[2 - 1] + 2*jacob[2 - 2]
							= jacob[1] + 2*jacob[0]
							= 3 + 2*1
							= 5
					-> jacob = [1, 3, 5]

				loop(2): 5 < 6
					-> next = jacob[3 - 1] + 2*jacob[3 - 2]
							= jacob[2] + 2*jacob[1]
							= 5 + 2*3
							= 5 + 6
							= 11
					-> jacob = [1, 3, 5, 11]
			*/
			while (jacob.back() < n)
			{
				size_t next = jacob[jacob.size() - 1] + 2 * jacob[jacob.size() - 2];
				jacob.push_back(next);
			}

			// Always insert '0' first to result(= 'order')
			if (n > 0)
				order.push_back(0);

			/*
				for (i = 1; i < 4; i++) -> 1, 2, 3	-> 3times
				for (j = 2; j > 0; j--)	-> 2, 1		-> 2times
				
								high	low		j		filter(if)			order.push_back(j): 
				loop(i = 1):	3		1		2		2 < 6, 2 != 0		2
												1		1 < 6, 1 != 0		1		2times
				loop(i = 2):	5		3		4		4 < 6, 4 != 0		4
												3		3 < 6, 3 != 0		3		2times
				loop(i = 3):	11		5		10		10 < 6 (fail)		X
												9		9 < 6				X
												8		8 < 6				X
												7		7 < 6				X
												6		6 < 6				X
												5		5 < 6, 5 != 0		5
												4		j:4 > 4 (fail)		Stop loop.
				
				-> order = [0, 2, 1, 4, 3, 5]
			*/
			for (size_t i = 1; i < jacob.size(); ++i)
			{
				size_t high = jacob[i];
				size_t low = jacob[i - 1];
				/*
					j: Jacobsthal 수열을 기반으로 계산된 인덱스

					1. j < 2
						범위 초과 방지
						smalls.size() == n == 6
						jacob = [1, 3, 5, 11]
								jacob[0]: 1
								jacob[1]: 3
								jacob[2]: 5
								jacob[3]: 11

						if) i == 3
							i:3 < jacob.size():4
							-> high: jacob[3] == 11
								low : jacob[2] == 5

						for (j = 10; j > 4; --j)
							-> j: 10, 9, 8, 7, 6
							-> smalls의 유효 인덱스는 0 ~ 5이다.
							-> 만약 j < n 조건이 없을 경우
								존재하지 않는 smalls[6], [7], [8], [9], [10]을 생성한다.
							-> 하지만 smalls의 크기는 6이기에 6이상의 인덱스는 out-of-bounds오류다.

						요약
						= Jacobstahal수열에 따라 계산된 smalls의 인덱스 j가
							실제 smalls벡터가 존재하는 범위 안에 있을때만 push하라는 조건.

					2. j != 0
						skip case (j == 0). already push '0' to order over code(= order.push_back(0)).
				*/
				for (size_t j = high - 1; j > low - 1; --j)
				{
					if (j < n && j != 0)
						order.push_back(j);
				}
			}
			return order;
		}

		/*	[f]
			[ Process ]
			(1) Input
				std::string input = "1 3 7 4 9 6 8 2"
		
			(2) Container& original
				vector('original') =
				[
					node{ value: 1, small: NULL },
					node{ value: 3, small: NULL },
					node{ value: 7, small: NULL },
					node{ value: 4, small: NULL },
					node{ value: 9, small: NULL },
					node{ value: 6, small: NULL },
					node{ value: 8, small: NULL },
					node{ value: 2, small: NULL }
				]

			(3) splitContainerIntoPairs(original, winners);
				vector('winners')
				[
					node { value: 3, small -> point -> node{1} },		// (1, 3)
					node { value: 7, small -> point -> node{4} },		// (7, 4)
					node { value: 9, small -> point -> node{6} },		// (9, 6)
					node { value: 8, small -> point -> node{2} }		// (8, 2)
				]
			
			(4) sortedWinners
				vector('winners')		== [3, 7, 9 ,8]
				vector('sortedWinners')	== [3, 7, 8, 9]

			(5) extractSmallElementsFromPairs(original, winners, smalls);
				vector('smalls')
				[
					node{ value: 1, small: NULL },
					node{ value: 4, small: NULL },
					node{ value: 6, small: NULL },
					node{ value: 2, small: NULL }
				]

			(6) generateInsertionOrderByJacobsthal<std::vector<int> >(smalls.size());
				result(order) = [0, 2, 1, 3]
				insertOrder = smalls[0], smalls[2], smalls[1], smalls[3]
								  1		     6			4		   2
	
			(7)	binaryInsert(sortedWinners, smalls[idx].value);
				insert param(2) to param(1) following by insertOrder.

							value			sortedWinners
				Start						[3, 7, 8, 9]
				insert(1)	smalls[0]: 1	[1, 3, 7, 8, 9]
				insert(2)	smalls[2]: 6	[1, 3, 6, 7, 8, 9]
				insert(3)	smalls[1]: 4	[1, 3, 4, 6, 7, 8, 9]
				insert(4)	smalls[3]: 2	[1, 2, 3, 4, 6, 7, 8, 9]

			[ Summary ]
			1. input            "1 3 7 4 9 6 8 2"
			2. original         [1, 3, 7, 4, 9, 6, 8, 2]
			3. winners          [3, 7, 9, 8]       			points each element (1,4,6,2)
			4. sortedWinners    [3, 7, 8, 9]       			Ford-Johnson recursive sort
			5. smalls           [1, 4, 6, 2]
			6. insertOrder      [0, 2, 1, 3]       			Order based on Jacobsthal
			7. binaryInsert
				- insert smalls[0]: 1
				- insert smalls[2]: 6
				- insert smalls[1]: 4
				- insert smalls[3]: 2

			8. Final result after sort.
				[1, 2, 3, 4, 6, 7, 8, 9]

			[ Logic ]
			1. winners: Make pair(bigger, smaller) and link element eachother.
			2. sortedWinners: Sort the bigger elements only using Ford-Johnson algorithm.
			3. smalls: Extract smalls from winners.
			4. insertOrder:
				Generate the order in which small elements will be inserted,
				based on the Jacobsthal sequence. // Exercise's rule.
		*/
		template<typename Container>
		Container fordJohnsonSort(Container& original)
		{
			if(original.size() < 2)	// No need to sort.
				return original;

			Container winners;
			splitContainerIntoPairs(original, winners);

			Container sortedWinners = fordJohnsonSort(winners);

			Container smalls;
			extractSmallElementsFromPairs(original, winners, smalls);

			/*
				WHY? vector type
				= 내부 연산용 변수일뿐, 함수 동작과 return에 전혀 관련이 없다.
				= char type return함수 중간에 int i를 연산용으로 사용하는것과 같은 개념.
			*/
			std::vector<int> insertOrder;
			insertOrder = generateInsertionOrderByJacobsthal<std::vector<int> >(smalls.size());

			/*
				insertOrder순서에 따라 smalls의 요소들을
				[f]binaryInsert()를 사용해서 sortedWinners에 하나씩 삽입한다.
			*/
			for (size_t i = 0; i < insertOrder.size(); ++i)
			{
				size_t smallsIndex = insertOrder[i];
				if (smallsIndex < smalls.size())
					binaryInsert(sortedWinners, smalls[smallsIndex].value);
			}
			return sortedWinners;
		}

		/*	[f]
			[ Background ]
			1. Binary Search
				Fast algorithm for finding a target value in a sorted list,
				by repeatedly dividing the search range in half.
				Cut the list in half each time until finding the value.
			
			[ Role ]
			Inserts a value into a sorted container at the correct position
			to maintain ascending order, using binary search to minimize comparison operations.

			[ How ]
			1. Search where the Param(2):'value' should go using binary search(= fast way to find the position).
			2. Make a new node, holds that value.
			3. Put the new node into the Param(1):'container' at the right spot.

			[ Example ]
			container = [3, 6, 9]
			value = 7

					left		right		mid		container[mid].value < value
			try1	0			3			1		container[1].value: 6 < 7		left = mid + 1;
			try2	2			3			2		container[2].value: 9 > 7		right = mid;
			----------------------------------------------------------------------------------------
			try3	2			2			// left == right -> exit while loop.

			current(7, small -> null)
			= insert 'current' to begin() + left(2)
			= [3, 6, 7, 9];
		*/
		template<typename Container>
		void binaryInsert(Container& sortedContainer, int value)
		{
			size_t left = 0;
			size_t right = sortedContainer.size();

			while (left < right)
			{
				size_t mid = (left + right) / 2;
				if (sortedContainer[mid].value < value)
					left = mid + 1;
				else
					right = mid;
			}

			node newNodeForInsertion(value, NULL);
			sortedContainer.insert(sortedContainer.begin() + left, newNodeForInsertion);
		}
};
