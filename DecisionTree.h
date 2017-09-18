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
        int contextCount;

        std::string contextQueryString = "";
        if (!contextString.empty()) {
            contextQueryString = contextString.begin()->first + " = " + contextString.begin()->second;
            for (auto it = contextString.begin() + 1; it != contextString.end(); it++) {
                contextQueryString += " and " + it->first + " = '" + it->second + "'";
            }
            // std::cout << contextQueryString << std::endl;
            contextCount = dataEngine.getCount(contextQueryString);
        } else {
            contextCount = dataEngine.getAllCount();
        }

        int attributeCount;
        float instanceProbability;
        float positiveProbability;
        float negativeProbability;
        float positiveEntropy;
        float negativeEntropy;
        float entropy;
        float totalEntropyGain = 0;
        // now we have to find number of each value of the chosen attribute
        std::string attributeQueryString;
        for (auto it = attributesList.begin(); it != attributesList.end(); it++) {
            attributeQueryString = contextQueryString;
            if (contextString.empty()) {
                attributeQueryString += attribute + " = '" + *it + "'";
            } else {
                attributeQueryString += " and " + attribute + " = '" + *it + "'";
            }
            // std::cout << attributeQueryString << std::endl;
            attributeCount = dataEngine.getCount(attributeQueryString);

            instanceProbability = (float) attributeCount / contextCount;

            positiveProbability = dataEngine.getProbability(attributeQueryString, '+');
            negativeProbability = dataEngine.getProbability(attributeQueryString, '-');

            positiveEntropy = (positiveProbability == 0) ? 0 : positiveProbability*log2(positiveProbability);
            negativeEntropy = (negativeProbability == 0) ? 0 : negativeProbability*log2(negativeProbability);

            // positiveEntropy = positiveProbability*log2(positiveProbability);
            // negativeEntropy = negativeProbability*log2(negativeProbability);

            entropy = positiveEntropy + negativeEntropy;

            totalEntropyGain += instanceProbability*entropy;
        }
        return totalEntropyGain;
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
