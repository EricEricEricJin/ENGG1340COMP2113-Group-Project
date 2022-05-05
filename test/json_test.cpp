#include <iostream>
#include <fstream>
#include <vector>
#include <json.hpp>

using namespace std;

int main()
{
   nlohmann::json json_data;
    try
    {
        json_data.get<int>();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
   cout << (json_data["A"] == nullptr) << endl;
    
    return 0;
}