// #include "DataEngine.h"
#include "DecisionTree.h"
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

int main()
{
    std::vector<std::string> v;
    v.push_back("outlook");
    v.push_back("temperature");
    v.push_back("humidity");
    v.push_back("wind");

    DecisionTree decisionTree(v, dataPath, tableAttrib, "playtennis", "yes", "no");

    // decisionTree.traverseTree(&decisionTree.rootNode);
    return 0;
}
