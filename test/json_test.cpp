#include <iostream>
#include <fstream>
#include <vector>
#include <json.hpp>

using namespace std;

int main()
{
    nlohmann::json json_data;
    ifstream file_stream("../resource/map/0.json", fstream::in);
    file_stream >> json_data;

    vector<float> vec;
    cout << (json_data["lines"].is_number_integer()) << endl;
    
    return 0;
}