class ZDLVariables;

class ZDLConf {
	friend class ZDLVariables;
public:
	int readINI(char *file);
	int writeINI(char *file);
	char* getValue(char *section, char *variable);
	int hasValue(char *section, char *variable);
	void deleteValue(char *lsection, char *variable);
	int setValue(char *lsection, char *variable, int value);
	int setValue(char *lsection, char *variable, char *szBuffer);
	int numberOfSections();
	~ZDLConf();
	ZDLConf();
	list<ZDLSection*> sections;
	int writeStream(ostream &stream);
private:
	int reads;
	int writes;
	void parse(string in, ZDLSection* current);
	ZDLSection *getSection(char *lsection);
	ZDLVariables *vars;
};
