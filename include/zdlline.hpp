class ZDLLine{
	friend class ZDLVariables;
public:
	ZDLLine(const char *inLine);
	~ZDLLine();
	int getType();
	const char* getValue();
	const char* getVariable();
	const char* getLine();
	int setValue(const char *inValue);
private:
	int reads;
	int writes;
	void parse();
	int type;
	string line;
	string value;
	string variable;
	int slashConvert;
};
