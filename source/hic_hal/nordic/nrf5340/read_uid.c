#include "read_uid.h"
#include "device.h"

void read_unique_id(uint32_t *id)
{
    id[0] = 0xDEAD;
    id[1] = 0xBEEF;
    id[2] = 0xAAAA;
    id[3] = 0xAAAA;
}
