#include "res_wrk.h"

#include "string"
#include "wchar.h"

using namespace std;

Res::Res() {
    pointer = NULL;
    size = 0;
}
Res::Res(WORD id, WORD type) {
    Load(id, type);
}

void Res::Load(WORD id, WORD type) {
    wstring w_id;
    wstring w_type;

    w_id = L"#" + to_wstring(id);
    w_type = L"#" + to_wstring(type);

    HRSRC fres = FindResource(HMODULE(NULL), w_id.c_str(), w_type.c_str());
    HGLOBAL res = LoadResource(HMODULE(NULL), fres);


    pointer = LockResource(res);
    size = SizeofResource(HMODULE(NULL), fres);
}
