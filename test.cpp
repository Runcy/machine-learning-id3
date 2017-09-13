#include <stdio.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

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

bool has_only_digits(const std::string s)
{
  return s.find_first_not_of( "0123456789" ) == std::string::npos;
}

void prepareSqlString(std::vector<std::string> v)
{
    std::string resultString;
    for (auto it = v.begin(); it != v.end(); it++) {
        if (has_only_digits(*it)) {
            resultString += *it + ", ";
        } else {
            resultString += '"' + *it + '"' + ", ";
        }
    }
    // remove last 2 chars
    resultString.pop_back();
    resultString.pop_back();
    // std::cout << "RESULTx " << resultString << std::endl;
}

void printString(std::string s)
{
    std::cout << s << std::endl;
}

int main(int argc, char* argv[])
{
    std::string line;
    std::ifstream input("../adult.data");
    std::vector<std::string> result;
    SQLite::Database db(":memory:",SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
    try {
        // Open a database file

        SQLite::Transaction transaction(db);
        db.exec("create table census (age INTEGER, workclass TEXT, fnlwgt INTEGER,"
                " education TEXT, education_num INTEGER, marital_status TEXT,"
                " occupation TEXT, relationship TEXT, race TEXT, sex TEXT,"
                " capital_gain INTEGER, capital_loss INTEGER,"
                " hours_per_week INTEGER, native_country TEXT, result TEXT)");
        db.exec("insert into census values(50, \"Self-emp-not-inc\", 83311, \"Bachelors\", 13, \"Married-civ-spouse\", \"Exec-managerial\", \"Husband\", \"White\", \"Male\", 0, 0, 13, \"United-States\", \"<=50K\")");
        transaction.commit();

        // SQLite::Statement query(db, "SELECT * FROM census");
        //
        // while (query.executeStep()) {
        //     int columns = query.getColumnCount();
        //     for (int i = 0; i < columns; i++) {
        //         std::cout << query.getColumn(i) << ' ';
        //     }
        //     std::cout << std::endl;
        //     // int id = query.getColumn(0);
        //     // std::string value = query.getColumn(1);
        //     // std::cout << "row: " << id << ", " << value << std::endl;
        }
    } catch (std::exception& e) {
        std::cout << "exception: " << e.what() << std::endl;
    }

    if (input.is_open()) {
        while (getline(input, line)) {
            // std::cout << line << std::endl;
            result = split(line, ", ");
            prepareSqlString(result);
            // for_each(result.begin(), result.end(), printString);
        }
        input.close();
    }
}
