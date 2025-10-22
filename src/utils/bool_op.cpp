#include <cassert>
#include "bool_op.h"

// BoolOp's ----------------------------------------------------------
BoolOp trueOp =          { {true,   true},    //  \a.\b.true
                           {true,   true}
                         };
BoolOp falseOp =         { {false, false},    //  \a.\b.false
                           {false, false}
                         };
BoolOp andOp =           { {false, false},    //  \a.\b.(a && b)
                           {false,  true}
                         };
BoolOp nandOp =          { {true,   true},    //  \a.\b.!(a && b)
                           {true,  false}
                         };
BoolOp orOp  =           { {false,  true},    //  \a.\b.(a || b)
                           {true,   true}
                         };
BoolOp norOp =           { {true,  false},    //  \a.\b.!(a || b)
                           {false, false}
                         };
BoolOp iffOp =           { {true,  false},    //  \a.\b.(a == b)
                           {false,  true}
                         };
BoolOp exclusiveOrOp =   { {false,  true},    //  \a.\b.(a != b)
                           {true,  false}
                         };
BoolOp impliesOp =       { {true,   true},    //  \a.\b.(!a || b)
                           {false,  true}
                         };
BoolOp minusOp =         { {false, false},    //  \a.\b.(a && !b)
                           {true,  false}
                         };
BoolOp quotientOp =      { {true,  false},    //  \a.\b.(a || !b)
                           {true,   true}
                         };
BoolOp notQuotientOp =   { {false,  true},    //  \a.\b.(!a && b)
                           {false, false}
                         };
BoolOp firstOp =         { {false, false},    //  \a.\b.a
                           {true,   true}
                         };
BoolOp notFirstOp =      { {true,   true},    //  \a.\b.!a
                           {false, false}
                         };
BoolOp secondOp =        { {false,  true},    //  \a.\b.b
                           {false,  true}
                         };
BoolOp notSecondOp =     { {true,  false},    //  \a.\b.!b
                           {true,  false}
                         };


int TrueFunc(int a, int b) {
	assert(0 <= a && a <= 1 && 0 <= b && b <= 1);
	return trueOp[a][b];
}

int FalseFunc(int a, int b) {
	assert(0 <= a && a <= 1 && 0 <= b && b <= 1);
	return falseOp[a][b];
}

int AndFunc(int a, int b) {
	assert(0 <= a && a <= 1 && 0 <= b && b <= 1);
	return andOp[a][b];
}

int NandFunc(int a, int b) {
	assert(0 <= a && a <= 1 && 0 <= b && b <= 1);
	return nandOp[a][b];
}

int OrFunc(int a, int b) {
	assert(0 <= a && a <= 1 && 0 <= b && b <= 1);
	return orOp[a][b];
}

int NorFunc(int a, int b) {
	assert(0 <= a && a <= 1 && 0 <= b && b <= 1);
	return norOp[a][b];
}

int IffFunc(int a, int b) {
	assert(0 <= a && a <= 1 && 0 <= b && b <= 1);
	return iffOp[a][b];
}

int ExclusiveOrFunc(int a, int b) {
	assert(0 <= a && a <= 1 && 0 <= b && b <= 1);
	return exclusiveOrOp[a][b];
}

int ImpliesFunc(int a, int b) {
	assert(0 <= a && a <= 1 && 0 <= b && b <= 1);
	return impliesOp[a][b];
}

int MinusFunc(int a, int b) {
	assert(0 <= a && a <= 1 && 0 <= b && b <= 1);
	return minusOp[a][b];
}

int QuotientFunc(int a, int b) {
	assert(0 <= a && a <= 1 && 0 <= b && b <= 1);
	return quotientOp[a][b];
}

int NotQuotientFunc(int a, int b) {
	assert(0 <= a && a <= 1 && 0 <= b && b <= 1);
	return notQuotientOp[a][b];
}

int FirstFunc(int a, int b) {
	assert(0 <= a && a <= 1 && 0 <= b && b <= 1);
	return firstOp[a][b];
}

int NotFirstFunc(int a, int b) {
	assert(0 <= a && a <= 1 && 0 <= b && b <= 1);
	return notFirstOp[a][b];
}

int SecondFunc(int a, int b) {
	assert(0 <= a && a <= 1 && 0 <= b && b <= 1);
	return notSecondOp[a][b];
}

int NotSecondFunc(int a, int b) {
	assert(0 <= a && a <= 1 && 0 <= b && b <= 1);
	return notSecondOp[a][b];
}


// BoolOp3's ----------------------------------------------------------
// \a.\b.\c.(a && b) || (!a && c)
//                           ________________________________________________________
//                                         b
//                                false          true
//                           --------------------------------------------------------
//                                  c               c
//                            false   true    false   true 
//                            _______________________________________________________
BoolOp3 ifThenElseOp =   { { {false,  true}, {false,  true} },    //  | false |  a
                           { {false, false}, { true,  true} }     //  | true  |
                         };

// \a.\b.\c.(b && !a) || (c && !a) || (b && c)
//                          ________________________________________________________
//                                        b
//                               false          true
//                          --------------------------------------------------------
//                                 c               c
//                           false   true    false   true 
//                           _______________________________________________________
BoolOp3 negMajorityOp = { { {false,  true}, {true,  true} },    //  | false |  a
                          { {false, false}, {false, true} }     //  | true  |
                         };

int IfThenElseFunc(int a, int b, int c) {           //  \a.\b.\c.(a && b) || (!a && c)
	assert(0 <= a && a <= 1 && 0 <= b && b <= 1 && 0 <= c && c <= 1);
	return ifThenElseOp[a][b][c];
}

int NegMajorityFunc(int a, int b, int c) {          //  \a.\b.\c.(b && !a) || (c && !a) || (b && c)
	assert(0 <= a && a <= 1 && 0 <= b && b <= 1 && 0 <= c && c <= 1);
	return negMajorityOp[a][b][c];
}


