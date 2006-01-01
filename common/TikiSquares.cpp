/*
 *  TikiTest.cpp
 *  TikiTest
 *
 *  Copyright 2005 Cryptic Allusion, LLC. All rights reserved.
 *
 */

#include <Tiki/Tiki.h>

#if TIKI_PLAT == TIKI_WIN32
static char szAppName[] = "TikiTest";
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
extern "C" int tiki_main(int argc, char *argv[]);
int main(int argc, char *argv[])
#endif
{
#if TIKI_PLAT != TIKI_WIN32
	return tiki_main(argc, argv);
#else
	return Tiki::DoMain(szAppName, hInst, hPrevInstance, lpCmdLine, nCmdShow);
#endif
}
