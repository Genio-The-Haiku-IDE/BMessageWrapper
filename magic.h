#include <iostream>

// concatenation
#define CAT(x, y) CAT_(x, y)
#define CAT_(x,y) x ## y // error from CHAIN_COMMA: passed 4 arguments

//comma-separated (variadic)
#define COMMA() ,
#define EMPTY()
#define DEFER(id) id EMPTY()
#define COMMY(list) CAT(PR_0 list, _E)
#define PR_0(A) A PR_1
#define PR_1(A) DEFER(COMMA)() A PR_2
#define PR_2(A) DEFER(COMMA)() A PR_1
#define PR_1_E
#define PR_2_E

//KV constructor
#define KVS(list) CAT( KVS_1 list, _E)
#define KVS_1(A) XXX(A) KVS_2
#define KVS_2(A) XXX(A) KVS_1
#define KVS_1_E
#define KVS_2_E

#define XXX(T) void X(T v){};

#define SUPPORTED_TYPES (int32) (bool) (const char*) (BString) (GMessage) (void*) (entry_ref)
void magic()
{
	std::variant<COMMY( SUPPORTED_TYPES )> map;

};
KVS((int) (const char*));

