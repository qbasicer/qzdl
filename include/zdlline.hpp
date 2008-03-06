class ZDLLine{
	friend class ZDLVariables;
public:
	ZDLLine(char *inLine);
	~ZDLLine();
	int getType();
	char* getValue();
	char* getVariable();
	char* getLine();
	int setValue(char *inValue);
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
