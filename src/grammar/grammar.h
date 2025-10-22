#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>

class GrammarNode;
class GrammarNonTerminalNode;
class GrammarTerminalNode;

struct GrammarNodeHash {
    size_t operator()(const GrammarNode* node) const {
        // For example, use pointer address for hashing (fast)
        // Or implement a hash based on node content
        return reinterpret_cast<std::size_t>(node);
    }
};

struct GrammarNodeEqual {
    bool operator()(const GrammarNode* a,
                    const GrammarNode* b) const {
        return a == b; // uses your GrammarNode::operator==
    }
};

class Grammar {
public:
    Grammar();  // Constructor
    ~Grammar(); // Destructor
    std::shared_ptr<GrammarNode> root; // Root of the grammar tree
    std::ostream& print(std::ostream & out = std::cout) const;
    // Construct the grammar from a list of production rules and a start symbol
    // Eg: S 5 -> S 4 S 4
    // Eg: S 5 -> S 1 S 4
    // Eg: S 1 -> a (only one terminal symbol allowed)
    void constructGrammar(std::vector<std::string> &productions, const std::string &startSymbol);
    void InstallNumVars();
    void updateLevel();
    bool operator!= (const Grammar & g) const;  // Overloaded !=
    bool operator== (const Grammar & g) const;  // Overloaded ==
    Grammar operator= (const Grammar & g); // Overloaded assignment
    static std::shared_ptr<GrammarTerminalNode> terminalNode; // Singleton terminal node
};

class GrammarNode {
public:
    GrammarNode();  // Constructor
    GrammarNode(unsigned int level);
    virtual ~GrammarNode(); // Destructor
    unsigned int level; // Level of the node in the grammar tree
    virtual bool isTerminal() const = 0; // Check if the node is terminal

    virtual bool operator!= (const GrammarNode & n) const = 0;  // Overloaded !=
    virtual bool operator== (const GrammarNode & n) const = 0;  // Overloaded ==
    virtual std::ostream& print(std::ostream & out = std::cout) const = 0;
    virtual void InstallNumVars(std::unordered_set<GrammarNode*, GrammarNodeHash, GrammarNodeEqual>& visited) = 0;
    virtual void updateLevel() = 0;
    unsigned int numVars;
    bool isLevelSet;
};

class GrammarNonTerminalNode : public GrammarNode {
public:
    GrammarNonTerminalNode();  // Constructor
    GrammarNonTerminalNode(unsigned int level);
    GrammarNonTerminalNode(const GrammarNonTerminalNode &other);
    ~GrammarNonTerminalNode(); // Destructor
    std::string productionRule; // Production rule applied at this non-terminal node
    std::vector<std::shared_ptr<GrammarNode>> children; // Children of the non-terminal node
    bool isTerminal() const { return false; }
    void addChild(const std::shared_ptr<GrammarNode> &child); // Add a child node
    void setProductionRule(const std::string &rule); // Set the production rule
    bool operator!= (const GrammarNode & n) const;  // Overloaded !=
    bool operator== (const GrammarNode & n) const;  // Overloaded ==
    std::ostream& print(std::ostream & out = std::cout) const;
    GrammarNonTerminalNode& operator= (const GrammarNonTerminalNode &other); // Overloaded assignment
    void InstallNumVars(std::unordered_set<GrammarNode*, GrammarNodeHash, GrammarNodeEqual>& visited);
    void updateLevel();
};

class GrammarTerminalNode : public GrammarNode {
public:
    GrammarTerminalNode();  // Constructor
    ~GrammarTerminalNode(); // Destructor
    bool isTerminal() const { return true; }
    bool operator!= (const GrammarNode & n) const;  // Overloaded !=
    bool operator== (const GrammarNode & n) const;  // Overloaded ==
    GrammarTerminalNode& operator= (const GrammarTerminalNode &other); // Overloaded assignment
    std::ostream& print(std::ostream & out = std::cout) const;
    void InstallNumVars(std::unordered_set<GrammarNode*, GrammarNodeHash, GrammarNodeEqual>& visited);
    void updateLevel();
};

#endif // GRAMMAR_H