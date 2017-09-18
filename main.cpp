#include "DataEngine.h"

std::string tableAttrib = "create table dataTable (age INTEGER, workclass TEXT, fnlwgt INTEGER,"
" education TEXT, education_num INTEGER, marital_status TEXT,"
" occupation TEXT, relationship TEXT, race TEXT, sex TEXT,"
" capital_gain INTEGER, capital_loss INTEGER,"
" hours_per_week INTEGER, native_country TEXT, result TEXT)";
std::string dataPath = "../adult.data";
typedef std::pair<std::string, std::string> ItemPair;

DataEngine dataEngine(dataPath, tableAttrib, "result", "<=50K", ">50K");

float getEntropyGain(std::vector<ItemPair> contextString, std::string attribute)
{
    std::vector<std::string> attributesList;
    dataEngine.getDistinctAttributeValues(attributesList, attribute);

    std::string contextQueryString = contextString.begin()->first + " = " + contextString.begin()->second;
    for (auto it = contextString.begin() + 1; it != contextString.end(); it++) {
        contextQueryString += " and " + it->first + " = '" + it->second + "'";
    }
    // std::cout << contextQueryString << std::endl;
    int contextCount = dataEngine.getCount(contextQueryString);
    int attributeCount;
    float instanceProbability;
    float positiveProbability;
    float negativeProbability;
    float positiveEntropy;
    float negativeEntropy;
    float entropy;
    float totalEntropyGain = 0;
    // now we have to find number of each value of the chosen attribute
    std::string attributeQueryString;
    for (auto it = attributesList.begin(); it != attributesList.end(); it++) {
        attributeQueryString = contextQueryString;
        attributeQueryString += " and " + attribute + " = '" + *it + "'";
        // std::cout << attributeQueryString << std::endl;
        attributeCount = dataEngine.getCount(attributeQueryString);

        instanceProbability = (float) attributeCount / contextCount;

        positiveProbability = dataEngine.getProbability(attributeQueryString, '+');
        negativeProbability = dataEngine.getProbability(attributeQueryString, '-');

        positiveEntropy = (positiveProbability == 0) ? 0 : positiveProbability*log2(positiveProbability);
        negativeEntropy = (negativeProbability == 0) ? 0 : negativeProbability*log2(negativeProbability);

        // positiveEntropy = positiveProbability*log2(positiveProbability);
        // negativeEntropy = negativeProbability*log2(negativeProbability);

        entropy = positiveEntropy + negativeEntropy;

        totalEntropyGain += instanceProbability*entropy;
    }
    return totalEntropyGain;
    // then computing entropy gain is not that hard
}

int main()
{
    //
    // std::cout << dataEngine.getCount("education=\"Bachelors\" and age > 50") << std::endl;
    // std::cout << dataEngine.getCount("education=\"Bachelors\" and age > 50"" and result = '<=50K'") << std::endl;
    // std::cout << dataEngine.getCount("education=\"Bachelors\" and age > 50"" and result = '>50K'") << std::endl;

    // std::cout << dataEngine.getProbability("education=\"Bachelors\" and age > 50", '+');
    std::vector<std::string> data;
    dataEngine.getDistinctAttributeValues(data, "occupation");
    for (auto it = data.begin(); it != data.end(); it++) {
        // std::cout << *it << std::endl;
    }

    std::vector<std::pair<std::string, std::string>> contextString;
    std::vector<std::string> attributesList;
    // dataEngine.getDistinctAttributeValues(attributesList, attribute);

    contextString.push_back(std::make_pair("occupation", "'Craft-repair'"));
    contextString.push_back(std::make_pair("education", "Bachelors"));
    // for (int i = 0; i < 100; i++) {
        std::cout << getEntropyGain(contextString, "sex");
    // }
    // std::string contextQueryString = contextString.begin()->first + " = " + contextString.begin()->second;
    // for (auto it = contextString.begin() + 1; it != contextString.end(); it++) {
    //     contextQueryString += " and " + it->first + " = " + it->second;
    // }
    // std::cout << contextQueryString << std::endl;
    // std::cout << dataEngine.getCount(contextQueryString);

    return 0;
}
