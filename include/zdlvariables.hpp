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
	char *getVariable(const char *lsection, const char *variable, int* status);
	int hasVariable(const char *lsection, const char *variable, int* status);
	/* Set usage flags */
	void setFlags(int flags);
	/* Set the value of a variable.
	 * Set the temp flag to not write it out at close
	 */
	int setVariable(const char *section, const char *variable, const char *value, int temp);
private:
	int containsUnresolved(const char *inBuffer);
	char *resolveVariable(const char *inBuffer);
	int getVariableStart(const char *inBuffer);
	int getVariableEnd(const char *inBuffer);
	char *demangle(const char *inBuffer);
	int demangleHelper(const char *inBuffer);
	int _flags;
	int depth;
	ZDLConf *_parent;
	string blank;
	//list <ZDLVariable *>variableList;
	static int trendBytes;
	static int trendNum;
	
};

	

