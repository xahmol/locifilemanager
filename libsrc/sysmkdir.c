#include <loci.h>
#include <errno.h>
#include <string.h>

int __fastcall__ _sysmkdir (const char* name)
{
    int ret;
    size_t namelen = strlen(name);
    while(namelen) {
        mia_push_char (((char*)name)[--namelen]);
    }
    return mia_call_int_errno (MIA_OP_MKDIR);

    return ret;
}