#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include "DataEngine.h"

typedef std::pair<std::string, std::string> ItemPair;

class DecisionTree {
private:
    std::vector<std::string> totalAttributes;
    std::vector<std::string> contAttributes;
    std::string resultString;
    DataEngine dataEngine;

    std::pair<float, float> getContinuousEntropyGain(std::vector<ItemPair> contextString, std::string attribute)
    {
        std::vector<int> contValuesPositive;
        std::vector<int> contValuesNegative;

        std::vector<float> boundaryValues;
    // do +ve/-ve
        std::string queryStringPositive = prepareQueryString(contextString);
        std::string queryStringNegative = prepareQueryString(contextString);
        if (!contextString.empty()) {
            queryStringPositive += " and result = '>50K'";

            queryStringNegative += " and result = '<=50K'";
        } else {
            queryStringPositive = " result = '>50K'";
            queryStringNegative = " result = '<=50K'";
        }
        dataEngine.getContinuousAttributeValues(contValuesNegative, attribute, queryStringNegative);
        dataEngine.getContinuousAttributeValues(contValuesPositive, attribute, queryStringPositive);
        // std::cout <<"SUCCESS";
        std::sort(contValuesPositive.begin(), contValuesPositive.end());
        std::sort(contValuesNegative.begin(), contValuesNegative.end());

        auto posItr = contValuesPositive.begin();
        auto negItr = contValuesNegative.begin();
        float boundaryValue;
        // like mergesort
        while(posItr < contValuesPositive.end() && negItr < contValuesNegative.end()) {
            if (*posItr <= *negItr) {
                boundaryValue = (*posItr+*negItr)/2.0;
                boundaryValues.push_back(boundaryValue);
                *posItr++;
            } else {
                boundaryValue = (*posItr+*negItr)/2.0;
                boundaryValues.push_back(boundaryValue);
                *negItr++;
            }
        }
        std::string attributeQueryString;
        std::string contextQueryString = "";
        int contextCount;
        int attributeCount;
        float instanceProbability;
        float positiveProbability;
        float negativeProbability;
        float positiveEntropy;
        float negativeEntropy;
        float entropy;
        if (!contextString.empty()) {
            contextQueryString = prepareQueryString(contextString);
            // std::cout << "qs " << contextQueryString << std::endl;
            contextCount = dataEngine.getCount(contextQueryString);
        } else {
            contextCount = dataEngine.getAllCount();
        }
        // std::cout << contextCount;

        float maxEntropy = -1;
        float bestVal = 0;
        float totalEntropyGain;
        for (auto it = boundaryValues.begin(); it != boundaryValues.end(); it++) {
            totalEntropyGain = 0;

            attributeQueryString = prepareQueryString(contextString);
            if (contextString.empty()) {
                attributeQueryString += attribute + ">" + std::to_string(*it);
            } else {
                attributeQueryString += " and " + attribute + ">" + std::to_string(*it);
            }
            // std::cout << attributeQueryString;
            attributeCount = dataEngine.getCount(attributeQueryString);
            // std::cout << "AF" << attributeCount << ' ' << attributeQueryString << std::endl;
            instanceProbability = (float) attributeCount / contextCount;

            positiveProbability = dataEngine.getProbability(attributeQueryString, '+');
            negativeProbability = dataEngine.getProbability(attributeQueryString, '-');

            positiveEntropy = (positiveProbability == 0) ? 0 : positiveProbability*log2(positiveProbability);
            negativeEntropy = (negativeProbability == 0) ? 0 : negativeProbability*log2(negativeProbability);

            entropy = positiveEntropy + negativeEntropy;

            // std::cout << *it << "GRAT" << instanceProbability << std::endl;
            totalEntropyGain += entropy*instanceProbability;

            attributeQueryString = prepareQueryString(contextString);
            if (contextString.empty()) {
                attributeQueryString += attribute + "<=" + std::to_string(*it);
            } else {
                attributeQueryString += " and " + attribute + "<=" + std::to_string(*it);
            }
            // std::cout << *it << std::endl;
            // std::cout << attributeQueryString;
            attributeCount = dataEngine.getCount(attributeQueryString);
            // std::cout << attributeCount << std::endl;
            instanceProbability = (float) attributeCount / contextCount;

            positiveProbability = dataEngine.getProbability(attributeQueryString, '+');
            negativeProbability = dataEngine.getProbability(attributeQueryString, '-');

            positiveEntropy = (positiveProbability == 0) ? 0 : positiveProbability*log2(positiveProbability);
            negativeEntropy = (negativeProbability == 0) ? 0 : negativeProbability*log2(negativeProbability);
            entropy = positiveEntropy + negativeEntropy;
            totalEntropyGain += entropy*instanceProbability;

            // std::cout << *it << ' ' << totalEntropyGain << std::endl;
            if (totalEntropyGain > maxEntropy) {
                maxEntropy = totalEntropyGain;
                bestVal = *it;
            }
        }
        // std::cout << "BEST" << bestVal << ' ' << "MAX" << maxEntropy << ' ' << contextQueryString << ' ' << attribute << std::endl;
        return std::make_pair(bestVal, maxEntropy*-1);
    }

    float getEntropyGain(std::vector<ItemPair> contextString, std::string attribute)
    {
        std::vector<std::string> attributesList;
        dataEngine.getDistinctAttributeValues(attributesList, attribute);
        int contextCount;

        std::string contextQueryString = "";
        if (!contextString.empty()) {
            contextQueryString = prepareQueryString(contextString);
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
            // std::cout << "attribs for " << attribute << ' ' << *it << std::endl;
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
        // std::cout << contextQueryString << ' ' << attribute << ' ' << totalEntropyGain <<std::endl;
        return totalEntropyGain*-1;
        // then computing entropy gain is not that hard
    }

    std::string prepareQueryString(std::vector<ItemPair> contextString)
    {
        std::string contextQueryString;
        if (contextString.empty()) {
            return "";
        }
        if (isContAttribute(contextString.begin()->first)) {
            contextQueryString = contextString.begin()->first + contextString.begin()->second;
        } else {
            contextQueryString = contextString.begin()->first + " = " + contextString.begin()->second;
        }
        for (auto it = contextString.begin() + 1; it != contextString.end(); it++) {
            if (isContAttribute(it->first)) {
                contextQueryString += " and " + it->first + it->second;
            } else {
                contextQueryString += " and " + it->first + " = " + it->second;
            }
        }
        return contextQueryString;
    }

    bool isContAttribute(std::string attribute)
    {
        for (auto it = contAttributes.begin(); it != contAttributes.end(); it++) {
            if (*it == attribute) {
                return true;
            }
        }
        return false;
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

    DecisionTreeNode rootNode, myRoot;

    DecisionTree(std::vector<std::string> &attributes,
                std::vector<std::string> &_contAttributes,
                const std::string _trainingDataPath,
                const std::string _tableAttributes,
                const std::string _resultString,
                const std::string _positiveInstanceString,
                const std::string _negativeInstanceString) :
                dataEngine(_trainingDataPath, _tableAttributes, _resultString,
                            _positiveInstanceString, _negativeInstanceString)
    {
        resultString = _resultString;
        contAttributes = _contAttributes;
        totalAttributes = attributes;
        std::vector<std::pair<std::string, std::string>> nodeContext;
        rootNode.type = NodeType::RootNode;
        myRoot.type = NodeType::RootNode;
        buildTree(&rootNode, nodeContext, totalAttributes);
    }

    DecisionTree(std::vector<std::string> &attributes,
                std::vector<std::string> &_contAttributes,
                const std::string _trainingDataPath,
                const std::string _tableAttributes,
                const std::string _resultString,
                const std::string _positiveInstanceString,
                const std::string _negativeInstanceString, bool noBuild) :
                dataEngine(_trainingDataPath, _tableAttributes, _resultString,
                            _positiveInstanceString, _negativeInstanceString)
    {
        resultString = _resultString;
        contAttributes = _contAttributes;
        totalAttributes = attributes;
        std::vector<std::pair<std::string, std::string>> nodeContext;
        rootNode.type = NodeType::RootNode;
        myRoot.type = NodeType::RootNode;
        // buildTree(&rootNode, nodeContext, totalAttributes);
    }


    // DecisionTreeNode* findNode(DecisionTreeNode* node, std::string attribute, std::string attributeValue) //this code sucks
    // {
    //     if (node->type == NodeType::TerminalNode) {
    //         for (auto it = node->children.begin(); it != node->children.end(); it++) {
    //             return findNode(*it, attribute, attributeValue);
    //         }
    //     } else if (node->type == NodeType::AttributeNode) {
    //         if (node->attributePair.first == attribute && node->attributePair.second == attributeValue) {
    //             return node;
    //         } else {
    //             for (auto it = node->children.begin(); it != node->children.end(); it++) {
    //                 return findNode(*it, attribute, attributeValue);
    //             }
    //         }
    //     } else if (node->type == NodeType::RootNode) {
    //         return nullptr;
    //     }
    // }

    DecisionTreeNode* findNode(DecisionTreeNode* node, std::string attribute, std::string attributeValue)
    {
        // std::cout << attribute <<  ' ' << attributeValue;
        // std::cout << "ATNODE" + node->attributePair.first + node->attributePair.second <<std::endl;
        if (node->attributePair.first == attribute && node->attributePair.second == attributeValue) {
            return node;
        }
        DecisionTreeNode* tmp;
        for (auto it = node->children.begin(); it != node->children.end(); it++) {
            // std::cout << "SEARCHING!!" << (*it)->attributePair.first << " " << (*it)->attributePair.second << std::endl;
            tmp = findNode(*it, attribute, attributeValue);
            if (tmp!=nullptr) {
                return tmp;
            }
        }
        return nullptr;
    }

    void buildTreeFromRule(DecisionTreeNode* node, std::queue<ItemPair> ruleQueue) //attribute is taken and then discarded
    {
        if (ruleQueue.empty()) {
            return;
        }
        ItemPair ruleAttributePair = ruleQueue.front();
        ruleQueue.pop();

        DecisionTreeNode* searchNode = nullptr;
        DecisionTreeNode* insertionNode = nullptr;

        std::string attribute = ruleAttributePair.first;
        std::string attributeValue = ruleAttributePair.second;

        std::vector<std::string> distinctAttributeList;
        searchNode = findNode(node, attribute, attributeValue); //only works in discrete case

        if (searchNode == nullptr) {
            // std::cout << "search for" << attribute << ' ' <<attributeValue << "not found\n";
            insertionNode = node;
        } else {
            // std::cout << "search for" << attribute << ' ' <<attributeValue << "found\n";
            insertionNode = searchNode;
        }
        if (attribute == resultString) {
            DecisionTreeNode* terminalNode = new DecisionTreeNode();
            terminalNode->attributePair = std::make_pair(attribute, attributeValue);
            terminalNode->type = NodeType::TerminalNode;
            node->children.push_back(terminalNode);
            return;
        }
        DecisionTreeNode* nextNode;
        if (isContAttribute(attribute) && searchNode == nullptr) {
            std::string contValue;
            std::size_t posMore = attributeValue.find(">");

            contValue = (posMore != std::string::npos) ? attributeValue.substr(1) : attributeValue.substr(2); //1 for >, 2 for <=

            DecisionTreeNode* positiveContNode = new DecisionTreeNode();
            positiveContNode->attributePair = std::make_pair(attribute, ">" + contValue);
            positiveContNode->type = NodeType::AttributeNode;

            DecisionTreeNode* negativeContNode = new DecisionTreeNode();
            negativeContNode->attributePair = std::make_pair(attribute, "<=" + contValue);
            negativeContNode->type = NodeType::AttributeNode;

            insertionNode->children.push_back(positiveContNode);
            insertionNode->children.push_back(negativeContNode);

            nextNode = (posMore != std::string::npos) ? positiveContNode : negativeContNode;
        } else if (!isContAttribute(attribute) && searchNode == nullptr) {
            dataEngine.getDistinctAttributeValues(distinctAttributeList, attribute);
            for (auto it = distinctAttributeList.begin(); it != distinctAttributeList.end(); it++) {
                DecisionTreeNode* childNode = new DecisionTreeNode();
                childNode->attributePair = std::make_pair(attribute, "'" + *it + "'");
                childNode->type = NodeType::AttributeNode;
                // std::cout << "doing " "'" + *it + "'" << " " << attribute <<std::endl;
                insertionNode->children.push_back(childNode);
                if ("'" + *it + "'" == attributeValue) {
                    // std::cout << "match";
                    nextNode = childNode;
                }
            }
        } else {
            nextNode = searchNode;
        }
        buildTreeFromRule(nextNode, ruleQueue);
    }

    std::string searchAttributeValue(std::string attribute, std::vector<ItemPair> instanceList)
    {
        for (auto it = instanceList.begin(); it != instanceList.end(); it++) {
            if (attribute == it->first) {
                return it->second;
            }
        }
        return "Oh dear";
    }

    std::string evaluateInstance(DecisionTreeNode* node, std::vector<ItemPair> instanceList)
    {
        std::string attribute = (*(node->children.begin()))->attributePair.first;
        if (attribute == resultString) {
            return (*(node->children.begin()))->attributePair.second;
        }
        std::string attributeValue = searchAttributeValue(attribute, instanceList);
        std::cout << attribute << ' ' << attributeValue << std::endl;

        std::string nodeAttr;
        std::string nodeVal;
        for (auto it = node->children.begin(); it != node->children.end(); it++) {
            nodeAttr = (*it)->attributePair.first;
            nodeVal = (*it)->attributePair.second;
            if ((nodeAttr == attribute) && (nodeVal == attributeValue)) {
                return evaluateInstance(*it, instanceList);
            }
        }
        return "Oh Dear";
    }

    void traverseTree(DecisionTreeNode* node, std::string rule)
    {
        if (node->type == NodeType::RootNode) {
            for (auto it = node->children.begin(); it != node->children.end(); it++) {
                traverseTree(*it, rule);
            }
            return;
        }
        rule += node->attributePair.first + ' ' + node->attributePair.second + '\n';
        if (node->type == NodeType::AttributeNode) {
            for (auto it = node->children.begin(); it != node->children.end(); it++) {
                traverseTree(*it, rule);
            }
        }
        if (node->type == NodeType::TerminalNode) {
            std::cout << "RULE: " << std::endl << rule;
        }
    }

    void buildTree(DecisionTreeNode* node, std::vector<ItemPair> nodeContext, std::vector<std::string> availableAttributes)
    {
        if (availableAttributes.empty()) {
            return;
        }
        float minEntropy = 999;
        float attributeEntropy;
        auto bestAttributeItr = availableAttributes.begin();
        std::vector<std::string> distinctAttributeList;
        bool terminalNodeReached = false;
        std::string terminalString;
        float bestContValue;

        if (node->type == NodeType::AttributeNode) {
            nodeContext.push_back(node->attributePair);
        }
        // std::cout << "context ";
        // for (auto it = nodeContext.begin(); it != nodeContext.end(); it++) {
        //     std::cout << it->first << ' ' << it->second << '\t';
        // }
        // std::cout << std::endl;
        std::pair<float, float> contEntropyResult;
        int terminalResult;
        if (!nodeContext.empty()) {
            terminalResult = dataEngine.checkUnique(prepareQueryString(nodeContext));
            if (terminalResult == -1) {
                return;
            } else if (terminalResult == 1) {
                terminalNodeReached = true;
            } else {
                terminalNodeReached = false;
            }
        }
        if (terminalNodeReached) {
            terminalString = dataEngine.getResultString(prepareQueryString(nodeContext));
            // std::cout << "TERMINAL! " << terminalString << std::endl;
            DecisionTreeNode* terminalNode = new DecisionTreeNode();
            terminalNode->attributePair = std::make_pair(resultString, terminalString);
            terminalNode->type = NodeType::TerminalNode;
            node->children.push_back(terminalNode);
            return;
        } else {
            for (auto it = availableAttributes.begin(); it != availableAttributes.end(); it++) {
                if (isContAttribute(*it)) {
                    contEntropyResult = getContinuousEntropyGain(nodeContext, *it);
                    bestContValue = contEntropyResult.first;
                    attributeEntropy = contEntropyResult.second;
                    // std::cout << *it << ": " << attributeEntropy <<  ' ' << bestContValue << std::endl;
                } else {
                    attributeEntropy = getEntropyGain(nodeContext, *it);
                    // std::cout << *it << ": " << attributeEntropy << std::endl;
                }
                if (attributeEntropy < minEntropy) {
                    minEntropy = attributeEntropy;
                    bestAttributeItr = it;
                }
            }

            std::string bestAttributeString = *bestAttributeItr;
            availableAttributes.erase(bestAttributeItr);

            if (isContAttribute(bestAttributeString)) {
                DecisionTreeNode* positiveContNode = new DecisionTreeNode();
                positiveContNode->attributePair = std::make_pair(bestAttributeString, ">"+std::to_string(bestContValue));
                positiveContNode->type = NodeType::AttributeNode;
                node->children.push_back(positiveContNode);

                DecisionTreeNode* negativeContNode = new DecisionTreeNode();
                negativeContNode->attributePair = std::make_pair(bestAttributeString, "<=" + std::to_string(bestContValue));
                negativeContNode->type = NodeType::AttributeNode;
                node->children.push_back(negativeContNode);
            } else {
                dataEngine.getDistinctAttributeValues(distinctAttributeList, bestAttributeString);
                for (auto it = distinctAttributeList.begin(); it != distinctAttributeList.end(); it++) {
                    DecisionTreeNode* childNode = new DecisionTreeNode();
                    childNode->attributePair = std::make_pair(bestAttributeString, "'" + *it + "'");
                    childNode->type = NodeType::AttributeNode;
                    node->children.push_back(childNode);
                }
            }

            for (auto it = node->children.begin(); it != node->children.end(); it++) {
                buildTree(*it, nodeContext, availableAttributes);
            }
        }
    }
};

#endif
