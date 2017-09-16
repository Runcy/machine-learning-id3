#include "DataEngine.h"

int main()
{
    std::string tableAttrib = "create table census (age INTEGER, workclass TEXT, fnlwgt INTEGER,"
                " education TEXT, education_num INTEGER, marital_status TEXT,"
                " occupation TEXT, relationship TEXT, race TEXT, sex TEXT,"
                " capital_gain INTEGER, capital_loss INTEGER,"
                " hours_per_week INTEGER, native_country TEXT, result TEXT)";
    std::string dataPath = "../adult.data";

    DataEngine dataEngine(dataPath, tableAttrib);
    std::cout << dataEngine.getCount("education=\"Bachelors\" and age > 50") << std::endl;
    std::cout << dataEngine.getCount("education=\"Bachelors\" and age > 50"" and result = '<=50K'") << std::endl;
    std::cout << dataEngine.getCount("education=\"Bachelors\" and age > 50"" and result = '>50K'") << std::endl;

    std::cout << dataEngine.getProbability("education=\"Bachelors\" and age > 50", '+');
    std::vector<std::string> data;
    dataEngine.getDistinctAttributeValues(data, "occupation");
    for (auto it = data.begin(); it != data.end(); it++) {
        // std::cout << *it << std::endl;
    }

    std::vector<std::pair<std::string, std::string>> contextString;
    std::vector<std::string> attributesList;
    // dataEngine.getDistinctAttributeValues(attributesList, attribute);

    contextString.push_back(std::make_pair("occupation", "'Craft-repair'"));
    contextString.push_back(std::make_pair("education", "'Bachelors'"));


    std::string contextQueryString = contextString.begin()->first + " = " + contextString.begin()->second;
    for (auto it = contextString.begin() + 1; it != contextString.end(); it++) {
        contextQueryString += " and " + it->first + " = " + it->second;
    }
    std::cout << contextQueryString << std::endl;
    std::cout << dataEngine.getCount(contextQueryString);

    return 0;
}
