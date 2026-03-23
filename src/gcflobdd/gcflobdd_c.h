#ifndef GCFLOBDD_C_H
#define GCFLOBDD_C_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>
#if defined(_WIN32)
    #ifdef GCFLOBDD_C_EXPORTS
        #define GCFLOBDD_C_API __declspec(dllexport)
    #else
        #define GCFLOBDD_C_API __declspec(dllimport)
    #endif
#else
    #define GCFLOBDD_C_API __attribute__((visibility("default")))
#endif

// Opaque pointer to the C++ CFLOBDD object
typedef size_t GCFLOBDD;
typedef void *GCFLOBDD_Grammar;
// also use ref to indicate that the function will not destroy the input CFLOBDD
typedef size_t GCFLOBDD_Ref;
typedef void *GCFLOBDD_Grammar_Ref;


GCFLOBDD_C_API void GCFLOBDD_module_init();
GCFLOBDD_C_API void GCFLOBDD_module_dispose();

GCFLOBDD_C_API GCFLOBDD_Grammar GCFLOBDD_createGrammar(const char *productions[], size_t num_productions, const char *start_symbol);
GCFLOBDD_C_API void GCFLOBDD_Grammar_delete(GCFLOBDD_Grammar grammar);
// Another "level" is needed since it do not have a "engine" concept.
GCFLOBDD_C_API GCFLOBDD GCFLOBDD_createVar(uint32_t i, int level, GCFLOBDD_Grammar_Ref grammar);
GCFLOBDD_C_API GCFLOBDD GCFLOBDD_createTrue(int level, GCFLOBDD_Grammar_Ref grammar);
GCFLOBDD_C_API GCFLOBDD GCFLOBDD_createFalse(int level, GCFLOBDD_Grammar_Ref grammar);
// copy constructor
GCFLOBDD_C_API GCFLOBDD GCFLOBDD_copy(const GCFLOBDD_Ref cflobdd);
GCFLOBDD_C_API void GCFLOBDD_delete(GCFLOBDD cflobdd);
// Operators
GCFLOBDD_C_API GCFLOBDD GCFLOBDD_and(const GCFLOBDD_Ref a, const GCFLOBDD_Ref b);
GCFLOBDD_C_API GCFLOBDD GCFLOBDD_or(const GCFLOBDD_Ref a, const GCFLOBDD_Ref b);
GCFLOBDD_C_API GCFLOBDD GCFLOBDD_minus(const GCFLOBDD_Ref a, const GCFLOBDD_Ref b);
GCFLOBDD_C_API GCFLOBDD GCFLOBDD_not(const GCFLOBDD_Ref a);
GCFLOBDD_C_API GCFLOBDD GCFLOBDD_xor(const GCFLOBDD_Ref a, const GCFLOBDD_Ref b);
GCFLOBDD_C_API GCFLOBDD GCFLOBDD_implies(const GCFLOBDD_Ref a, const GCFLOBDD_Ref b);
GCFLOBDD_C_API GCFLOBDD GCFLOBDD_exists(const GCFLOBDD_Ref a, uint32_t i);
GCFLOBDD_C_API GCFLOBDD GCFLOBDD_forall(const GCFLOBDD_Ref a, uint32_t i);
// GCFLOBDD_C_API bool GCFLOBDD_isValid(const GCFLOBDD_Ref a);
GCFLOBDD_C_API uint32_t GCFLOBDD_hash(const GCFLOBDD_Ref cflobdd);
GCFLOBDD_C_API uint32_t GCFLOBDD_countNodes(const GCFLOBDD_Ref cflobdd);
GCFLOBDD_C_API uint32_t GCFLOBDD_getLevel(const GCFLOBDD_Ref cflobdd);
GCFLOBDD_C_API void *GCFLOBDD_getRoot(const GCFLOBDD_Ref cflobdd);
#ifdef PATH_COUNTING_ENABLED
GCFLOBDD_C_API uint32_t GCFLOBDD_numSatisfyingAssignments(const GCFLOBDD_Ref cflobdd);
#endif

/// Get one satisfying assignment of the CFLOBDD.
/// @param cflobdd The CFLOBDD.
/// @param assignment_buffer The buffer to store the satisfying assignment.
/// @param assignment_buffer_size The size (both in byte and in number of booleans) of the assignment_buffer.
/// @return 0 if no satisfying assignment is found. If there is a satisfiable assignment,
/// The size of buffer used to store the satisfying assignment if success, -1 if the assignment_buffer is too small.
GCFLOBDD_C_API ssize_t GCFLOBDD_getOneSatisfyingAssignment(const GCFLOBDD_Ref cflobdd, bool *assignment_buffer, size_t assignment_buffer_size);
GCFLOBDD_C_API bool GCFLOBDD_eq(const GCFLOBDD_Ref a, const GCFLOBDD_Ref b);
GCFLOBDD_C_API void GCFLOBDD_print(const GCFLOBDD_Ref a);
#ifdef __cplusplus
}
#endif

#endif // GCFLOBDD_C_H