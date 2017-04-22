#include <windows.h>

struct Res {
    void* pointer;
    DWORD size;

    Res();
    Res(WORD id, WORD type);

    void Load(WORD id, WORD type);
};
