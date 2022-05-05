#include <iostream>
#include <fstream>
#include <vector>
#include <json.hpp>

using namespace std;

int main()
{
    map<string, int> zombie = {{"a", 1}, {"b", 2}};
    vector<vector<int>> mapwall = {{1, 2, 3}, {4, 5, 6}};    
    nlohmann::json json_data;
    // json_data["zombies"] = {};
    json_data["map"]["wall"] = mapwall;
    cout << json_data << endl;
    
    return 0;
}