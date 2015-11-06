#include "Common.h"

INT WINAPI WinMain(HINSTANCE hinst, HINSTANCE, LPSTR, INT)
{
    hinst;

    String s(u"Hi there! c:");
    s.append(String::from_utf8(u8" Woop woop!"));
    s.append(u" Woot!");

    return 0;
}
