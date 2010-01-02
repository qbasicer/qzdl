class ZDLVariables;

#

class ZDLConf {
	friend class ZDLVariables;
public:
	enum modes{
		ReadOnly  = 0x01,
		WriteOnly = 0x02,
		ReadWrite = ReadOnly | WriteOnly
	};
	
	int readINI(const char *file);
	int writeINI(const char *file);
	//char* getValue(const char *section, const char *variable);
	const char *getValue(const char *lsection, const char *variable, int *status);
	int hasValue(const char *section, const char *variable);
	void deleteValue(const char *lsection, const char *variable);
	int setValue(const char *lsection, const char *variable, int value);
	int setValue(const char *lsection, const char *variable, const char *szBuffer);
	int numberOfSections();
	~ZDLConf();
	ZDLConf(int mode = ZDLConf::ReadWrite);
	list<ZDLSection*> sections;
	int writeStream(ostream &stream);
	ZDLSection *getSection(const char* section);
private:
	int mode;
	int reads;
	int writes;
	void parse(string in, ZDLSection* current);
	ZDLVariables *vars;
};
