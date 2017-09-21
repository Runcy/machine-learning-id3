#ifndef DATAENGINE_H
#define DATAENGINE_H

#include <stdio.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
// #include <pair>
#include <algorithm>

class DataEngine
{
private:
    const std::string trainingDataPath;
    const std::string tableAttributes;
    const std::string tableName;
    const std::string resultString;
    const std::string positiveInstanceString;
    const std::string negativeInstanceString;
    SQLite::Database *db;

    bool isNumber(const std::string s)
    {
        return s.find_first_not_of( "0123456789" ) == std::string::npos;
    }

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

    std::string prepareSqlString(std::vector<std::string> v)
    {
        std::string resultString;
        for (auto it = v.begin(); it != v.end(); it++) {
            if (isNumber(*it)) {
                resultString += *it + ", ";
            } else {
                resultString += '"' + *it + '"' + ", ";
            }
        }
        // remove last 2 chars
        resultString.pop_back();
        resultString.pop_back();
        return resultString;
    }

    void populateDatabase()
    {
        std::ifstream input(trainingDataPath);
        std::vector<std::string> result;
        std::string resultString;
        std::string line;
        int i = 0;
        if (input.is_open()) {
            while (getline(input, line)) {
                if (line.find('?') != std::string::npos) { //ignore missing for now
                    continue;
                }
                result = split(line, ", ");
                resultString = prepareSqlString(result);
                // //std::cout << resultString;
                try {
                    SQLite::Transaction transaction(*db);
                    // //std::cout << "insert into " + tableName + " values(" + resultString + ")";
                    db->exec("insert into " + tableName + " values(" + resultString + ")");
                    transaction.commit();
                } catch (std::exception& e) {
                    //std::cout << "exception: " << e.what() << std::endl;
                }
            }
        }
    }

public:
    DataEngine(const std::string _trainingDataPath, const std::string _tableAttributes,
                const std::string _resultString, const std::string _positiveInstanceString, const std::string _negativeInstanceString) :
                    trainingDataPath(_trainingDataPath), tableAttributes(_tableAttributes), tableName("dataTable"), resultString(_resultString),
                    positiveInstanceString(_positiveInstanceString), negativeInstanceString(_negativeInstanceString)
    {
        db = new SQLite::Database(":memory:", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
        try {
            SQLite::Transaction transaction(*db);
            db->exec(tableAttributes);
            transaction.commit();
        } catch (std::exception &e) {
            //std::cout << e.what() << std::endl;
        }
        populateDatabase();
        // SQLite::Statement query(*db, "SELECT count(*) FROM census where education=\"Bachelors\""
    }

    void executeQuery(std::string sqlString, std::vector<std::vector<std::string>> &v)
    {
        SQLite::Statement query(*db, sqlString);
        while (query.executeStep()) {
            std::vector<std::string> result;
            int columns = query.getColumnCount();
            for (int i = 0; i < columns; i++) {
                result.push_back(query.getColumn(i).getText());
            }
            v.push_back(result);
        }
    }

    int getAllCount()
    {
        std::string sqlString = "select count(*) from " + tableName;
        // //std::cout << sqlString << std::endl;
        SQLite::Statement query(*db, sqlString);
        query.executeStep();
        int val = query.getColumn(0).getInt();
        //std::cout << sqlString << std::endl << val << std::endl;
        return val;
    }

    int getCount(std::string whereString)
    {
        std::string sqlString = "select count(*) from " + tableName + " where " + whereString;
        // //std::cout << sqlString << std::endl;
        SQLite::Statement query(*db, sqlString);
        query.executeStep();
        int val = query.getColumn(0).getInt();
        //std::cout << sqlString << std::endl << val << std::endl;
        return val;
    }

    float getProbability(std::string queryString, char value) //make this more general
    {
        std::string probString = queryString;
        if (value == '+') {
            probString += " and " + resultString + "= '" + positiveInstanceString + "'";
        } else {
            probString += " and " + resultString + "= '" + negativeInstanceString + "'";
        }
        float count = getCount(probString);
        float totalCount = getCount(queryString);

        if (totalCount == 0) { // to take care of 0 probs, idk if this is a best stragegy
            return 0;
        }

        return count / totalCount;
    }

    bool checkUnique(std::string queryString)
    {
        int i = 0;
        std::string sqlString = "select distinct " + resultString + " from " + tableName + " where " + queryString;
        SQLite::Statement query(*db, sqlString);
        while (query.executeStep()) {
            i++;
        }
        // return i;
        if (i > 1) {
            return false;
        }
        return true;
    }

    std::string getResultString(std::string queryString)
    {
        std::string sqlString = "select distinct " + resultString + " from " + tableName + " where " + queryString;
        SQLite::Statement query(*db, sqlString);
        query.executeStep();
        return query.getColumn(0).getString();
    }

    void getDistinctAttributeValues(std::vector<std::string> &valueList, std::string attribute)
    {
        std::string sqlQuery = "select distinct " + attribute + " from " + tableName;
        //std::cout << sqlQuery << std::endl;
        SQLite::Statement query(*db, sqlQuery);
        while (query.executeStep()) {
            valueList.push_back(query.getColumn(0));
            // //std::cout << query.getColumn(0) << std::endl;
        }
    }

    void getContinuousAttributeValues(std::vector<int> &contList, std::string attribute, std::string queryString)
    {
        std::string sqlQuery = "select distinct " + attribute + " from " + tableName + " where " + queryString;
        std::cout << sqlQuery << std::endl;
        SQLite::Statement query(*db, sqlQuery);
        try {
            while (query.executeStep()) {
                contList.push_back(query.getColumn(0).getInt());
                std::cout << query.getColumn(0) << std::endl;
            }
        } catch (std::exception& e) {
            std::cout << "exception!" << e.what();
        }
    }
};

#endif
