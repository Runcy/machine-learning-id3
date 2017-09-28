// Class for handling all the database queries which come up in the construction of a
// DecisionTree. Works by creating an SQLite database in RAM. It uses inline functions
// for making queries to the databse.

#ifndef DATAENGINE_H
#define DATAENGINE_H

#include <stdio.h>
#include <queue>
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
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

        if (input.is_open()) {
            while (getline(input, line)) {
                if (line.find('?') != std::string::npos || line.size() <= 1) {
                    continue;
                }
                result = split(line, ", ");
                resultString = prepareSqlString(result);
                try {
                    SQLite::Transaction transaction(*db);
                    db->exec("insert into " + tableName + " values(" + resultString + ")");
                    transaction.commit();
                } catch (std::exception& e) {
                    std::cout << "exception: " << e.what() << std::endl;
                }
            }
        }
        input.close();
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
            std::cout << e.what() << std::endl;
        }
        populateDatabase();
    }

    ~DataEngine()
    {
        delete db;
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
        SQLite::Statement query(*db, sqlString);
        if (query.executeStep()) {
            int val = query.getColumn(0).getInt();
            return val;
        }
        return 0;
    }

    int getCount(std::string whereString)
    {
        std::string sqlString = "select count(*) from " + tableName + " where " + whereString;
        SQLite::Statement query(*db, sqlString);
        if (query.executeStep()) {
            int val = query.getColumn(0).getInt();
            return val;
        }
        return 0;
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

    int checkUnique(std::string queryString)
    {
        int i = 0;
        std::string sqlString = "select distinct " + resultString + " from " + tableName + " where " + queryString;
        SQLite::Statement query(*db, sqlString);
        while (query.executeStep()) {
            i++;
        }
        if (i == 0) {
            return -1;
        }
        if (i == 1) {
            return 1;
        }
        return 0;
    }

    std::string getResultString(std::string queryString)
    {
        std::string sqlString = "select distinct " + resultString + " from " + tableName + " where " + queryString;
        SQLite::Statement query(*db, sqlString);

        if (query.executeStep()) {
            return query.getColumn(0).getString();
        }
        return "";
    }

    void getDistinctAttributeValues(std::vector<std::string> &valueList, std::string attribute)
    {
        std::string sqlQuery = "select distinct " + attribute + " from " + tableName;
        SQLite::Statement query(*db, sqlQuery);
        while (query.executeStep()) {
            valueList.push_back(query.getColumn(0));
        }
    }

    void getContinuousAttributeValues(std::vector<int> &contList, std::string attribute, std::string queryString)
    {
        std::string sqlQuery = "select distinct " + attribute + " from " + tableName + " where " + queryString;
        SQLite::Statement query(*db, sqlQuery);
        try {
            while (query.executeStep()) {
                contList.push_back(query.getColumn(0).getInt());
            }
        } catch (std::exception& e) {
            std::cout << "exception!" << e.what();
        }
    }
};

#endif
