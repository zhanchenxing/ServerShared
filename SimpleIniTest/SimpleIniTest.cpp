// SimpleIniTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>

#include "SimpleIni.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CSimpleIniA ini;
	//ini.SetUnicode();
	ini.LoadFile("myfile.ini");
	const char * pVal = ini.GetValue("section", "key", "default");

	using namespace std;
	cout<<"value = '"<<pVal<<"'"<<endl;
	
	ini.SetValue("section", "key", "newvalue");

	return 0;
}

