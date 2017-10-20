/*****************************************************************/
/*                                                               */
/*  FileName: HELOINST.CMD                                       */
/*                                                               */
/*  Purpose:  Installation command file for the CNTNRCTL part.   */
/*            Calls the Opendoc Rexx API to register the part.   */
/*                                                               */
/*  Change History:                                              */
/*                                                               */
/*****************************************************************/
/* add the registration function  ( and utilities )*/

SAY " Adding functions ";
call RXFUNCADD 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

call SysCls
SAY
SAY "**************************************"
SAY "*                                    *"
SAY "*  Registering the Hello Part        *"
SAY "*                                    *"
SAY "**************************************"


 /* Load the REXX functions */


 rc = RXFUNCQUERY('ODLoadOpenDocFuncs');
 SAY " rc = " rc;
 IF rc \= 0 then DO
   SAY " Adding ODLoadOpenDocFuncs"
   rc = RXFUNCADD('ODLoadOpenDocFuncs', 'OPENDOC', 'ODLoadOpenDocFuncs');
   SAY " RXFUNCADD rc for ODLoadOpendocFuncs = " rc
      IF rc <  0 THEN DO
        SAY " Unable to add  Opendoc functions, EXITING";
        RETURN;
      END
 END

call ODLoadOpenDocFuncs;



className = "HelloPart"
dllName   = "hello2"
cTemplate = "TRUE"
somIRName = ""


SAY "Registering Hello Part"

rc = ODRegisterPartHandlerClass( className, dllName, cTemplate, somIRName);

/* rc of 0 = successfully added, 4 = successfully replaced */

IF rc \=  0 & rc \= 4 then do
        call ODUnLoadOpenDocFuncs
        call RxFuncDrop 'ODLoadOpenDocFuncs'
        SAY " Registration failed, rc = " rc;
        return;
end

        call ODUnLoadOpenDocFuncs
        call RxFuncDrop 'ODLoadOpenDocFuncs'
        SAY "Hello Part successfully registered";
RETURN
