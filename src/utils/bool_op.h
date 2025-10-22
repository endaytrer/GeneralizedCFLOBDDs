#ifndef BOOL_OP_GUARD
#define BOOL_OP_GUARD

// BoolOp's ----------------------------------------------------------
typedef bool BoolOp[2][2];
extern BoolOp trueOp;           //  \a.\b.true
extern BoolOp falseOp;          //  \a.\b.false
extern BoolOp andOp;            //  \a.\b.(a && b)
extern BoolOp nandOp;           //  \a.\b.!(a && b)
extern BoolOp orOp;             //  \a.\b.(a || b)
extern BoolOp norOp;            //  \a.\b.!(a || b)
extern BoolOp iffOp;            //  \a.\b.(a == b)
extern BoolOp exclusiveOrOp;    //  \a.\b.(a != b)
extern BoolOp impliesOp;        //  \a.\b.(!a || b)
extern BoolOp minusOp;          //  \a.\b.(a && !b)
extern BoolOp quotientOp;       //  \a.\b.(!b || a)
extern BoolOp notQuotientOp;    //  \a.\b.(b && !a)
extern BoolOp firstOp;          //  \a.\b.a
extern BoolOp notFirstOp;       //  \a.\b.!a
extern BoolOp secondOp;         //  \a.\b.b
extern BoolOp notSecondOp;      //  \a.\b.!b

extern int TrueFunc(int a, int b);           //  \a.\b.true
extern int FalseFunc(int a, int b);          //  \a.\b.false
extern int AndFunc(int a, int b);            //  \a.\b.(a && b)
extern int NandFunc(int a, int b);           //  \a.\b.!(a && b)
extern int OrFunc(int a, int b);             //  \a.\b.(a || b)
extern int NorFunc(int a, int b);            //  \a.\b.!(a || b)
extern int IffFunc(int a, int b);            //  \a.\b.(a == b)
extern int ExclusiveOrFunc(int a, int b);    //  \a.\b.(a != b)
extern int ImpliesFunc(int a, int b);        //  \a.\b.(!a || b)
extern int MinusFunc(int a, int b);          //  \a.\b.(a && !b)
extern int QuotientFunc(int a, int b);       //  \a.\b.(!b || a)
extern int NotQuotientFunc(int a, int b);    //  \a.\b.(b && !a)
extern int FirstFunc(int a, int b);          //  \a.\b.a
extern int NotFirstFunc(int a, int b);       //  \a.\b.!a
extern int SecondFunc(int a, int b);         //  \a.\b.b
extern int NotSecondFunc(int a, int b);      //  \a.\b.!b

// BoolOp3's ----------------------------------------------------------
typedef bool BoolOp3[2][2][2];
extern BoolOp3 ifThenElseOp;           //  \a.\b.\c.(a && b) || (!a && c)
extern BoolOp3 negMajorityOp;          //  \a.\b.\c.(b && !a) || (c && !a) || (b && c)

extern int IfThenElseFunc(int a, int b, int c);           //  \a.\b.\c.(a && b) || (!a && c)
extern int NegMajorityFunc(int a, int b, int c);          //  \a.\b.\c.(b && !a) || (c && !a) || (b && c)

// Some polymorphic non-Boolean functions ------------------------------------------------
template <typename T>
T PlusFunc(T a, T b) {
	return a + b;
}

template <typename T>
T TimesFunc(T a, T b) {
	return a * b;
}

#endif
