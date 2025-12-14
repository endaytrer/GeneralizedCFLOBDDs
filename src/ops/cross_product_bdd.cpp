#include "cross_product_bdd.h"
#include "cross_product_cache_utils.h"

namespace G_CFL_OBDD {

static Hashtable<PairProductKey<BDDNodeHandle>, PairProductMemo<BDDNodeHandle>> *bddPairProductCache = NULL;

BDDNodeHandle PairProduct(BDDNodeHandle n1,
                          BDDNodeHandle n2,
                          PairProductMapHandle &pairProductMapHandle,
                          std::unordered_map<PairProductKey<BDDNodeHandle>, BDDNodeHandle, PairProductKey<BDDNodeHandle>::PairProductKey_Hash, PairProductKey<BDDNodeHandle>::PairProductKey_Equal> &pairProductMap
                        )
{
    PairProductMemo<BDDNodeHandle> cachedPairProductMemo;
    auto key1 = PairProductKey<BDDNodeHandle>(n1, n2);
    auto key2 = PairProductKey<BDDNodeHandle>(n2, n1);
    bool isCached = bddPairProductCache->Fetch(key1, cachedPairProductMemo);
    if (isCached) {
        // pairProductMapHandle.Extend(cachedPairProductMemo.pairProductMapHandle);
        pairProductMapHandle = cachedPairProductMemo.pairProductMapHandle;
        return cachedPairProductMemo.nodeHandle;
    }
    else if (bddPairProductCache->Fetch(key2, cachedPairProductMemo)) {
        // pairProductMapHandle.Extend(cachedPairProductMemo.pairProductMapHandle.Flip());
        pairProductMapHandle = cachedPairProductMemo.pairProductMapHandle.Flip();
        return cachedPairProductMemo.nodeHandle;
    }
    else {
        BDDNodeHandle answer;
        if (n1.handleContents->NodeKind() == BDD_INTERNAL && n2.handleContents->NodeKind() == BDD_INTERNAL) {
            answer  = PairProductBDDInternal(static_cast<BDDInternalNode*>(n1.handleContents),
                                             static_cast<BDDInternalNode*>(n2.handleContents),
                                             pairProductMapHandle,
                                             pairProductMap);
        }
        else if (n1.handleContents->NodeKind() == BDD_LEAF && n2.handleContents->NodeKind() == BDD_LEAF) {
            // Both are leaf nodes
            BDDLeafNode* leaf1 = static_cast<BDDLeafNode*>(n1.handleContents);
            BDDLeafNode* leaf2 = static_cast<BDDLeafNode*>(n2.handleContents);
            auto pair = PairProductKey<BDDNodeHandle>(n1, n2);
            if (pairProductMap.find(pair) == pairProductMap.end()) {
                auto index = pairProductMap.size();
                answer = BDDNodeHandle(new BDDLeafNode(
                    index, // Value
                    -1, // varID
                    0 // numVars
                ));
                pairProductMapHandle.AddToEnd(intpair(leaf1->value, leaf2->value));
                pairProductMapHandle.Canonicalize();
                pairProductMap.insert(std::make_pair(pair, answer));
            }
            else
                answer = pairProductMap[pair];
            
        }
        else if (n1.handleContents->NodeKind() == BDD_LEAF) {
            BDDInternalNode* internal2 = static_cast<BDDInternalNode*>(n2.handleContents);
            BDDInternalNode* resultNode = new BDDInternalNode(internal2->NumVars(), internal2->VarID());
            PairProductMapHandle thenMapHandle;
            PairProductMapHandle elseMapHandle;
            resultNode->thenBranch = PairProduct(n1,
                                                internal2->thenBranch,
                                                thenMapHandle,
                                                pairProductMap);
            resultNode->elseBranch = PairProduct(n1,
                                                internal2->elseBranch,
                                                elseMapHandle,
                                                pairProductMap);
            pairProductMapHandle.Extend(thenMapHandle);
            pairProductMapHandle.Extend(elseMapHandle);
            pairProductMapHandle.Canonicalize();
            if (resultNode->thenBranch == resultNode->elseBranch) {
                // Reduction rule: skip nodes whose then and else branches are the same
                answer = resultNode->thenBranch;
            }
            else
                answer = BDDNodeHandle(resultNode);
        }
        else { // n2 is leaf
            BDDInternalNode* internal1 = static_cast<BDDInternalNode*>(n1.handleContents);
            BDDInternalNode* resultNode = new BDDInternalNode(internal1->NumVars(), internal1->VarID());
            PairProductMapHandle thenMapHandle;
            PairProductMapHandle elseMapHandle;
            resultNode->thenBranch = PairProduct(internal1->thenBranch,
                                                n2,
                                                thenMapHandle,  
                                                pairProductMap);
            resultNode->elseBranch = PairProduct(internal1->elseBranch,
                                                n2,
                                                elseMapHandle,
                                                pairProductMap);
            pairProductMapHandle.Extend(thenMapHandle);
            pairProductMapHandle.Extend(elseMapHandle);
            pairProductMapHandle.Canonicalize();
            if (resultNode->thenBranch == resultNode->elseBranch) {
                // Reduction rule: skip nodes whose then and else branches are the same
                answer = resultNode->thenBranch;
            }
            else
                answer = BDDNodeHandle(resultNode);
        }
        PairProductMemo<BDDNodeHandle> memo;
        memo.nodeHandle = answer;
        memo.pairProductMapHandle = pairProductMapHandle;
        bddPairProductCache->Insert(key1, memo);
        return answer;
    }
}

BDDNodeHandle PairProductBDDInternal(BDDInternalNode *n1,
                          BDDInternalNode *n2,
                          PairProductMapHandle &pairProductMapHandle,
                          std::unordered_map<PairProductKey<BDDNodeHandle>, BDDNodeHandle, PairProductKey<BDDNodeHandle>::PairProductKey_Hash, PairProductKey<BDDNodeHandle>::PairProductKey_Equal> &pairProductMap
                         )
{
    if (n1->VarID() == n2->VarID()) {
        PairProductMapHandle thenMapHandle;
        PairProductMapHandle elseMapHandle;
        BDDNodeHandle thenBranch = PairProduct(n1->thenBranch,
                                              n2->thenBranch,
                                              thenMapHandle,
                                              pairProductMap);
        BDDNodeHandle elseBranch = PairProduct(n1->elseBranch,
                                              n2->elseBranch,
                                              elseMapHandle,
                                              pairProductMap);
        pairProductMapHandle.Extend(thenMapHandle);
        pairProductMapHandle.Extend(elseMapHandle);
        pairProductMapHandle.Canonicalize();
        if (thenBranch == elseBranch) {
            // Reduction rule: skip nodes whose then and else branches are the same
            return thenBranch;
        }
        BDDInternalNode* resultNode = new BDDInternalNode(n1->NumVars(), n1->VarID());
        resultNode->thenBranch = thenBranch;
        resultNode->elseBranch = elseBranch;
        BDDNodeHandle resultHandle(resultNode);
        return resultHandle;
    }
    else if (n1->VarID() < n2->VarID()) {
        PairProductMapHandle thenMapHandle;
        PairProductMapHandle elseMapHandle;
        BDDNodeHandle thenBranch = PairProduct(n1->thenBranch,
                                              BDDNodeHandle(n2),
                                              thenMapHandle,
                                              pairProductMap);
        BDDNodeHandle elseBranch = PairProduct(n1->elseBranch,
                                              BDDNodeHandle(n2),
                                              elseMapHandle,
                                              pairProductMap);
        pairProductMapHandle.Extend(thenMapHandle);
        pairProductMapHandle.Extend(elseMapHandle);
        pairProductMapHandle.Canonicalize();
        if (thenBranch == elseBranch) {
            // Reduction rule: skip nodes whose then and else branches are the same
            return thenBranch;
        }
        BDDInternalNode* resultNode = new BDDInternalNode(n1->NumVars(), n1->VarID());
        resultNode->thenBranch = thenBranch;
        resultNode->elseBranch = elseBranch;
        BDDNodeHandle resultHandle(resultNode);
        return resultHandle;
    }
    else { // n1->VarID > n2->VarID
        PairProductMapHandle thenMapHandle;
        PairProductMapHandle elseMapHandle;
        BDDNodeHandle thenBranch = PairProduct(BDDNodeHandle(n1),
                                              n2->thenBranch,
                                              thenMapHandle,
                                              pairProductMap);
        BDDNodeHandle elseBranch = PairProduct(BDDNodeHandle(n1),
                                              n2->elseBranch,
                                              elseMapHandle,
                                              pairProductMap);
        pairProductMapHandle.Extend(thenMapHandle);
        pairProductMapHandle.Extend(elseMapHandle);
        pairProductMapHandle.Canonicalize();
        if (thenBranch == elseBranch) {
            // Reduction rule: skip nodes whose then and else branches are the same
            return thenBranch;
        }
        BDDInternalNode* resultNode = new BDDInternalNode(n2->NumVars(), n2->VarID());
        resultNode->thenBranch = thenBranch;
        resultNode->elseBranch = elseBranch;
        BDDNodeHandle resultHandle(resultNode);
        return resultHandle;
    }
}

G_CFLOBDDNodeHandle PairProduct(G_CFLOBDDBDDNode* n1,
                                G_CFLOBDDBDDNode* n2,
                                PairProductMapHandle &pairProductMapHandle
                               )
{
    BDDNodeHandle n1Handle = *(n1->bddSection.entryPointHandle);
    BDDNodeHandle n2Handle = *(n2->bddSection.entryPointHandle);
    std::unordered_map<PairProductKey<BDDNodeHandle>, BDDNodeHandle, PairProductKey<BDDNodeHandle>::PairProductKey_Hash, PairProductKey<BDDNodeHandle>::PairProductKey_Equal> pairProductMap;
    auto result = PairProduct(n1Handle,
                              n2Handle,
                              pairProductMapHandle,
                              pairProductMap);

    G_CFLOBDDBDDNode* resultNode = new G_CFLOBDDBDDNode(n1->numVars);
    G_CFLOBDDReturnMapHandle mI;
    for (unsigned int i = 0; i < pairProductMap.size(); i++) {
        mI.AddToEnd(i);
    }
    mI.Canonicalize();
    // for (const auto& entry : pairProductMap) {
    //     BDDLeafNode* leafNode1 = static_cast<BDDLeafNode*>(entry.first.NodeHandle1().handleContents);
    //     BDDLeafNode* leafNode2 = static_cast<BDDLeafNode*>(entry.first.NodeHandle2().handleContents);
    //     pairProductMapHandle.AddToEnd(intpair(leafNode1->value,
    //                                           leafNode2->value));
    // }
    pairProductMapHandle.Canonicalize();
    resultNode->bddSection = Section(result, mI);
    G_CFLOBDDNodeHandle resultHandle(resultNode);
    return resultHandle;
}

void InitBDDPairProductCache()
{
    if (bddPairProductCache == NULL) {
        bddPairProductCache = new Hashtable<PairProductKey<BDDNodeHandle>, PairProductMemo<BDDNodeHandle>>(HASHSET_NUM_BUCKETS);
    }
}

void DisposeOfBDDPairProductCache()
{
    if (bddPairProductCache != NULL) {
        delete bddPairProductCache;
        bddPairProductCache = NULL;
    }
}
} // namespace G_CFL_OBDD