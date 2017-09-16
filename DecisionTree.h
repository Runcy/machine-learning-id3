#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include "DataEngine.h"

class DecisionTree {
private:
    std::vector<string> totalAttributes;
    const static DataEngine dataEngine;

    float getEntropy(std::string contextString, std::string)
    {

    }

public:
    NodeType type;
    std::vector<DecisionTree*> children;
    std::pair<std::string, std::string> attributePair;
    enum NodeType {
        TerminalNode,
        AttributeNode
    };
    DecisionTree(const DataEngine &dataEngine, std::vector<string> &attributes)
    {
        this->dataEngine = dataEngine;
        totalAttributes = attributes;
    }
    DecisionTree(std::string attribute, std::string value)
    {
        type = NodeType::TerminalNode;
        attributePair.first = attribute;
        attributePair.second = value;
        children = nullptr;
    }
    DecisionTree buildTree(DecisionTree &dt, std::vector<string> currentAttributes, std::vector<string> remainingAttributes)
    {
        int result = checkResult(currentAttribute);
        if (result > 0) {
            return new DecisionTree("result", ">=50K");
        } else if (result < 0) {
            return new DecisionTree("result", "<50K");
        } else {
            //if current == empty, we know it's root
        }
    }
};

#endif
