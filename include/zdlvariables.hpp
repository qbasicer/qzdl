class ZDLConf;

class ZDLVariables{
public:
	/* The variable class must be aware of the conf to be able to resolve
	 * some of the more complex varible assignments.
	 */
	ZDLVariables (ZDLConf* parent);
	ZDLVariables (ZDLConf* parent, int flags);
	~ZDLVariables();
	/* Call this to get the variable.  Automatic resolving will be used. */
	char *getVariable(char *lsection, char *variable, int* status);
	int hasVariable(char *lsection, char *variable, int* status);
	/* Set usage flags */
	void setFlags(int flags);
	/* Set the value of a variable.
	 * Set the temp flag to not write it out at close
	 */
	int setVariable(char *section, char *variable, char *value, int temp);
private:
	int containsUnresolved(char *inBuffer);
	char *resolveVariable(char *inBuffer);
	int getVariableStart(char *inBuffer);
	int getVariableEnd(char *inBuffer);
	char *demangle(char *inBuffer);
	int demangleHelper(char *inBuffer);
	int _flags;
	int depth;
	ZDLConf *_parent;
	string blank;
	//list <ZDLVariable *>variableList;
	static int trendBytes;
	static int trendNum;
	
};

	

