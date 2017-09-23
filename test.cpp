// #include "DataEngine.h"
#include "DecisionTree.h"
std::string tableAttrib = "create table dataTable (age INTEGER, workclass TEXT, fnlwgt INTEGER,"
" education TEXT, education_num INTEGER, marital_status TEXT,"
" occupation TEXT, relationship TEXT, race TEXT, sex TEXT,"
" capital_gain INTEGER, capital_loss INTEGER,"
" hours_per_week INTEGER, native_country TEXT, result TEXT)";
std::vector<std::string> contAttributes;
// std::string tableAttrib = "create table dataTable (outlook TEXT, temperature TEXT, humidity TEXT, wind TEXT, playtennis TEXT)";

std::string dataPath = "../adult.data";

// std::string dataPath = "../playtennis.csv";

typedef std::pair<std::string, std::string> ItemPair;
using namespace std;
int main()
{
    std::vector<std::string> v;
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
    v.push_back("hours_per_week");
    v.push_back("native_country");

    contAttributes.push_back("age");
    contAttributes.push_back("fnlwgt");
    contAttributes.push_back("capital_gain");
    contAttributes.push_back("capital_loss");
    contAttributes.push_back("hours_per_week");
    // DecisionTree decisionTree(v, contAttributes, dataPath, tableAttrib, "result", "<=50K", ">50K");
    DecisionTree decisionTree(v, contAttributes, dataPath, tableAttrib, "result", "<=50K", ">50K", false);

    ifstream input("rule_base");
    queue<ItemPair> ruleQueue;
    string line;
    int i =0;
    for (; getline(input, line);) {
        // std::cout <<line <<std::endl;
        if (line == "RULE: ") {
            std::cout << i++ << std::endl;
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

    decisionTree.traverseTree(&decisionTree.myRoot, "");
    return 0;
}
