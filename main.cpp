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


    return 0;
}
