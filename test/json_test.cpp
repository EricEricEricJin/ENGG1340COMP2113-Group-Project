#include <iostream>
#include <fstream>
#include <vector>
#include <json.hpp>

using namespace std;

int main()
{
    nlohmann::json json_data;
    ifstream file_stream("test.json", fstream::in);
    file_stream >> json_data;

    vector<float> vec;
    cout << json_data.is_array() << endl;
    if (json_data.is_array())
    {
        for (auto &line : json_data)
        {
            for (auto &number : line)
            {
                cout << number.get<float>() + 1 << " ";
            }
            cout << endl;
        }
    }
    return 0;
}