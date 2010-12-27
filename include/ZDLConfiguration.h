#ifndef _ZDLCONFIGURATION_H_
#define _ZDLCONFIGURATION_H_

#include <QtCore>
#include <zdlcommon.h>

/**
 * ZDLConfiguration
 * This class is intented for use by classes instead of directly
 * accessing the ZDLConf objects directly.  It provides the required
 * functionality to provide at least 3 levels of configuration:
 * System level: A system wide list of source ports and IWADs
 * User Level: A user level list of source ports and IWADs
 * File Level: .ZDL files and zdl.ini level config
 */

class ZDLConfiguration{
	public:
		ZDLConfiguration();
		// NUM_CONFS *MUST* be last!
		enum ConfScope {CONF_SYSTEM, CONF_USER, CONF_FILE, NUM_CONFS};
		
		/* ScopeRules: defines how to search for the key.
		 * SCOPE_HIGHER: Search the specified scope and higher (ie, if user is specified, system is too)
		 * SCOPE_THIS: Only the specified scope is searched
		 * SCOPE_LOWER: Only the specified scope and lower
		 * NOTE! This only works when ConfScope is not NUM_CONFS
		 */
		enum ScopeRules {SCOPE_HIGHER, SCOPE_THIS, SCOPE_LOWER, SCOPE_ALL};
		QString getPath(ConfScope scope);
		ZDLConf* getConf(ConfScope scope);
		
		QString getString(QString section, QString key, int *ok,ConfScope scope = NUM_CONFS, ScopeRules rules = SCOPE_ALL);
		int getInt(QString section, QString key, int *ok, ConfScope scope = NUM_CONFS, ScopeRules rules = SCOPE_ALL);
		
		bool setString(QString section, QString key, QString value, ConfScope scope = NUM_CONFS, ScopeRules rules = SCOPE_ALL);
		bool setInt(QString section, QString key, int value, ConfScope scope = NUM_CONFS, ScopeRules rules = SCOPE_ALL);
		
		bool hasVariable(QString section, QString key, ConfScope scope = NUM_CONFS, ScopeRules rules = SCOPE_ALL);
		
	private:
		void bootstrap(QString path);
		ZDLConf* confs[NUM_CONFS];
		QString paths[NUM_CONFS];
};

#endif
