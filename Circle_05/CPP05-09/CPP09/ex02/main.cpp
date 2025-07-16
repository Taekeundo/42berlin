#include "PmergeMe.hpp"

/*	[f]
	[ Note ]
	For clarity and maintainability,
	shared logic for vector and deque operations is implemented using templates.
	This avoids code duplication while still showcasing behavior on both containers independently.

	[ Background ]
	1. Binary insert
		-> 리스트(컨테이너)의 정렬 상태를 유지하면서 적절한 위치를 탐색해서 원소를 삽입하는 방식.
		-> 일반적인 삽입(push_back(), insert(vec.end(), value)처럼 단순히 끝에 요소를
		   추가하는 방식이 아닌, 이미 정렬된 컨테이너의 정렬 상태를 유지하면서 적절한 위치를
		   찾아 삽입하는 방식이다.
	
		#include <algorithm>

		void binaryInsert(std::vector<int>& vec, int value)
		{
			auto pos = std::lower_bound(vec.begin(), vec.end(), value);
			vec.insert(pos, value);
		}

	2. Jacobsthal sequence
		-> J(n)	= J(n - 1) + 2 * J(n - 2)
		   J(0) = 0		// base case	-> 수동 정의: 무조건 0로 취급한다.
		   J(1) = 1		// base case	-> 수동 정의: 무조건 1로 취급한다.
		   J(2) = 1		// for (n >= 2)	-> n이 2이상 부터 공식을 적용한다.
		   J(3) = 3
		   J(4) = 5
		   J(5) = 11
		   J(6) = 21
		   		.
				.
				.

		->	std::set<int> idxSet;
			std::vector<int> jocobIndices;

			int jacobsthal(int n)
			{
				if (n == 0)
					return 0;
				if (n == 1)
					return 1;
				return jacobsthal(n - 1) + 2 * jacobsthal(n - 2);
			}

			for (int i = 0; ; ++i)
			{
				int j = jocobstahl(i);
				if (j >= small.size())
					break;
				if (idxSet.insert(j).second)		// prevent dup
					jacobIndices.push_back(j);
			}

	3. Ford-Johnson algorithm
		-> Merge-Insertion Sort
		-> 비교 횟수를 최소화하는 정렬 알고리즘.
		-> 입력 크기가 작고, 비교 횟수를 최소화하는게 중요한 경우 사용한다.
		-> 알고리즘이 복잡하고 유지보수가 어려워 실제로는 널리 쓰이지 않는다.

		(HOW)
		1. 입력을 pair로 묶는다.
			-> 6, 3, 9, 2, 7, 4
			-> (6, 3), (9, 2), (7, 4)

			if (size % 2 != 0)
				last_element = container.back();	// 홀수일 경우 원소 따로 저장.
		
		2. 각 pair에서 큰 숫자를 winner리스트, 작은 수를 small리스트에 저장한다.
			-> winner: [6, 9, 7]
			-> small : [3, 2, 4]

		3. winner리스트 먼저 정렬한다.
			-> sorted winner: [6, 7, 9]

		4. small값을 'Jacobsthal'순서에 맞게 winner리스트에 binary-insert해서 최종 정렬을 완성한다.
			-> 일반 순서로 binary-insert할 경우
				small [3, 2, 4] + winner: [6, 7, 9]
				Insert[3] + winner: [3, 6, 7, 9]
				Insert[2] + winner: [2, 3, 6, 7, 9]
				Insert[4] + winner: [2, 3, 4, 6, 7, 9]

			-> Jacobsthal순서로 binary-insert할 경우
				Jacobstahl: [0, 1, 1, 3, 5, 11, 21, ...]

				small = [3, 2, 4]
				index = 0, 1, 2
					N = 3 (= small리스트의 원소 개수)
				
				규칙상 Jacobstahl수열에서 N(= 3)미만 index만 사용 가능하다.
				-> Jacobstahl[0]: 0
				-> Jacobstahl[1]: 1
				-> Jacobstahl[2]: 1
				-> Jacobstahl[2]: {0, 1, 1};
								   --------
								   = 인덱스
				-----------------------------------------------------
				-> Jacobstahl수열의 값들은 small리스트에서 인덱스를 의미한다.
				-> 두개의 인덱스가 1로 중복 될 수 없기에 '중복 인덱스'를 제거한다.
				-> Jacobstahl[1]: {0, 1};
				-----------------------------------------------------
				-> 삽입 순서:
					0번 인덱스 -> 1순위 small[0]: 3
					1번 인덱스 -> 2순위 small[1]: 2
					기타      -> 3순위 small[2]: 4
				-----------------------------------------------------
				Try(1): Insert[3] + winner: [3, 6, 7, 9]
				Try(2): Insert[2] + winner: [2, 3, 6, 7, 9]
				Try(3): Insert[4] + winner: [2, 3, 4, 6, 7, 9]

			-> 삽입 방식 별 비교횟수 점검
						일반순서 삽입	|	Jacobsthal순서 삽입
					   ------------------------------------
				비교횟수 :	   6회				  6회
				= 비교횟수는 삽입한 값을 배열의 중간값과 비교하며 좌, 우로 넣어준다.
				
			-> 결론
				Ford–Johnson알고리즘은 항상 가장 적은 비교 횟수를 보장하지는 않지만,
				입력 크기인 N이 커질수록, 또는 삽입 횟수가 많아질수록 점차 효율이 좋아지는 경향이 있다.
				= "큰 입력"이 알고리즘의 핵심이다.

			-> 사례
				Ford–Johnson알고리즘이 효율적인 케이스: small list(N) == 6

				input		: [10,1, 9,2, 8,3, 7,4, 6,5, 12,0]
				pair		: [10, 1], [9, 2], [8, 3], [7, 4], [6, 5], [12, 0]
				winner		: [10, 9, 8, 7, 6, 12]
				sortedWinner: [6, 7, 8, 9, 10, 12]
				small		: [1, 2, 3, 4, 5, 0]
				---------------------------------------------------------------------
				N			: 6
				Jacobsthal	: [0, 1, 1, 3, 5]	// Jacobsthal's index < N (must be)
				remove dup  : [0, 1, 3, 5]		// 삽입 순서
				---------------------------------------------------------------------
				삽입 순서에 맞게 삽입 진행			  sorted winner
				Insert(1)	: small[0]: 1	-> [1, 6, 7, 8, 9, 10, 12]
				Insert(2)	: small[1]: 2	-> [1, 2, 6, 7, 8, 9, 10, 12]
				Insert(3)	: small[3]: 4	-> [1, 2, 4, 6, 7, 8, 9, 10, 12]
				Insert(4)	: small[5]: 0	-> [0, 1, 2, 4, 6, 7, 8, 9, 10, 12]
				---------------------------------------------------------------------
				Jacobsthal에 나와있지 않는 index의 경우 오름차순 순서로 삽입 진행.
				Insert(5)	: small[2]: 3	-> [0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 12]
				Insert(6)	: small[4]: 5	-> [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12]
	
	4. PmergeMe
		a. 사용자로부터 최소 3,000개 이상의 정수를 입력 받고,
		 -> 사용자가 프로그램 실행 시 3,000개 이상의 숫자들을 입력해야 한다.

		b. 이를 Ford-Johnson 알고리즘를 사용하여 정렬하고,
		 -> '비교 횟수를 최소화하는 알고리즘'을 직접 구현해서 사용해야 한다.

		c. std::vector, std::deque에 각각 적용한 뒤,
		 -> 동일한 입력 데이터를 두 종류의 컨테이너에 저장하여 정렬을 수행한다.

		d. 정렬 시간과 성능을 비교하는 프로그램이다.
		 -> 두 컨테이너 각각의 정렬 시간을 측정해서 성능을 비교하고 출력한다.

		= 결국 Ford-Johnson 알고리즘을 vector와 deque에 각각 적용해보고,
		  둘 중 어떤 컨테이너가 더 빠른지 정렬 시간을 측정해 비교하는 프로그램이다.

	5. Vector vs Deque
		v:
		-> contiguous memory.
		-> fast for accessing elements, with random access (like using index)
		-> 데이터가 저장된 인덱스를 알고 있기에 그 위치로 이동해서 빠르게 데이터를 가져올수 있다.

		d:
		-> fast insert and delete at both the front and back.
		-> Good for push / pop operations.
		-> 비연속적인 메모리 블록(Chunk)에 데이터가 나눠서 저장된다.
		   먼저 포인터로 블럭을 찾아간 뒤 그 안의 위치를 찾아야하는 2단계라서 복잡하고 느리다.

						vector			deque
		메모리		  	  연속저장			분산저장
		랜덤엑세스 속도		  빠름			  느림
		중간삽입 / 삭제		  느림			  빠름
		앞 삽입 / 삭제       느림			 빠름
		용도			 읽기/탐색		양방향 push / pop

		두 컨테이너들(v, d)의 템플릿 호환성:
		-> push_back(), operator[], size(), insert(begin() + n)
		-> vector, deque모두 위 기능들을 지원한다.
		-> 각 컨테이너의 특성이 템플릿 사용에 큰 영향을 미치지 않기에, 템플릿 기반으로 코드 작성함.
		-> 만약 erase()같은 연산이 있을 경우 각 컨테이너별 맞춤으로 구현했을듯싶다.

		example:
		template<typename Container>
		void insertInMiddle(Container& c, int value)
		{
			auto it = c.begin() + c.size() / 2;		// 가운데 삽입
			c.insert(it, node{value, NULL});

			auto it2 = c.begin();
			c.erase(it2);
		}
		= begin() + n:
			vector: OK	// 중간 insert삽입 기능.
			deque : OK	// 겉으로는 ok, 내부 구조는 블록 구조라 삽입 비용은 위치에 따라 다르다.
			list  : KO	// begin() + n 기능 사용불가, 컴파일 오류.
	
		= erase():
			vector: OK	// 연산 후 뒤 모든 요소들 shift
			deque : OK	// 앞 요소 지워도 shift거의 없음
			-> erase()처럼 컨테이너 특성에 따라 동작 비용이 달리지는 연산이 포함되어있다면
			   템플릿 사용보단 각 컨테이너별로 분리 구현하는게 더 바람직하다.

	[ Execution Flow tree ]

		main()
		├── Validate input(ac, av)
		│
		├── Create object (PmergeMe)
		│
		├── readInput() 
		│	 ├── isOnlyWhitespace()
		│    └── isValidInputSyntax()
		│
		├── runVectorSort()
		│    ├── clock(): Check time to start
		│	 │
		│	 ├── parseInputToContainer()
		│	 │
		│    ├── fordJohnsonSort()
		│    │    ├── splitContainerIntoPairs()				// pairs(winners, smalls)로 나눔
		│    │    ├── recursive: fordJohnsonSort(winners)	// winners먼저 정렬
		│    │    ├── extractSmallElementsFromPairs()		// pair에서 작은 값만 모음
		│    │    ├── generateInsertionOrderByJacobsthal()				// 삽입 순서 계산용 수열 생성
		│    │	  └── binaryInsert()						// 정렬 상태 유지 삽입
		│	 │
		│	 └── clock(): Check time to end
		│
		├── runDequeSort() 									// runVectorSort()과 동일한 로직
		│
		├── printResult()
		│
		└── resetData()

	[ Test ]
	1. macOS
		jot -r 2000 1 10000 |		// 1-10000중 난수 2000개 생성
		awk '!seen[$0]++' |			// 중복 제거
		head -n 3000 |				// 최대 3000개까지 제한
		tr '\n' ' ' |				// \n(줄바꿈)을 ' '(공백)으로 변환
		tee >(wc -w) |				// 인자 개수 확인 (출력)
		xargs ./PmergeMe			// 프로그램 실행 (xargs: 인자를 쉘에서 직접 전달)

	2. 사용불가 명령어 macOS기준
		./PmergeMe $(shuf -i 1-5000 -n 3000)
		./PmergeMe `shuf -i 1-8000 -n 5000 | tr "\n" " " `
		= shuf + `백틱` 조합은 macOS에서 사용 불가. Linux에선 가능.
		
	3. jot기반 명령어
		jot -r 2000 1 100000 | awk '!seen[$0]++' | head -n 3000 | tr '\n' ' ' | xargs ./PmergeMe
		= shuf대신 macOS에서 테스트할떄 사용.
*/
int main(int ac, char** av)
{
	// 1. Validate input (ac, av)
    if(ac < 2 || (ac == 2 && std::string(av[1]).empty()))
	{
        std::cerr << "[Error]: Invalid input (ac, av)" << std::endl;
		std::cerr << "Usage: ./PmergeMe <num1> <num2> ... <numN>" << std::endl;
        return 1;
    }

    PmergeMe pmergeme;					// 2. Create 'PmergeMe' object.
    if (!pmergeme.readInput(ac, av))	// 3. Read & validate Input(ac, av)
		return 1;
    pmergeme.runVectorSort();			// 4. Sort vector
    pmergeme.runDequeSort();			// 5. Sort deque
    pmergeme.printResult();				// 6. Print result
    pmergeme.resetData();				// 7. Clear memory
    return 0;
}
