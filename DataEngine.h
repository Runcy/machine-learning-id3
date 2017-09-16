#ifndef DATAENGINE_H
#define DATAENGINE_H

#include <stdio.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

class DataEngine
{
private:
    const std::string trainingDataPath;
    const std::string tableAttributes;
    const std::string tableName;
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
    }

public:
    DataEngine(const std::string _trainingDataPath, const std::string _tableAttributes, const std::string _tableName = "census") : trainingDataPath(_trainingDataPath), tableAttributes(_tableAttributes), tableName(_tableName)
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

    int getCount(std::string whereString)
    {
        std::string sqlString = "select count(*) from " + tableName + " where " + whereString;
        SQLite::Statement query(*db, sqlString);
        query.executeStep();
        return query.getColumn(0).getInt();
    }

    float getProbability(std::string queryString, char value) //make this more general
    {
        std::string probString = queryString;
        if (value == '+') {
            probString += " and result = '>50K'";
        } else {
            probString += " and result = '<=50K'";
        }
        float count = getCount(probString);
        float totalCount = getCount(queryString);

        return count / totalCount;
    }

    void getDistinctAttributeValues(std::vector<std::string> &valueList, std::string attribute)
    {
        std::string sqlQuery = "select distinct " + attribute + " from " + tableName;
        SQLite::Statement query(*db, sqlQuery);
        while (query.executeStep()) {
            valueList.push_back(query.getColumn(0));
            // std::cout << query.getColumn(0) << std::endl;
        }
    }
};

#endif
