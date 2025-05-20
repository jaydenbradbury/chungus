#ifndef _SECRET_HH_
#define _SECRET_HH_

namespace jayden{

using BYTE = unsigned char;

void init();
void quitTest();
void setPixel(unsigned x, unsigned y, BYTE r, BYTE g, BYTE b);
void update();
[[noreturn]] void finish();
double dt();

}

#endif
