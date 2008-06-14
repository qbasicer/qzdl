class ZDLVariables;

class ZDLConf {
	friend class ZDLVariables;
public:
	int readINI(const char *file);
	int writeINI(const char *file);
	char* getValue(const char *section, const char *variable);
	int hasValue(const char *section, const char *variable);
	void deleteValue(const char *lsection, const char *variable);
	int setValue(const char *lsection, const char *variable, int value);
	int setValue(const char *lsection, const char *variable, const char *szBuffer);
	int numberOfSections();
	~ZDLConf();
	ZDLConf();
	list<ZDLSection*> sections;
	int writeStream(ostream &stream);
	ZDLSection *getSection(const char* section);
private:
	int reads;
	int writes;
	void parse(string in, ZDLSection* current);
	ZDLVariables *vars;
};
