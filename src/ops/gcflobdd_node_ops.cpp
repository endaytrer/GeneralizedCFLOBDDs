#include "gcflobdd_node_ops.h"
#include <unordered_map>
#include "../gcflobdd/gcflobdd_node.h"

namespace G_CFL_OBDD {

G_CFLOBDDNodeHandle MkNoDistinction(unsigned int level, std::shared_ptr<GrammarNode>& grammar) {
    NoDistinctionCacheKey key(level, grammar);
    if (G_CFLOBDDNodeHandle::NoDistinctionNode.find(key) != G_CFLOBDDNodeHandle::NoDistinctionNode.end()) {
        return G_CFLOBDDNodeHandle::NoDistinctionNode[key];
    }
    if (grammar->level == 0) {
        GrammarNonTerminalNode* gNode = dynamic_cast<GrammarNonTerminalNode*>(grammar.get());
        assert(gNode != nullptr);
        assert(gNode->children.size() == 1);
        assert(gNode->children[0]->isTerminal());
        assert(gNode->children[0]->level == 0);
        G_CFLOBDDNodeHandle::NoDistinctionNode[key] = G_CFLOBDDNodeHandle::G_CFLOBDDDontCareNodeHandle;
        return G_CFLOBDDNodeHandle::G_CFLOBDDDontCareNodeHandle;
    }

    G_CFLOBDDInternalNode *node = new G_CFLOBDDInternalNode(level);
    node->grammar = grammar;
    // number of children of grammar node is equal to number of layers of G_CFLOBDD node
    unsigned int numChildren = 0;
    GrammarNonTerminalNode* gNode = dynamic_cast<GrammarNonTerminalNode*>(grammar.get());
    assert(gNode != nullptr);
    numChildren = gNode->children.size();
    assert(numChildren > 0);

    node->numLayers = numChildren;
    auto connections = new ConnectionList[numChildren];
    G_CFLOBDDReturnMapHandle m0; m0.AddToEnd(0); m0.Canonicalize();
    for (unsigned int i = 0; i < numChildren; i++) {
        auto& childGrammar = gNode->children[i];
        auto layer_i = MkNoDistinction(level - 1, childGrammar);
        auto conn = ConnectionT<G_CFLOBDDReturnMapHandle>(layer_i, m0);
        connections[i].AddConnection(conn);
        connections[i].Canonicalize();
    }

    node->connections = connections;
    node->numExits = 1;
    node->grammar = grammar;

    auto handle = G_CFLOBDDNodeHandle(node);
    G_CFLOBDDNodeHandle::NoDistinctionNode[key] = handle;
    return handle;
}

long int findChildIndexForVariable(unsigned int i, std::shared_ptr<GrammarNode>& grammar) {
    GrammarNonTerminalNode* gNode = dynamic_cast<GrammarNonTerminalNode*>(grammar.get());
    assert(gNode != nullptr);
    for (unsigned int index = 0; index < gNode->children.size(); index++) {
        std::shared_ptr<GrammarNode> child = gNode->children[index];
        // Variable ordering corrresponds to number of variables in the subtree rooted at child
        if (i  < child->numVars) {
            return index;
        }
        else {
            i = i - child->numVars;
        }
    }
    // Should not reach here
    assert(false);
    return -1;
}

G_CFLOBDDNodeHandle MkDistinction(unsigned int level, unsigned int i, std::shared_ptr<GrammarNode>& grammar) {
    if (grammar->level == 0) {
        assert(i == 0);
        return G_CFLOBDDNodeHandle::G_CFLOBDDForkNodeHandle;
    }

    G_CFLOBDDInternalNode *node = new G_CFLOBDDInternalNode(level);
    node->grammar = grammar;
    // number of children of grammar node is equal to number of layers of G_CFLOBDD node
    unsigned int numChildren = 0;
    GrammarNonTerminalNode* gNode = dynamic_cast<GrammarNonTerminalNode*>(grammar.get());
    assert(gNode != nullptr);
    numChildren = gNode->children.size();
    assert(numChildren > 0);

    node->numLayers = numChildren;
    auto connections = new ConnectionList[numChildren];
    auto index = findChildIndexForVariable(i, grammar);
    assert(index >= 0 && index < numChildren);

    G_CFLOBDDReturnMapHandle m0; m0.AddToEnd(0); m0.Canonicalize();
    unsigned int numVarsBeforeIndex = 0;
    for (unsigned int layer = 0; layer < index; layer++) {
        auto& childGrammar = gNode->children[layer];
        auto layer_i = MkNoDistinction(level - 1, childGrammar);
        auto conn = ConnectionT<G_CFLOBDDReturnMapHandle>(layer_i, m0);
        connections[layer].AddConnection(conn);
        connections[layer].Canonicalize();
        numVarsBeforeIndex += childGrammar->numVars;
    }
    auto childGrammarAtIndex = gNode->children[index];
    auto layer_index = MkDistinction(level - 1, i - numVarsBeforeIndex, childGrammarAtIndex);
    G_CFLOBDDReturnMapHandle m01; m01.AddToEnd(0); m01.AddToEnd(1); m01.Canonicalize();
    auto conn0 = ConnectionT<G_CFLOBDDReturnMapHandle>(layer_index, m01);
    connections[index].AddConnection(conn0);
    connections[index].Canonicalize();

    G_CFLOBDDReturnMapHandle m1; m1.AddToEnd(1); m1.Canonicalize();
    for (unsigned int layer = index + 1; layer < numChildren; layer++) {
        auto& childGrammar = gNode->children[layer];
        auto layer_i = MkNoDistinction(level - 1, childGrammar);
        auto conn_0 = ConnectionT<G_CFLOBDDReturnMapHandle>(layer_i, m0);
        auto conn_1 = ConnectionT<G_CFLOBDDReturnMapHandle>(layer_i, m1);
        connections[layer].AddConnection(conn_0);
        connections[layer].AddConnection(conn_1);
        connections[layer].Canonicalize();
    }

    node->connections = connections;
    node->numExits = 2;
    node->grammar = grammar;

    return G_CFLOBDDNodeHandle(node);
}

G_CFLOBDDNodeHandle MkParity(unsigned int level, std::shared_ptr<GrammarNode>& grammar) {
    if (grammar->level == 0) {
        return G_CFLOBDDNodeHandle::G_CFLOBDDForkNodeHandle;
    }

    G_CFLOBDDInternalNode *node = new G_CFLOBDDInternalNode(level);
    node->grammar = grammar;
    // number of children of grammar node is equal to number of layers of G_CFLOBDD node
    unsigned int numChildren = 0;
    GrammarNonTerminalNode* gNode = dynamic_cast<GrammarNonTerminalNode*>(grammar.get());
    assert(gNode != nullptr);
    numChildren = gNode->children.size();
    assert(numChildren > 0);

    node->numLayers = numChildren;
    auto connections = new ConnectionList[numChildren];

    G_CFLOBDDReturnMapHandle m01; m01.AddToEnd(0); m01.AddToEnd(1); m01.Canonicalize();
    G_CFLOBDDReturnMapHandle m10; m10.AddToEnd(1); m10.AddToEnd(0); m10.Canonicalize();
    unsigned int numVars = 0;
    for (unsigned int layer = 0; layer < numChildren; layer++) {
        auto& childGrammar = gNode->children[layer];
        auto layer_i = MkParity(level - 1, childGrammar);
        auto conn0 = ConnectionT<G_CFLOBDDReturnMapHandle>(layer_i, m01);
        auto conn1 = ConnectionT<G_CFLOBDDReturnMapHandle>(layer_i, m10);
        connections[layer].AddConnection(conn0);
        connections[layer].AddConnection(conn1);
        connections[layer].Canonicalize();
        numVars += childGrammar->numVars;
    }

    node->connections = connections;
    node->numExits = 2;
    node->grammar = grammar;

    return G_CFLOBDDNodeHandle(node); 
}

G_CFLOBDDNodeHandle Restrict(G_CFLOBDDNodeHandle g, unsigned int i, bool val, G_CFLOBDDReturnMapHandle &MapHandle) {
    // TODO: Not implemented yet
    abort();
}

G_CFLOBDDNodeHandle Restrict(G_CFLOBDDInternalNode *g, unsigned int i, bool val, G_CFLOBDDReturnMapHandle &MapHandle) {
    // TODO: Not implemented yet
    abort();
}

}