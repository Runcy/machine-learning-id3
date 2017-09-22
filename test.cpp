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
    DecisionTree decisionTree(v, contAttributes, dataPath, tableAttrib, "result", "<=50K", ">50K");

    decisionTree.traverseTree(&decisionTree.rootNode, "");
    return 0;
}
