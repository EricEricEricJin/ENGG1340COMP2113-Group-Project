#include <json.hpp>
#include <fstream>


using namespace std;

int main()
{
    nlohmann::json json_data;
    ofstream f("wtest.json");
    json_data["key"] = 1.14;
    f << json_data;
    f.close();
    return 0;
}