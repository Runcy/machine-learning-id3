#include "DecisionTree.h"
using namespace std;
std::string tableAttrib = "create table dataTable (age INTEGER, workclass TEXT, fnlwgt INTEGER,"
" education TEXT, education_num INTEGER, marital_status TEXT,"
" occupation TEXT, relationship TEXT, race TEXT, sex TEXT,"
" capital_gain INTEGER, capital_loss INTEGER,"
" hours_per_week INTEGER, native_country TEXT, result TEXT)";


typedef std::pair<std::string, std::string> ItemPair;
int main()
{
    const string randomForestRuleBase = "../random_rule_bases/random_rule_base";

    std::vector<std::string> contAttributes;
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
    vector<DecisionTree*> forest;

    for (int i = 0; i < 128; i++) {
        queue<ItemPair> ruleQueue;
        string line;
        string filePath = randomForestRuleBase+to_string(i);
        ifstream input(filePath);
        DecisionTree decisionTree(v, contAttributes, filePath, tableAttrib, "result", "<=50K", ">50K", false);
        for (; getline(input, line);) {
            // std::cout <<line <<std::endl;
            if (line == "RULE: ") {
                std::cout << "RULELINE\n";
                // std::cout << i++ << std::endl;
                // decisionTree->buildTreeFromRule(&(decisionTree->myRoot), ruleQueue);
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
        // forest.push_back(decisionTree);
        cout << "Tree built " << i << endl;
    }
    return 0;
}
