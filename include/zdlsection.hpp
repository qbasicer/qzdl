class ZDLSection{
	friend class ZDLVariables;
public:
	ZDLSection(char *name);
	~ZDLSection();
	int addLine(char *linedata);
	char *getName();
	void setSpecial(int inFlags);
	char *findVariable(char* variable);
	int hasVariable(char* variable);
	void deleteVariable(char* variable);
	int setValue(char *variable, const char *value);
	int streamWrite(ostream &stream);
	list<ZDLLine*> lines;
private:
	int reads;
	int writes;
	ZDLLine *findLine(char *inVar);
	int flags;
	string sectionName;
};
