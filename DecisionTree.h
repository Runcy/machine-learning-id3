// Class for implmenting the Decison Tree. This is used for implementing all the
// Decision Trees in this project, including the one used for reduced error pruning and
// random forests.

#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include "DataEngine.h"

// Used extensively for nodes. The first element is the attribute of the node at
// that point and the second is the value.
typedef std::pair<std::string, std::string> ItemPair;

class DecisionTree {
private:
    std::vector<std::string> totalAttributes; // list of attributes in the dataset
    std::vector<std::string> contAttributes; // list of contd. attributes
    std::string resultString;
    std::string positiveString;
    std::string negativeString;
    DataEngine dataEngine;

    // We deduce the best continuous attribute by scanning instances where the result
    // of the instance changes from positive to negative and vice versa. The entropy for
    // the subsets is calculated and compared with those in the discrete case.

    std::pair<float, float> getContinuousEntropyGain(std::vector<ItemPair> contextString, std::string attribute)
    {
        std::vector<int> contValuesPositive;
        std::vector<int> contValuesNegative;

        std::vector<float> boundaryValues;
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
            contextCount = dataEngine.getCount(contextQueryString);
        } else {
            contextCount = dataEngine.getAllCount();
        }

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
            attributeCount = dataEngine.getCount(attributeQueryString);
            instanceProbability = (float) attributeCount / contextCount;

            positiveProbability = dataEngine.getProbability(attributeQueryString, '+');
            negativeProbability = dataEngine.getProbability(attributeQueryString, '-');

            positiveEntropy = (positiveProbability == 0) ? 0 : positiveProbability*log2(positiveProbability);
            negativeEntropy = (negativeProbability == 0) ? 0 : negativeProbability*log2(negativeProbability);

            entropy = positiveEntropy + negativeEntropy;

            totalEntropyGain += entropy*instanceProbability;

            attributeQueryString = prepareQueryString(contextString);
            if (contextString.empty()) {
                attributeQueryString += attribute + "<=" + std::to_string(*it);
            } else {
                attributeQueryString += " and " + attribute + "<=" + std::to_string(*it);
            }

            attributeCount = dataEngine.getCount(attributeQueryString);
            instanceProbability = (float) attributeCount / contextCount;

            positiveProbability = dataEngine.getProbability(attributeQueryString, '+');
            negativeProbability = dataEngine.getProbability(attributeQueryString, '-');

            positiveEntropy = (positiveProbability == 0) ? 0 : positiveProbability*log2(positiveProbability);
            negativeEntropy = (negativeProbability == 0) ? 0 : negativeProbability*log2(negativeProbability);
            entropy = positiveEntropy + negativeEntropy;
            totalEntropyGain += entropy*instanceProbability;

            if (totalEntropyGain > maxEntropy) {
                maxEntropy = totalEntropyGain;
                bestVal = *it;
            }
        }
        return std::make_pair(bestVal, maxEntropy*-1);
    }

    // Greedily checks for the attribute with the best entropy gain in the dataset.
    // The context string parameter gives the list of attributes satisfied by the given node.

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
            attributeQueryString = contextQueryString;
            if (contextString.empty()) {
                attributeQueryString += attribute + " = '" + *it + "'";
            } else {
                attributeQueryString += " and " + attribute + " = '" + *it + "'";
            }
            attributeCount = dataEngine.getCount(attributeQueryString);

            instanceProbability = (float) attributeCount / contextCount;

            positiveProbability = dataEngine.getProbability(attributeQueryString, '+');
            negativeProbability = dataEngine.getProbability(attributeQueryString, '-');

            positiveEntropy = (positiveProbability == 0) ? 0 : positiveProbability*log2(positiveProbability);
            negativeEntropy = (negativeProbability == 0) ? 0 : negativeProbability*log2(negativeProbability);

            entropy = positiveEntropy + negativeEntropy;

            totalEntropyGain += instanceProbability*entropy;
        }
        return totalEntropyGain*-1;
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
        std::vector<DecisionTreeNode*> children; // each node of the DecisionTreeNode has a vector of pointers to the next nodes in the tree
        NodeType type; // useful for letting us know whether we are at the "Top" (root) or "Bottom" (terminal) of the tree
        ItemPair attributePair; // attributes and values are stored as pairs, like <"outlook", "'sunny'">
    };

    DecisionTreeNode rootNode, myRoot;

    // Constructor for initialising the decision tree. The Data Engine is also constructed
    // and initialised in this step. If the last argument is set to false, the DecisionTree
    // is initialised without building the tree, whereas the tree is built if the last argument
    // is set to true.

    DecisionTree(std::vector<std::string> &attributes,
                std::vector<std::string> &_contAttributes,
                const std::string _trainingDataPath,
                const std::string _tableAttributes,
                const std::string _resultString,
                const std::string _positiveInstanceString,
                const std::string _negativeInstanceString, bool doBuild) :
                dataEngine(_trainingDataPath, _tableAttributes, _resultString,
                            _positiveInstanceString, _negativeInstanceString)
    {
        resultString = _resultString;
        contAttributes = _contAttributes;
        totalAttributes = attributes;
        std::vector<std::pair<std::string, std::string>> nodeContext;
        rootNode.type = NodeType::RootNode;
        myRoot.type = NodeType::RootNode;
        positiveString = _positiveInstanceString;
        negativeString = _negativeInstanceString;
        if (doBuild) {
            buildTree(&rootNode, nodeContext, totalAttributes);
        }
    }

    // Converts the context of a node into a string which can be submitted for SQL queries.

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

    // Recursively finds a node in the tree using custom DFS. Returns nullptr if no node is found.

    DecisionTreeNode* findNode(DecisionTreeNode* node, std::string attribute, std::string attributeValue)
    {
        if (node->attributePair.first == attribute && node->attributePair.second == attributeValue) {
            return node;
        }
        DecisionTreeNode* tmp;
        for (auto it = node->children.begin(); it != node->children.end(); it++) {
            tmp = findNode(*it, attribute, attributeValue);
            if (tmp!=nullptr) {
                return tmp;
            }
        }
        return nullptr;
    }

    // Used for building a tree from the rules deduced in a file.

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
        if (attribute == resultString) {
            DecisionTreeNode* terminalNode = new DecisionTreeNode();
            terminalNode->attributePair = std::make_pair(attribute, attributeValue);
            terminalNode->type = NodeType::TerminalNode;
            node->children.push_back(terminalNode);
            return;
        }
        searchNode = findNode(node, attribute, attributeValue);

        if (searchNode == nullptr) {
            insertionNode = node;
        } else {
            insertionNode = searchNode;
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
                insertionNode->children.push_back(childNode);
                if ("'" + *it + "'" == attributeValue) {
                    nextNode = childNode;
                }
            }
        } else {
            nextNode = searchNode;
        }
        buildTreeFromRule(nextNode, ruleQueue);
    }

    std::string getMostCommonResult(std::vector<ItemPair> nodeContext)
    {
        std::string contextQueryString = "";
        contextQueryString = prepareQueryString(nodeContext);
        float positiveProbability = dataEngine.getProbability(contextQueryString, '+');
        float negativeProbability = dataEngine.getProbability(contextQueryString, '-');

        std::string resultString = (positiveProbability > negativeProbability) ? positiveString : negativeString;
        return resultString;
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

    // Searches through the tree to find if a test instance is positive or negative.

    std::string evaluateInstance(DecisionTreeNode* node, std::vector<ItemPair> instanceList)
    {
        std::string attribute = (*(node->children.begin()))->attributePair.first;

        if (attribute == resultString) {
            return (*(node->children.begin()))->attributePair.second;
        }

        std::string attributeValue = searchAttributeValue(attribute, instanceList);

        std::string nodeAttr;
        std::string nodeVal;
        if (isContAttribute(attribute)) {
            DecisionTreeNode* positiveContNode = *(node->children.begin());
            DecisionTreeNode* negativeContNode = *(node->children.begin()+1);

            nodeAttr = (positiveContNode)->attributePair.first;
            nodeVal = (positiveContNode)->attributePair.second;

            std::string number = nodeVal.substr(1);
            float nodeContNumber = std::stof(number);
            float instanceContNumber = std::stof(attributeValue);
            DecisionTreeNode* nextNode = (instanceContNumber > nodeContNumber) ? positiveContNode : negativeContNode;

            if (nextNode->children.empty()) {
                nextNode = (nextNode == positiveContNode) ? negativeContNode : positiveContNode;
            }
            return evaluateInstance(nextNode, instanceList);
        } else if (!isContAttribute(attribute)) {
            for (auto it = node->children.begin(); it != node->children.end(); it++) {
                nodeAttr = (*it)->attributePair.first;
                nodeVal = (*it)->attributePair.second;
                if ((nodeAttr == attribute) && (nodeVal == attributeValue)) {
                    if ((*it)->children.empty()) {
                        for (auto itr = node->children.begin(); itr != node->children.end(); itr++) {
                            if (!(*itr)->children.empty()) {
                                return evaluateInstance(*itr, instanceList);
                            }
                        }
                    }
                    return evaluateInstance(*it, instanceList);
                }
            }
        }
        return "Oh Dear";
    }

    static void traverseTree(DecisionTreeNode* node, std::string rule)
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
            std::cout << rule << "RULE: " << std::endl;
        }
    }

    // Overloaded method for traversing the tree. Writes the linearizied tree to the file specified in output.

    static void traverseTree(DecisionTreeNode* node, std::string rule, std::ofstream &output)
    {
        if (node->type == NodeType::RootNode) {
            for (auto it = node->children.begin(); it != node->children.end(); it++) {
                traverseTree(*it, rule, output);
            }
            return;
        }
        rule += node->attributePair.first + ' ' + node->attributePair.second + '\n';
        if (node->type == NodeType::AttributeNode) {
            for (auto it = node->children.begin(); it != node->children.end(); it++) {
                traverseTree(*it, rule, output);
            }
        }
        if (node->type == NodeType::TerminalNode) {
            output << rule << "RULE: " << std::endl;
        }
    }

    // Called from the constructor to recursively build the tree given the training examples.

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
                } else {
                    attributeEntropy = getEntropyGain(nodeContext, *it);
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
