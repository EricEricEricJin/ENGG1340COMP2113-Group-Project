#include <iostream>
#include <json.hpp>

using namespace std;

template <typename T>
T f() { return T(); }

int main()
{
    nlohmann::json json_data;
    json_data["int"] = 1;
    json_data["float"] = 1.14;
    json_data["array"] = {1, 2};
    json_data["string"] = "HAMA";

    cout << (json_data["int"].type() == nlohmann::json::value_t::number_integer) << endl;
    try
    {
        cout << json_data["string"].get<string>() << endl;
    }
    catch (const std::exception &e)
    {
        cout << "ERR" << endl;
    }

    return 0;
}