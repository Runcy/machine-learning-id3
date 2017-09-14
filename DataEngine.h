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
        SQLite::Database db;

        bool isNumber(const std::string s)
        {
          return s.find_first_not_of( "0123456789" ) == std::string::npos;
        }

        std::vector<std::string> split(const std::string &text, std::string sep) {
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
            std::ifstream input(_trainingDataPath);
            std::string result;
            std::string resultString;

            if (input.is_open()) {
                while (getline(input, line)) {
                    // std::cout << line << std::endl;
                    result = split(line, ", ");
                    resultString = prepareSqlString(result);
                    // std::cout << "insert into census (" + resultString + ")"
                    try {
                        SQLite::Transaction transaction(db);
                        db.exec("insert into census values(" + resultString + ")");
                        transaction.commit();
                    } catch (std::exception& e) {
                        std::cout << "exception: " << e.what() << std::endl;
                    }
                }
            }
        }

    public:
        DataEngine(const std::string _trainingDataPath, const std::string _tableAttributes) : trainingDataPath(_trainingDataPath), tableAttributes(_tableAttributes), db(":memory:",SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE)
        {
            try {
                SQLite::Transaction(db);
                db.exec(tableAttributes);
                transaction.commit();
            } catch (std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        }
}
