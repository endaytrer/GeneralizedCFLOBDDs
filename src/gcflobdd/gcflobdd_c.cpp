#include "gcflobdd_c.h"
#include "../grammar/grammar.h"
#include "gcflobdd_t.h"
#include "gcflobdd_node.h"
#include "../ops/gcflobdd_int.h"
#include "../ops/cross_product.h"
#include "../ops/cross_product_bdd.h"
#include "../utils/assignment.h"

static_assert(sizeof(G_CFL_OBDD::G_CFLOBDD) == sizeof(GCFLOBDD));
extern "C" {

void GCFLOBDD_module_init() {
    G_CFL_OBDD::G_CFLOBDDNodeHandle::InitLeafNodes();
    G_CFL_OBDD::InitPairProductCache();
    G_CFL_OBDD::InitBDDPairProductCache();
    G_CFL_OBDD::G_CFLOBDDNodeHandle::InitReduceCache();
    G_CFL_OBDD::BDDNodeHandle::InitReduceCache();
}

void GCFLOBDD_module_dispose() {
    G_CFL_OBDD::G_CFLOBDDNodeHandle::DisposeOfReduceCache();
    G_CFL_OBDD::DisposeOfPairProductCache();
    G_CFL_OBDD::DisposeOfBDDPairProductCache();
    G_CFL_OBDD::BDDNodeHandle::DisposeOfReduceCache();
}

class GCFLOBDD_Nodestruct {
private:
    union {
        G_CFL_OBDD::G_CFLOBDD gcflobdd;
    };
public:
    GCFLOBDD_Nodestruct(G_CFL_OBDD::G_CFLOBDD &&gcflobdd) : gcflobdd(std::move(gcflobdd)) {}
    ~GCFLOBDD_Nodestruct() {}
    GCFLOBDD get_value() {
        return *(GCFLOBDD *)&gcflobdd;
    }
};

GCFLOBDD cast(G_CFL_OBDD::G_CFLOBDD &&gcflobdd) {
    GCFLOBDD_Nodestruct a1(std::move(gcflobdd));
    return a1.get_value();
}
G_CFL_OBDD::G_CFLOBDD retrieve(GCFLOBDD gcflobdd) {
    return G_CFL_OBDD::G_CFLOBDD(std::move(*(G_CFL_OBDD::G_CFLOBDD *)(&gcflobdd)));
}

void forget(G_CFL_OBDD::G_CFLOBDD &&gcflobdd) {
    GCFLOBDD_Nodestruct a1(std::move(gcflobdd));
}

GCFLOBDD_Grammar GCFLOBDD_createGrammar(const char *productions[], size_t num_productions, const char *start_symbol) {
    std::shared_ptr<Grammar> *grammar = new std::shared_ptr<Grammar>(new Grammar());
    std::vector<std::string> productions_vec(productions, productions + num_productions);

    (*grammar)->constructGrammar(productions_vec, start_symbol);
    
    (*grammar)->InstallNumVars();
    (*grammar)->updateLevel();
    return (void *)grammar;
}

void GCFLOBDD_Grammar_delete(GCFLOBDD_Grammar grammar) {
    delete (std::shared_ptr<Grammar> *)grammar;
}

GCFLOBDD GCFLOBDD_createVar(uint32_t i, int level, GCFLOBDD_Grammar_Ref grammar) {
    auto grammar1 = (std::shared_ptr<Grammar> *)grammar;
    return cast(std::move(G_CFL_OBDD::MkProjection(i, level, *grammar1)));
}
GCFLOBDD GCFLOBDD_createTrue(int level, GCFLOBDD_Grammar_Ref grammar) {
    auto grammar1 = (std::shared_ptr<Grammar> *)grammar;
    return cast(std::move(G_CFL_OBDD::MkTrue(level, *grammar1)));
}
GCFLOBDD GCFLOBDD_createFalse(int level, GCFLOBDD_Grammar_Ref grammar) {
    auto grammar1 = (std::shared_ptr<Grammar> *)grammar;
    return cast(std::move(G_CFL_OBDD::MkFalse(level, *grammar1)));
}

void GCFLOBDD_delete(GCFLOBDD gcflobdd) {
    auto a1 = retrieve(gcflobdd);
    // a1 will go out of scope and call destructor
}

// return the root address of GCFLOBDD; can be used in hash
GCFLOBDD_C_API void *GCFLOBDD_getRoot(const GCFLOBDD_Ref gcflobdd) {
    auto a1 = retrieve(gcflobdd);
    void *ans = a1.root.get_ptr();
    forget(std::move(a1));
    return ans;
}

GCFLOBDD GCFLOBDD_copy(const GCFLOBDD_Ref gcflobdd) {
    auto a1 = retrieve(gcflobdd);
    auto ans = cast(G_CFL_OBDD::G_CFLOBDD(a1));
    forget(std::move(a1));
    return ans;
}



GCFLOBDD GCFLOBDD_and(const GCFLOBDD_Ref a, const GCFLOBDD_Ref b) {
    auto a1 = retrieve(a);
    auto b1 = retrieve(b);
    auto ans = cast(G_CFL_OBDD::MkAnd(a1, b1));
    forget(std::move(a1));
    forget(std::move(b1));
    return ans;
}

GCFLOBDD GCFLOBDD_or(const GCFLOBDD_Ref a, const GCFLOBDD_Ref b) {
    auto a1 = retrieve(a);
    auto b1 = retrieve(b);
    auto ans = cast(G_CFL_OBDD::MkOr(a1, b1));
    forget(std::move(a1));
    forget(std::move(b1));
    return ans;
}

GCFLOBDD GCFLOBDD_minus(const GCFLOBDD_Ref a, const GCFLOBDD_Ref b) {
    auto a1 = retrieve(a);
    auto b1 = retrieve(b);
    auto ans = cast(G_CFL_OBDD::MkMinus(a1, b1));
    forget(std::move(a1));
    forget(std::move(b1));
    return ans;
}

GCFLOBDD GCFLOBDD_not(const GCFLOBDD_Ref a) {
    auto a1 = retrieve(a);
    auto ans = cast(G_CFL_OBDD::MkNot(a1));
    forget(std::move(a1));
    return ans;
}

GCFLOBDD GCFLOBDD_xor(const GCFLOBDD_Ref a, const GCFLOBDD_Ref b) {
    auto a1 = retrieve(a);
    auto b1 = retrieve(b);
    auto ans = cast(G_CFL_OBDD::MkExclusiveOr(a1, b1));
    forget(std::move(a1));
    forget(std::move(b1));
    return ans;
}

GCFLOBDD GCFLOBDD_implies(const GCFLOBDD_Ref a, const GCFLOBDD_Ref b) {
    auto a1 = retrieve(a);
    auto b1 = retrieve(b);
    auto ans = cast(G_CFL_OBDD::MkImplies(a1, b1));
    forget(std::move(a1));
    forget(std::move(b1));
    return ans;
}

GCFLOBDD GCFLOBDD_exists(const GCFLOBDD_Ref a, uint32_t i) {
    auto a1 = retrieve(a);
    auto ans = cast(G_CFL_OBDD::MkExists(a1, i));
    forget(std::move(a1));
    return ans;
}

GCFLOBDD GCFLOBDD_forall(const GCFLOBDD_Ref a, uint32_t i) {
    auto a1 = retrieve(a);
    auto ans = cast(G_CFL_OBDD::MkForall(a1, i));
    forget(std::move(a1));
    return ans;
}
GCFLOBDD_C_API uint32_t GCFLOBDD_getLevel(const GCFLOBDD_Ref gcflobdd) {
    auto a1 = retrieve(gcflobdd);
    auto ans = (*a1.root).level;
    forget(std::move(a1));
    return ans;
}
#ifdef PATH_COUNTING_ENABLED
GCFLOBDD_C_API uint32_t GCFLOBDD_numSatisfyingAssignments(const GCFLOBDD_Ref gcflobdd) {
    auto a1 = retrieve(gcflobdd);
    auto ans = a1.NumSatisfyingAssignments();
    forget(std::move(a1));
    return ans;
}
#endif

GCFLOBDD_C_API ssize_t GCFLOBDD_getOneSatisfyingAssignment(const GCFLOBDD_Ref gcflobdd, bool *assignment_buffer, size_t assignment_buffer_size) {
    auto a1 = retrieve(gcflobdd);
    uint32_t num_vars = a1.root->grammar->root->numVars;
    SH_OBDD::Assignment *assignment;
    bool ans = a1.FindOneSatisfyingAssignment(assignment);
    forget(std::move(a1));
    if (!ans) {
        return 0;
    }
    // Only copies min(assignment_buffer_size, num_vars) bytes
    size_t copy_size = assignment_buffer_size < num_vars ? assignment_buffer_size : num_vars;
    memcpy(assignment_buffer, assignment->get_data(), copy_size);
    delete assignment;
    return copy_size;
}

bool GCFLOBDD_eq(const GCFLOBDD_Ref a, const GCFLOBDD_Ref b) {
    auto a1 = retrieve(a);
    auto b1 = retrieve(b);
    auto ans = a1 == b1;
    forget(std::move(a1));
    forget(std::move(b1));
    return ans;
}

uint32_t GCFLOBDD_hash(const GCFLOBDD_Ref gcflobdd) {
    auto a1 = retrieve(gcflobdd);
    uint32_t ans = a1.Hash(1 << 32);
    forget(std::move(a1));
    return ans;
}

uint32_t GCFLOBDD_countNodes(const GCFLOBDD_Ref gcflobdd) {
    auto a1 = retrieve(gcflobdd);
    uint32_t nodes = 0, edges = 0;
    a1.CountNodesAndEdges(nodes, edges);
    forget(std::move(a1));
    return nodes;
}

void GCFLOBDD_print(const GCFLOBDD_Ref a) {
    auto a1 = retrieve(a);
    G_CFL_OBDD::PrintCFLOBDD(a1);
    forget(std::move(a1));
}
}