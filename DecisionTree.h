#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include "DataEngine.h"

typedef std::pair<std::string, std::string> ItemPair;

class DecisionTree {
private:
    std::vector<std::string> totalAttributes;
    DataEngine dataEngine;

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
        return totalEntropyGain*-1;
        // then computing entropy gain is not that hard
    }

public:
    enum NodeType {
        TerminalNode,
        AttributeNode,
        RootNode
    };

    struct DecisionTreeNode {
        std::vector<DecisionTreeNode*> children;
        NodeType type;
        ItemPair attributePair;
    };

    DecisionTreeNode rootNode;

    DecisionTree(std::vector<std::string> &attributes,
                const std::string _trainingDataPath,
                const std::string _tableAttributes,
                const std::string _resultString,
                const std::string _positiveInstanceString,
                const std::string _negativeInstanceString) :
                dataEngine(_trainingDataPath, _tableAttributes, _resultString,
                            _positiveInstanceString, _negativeInstanceString)
    {
        totalAttributes = attributes;
        std::vector<std::pair<std::string, std::string>> nodeContext;
        rootNode.type = NodeType::RootNode;
        buildTree(&rootNode, nodeContext, totalAttributes);
    }

    void buildTree(DecisionTreeNode* node, std::vector<ItemPair> nodeContext, std::vector<std::string> availableAttributes)
    {
        float minEntropy = 999;
        float attributeEntropy;
        auto bestAttributeItr = availableAttributes.begin();
        std::vector<std::string> distinctAttributeList;
        bool terminalNodeReached = false;
        for (auto it = availableAttributes.begin(); it != availableAttributes.end(); it++) {
            attributeEntropy = getEntropyGain(nodeContext, *it);
            if (attributeEntropy < minEntropy) {
                minEntropy = attributeEntropy;
                bestAttributeItr = it;
                if (minEntropy  == 0) {
                    terminalNodeReached = true;
                    break;
                }
            }
        }

        std::string bestAttributeString = *bestAttributeItr;
        dataEngine.getDistinctAttributeValues(distinctAttributeList, bestAttributeString);

        if (terminalNodeReached) {
            for (auto it = distinctAttributeList.begin(); it != distinctAttributeList.end(); it++) {
                DecisionTreeNode* childNode = new DecisionTreeNode();
                childNode->attributePair = std::make_pair(bestAttributeString, "'" + *it + "'");
                childNode->type = NodeType::AttributeNode;

                DecisionTreeNode* terminalNode = new DecisionTreeNode();
                terminalNode->attributePair = std::make_pair("result", "+");
                terminalNode->type = NodeType::TerminalNode;

                childNode->children.push_back(terminalNode); //change to actual string later
                node->children.push_back(childNode);
            }
            return;
        }

        if (node->type == NodeType::AttributeNode) {
            nodeContext.push_back(node->attributePair);
        }
        availableAttributes.erase(bestAttributeItr);

        for (auto it = distinctAttributeList.begin(); it != distinctAttributeList.end(); it++) {
            DecisionTreeNode* childNode = new DecisionTreeNode();
            childNode->attributePair = std::make_pair(bestAttributeString, "'" + *it + "'");
            childNode->type = NodeType::AttributeNode;
            node->children.push_back(childNode);
        }

        for (auto it = node->children.begin(); it != node->children.end(); it++) {
            buildTree(*it, nodeContext, availableAttributes);
        }
    }

};

#endif
