package org.trishullab.gcflobdd;

import java.lang.foreign.*;
import java.lang.invoke.MethodHandle;
// import java.util.Arrays;
import java.util.Arrays;

public class GCFLOBDD implements Cloneable {
    private static final Linker linker;
    private static final Arena arena = Arena.ofShared();
    private static final MethodHandle moduleInitHandle, moduleDisposeHandle, createGrammarHandle, grammarDeleteHandle,
            createVarHandle, createTrueHandle, createFalseHandle, deleteHandle, copyHandle, getRootHandle, andHandle,
            andToHandle, orHandle, orToHandle, minusHandle, notHandle, xorHandle, xorToHandle, impliesHandle,
            existsHandle, forallHandle/* , isValidHandle */, eqHandle, printHandle, getLevelHandle, hashHandle,
            countNodesHandle/*, numSatisfyingAssignmentsHandle */, getOneSatisfyingAssignmentHandle;
    
    public static class Grammar {
        private long grammar_addr;
        public Grammar(long addr) {
            this.grammar_addr = addr;
        }
        public void delete() {
            try {
                grammarDeleteHandle.invokeWithArguments(grammar_addr);
            } catch (Throwable e) {
                throw new RuntimeException(e);
            }
        }
    }
    private long bdd_addr;

    static {
        System.loadLibrary("gcflobdd");
        linker = Linker.nativeLinker();
        // Use system default library lookup which respects java.library.path and
        // system-specific paths
        SymbolLookup dylibs = SymbolLookup.loaderLookup();
        moduleInitHandle = linker.downcallHandle(dylibs.find("GCFLOBDD_module_init").orElseThrow(),
                FunctionDescriptor.ofVoid());
        moduleDisposeHandle = linker.downcallHandle(dylibs.find("GCFLOBDD_module_dispose").orElseThrow(),
                FunctionDescriptor.ofVoid());
        try {
            moduleInitHandle.invokeWithArguments();
        } catch (Throwable e) {
            throw new ExceptionInInitializerError(e);
        }
        createGrammarHandle = linker.downcallHandle(dylibs.find("CFLOBDD_createGrammar").orElseThrow(),
                FunctionDescriptor.of(ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG));
        grammarDeleteHandle = linker.downcallHandle(dylibs.find("CFLOBDD_Grammar_delete").orElseThrow(),
                FunctionDescriptor.of(ValueLayout.JAVA_LONG));
        createVarHandle = linker.downcallHandle(dylibs.find("CFLOBDD_createVar").orElseThrow(),
                FunctionDescriptor.of(ValueLayout.JAVA_LONG, ValueLayout.JAVA_INT, ValueLayout.JAVA_INT, ValueLayout.JAVA_LONG));
        createTrueHandle = linker.downcallHandle(dylibs.find("CFLOBDD_createTrue").orElseThrow(),
                FunctionDescriptor.of(ValueLayout.JAVA_LONG, ValueLayout.JAVA_INT, ValueLayout.JAVA_LONG));
        createFalseHandle = linker.downcallHandle(dylibs.find("CFLOBDD_createFalse").orElseThrow(),
                FunctionDescriptor.of(ValueLayout.JAVA_LONG, ValueLayout.JAVA_INT, ValueLayout.JAVA_LONG));
        deleteHandle = linker.downcallHandle(dylibs.find("CFLOBDD_delete").orElseThrow(),
                FunctionDescriptor.ofVoid(ValueLayout.JAVA_LONG));
        copyHandle = linker.downcallHandle(dylibs.find("CFLOBDD_copy").orElseThrow(),
                FunctionDescriptor.of(ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG));
        getRootHandle = linker.downcallHandle(dylibs.find("CFLOBDD_getRoot").orElseThrow(),
                FunctionDescriptor.of(ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG));
        andHandle = linker.downcallHandle(dylibs.find("CFLOBDD_and").orElseThrow(),
                FunctionDescriptor.of(ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG));
        andToHandle = linker.downcallHandle(dylibs.find("CFLOBDD_and_to").orElseThrow(),
                FunctionDescriptor.of(ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG));
        orHandle = linker.downcallHandle(dylibs.find("CFLOBDD_or").orElseThrow(),
                FunctionDescriptor.of(ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG));
        orToHandle = linker.downcallHandle(dylibs.find("CFLOBDD_or_to").orElseThrow(),
                FunctionDescriptor.of(ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG));
        minusHandle = linker.downcallHandle(dylibs.find("CFLOBDD_minus").orElseThrow(),
                FunctionDescriptor.of(ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG));
        notHandle = linker.downcallHandle(dylibs.find("CFLOBDD_not").orElseThrow(),
                FunctionDescriptor.of(ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG));
        xorHandle = linker.downcallHandle(dylibs.find("CFLOBDD_xor").orElseThrow(),
                FunctionDescriptor.of(ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG));
        xorToHandle = linker.downcallHandle(dylibs.find("CFLOBDD_xor_to").orElseThrow(),
                FunctionDescriptor.of(ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG));
        impliesHandle = linker.downcallHandle(dylibs.find("CFLOBDD_implies").orElseThrow(),
                FunctionDescriptor.of(ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG));
        existsHandle = linker.downcallHandle(dylibs.find("CFLOBDD_exists").orElseThrow(),
                FunctionDescriptor.of(ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG, ValueLayout.JAVA_INT));
        forallHandle = linker.downcallHandle(dylibs.find("CFLOBDD_forall").orElseThrow(),
                FunctionDescriptor.of(ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG, ValueLayout.JAVA_INT));
        // isValidHandle = linker.downcallHandle(dylibs.find("CFLOBDD_isValid").orElseThrow(),
        //         FunctionDescriptor.of(ValueLayout.JAVA_BOOLEAN, ValueLayout.JAVA_LONG));
        eqHandle = linker.downcallHandle(dylibs.find("CFLOBDD_eq").orElseThrow(),
                FunctionDescriptor.of(ValueLayout.JAVA_BOOLEAN, ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG));
        printHandle = linker.downcallHandle(dylibs.find("CFLOBDD_print").orElseThrow(),
                FunctionDescriptor.ofVoid(ValueLayout.JAVA_LONG));
        getLevelHandle = linker.downcallHandle(dylibs.find("CFLOBDD_getLevel").orElseThrow(),
                FunctionDescriptor.of(ValueLayout.JAVA_INT, ValueLayout.JAVA_LONG));
        hashHandle = linker.downcallHandle(dylibs.find("CFLOBDD_hash").orElseThrow(),
                FunctionDescriptor.of(ValueLayout.JAVA_INT, ValueLayout.JAVA_LONG));
        countNodesHandle = linker.downcallHandle(dylibs.find("CFLOBDD_countNodes").orElseThrow(),
                FunctionDescriptor.of(ValueLayout.JAVA_INT, ValueLayout.JAVA_LONG));
        // numSatisfyingAssignmentsHandle =
        // linker.downcallHandle(dylibs.find("CFLOBDD_numSatisfyingAssignments").orElseThrow(),
        // FunctionDescriptor.of(ValueLayout.JAVA_INT, ValueLayout.JAVA_LONG));
        getOneSatisfyingAssignmentHandle = linker.downcallHandle(
                dylibs.find("CFLOBDD_getOneSatisfyingAssignment").orElseThrow(),
                FunctionDescriptor.of(ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG, ValueLayout.JAVA_LONG,
                        ValueLayout.JAVA_LONG));
    }

    private GCFLOBDD(long bdd_addr) {
        this.bdd_addr = bdd_addr;
    }

    @Override
    public GCFLOBDD clone() {
        try {
            long ans = (long) copyHandle.invokeWithArguments(bdd_addr);
            return new GCFLOBDD(ans);
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
    }

    public long getRoot() {
        try {
            return (long) getRootHandle.invokeWithArguments(bdd_addr);
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
    }

    public static void dispose_module() {
        try {
            moduleDisposeHandle.invokeWithArguments();
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
        arena.close();
    }
    public static Grammar createGrammar(String[] productions, String start_symbol) {
        try {
            long ans = (long) createGrammarHandle.invokeWithArguments(productions, productions.length, start_symbol);
            return new Grammar(ans);
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
    }

    public static GCFLOBDD createVar(int varId, int level, Grammar grammar) {
        try {
            long ans = (long) createVarHandle.invokeWithArguments(varId, level, grammar.grammar_addr);
            return new GCFLOBDD(ans);
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
    }

    public void deref() {
        try {
            deleteHandle.invokeWithArguments(bdd_addr);
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
    }

    public static GCFLOBDD createTrue(int level, Grammar grammar) {
        try {
            long ans = (long) createTrueHandle.invokeWithArguments(level, grammar.grammar_addr);
            return new GCFLOBDD(ans);
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
    }

    public static GCFLOBDD createFalse(int level, Grammar grammar) {
        try {
            long ans = (long) createFalseHandle.invokeWithArguments(level, grammar.grammar_addr);
            return new GCFLOBDD(ans);
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
    }

    public static GCFLOBDD not(GCFLOBDD bdd) {
        try {
            long ans = (long) notHandle.invokeWithArguments(bdd.bdd_addr);
            return new GCFLOBDD(ans);
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
    }

    public static GCFLOBDD or(GCFLOBDD bdd1, GCFLOBDD bdd2) {
        try {
            long ans = (long) orHandle.invokeWithArguments(bdd1.bdd_addr, bdd2.bdd_addr);
            return new GCFLOBDD(ans);
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
    }

    public void orWith(GCFLOBDD other) {
        try {
            long ans = (long) orToHandle.invokeWithArguments(bdd_addr, other.bdd_addr);
            this.bdd_addr = ans;
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
    }

    public static GCFLOBDD and(GCFLOBDD bdd1, GCFLOBDD bdd2) {
        try {
            long ans = (long) andHandle.invokeWithArguments(bdd1.bdd_addr, bdd2.bdd_addr);
            return new GCFLOBDD(ans);
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
    }

    public void andWith(GCFLOBDD other) {
        try {
            long ans = (long) andToHandle.invokeWithArguments(bdd_addr, other.bdd_addr);
            this.bdd_addr = ans;
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
    }

    public static GCFLOBDD diff(GCFLOBDD bdd1, GCFLOBDD bdd2) {
        try {
            long ans = (long) minusHandle.invokeWithArguments(bdd1.bdd_addr, bdd2.bdd_addr);
            return new GCFLOBDD(ans);
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
    }

    public static GCFLOBDD xor(GCFLOBDD bdd1, GCFLOBDD bdd2) {
        try {
            long ans = (long) xorHandle.invokeWithArguments(bdd1.bdd_addr, bdd2.bdd_addr);
            return new GCFLOBDD(ans);
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
    }

    public void xorWith(GCFLOBDD other) {
        try {
            long ans = (long) xorToHandle.invokeWithArguments(bdd_addr, other.bdd_addr);
            this.bdd_addr = ans;
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
    }

    public static GCFLOBDD imp(GCFLOBDD bdd1, GCFLOBDD bdd2) {
        try {
            long ans = (long) impliesHandle.invokeWithArguments(bdd1.bdd_addr, bdd2.bdd_addr);
            return new GCFLOBDD(ans);
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
    }

    public static GCFLOBDD exists(GCFLOBDD bdd, int varId) {
        try {
            long ans = (long) existsHandle.invokeWithArguments(bdd.bdd_addr, varId);
            return new GCFLOBDD(ans);
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
    }

    public static GCFLOBDD forall(GCFLOBDD bdd, int varId) {
        try {
            long ans = (long) forallHandle.invokeWithArguments(bdd.bdd_addr, varId);
            return new GCFLOBDD(ans);
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
    }

    // public boolean isValid() {
    //     try {
    //         return (boolean) isValidHandle.invokeWithArguments(bdd_addr);
    //     } catch (Throwable e) {
    //         throw new RuntimeException(e);
    //     }
    // }

    public void print() {
        try {
            printHandle.invokeWithArguments(bdd_addr);
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
    }

    public int satCount() {
        System.err.println("satCount is not enabled due to disabled flag of PATH_COUNTING_ENABLED");
        return -1;
        // try {
        // return (int) numSatisfyingAssignmentsHandle.invokeWithArguments(bdd_addr);
        // } catch (Throwable e) {
        // throw new RuntimeException(e);
        // }
    }

    public int nodeCount() {
        try {
            return (int) countNodesHandle.invokeWithArguments(bdd_addr);
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
    }

    public int getLevel() {
        try {
            return (int) getLevelHandle.invokeWithArguments(bdd_addr);
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
    }

    public int[] oneSat() {
        // always return full vector,
        // collected by system gc.
        // if unsat, return full -1;
        int ans_length = 1 << getLevel();
        int[] ans = new int[ans_length];
        try (Arena tempArena = Arena.ofConfined()) {
            MemorySegment c_ans = tempArena.allocate(ans_length);
            long ret;
            try {
                ret = (long) getOneSatisfyingAssignmentHandle.invokeWithArguments(bdd_addr, c_ans.address(),
                        ans_length);
            } catch (Throwable e) {
                throw new RuntimeException(e);
            }
            if (ret == 0) {
                Arrays.fill(ans, -1);
            } else if (ret < 0) {
                throw new RuntimeException("getOneSatisfyingAssignment failed");
            } else {
                for (int i = 0; i < ans_length; i++) {
                    ans[i] = c_ans.get(ValueLayout.JAVA_BOOLEAN, i) ? 1 : 0;
                }
            }
        }
        return ans;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj == null) {
            return false;
        }
        if (obj == this) {
            return true;
        }
        if (obj instanceof GCFLOBDD) {
            GCFLOBDD other = (GCFLOBDD) obj;
            try {
                return (boolean) eqHandle.invokeWithArguments(bdd_addr, other.bdd_addr);
            } catch (Throwable e) {
                throw new RuntimeException(e);
            }
        }
        return false;
    }

    @Override
    public int hashCode() {
        try {
            return (int) hashHandle.invokeWithArguments(bdd_addr);
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
    }

    public static void main(String[] args) {
        Grammar grammar = createGrammar(new String[]{"S 1 -> S 0 S 0", "S 0 -> a"}, "S1");
        GCFLOBDD zero = createFalse(-1, grammar);
        GCFLOBDD a = createVar(0, -1, grammar);
        GCFLOBDD b = createVar(1, -1, grammar);
        System.out.printf("z @ %x: %x\n", zero.bdd_addr, zero.hashCode());
        System.out.printf("a @ %x: %x\n", a.bdd_addr, a.hashCode());
        System.out.printf("b @ %x: %x\n", b.bdd_addr, b.hashCode());
        // System.out.println(Arrays.toString(a.oneSat()));
        zero.deref();
        a.deref();
        b.deref();
        dispose_module();
    }
}