// #include "DataEngine.h"
#include "DecisionTree.h"
using namespace std;
std::string tableAttrib = "create table dataTable (age INTEGER, workclass TEXT, fnlwgt INTEGER,"
" education TEXT, education_num INTEGER, marital_status TEXT,"
" occupation TEXT, relationship TEXT, race TEXT, sex TEXT,"
" capital_gain INTEGER, capital_loss INTEGER,"
" hours_per_week INTEGER, native_country TEXT, result TEXT)";

std::vector<std::string> contAttributes;
std::vector<std::string> v;
typedef std::pair<std::string, std::string> ItemPair;

std::string dataPath = "../adult.data";
std::string testDataPath = "../adult.data";
string positiveString = ">50K";
string negativeString = "<=50K";

std::vector<std::string> split(const std::string &text, std::string sep)
{
    std::vector<std::string> tokens;
    std::size_t start = 0, end = 0;
    while ((end = text.find(sep, start)) != std::string::npos) {
        tokens.push_back(text.substr(start, end - start));
        start = end + sep.length();
    }
    tokens.push_back(text.substr(start));
    return tokens;
}

float getAccuracy(DecisionTree::DecisionTreeNode* rootNode)
{
    DecisionTree decisionTree(v, contAttributes, dataPath, tableAttrib, "result", "<=50K", ">50K", false);
    decisionTree.myRoot = *rootNode;
    std::ifstream input(testDataPath);
    std::vector<std::string> result;
    std::string resultString;
    std::string line;

    float positive = 0;
    float negative = 0;
    float true_positive = 0;
    float true_negative = 0;
    float false_positive = 0;
    float false_negative = 0;

    if (input.is_open()) {
        while (getline(input, line)) {
            if (line.find('?') != std::string::npos) { //ignore missing for now
                continue;
            }
            result = split(line, ", ");
            vector<ItemPair> instanceList;

            instanceList.push_back(make_pair("age", result[0]));
            instanceList.push_back(make_pair("workclass", "'" +result[1]+"'"));
            instanceList.push_back(make_pair("education", "'"+ result[3] + "'"));
            instanceList.push_back(make_pair("marital_status", "'"+result[5]+"'"));
            instanceList.push_back(make_pair("occupation", "'"+result[6]+"'"));
            instanceList.push_back(make_pair("relationship", "'"+result[7]+"'"));
            instanceList.push_back(make_pair("race", "'"+result[8]+"'"));
            instanceList.push_back(make_pair("sex", "'"+result[9]+"'"));
            instanceList.push_back(make_pair("capital_gain", result[10]));
            instanceList.push_back(make_pair("capital_loss", result[11]));
            instanceList.push_back(make_pair("hours_per_week", result[12]));
            instanceList.push_back(make_pair("native_country", "'"+result[13]+"'"));
            std::string resultVal = result[14];
            string instanceResult = decisionTree.evaluateInstance(&decisionTree.myRoot, instanceList);
            if (resultVal == positiveString && instanceResult == positiveString) {
                positive++;
                true_positive++;
            } else if (resultVal == positiveString && instanceResult == negativeString) {
                positive++;
                false_negative++;
            } else if (resultVal == negativeString && instanceResult == negativeString) {
                negative++;
                true_negative++;
            } else if (resultVal == negativeString && instanceResult == positiveString) {
                negative++;
                false_positive++;
            }
        }
    }
    float accuracy = (true_positive+true_negative) / (true_positive + false_negative + false_positive + true_negative);
    float precision = true_positive / (true_positive + false_positive);
    float recall = true_positive / positive;
    float f1_score = 2 * (precision*recall) / (precision+recall);
    return accuracy;
}

void pruneRules(DecisionTree::DecisionTreeNode* node, std::vector<ItemPair> nodeContext, DecisionTree &dummyTree)
{
    if (node->type == DecisionTree::NodeType::RootNode) {
        for (auto it = node->children.begin(); it != node->children.end(); it++) {
            pruneRules(*it, nodeContext, dummyTree);
        }
    }
    if (node->children.empty()) {
        return;
    }
    nodeContext.push_back(node->attributePair);
    if (node->type == DecisionTree::NodeType::AttributeNode) {
        for (auto it = node->children.begin(); it != node->children.end(); it++) {
            float origAccuracy = getAccuracy(&dummyTree.myRoot);
            std::string mostCommonResult = dummyTree.getMostCommonResult(nodeContext); //fill this in
            std::vector<DecisionTree::DecisionTreeNode*> temp = node->children;

            DecisionTree::DecisionTreeNode* terminalNode = new DecisionTree::DecisionTreeNode();
            terminalNode->attributePair = std::make_pair("result", "'" + mostCommonResult + "'");
            terminalNode->type = DecisionTree::NodeType::TerminalNode;
            std::vector<DecisionTree::DecisionTreeNode*> terminalVector;
            terminalVector.push_back(terminalNode);

            node->children = terminalVector;
            float newAccuracy = getAccuracy(&dummyTree.myRoot);
            if (newAccuracy > origAccuracy) {

                continue;
            }
            node->children = temp;
            pruneRules(*it, nodeContext, dummyTree);
        }
    }
    if (node->type == DecisionTree::NodeType::TerminalNode) {
        return;
    }
}

int main()
{
    v.push_back("age");
    v.push_back("workclass");
    v.push_back("education");
    v.push_back("marital_status");
    v.push_back("occupation");
    v.push_back("relationship");
    v.push_back("race");
    v.push_back("sex");
    v.push_back("capital_gain");
    v.push_back("capital_loss");
    // v.push_back("fnlwgt");
    v.push_back("hours_per_week");
    v.push_back("native_country");

    contAttributes.push_back("age");
    contAttributes.push_back("fnlwgt");
    contAttributes.push_back("capital_gain");
    contAttributes.push_back("capital_loss");
    contAttributes.push_back("hours_per_week");

    DecisionTree decisionTree(v, contAttributes, dataPath, tableAttrib, "result", "<=50K", ">50K", false);
    ifstream input("rule_base");
    queue<ItemPair> ruleQueue;
    string line;
    int i =0;


    for (; getline(input, line);) {
        if (line == "RULE: ") {
            decisionTree.buildTreeFromRule(&decisionTree.myRoot, ruleQueue);
            while(!ruleQueue.empty()) {
                ruleQueue.pop();
            }
            continue;
        }
        istringstream iss(line);
        string attribute, attributeValue;
        iss >> attribute >> attributeValue;
        ItemPair tempPair;
        tempPair = make_pair(attribute, attributeValue);
        ruleQueue.push(tempPair);
    }
    // cout << getAccuracy(&decisionTree.myRoot) <<endl;
    std::vector<ItemPair> nodeContext;
    pruneRules(&decisionTree.myRoot, nodeContext, decisionTree);
    decisionTree.traverseTree(&decisionTree.myRoot, "");
    return 0;
}
