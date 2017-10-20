// Header file for MLOGIMPL.CPP
// Message Log facility.  Defined as a Singleton Class as described in Design Patterns.

#ifndef _MLOGIMPL_H_
#define _MLOGIMPL_H_

#define INCL_PM
#define INCL_OD
#include <os2.h>
#include <od.h>
#include <odbindng.xh>
#include <winutils.h>

class _Export MessageLog {

public:
   static MessageLog * Instance();  // Instance returns the single instance or creates the single instance
   virtual ~MessageLog();           // Destructor resets all static variables

   void Log(long msgno, unsigned long rc, char * file, long line);  // Main logging functions
   void Log(long msgno, char * text);
   void Log(char msgno, char * text);
   void trace(char * file, long line);
   void dump(ODStorageUnit * su);
   void dump(ODDraft * draft);
   void dump(ODDocument * document);
   void dump(ODContainer * container);
   void dump(ODSession * session);
   void dump(ODSession * session, ODArbitrator * arbitrator);
   void dump(ODBinding * binding);
   void dump(ODClipboard * clipboard);
   void dump(ODDispatcher * dispatcher);
   void dump(ODDragAndDrop * draganddrop);
   void dump(ODEventData * eventdata);
   void dump(ODFrame * frame);
   void dump(ODMessageInterface * messageinterface);
   void dump(ODNameResolver * nameresolver);
   void dump(ODNameSpaceManager * namespacemanager);
   void dump(ODStorageSystem * storagesystem);
   void dump(ODTranslation * translation);
   void dump(ODUndo * undo);
   void dump(ODWindowState * windowstate);
   void dump(POINTL * pointl);
   void debugLog(char *formatString,...);
   void errorLog(char *formatString,...);
   void fatalLog(char *formatString,...);
   void dumpLog(char *formatString,...);

protected:
   MessageLog(){}

private:
   static MessageLog* _instance;
   char ErrorBuf[1024];
};

#define DEBUG_MSGNO 1
#define ERROR_MSGNO 2
#define FATAL_MSGNO 3

#define perr() MessageLog::Instance()->errorLog("%s %d",__FILE__,__LINE__)
#define DebugLog  MessageLog::Instance()->debugLog
#define ErrorLog  MessageLog::Instance()->errorLog
#define FatalLog  MessageLog::Instance()->fatalLog
#define Trace     MessageLog::Instance()->trace
#define Dump      MessageLog::Instance()->dump

#endif // _MLOGIMPL_H_
