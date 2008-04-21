class ZDLSection{
	friend class ZDLVariables;
public:
	ZDLSection(const char *name);
	~ZDLSection();
	int addLine(const char *linedata);
	char *getName();
	void setSpecial(int inFlags);
	char *findVariable(const char* variable);
	int hasVariable(const char* variable);
	void deleteVariable(const char* variable);
	int setValue(const char *variable, const char *value);
	int streamWrite(ostream &stream);
	int getRegex(const char* regex, vector<ZDLLine*> &vctr);
	list<ZDLLine*> lines;
private:
	int reads;
	int writes;
	ZDLLine *findLine(const char *inVar);
	int flags;
	string sectionName;
};
