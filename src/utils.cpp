
#include "utils.hpp"


using namespace std;


string get_env_var(const string& key)
{
    char* val = getenv(key.c_str());
    return val == NULL ? string("") : string(val);
}


void change_endianess(uint32_t n, uint8_t *dest)
{
    uint8_t *data_ptr = (uint8_t*) &n;

    for(ulong i=0; i<sizeof(uint32_t); ++i){
        dest[sizeof(uint32_t) - i - 1] = data_ptr[i];
    }
}
