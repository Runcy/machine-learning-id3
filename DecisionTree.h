#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include "DataEngine.h"

typedef std::pair<std::string, std::string> ItemPair;

class DecisionTree {
private:
    std::vector<string> totalAttributes;
    const static DataEngine dataEngine;

    float getEntropyGain(std::vector<ItemPair> contextString, std::string attribute)
    {
        std::vector<std::string> attributesList;
        dataEngine.getDistinctAttributeValues(attributesList, attribute);

        std::string contextQueryString = contextString.begin()->first + " = " + contextString.begin()->second;
        for (auto it = contextString.begin() + 1; it != contextString.end(); it++) {
            contextQueryString += " and " + it->first + " = " + it->second;
        }
        int contextCount = dataEngine.getCount(contextQueryString);

        // now we have to find number of each value of the chosen attribute
        // then computing entropy gain is not that hard
    }

public:
    NodeType type;
    std::vector<DecisionTree*> children;
    ItemPair attributePair;
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
    DecisionTree buildTree(DecisionTree &dt, std::vector<ItemPair> currentAttributes, std::vector<string> remainingAttributes)
    {
        int result = checkResult(currentAttribute);
        if (result > 0) {
            return new DecisionTree("result", ">=50K");
        } else if (result < 0) {
            return new DecisionTree("result", "<50K");
        } else {
            //if current == empty, we know it's root


            //anyhow, we will calculate entropies of everything and find
            // out the best greedy choice
            float maxEntropyGain = 0;
            float currentEntropyGain = 0;
            auto bestChoice = remainingAttributes.begin();
            for (auto *it = remainingAttributes.begin(); it != remainingAttributes.end(); it++) {
                currentEntropyGain = entropyGain(currentAtrributes, remainingAttributes);
                if (currentEntropyGain > maxEntropyGain) {
                    bestChoice = it;
                    maxEntropyGain = currentEntropyGain;
                }
            }
            //do something with best choice
            itemPair = *bestChoice;
            remainingAttributes.erase(bestChoice);
        }
    }
};

#endif
