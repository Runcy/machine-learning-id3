// #include "DataEngine.h"
#include "DecisionTree.h"
#include <fstream>
// std::string tableAttrib = "create table dataTable (age INTEGER, workclass TEXT, fnlwgt INTEGER,"
// " education TEXT, education_num INTEGER, marital_status TEXT,"
// " occupation TEXT, relationship TEXT, race TEXT, sex TEXT,"
// " capital_gain INTEGER, capital_loss INTEGER,"
// " hours_per_week INTEGER, native_country TEXT, result TEXT)";
std::vector<std::string> contAttrib;
std::string tableAttrib = "create table dataTable (outlook TEXT, temperature TEXT, humidity TEXT, wind TEXT, playtennis TEXT)";

// std::string dataPath = "../adult.data";

std::string dataPath = "../playtennis.csv";

typedef std::pair<std::string, std::string> ItemPair;
using namespace std;
int main()
{
    std::vector<std::string> v;
    v.push_back("outlook");
    v.push_back("temperature");
    v.push_back("humidity");
    v.push_back("wind");

    DecisionTree decisionTree(v, contAttrib, dataPath, tableAttrib, "playtennis", "yes", "no");

    ifstream input("tennis_rules");
    queue<ItemPair> ruleQueue;
    string line;
    for (; getline(input, line);) {
        // std::cout <<line <<std::endl;
        if (line == "RULE:") {
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
    // decisionTree.traverseTree(&decisionTree.myRoot, "");
    vector<ItemPair> instanceList;
    instanceList.push_back(make_pair("outlook", "'overcast'"));
    instanceList.push_back(make_pair("wind", "'strong'"));
    std::cout << "RESULT: " << decisionTree.evaluateInstance(&decisionTree.myRoot, instanceList);
    return 0;
}
