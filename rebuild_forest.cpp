#include "DecisionTree.h"
using namespace std;
std::string tableAttrib = "create table dataTable (age INTEGER, workclass TEXT, fnlwgt INTEGER,"
" education TEXT, education_num INTEGER, marital_status TEXT,"
" occupation TEXT, relationship TEXT, race TEXT, sex TEXT,"
" capital_gain INTEGER, capital_loss INTEGER,"
" hours_per_week INTEGER, native_country TEXT, result TEXT)";


typedef std::pair<std::string, std::string> ItemPair;

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
std::string testDataPath = "../adult.test";
string positiveString = ">50K";
string negativeString = "<=50K";

void parseData(DecisionTree &decisionTree)
{
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
            std::string resultVal = result[14];
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

int main()
{
    const string randomForestRuleBase = "../random_rule_bases/random_rule_base";
    const string randomForestFilePath = "../random_forest/adultTraining";

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
        string dataPath = randomForestFilePath+to_string(i);
        ifstream input(filePath);
        // DecisionTree decisionTree(v, contAttributes, dataPath, tableAttrib, "result", "<=50K", ">50K", false);
        DecisionTree* decisionTree = new DecisionTree(v, contAttributes, dataPath, tableAttrib, "result", "<=50K", ">50K", false);

        for (; getline(input, line);) {
            // std::cout <<line <<std::endl;
            if (line == "RULE: ") {
                // std::cout << i++ << std::endl;
                decisionTree->buildTreeFromRule(&(decisionTree->myRoot), ruleQueue);
                // decisionTree.buildTreeFromRule(&decisionTree.myRoot, ruleQueue);

                while(!ruleQueue.empty()) {
                    ruleQueue.pop();
                }
                continue;
            }
            if (line.length() <= 1) {
                continue;
            }
            istringstream iss(line);
            string attribute, attributeValue;
            iss >> attribute >> attributeValue;
            ItemPair tempPair;
            tempPair = make_pair(attribute, attributeValue);
            ruleQueue.push(tempPair);
        }
        forest.push_back(decisionTree);
        cout << "Tree built " << i << endl;
    }

    // for (auto it = forest.begin(); it != forest.end(); it++) {
    //     DecisionTree::traverseTree(&((*it)->myRoot), "");
    // }
    DecisionTree* f = forest[0];
    DecisionTree tmp = &f;
    parseData(tmp);
    return 0;
}
