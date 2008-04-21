#include <iostream>
#include <list>
#include <string>
#include <vector>
using namespace std;
#include <zdlcommon.h>

char* chomp(string in)
{
	while (in.length() > 0){
		if (in[0] == ' ' || in[0] == '\n' || in[0] == '\r' || in[0] == '\t'){
			in.erase(0);
		}else{
			break;
		}
	}
	while (in.length() > 0){
		if (in[in.length()-1] == ' ' || in[in.length()-1] == '\n' || in[in.length()-1] == '\r' || in[in.length()-1] == '\t'){
			in.erase(in.length()-1);
		}else{
			break;
		}
	}
	return (char*)in.c_str();
}

int countLines(ZDLConf *cnf)
{
	int count = 0;
	list<ZDLSection*>::iterator itr;
	for (itr = (cnf->sections).begin(); itr != (cnf->sections).end();itr++){
		list <ZDLLine*>::iterator itr2;
		ZDLSection *sc = (*itr);
		for (itr2 = (sc->lines).begin(); itr2 != (sc->lines).end();itr2++){
			count++;
		}
	}
	return count;

} 
