#include <iostream>

// concatenation
#define CAT(x, y) CAT_(x, y)
#define CAT_(x,y) x ## y // error from CHAIN_COMMA: passed 4 arguments

//variadic
#define COMMA() ,
#define EMPTY()
#define DEFER(id) id EMPTY()
#define COMMY(list) CAT(PR_0 list, _E)
#define PR_0(A) A PR_1
#define PR_1(A) DEFER(COMMA)() A PR_2
#define PR_2(A) DEFER(COMMA)() A PR_1
#define PR_1_E
#define PR_2_E

void magic()
{
	std::variant<COMMY( (int) (long) )> map;

};
