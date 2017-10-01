// This works by taking a rule base, that is the list of deduced rules from a file
// and building a DecisionTree from the deduced rules. The first argument passed to
// the program is used as the path to the file containing the list of rules.
//
// If the last argument if the constructor of the DecisionTree is set to ```false```,
// the tree is built from the rules defined in the rule base. If it is set to ```true```,
// the tree is built by training it from the training instances. This can be upto
// 100 times slower than just building the tree from the file.
//
// The properties of the DecisionTree, such as the schema of the SQL database,
// the list of discrete and continuous attributes, and the path of the training
// and test data is specified in this file.
//
// The pre-built ```rule_base``` file contains the list of the rules deduced from
// the standard ID3 algorithm.

#include "DecisionTree.h"
using namespace std;
string tableAttrib = "create table dataTable (age INTEGER, workclass TEXT, fnlwgt INTEGER,"
" education TEXT, education_num INTEGER, marital_status TEXT,"
" occupation TEXT, relationship TEXT, race TEXT, sex TEXT,"
" capital_gain INTEGER, capital_loss INTEGER,"
" hours_per_week INTEGER, native_country TEXT, result TEXT)";
vector<string> contAttributes;

string dataPath = "../adult.data";

vector<string> split(const string &text, string sep)
{
    vector<string> tokens;
    size_t start = 0, end = 0;
    while ((end = text.find(sep, start)) != string::npos) {
        tokens.push_back(text.substr(start, end - start));
        start = end + sep.length();
    }
    tokens.push_back(text.substr(start));
    return tokens;
}

string testDataPath = "../adult.test";
string positiveString = ">50K";
string negativeString = "<=50K";

void parseData(DecisionTree &decisionTree)
{
    ifstream input(testDataPath);
    vector<string> result;
    string resultString;
    string line;

    float positive = 0;
    float negative = 0;
    float true_positive = 0;
    float true_negative = 0;
    float false_positive = 0;
    float false_negative = 0;

    if (input.is_open()) {
        while (getline(input, line)) {
            result = split(line, ", ");
            // cout << line <<endl;
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
            string resultVal = result[14];
            // resultVal.pop_back();
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

    cout << "Accuracy: " << accuracy << endl;
    cout << "Precision: " << precision << endl;
    cout << "Recall: " << recall << endl;
    cout << "F1 Score: " << f1_score << endl;
}


typedef pair<string, string> ItemPair;
int main(int argc, char** argv)
{
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
    // v.push_back("fnlwgt");
    v.push_back("hours_per_week");
    v.push_back("native_country");

    contAttributes.push_back("age");
    contAttributes.push_back("fnlwgt");
    contAttributes.push_back("capital_gain");
    contAttributes.push_back("capital_loss");
    contAttributes.push_back("hours_per_week");

    DecisionTree decisionTree(v, contAttributes, dataPath, tableAttrib, "result", "<=50K", ">50K", false);
    DecisionTree::traverseTree(&decisionTree.rootNode, "");

    ifstream input;
    if (argc > 1) {
        input.open(argv[1]);
    } else {
        input.open("../rule_bases/rule_base");
    }
    queue<ItemPair> ruleQueue;
    string line;

    for (; getline(input, line);) {
        if (line == "RULE: ") {
            // cout << i++ << endl;
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

    cout << "Tree built!" << endl;
    parseData(decisionTree);
    return 0;
}
