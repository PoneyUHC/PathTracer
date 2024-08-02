
#include "utils.hpp"


using namespace std;


string get_env_var(const string& key)
{
    char* val = getenv(key.c_str());
    return val == NULL ? string("") : string(val);
}