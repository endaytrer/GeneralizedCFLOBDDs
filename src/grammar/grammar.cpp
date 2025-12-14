#include "grammar.h"
#include <sstream>
#include <unordered_map>

std::shared_ptr<GrammarTerminalNode> Grammar::terminalNode = std::make_shared<GrammarTerminalNode>();

Grammar::Grammar() : root(nullptr) {
    // Constructor implementation (if needed)
    root = nullptr;
}

Grammar::~Grammar() {
    // Destructor implementation (if needed)
}

std::ostream& Grammar::print(std::ostream & out) const {
    if (root) {
        root->print(out);
    } else {
        out << "Empty Grammar" << std::endl;
    }
    return out;
}

void Grammar::InstallNumVars() {
    if (root) {
        std::unordered_set<GrammarNode*, GrammarNodeHash, GrammarNodeEqual> visited;
        root->InstallNumVars(visited);
    }
}

void Grammar::updateLevel() {
    if (root) {
        root->updateLevel();
    }
}

// Construct the grammar from a list of production rules and a start symbol
// Eg: S 5 -> S 4 S 4
// Eg: S 5 -> S 0 S 4
// Eg: S 0 -> a (only one terminal symbol allowed)
void Grammar::constructGrammar(std::vector<std::string> &productions, const std::string &startSymbol) {
    std::unordered_map<std::string, std::shared_ptr<GrammarNonTerminalNode>> nonTerminalMap;
    for (const auto &prod: productions) {
        std::istringstream stream(prod);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(stream, token, ' ')) {
            if (!token.empty()) {
                tokens.push_back(token);
            }
        }
        // Now tokens holds parts of the production rule split by space.
        // For example, if prod is "S 5 -> S 4 S 4", tokens will contain:
        // {"S", "5", "->", "S", "4", "S", "4"}
        if (tokens[2] != "->") {
            throw std::invalid_argument("Invalid production rule: " + prod);
        }
        std::string lhs = tokens[0] + " " + tokens[1]; // e.g., "S 5"

        if (nonTerminalMap.find(lhs) == nonTerminalMap.end()) {
            nonTerminalMap[lhs] = std::make_shared<GrammarNonTerminalNode>(stoi(tokens[1]));
            nonTerminalMap[lhs]->setProductionRule(prod);
        } else {
            nonTerminalMap[lhs]->setProductionRule(prod); // Update production rule if already exists
        }

        auto lastToken = tokens.back();
        if (lastToken.size() == 1 && islower(lastToken[0])) {
            // Terminal production
            nonTerminalMap[lhs]->addChild(Grammar::terminalNode);
        } else if (lastToken.find("BDD(") == 0 && lastToken.back() == ')') {
            // BDD terminal production
            size_t startPos = lastToken.find('(') + 1;
            size_t endPos = lastToken.find(')');
            std::string varCountStr = lastToken.substr(startPos, endPos - startPos);
            unsigned int varCount = std::stoul(varCountStr);
            std::shared_ptr<GrammarTerminalNode> bddTerminalNode = std::make_shared<GrammarTerminalNode>();
            bddTerminalNode->numVars = varCount;
            nonTerminalMap[lhs]->addChild(bddTerminalNode);
        }
        else {
            // Non-terminal production
            for (size_t i = 3; i < tokens.size(); i += 2) {
                std::string childSymbol = tokens[i] + " " + tokens[i + 1];
                if (nonTerminalMap.find(childSymbol) == nonTerminalMap.end()) {
                    unsigned int level = stoi(tokens[i + 1]);
                    std::shared_ptr<GrammarNonTerminalNode> childNode = std::make_shared<GrammarNonTerminalNode>(level);
                    nonTerminalMap[childSymbol] = childNode;
                }
                nonTerminalMap[lhs]->addChild(nonTerminalMap[childSymbol]);
            }
        }
    }
    if (nonTerminalMap.find(startSymbol) == nonTerminalMap.end()) {
        throw std::invalid_argument("Start symbol not found in productions: " + startSymbol);
    }
    root = nonTerminalMap[startSymbol];
}

bool Grammar::operator!= (const Grammar & g) const {
    if (root && g.root) {
        return *root != *g.root;
    }
    return root != g.root; // One is null, the other is not
}

bool Grammar::operator== (const Grammar & g) const {
    return !(*this != g);
}

Grammar Grammar::operator= (const Grammar & g) {
    if (this != &g) { // Avoid self-assignment
        root = g.root;
    }
    return *this;
}


/************* GrammarNode Implementation *************/

GrammarNode::GrammarNode() : level(0), isLevelSet(false) {
    // Constructor implementation (if needed)
}

GrammarNode::GrammarNode(unsigned int level) : level(level), isLevelSet(true) {
    // Constructor implementation (if needed)
}

GrammarNode::~GrammarNode() {
    // Destructor implementation (if needed)
}

/************* GrammarNonTerminalNode Implementation *************/

GrammarNonTerminalNode::GrammarNonTerminalNode() : GrammarNode(), productionRule(""), children() {
    // Constructor implementation (if needed)
}

GrammarNonTerminalNode::GrammarNonTerminalNode(unsigned int level) : GrammarNode(level), productionRule(""), children() {
    // Constructor implementation (if needed)
}

GrammarNonTerminalNode::GrammarNonTerminalNode(const GrammarNonTerminalNode &other) : GrammarNode(other), productionRule(other.productionRule) {
    // Copy constructor implementation (if needed)
    for (const auto &child : other.children) {
        children.push_back(child);
    }
}

GrammarNonTerminalNode::~GrammarNonTerminalNode() {
    // Destructor implementation (if needed)
}

void GrammarNonTerminalNode::updateLevel() {
    if (isLevelSet) return; // Already set
    if (children.size() == 1 && children[0]->isTerminal()) {
        level = 0;
        isLevelSet = true;
        return;
    }
    unsigned int maxChildLevel = 0;
    for (const auto& child : children) {
        if (!child->isLevelSet) {
            child->updateLevel();
        }
        if (child->level > maxChildLevel) {
            maxChildLevel = child->level;
        }
    }
    level = maxChildLevel + 1;
    isLevelSet = true;
}

void GrammarNonTerminalNode::addChild(const std::shared_ptr<GrammarNode> &child) {
    children.push_back(child);
}

void GrammarNonTerminalNode::setProductionRule(const std::string &rule) {
    productionRule = rule;
}

bool GrammarNonTerminalNode::operator!= (const GrammarNode & n) const {
    const GrammarNonTerminalNode* other = dynamic_cast<const GrammarNonTerminalNode*>(&n);
    if (!other) return true; // Different types
    if (productionRule != other->productionRule) return true;
    if (children.size() != other->children.size()) return true;
    for (size_t i = 0; i < children.size(); ++i) {
        if (children[i] != other->children[i]) return true;
    }
    return false;
}

bool GrammarNonTerminalNode::operator== (const GrammarNode & n) const {
    return !(*this != n);
}

GrammarNonTerminalNode& GrammarNonTerminalNode::operator= (const GrammarNonTerminalNode &other) {
    if (this != &other) { // Avoid self-assignment
        GrammarNode::operator=(other); // Call base class assignment
        productionRule = other.productionRule;
        children.clear();
        for (const auto &child : other.children) {
            children.push_back(child);
        }
    }
    return *this;
}

std::ostream& GrammarNonTerminalNode::print(std::ostream & out) const {
    for (unsigned int i = 0; i < level; ++i) {
        out << "  ";
    }
    out << "Non-Terminal Node: " << productionRule << "\n";
    for (const auto& child : children) {
        child->print(out);
        out << "\n";
    }
    return out;
}

void GrammarNonTerminalNode::InstallNumVars(std::unordered_set<GrammarNode*, GrammarNodeHash, GrammarNodeEqual>& visited) {
    if (children.empty()) {
        throw std::runtime_error("Non-terminal node has no children");
    } else {
        if (visited.find(this) != visited.end()) {
            return; // Already visited
        }
        visited.insert(this);
        numVars = 0;
        for (const auto& child : children) {
            child->InstallNumVars(visited);
            numVars += child->numVars;
        }
    }
}

bool GrammarNonTerminalNode::isBDDGrammar() const {
    for (const auto& child : children) {
        if (child->isBDDGrammar()) {
            return true;
        }
    }
    return false;
}

/************* GrammarTerminalNode Implementation *************/
GrammarTerminalNode::GrammarTerminalNode() : GrammarNode() {
    // Constructor implementation (if needed)
    level = 0; // Terminal nodes are at level 0
}

GrammarTerminalNode::~GrammarTerminalNode() {
    // Destructor implementation (if needed)
}

bool GrammarTerminalNode::operator!= (const GrammarNode & n) const {
    const GrammarTerminalNode* other = dynamic_cast<const GrammarTerminalNode*>(&n);
    return other == nullptr; // Not equal if not the same type
}

bool GrammarTerminalNode::operator== (const GrammarNode & n) const {
    return !(*this != n);
}

GrammarTerminalNode& GrammarTerminalNode::operator= (const GrammarTerminalNode &other) {
    if (this != &other) { // Avoid self-assignment
        GrammarNode::operator=(other); // Call base class assignment
    }
    return *this;
}

std::ostream& GrammarTerminalNode::print(std::ostream & out) const {
    out << "Terminal Node" << std::endl;
    return out;
}

void GrammarTerminalNode::InstallNumVars(std::unordered_set<GrammarNode*, GrammarNodeHash, GrammarNodeEqual>& visited) {
    if (visited.find(this) != visited.end()) {
        return; // Already visited
    }
    visited.insert(this);
    if (!isBDDGrammar()) {
        numVars = 1; // Regular terminal nodes contribute one variable
        return;
    }
    // For BDD terminal nodes, numVars is already set during construction
    // So we do not change it here
}

void GrammarTerminalNode::updateLevel() {
    level = 0;
    isLevelSet = true;
}