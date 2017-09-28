// Creates the decision trees from the adultTraining* files. These decision trees are
// then linearized to a set of rules outputted in the random_rule_bases/ subdirectory

#include "DecisionTree.h"
using namespace std;
string tableAttrib = "create table dataTable (age INTEGER, workclass TEXT, fnlwgt INTEGER,"
" education TEXT, education_num INTEGER, marital_status TEXT,"
" occupation TEXT, relationship TEXT, race TEXT, sex TEXT,"
" capital_gain INTEGER, capital_loss INTEGER,"
" hours_per_week INTEGER, native_country TEXT, result TEXT)";


typedef pair<string, string> ItemPair;

int main()
{
    const string randomForestRuleBase = "../random_rule_bases/random_rule_base";
    const string randomForestFilePath = "../random_forest/adultTraining";

    vector<string> contAttributes;
    vector<string> v;
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

    for (int i = 0; i < 128; i++) {
        ofstream output(randomForestRuleBase+to_string(i));
        DecisionTree decisionTree(v, contAttributes, randomForestFilePath+to_string(i), tableAttrib, "result", "<=50K", ">50K", true);
        cout << "Tree built " << i << endl;
        decisionTree.traverseTree(&decisionTree.rootNode, "", output);
        cout << "Tree written" << endl;
        output.close();
    }
    return 0;
}
