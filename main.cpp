#include "DataEngine.h"
// #include "DecisionTree.h"
std::string tableAttrib = "create table dataTable (age INTEGER, workclass TEXT, fnlwgt INTEGER,"
" education TEXT, education_num INTEGER, marital_status TEXT,"
" occupation TEXT, relationship TEXT, race TEXT, sex TEXT,"
" capital_gain INTEGER, capital_loss INTEGER,"
" hours_per_week INTEGER, native_country TEXT, result TEXT)";

// std::string tableAttrib = "create table dataTable (outlook TEXT, temperature TEXT, humidity TEXT, wind TEXT, playtennis TEXT)";

std::string dataPath = "../adult.data";

// std::string dataPath = "../playtennis.csv";


DataEngine dataEngine(dataPath, tableAttrib, "result", "<=50K", ">50K");
// DataEngine dataEngine(dataPath, tableAttrib, "playtennis", "yes", "no");

typedef std::pair<std::string, std::string> ItemPair;


std::string prepareContextString(std::vector<ItemPair> contextString)
{
    if (contextString.empty()) {
        return "";
    }

    std::string contextQueryString = contextString.begin()->first + " = " + contextString.begin()->second;
    for (auto it = contextString.begin() + 1; it != contextString.end(); it++) {
        contextQueryString += " and " + it->first + " = " + it->second;
    }
    return contextQueryString;
}

float getEntropyGain(std::vector<ItemPair> contextString, std::string attribute)
{
    std::vector<std::string> attributesList;
    dataEngine.getDistinctAttributeValues(attributesList, attribute);
    int contextCount;

    std::string contextQueryString = "";
    if (!contextString.empty()) {
        contextQueryString = contextString.begin()->first + " = " + contextString.begin()->second;
        for (auto it = contextString.begin() + 1; it != contextString.end(); it++) {
            contextQueryString += " and " + it->first + " = " + it->second;
        }
        // std::cout << contextQueryString << std::endl;
        contextCount = dataEngine.getCount(contextQueryString);
    } else {
        contextCount = dataEngine.getAllCount();
    }

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
        if (contextString.empty()) {
            attributeQueryString += attribute + " = '" + *it + "'";
        } else {
            attributeQueryString += " and " + attribute + " = '" + *it + "'";
        }
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
std::string prepareQueryString(std::vector<ItemPair> contextString)
{
    if (!contextString.empty()) {
        std::string contextQueryString = contextString.begin()->first + " = " + contextString.begin()->second;
        for (auto it = contextString.begin() + 1; it != contextString.end(); it++) {
            contextQueryString += " and " + it->first + " = " + it->second;
        }
        return contextQueryString;
    }
    return "";
}

float getContinuousEntropyGain(std::vector<ItemPair> contextString, std::string attribute)
{
    std::vector<int> contValuesPositive;
    std::vector<int> contValuesNegative;

    std::vector<float> boundaryValues;
// do +ve/-ve
    std::string queryStringPositive = prepareQueryString(contextString);
    queryStringPositive += "result = '>50K'";
    dataEngine.getContinuousAttributeValues(contValuesPositive, attribute, queryStringPositive);

    std::string queryStringNegative = prepareQueryString(contextString);
    queryStringNegative += "result = '<=50K'";
    dataEngine.getContinuousAttributeValues(contValuesNegative, attribute, queryStringNegative);

    std::sort(contValuesPositive.begin(), contValuesPositive.end());
    std::sort(contValuesNegative.begin(), contValuesNegative.end());

    auto posItr = contValuesPositive.begin();
    auto negItr = contValuesNegative.begin();
    float boundaryValue;
    // like mergesort
    while(posItr < contValuesPositive.end() && negItr < contValuesNegative.end()) {
        if (*posItr <= *negItr) {
            boundaryValue = (*posItr+*negItr)/2.0;
            boundaryValues.push_back(boundaryValue);
            *posItr++;
        } else {
            boundaryValue = (*posItr+*negItr)/2.0;
            boundaryValues.push_back(boundaryValue);
            *negItr++;
        }
    }
    std::string attributeQueryString;
    std::string contextQueryString = "";
    int contextCount;
    int attributeCount;
    float instanceProbability;
    float positiveProbability;
    float negativeProbability;
    float positiveEntropy;
    float negativeEntropy;
    float entropy;
    if (!contextString.empty()) {
        contextQueryString = contextString.begin()->first + " = " + contextString.begin()->second;
        for (auto it = contextString.begin() + 1; it != contextString.end(); it++) {
            contextQueryString += " and " + it->first + " = " + it->second;
        }
        // std::cout << "qs " << contextQueryString << std::endl;
        contextCount = dataEngine.getCount(contextQueryString);
    } else {
        contextCount = dataEngine.getAllCount();
    }
    // std::cout << contextCount;

    float maxEntropy = -1;
    float bestVal = 0;
    float totalEntropyGain;
    for (auto it = boundaryValues.begin(); it != boundaryValues.end(); it++) {
        totalEntropyGain = 0;

        attributeQueryString = prepareContextString(contextString);
        if (contextString.empty()) {
            attributeQueryString += attribute + " > " + std::to_string(*it);
        } else {
            attributeQueryString += " and " + attribute + " > " + std::to_string(*it);
        }
        // std::cout << attributeQueryString;
        attributeCount = dataEngine.getCount(attributeQueryString);
        std::cout << "AF" << attributeCount << ' ' << attributeQueryString << std::endl;
        instanceProbability = (float) attributeCount / contextCount;

        positiveProbability = dataEngine.getProbability(attributeQueryString, '+');
        negativeProbability = dataEngine.getProbability(attributeQueryString, '-');

        positiveEntropy = (positiveProbability == 0) ? 0 : positiveProbability*log2(positiveProbability);
        negativeEntropy = (negativeProbability == 0) ? 0 : negativeProbability*log2(negativeProbability);

        // positiveEntropy = positiveProbability*log2(positiveProbability);
        // negativeEntropy = negativeProbability*log2(negativeProbability);
        entropy = positiveEntropy + negativeEntropy;

        std::cout << *it << "GRAT" << instanceProbability << std::endl;
        totalEntropyGain += entropy*instanceProbability;

        attributeQueryString = prepareContextString(contextString);
        if (contextString.empty()) {
            attributeQueryString += attribute + " <= " + std::to_string(*it);
        } else {
            attributeQueryString += " and " + attribute + " <= " + std::to_string(*it);
        }
        std::cout << *it << std::endl;
        // std::cout << attributeQueryString;
        attributeCount = dataEngine.getCount(attributeQueryString);
        // std::cout << attributeCount << std::endl;
        instanceProbability = (float) attributeCount / contextCount;

        positiveProbability = dataEngine.getProbability(attributeQueryString, '+');
        negativeProbability = dataEngine.getProbability(attributeQueryString, '-');

        positiveEntropy = (positiveProbability == 0) ? 0 : positiveProbability*log2(positiveProbability);
        negativeEntropy = (negativeProbability == 0) ? 0 : negativeProbability*log2(negativeProbability);
        entropy = positiveEntropy + negativeEntropy;
        totalEntropyGain += entropy*instanceProbability;

        std::cout << *it << ' ' << totalEntropyGain << std::endl;
        if (totalEntropyGain > maxEntropy) {
            maxEntropy = totalEntropyGain;
            bestVal = *it;
        }
    }
    std::cout << "BEST" << bestVal << ' ' << "MAX" << maxEntropy;
}

int main()
{
    //
    // std::cout << dataEngine.getCount("education=\"Bachelors\" and age > 50") << std::endl;
    // std::cout << dataEngine.getCount("education=\"Bachelors\" and age > 50"" and result = '<=50K'") << std::endl;
    // std::cout << dataEngine.getCount("education=\"Bachelors\" and age > 50"" and result = '>50K'") << std::endl;

    // std::cout << dataEngine.getProbability("education=\"Bachelors\" and age > 50", '+');
    std::vector<std::string> data;
    // dataEngine.getDistinctAttributeValues(data, "occupation");
    for (auto it = data.begin(); it != data.end(); it++) {
        // std::cout << *it << std::endl;
    }

    std::vector<std::pair<std::string, std::string>> contextString;

    std::vector<std::string> attributesList;
    // dataEngine.getDistinctAttributeValues(attributesList, attribute);

    // contextString.push_back(std::make_pair("occupation", "'Craft-repair'"));
    // contextString.push_back(std::make_pair("education", "Bachelors"));

    // contextString.push_back(std::make_pair("outlook", "'rain'"));
    // contextString.push_back(std::make_pair("wind", "'strong'"));

    // for (int i = 0; i < 100; i++) {
    // std::cout << dataEngine.checkUnique(prepareContextString(contextString));

        // std::cout << getEntropyGain(contextString, "temperature") << std::endl;
        // std::cout << getEntropyGain(contextString, "outlook") << std::endl;
        // std::cout << getEntropyGain(contextString, "humidity") << std::endl;

    // }
    // std::string contextQueryString = contextString.begin()->first + " = " + contextString.begin()->second;
    // for (auto it = contextString.begin() + 1; it != contextString.end(); it++) {
    //     contextQueryString += " and " + it->first + " = " + it->second;
    // }
    // std::cout << contextQueryString << std::endl;
    // std::cout << dataEngine.getCount(contextQueryString);
getContinuousEntropyGain(contextString, "capital_loss");
    return 0;
}
