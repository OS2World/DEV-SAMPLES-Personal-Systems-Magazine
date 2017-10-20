 process pgmname(mixed) test notypechk
       IDENTIFICATION DIVISION.

      ****************************************************************
      *    Class UserInterface:  Inherits from SOMObject             *
      *                          in the SOM Class Library.           *
      ****************************************************************

       CLASS-ID.   "UserInterface"     INHERITS SOMObject.

      ****************************************************************
      *    Class UserInterface contains the following methods:       *
      *    readAction     -    Gets the input command from the       *
      *                        system user.                          *
      *    readNumber     -    Gets the number of item from the      *
      *                        system user.                          *
      *    readCost       -    Gets the cost of item from the        *
      *                        system user.                          *
      *    writeMessage   -    Displays a system status message to   *
      *                        the system user.                      *
      *    writeOutput    -    Displays the cost of the order and    *
      *                        order to the system user.             *
      *    writeItem      -    Displays the attributes of an item    *
      *                        collected in the order.               *
      ****************************************************************


       ENVIRONMENT DIVISION.

      ****************************************************************
      *    Define which classes will be used by the methods in       *
      *    this class.                                               *
      ****************************************************************

       CONFIGURATION SECTION.
       REPOSITORY.
           CLASS SOMObject            IS "SOMObject".

      ****************************************************************
      *    Define the UserInterface Object.                          *
      ****************************************************************

       DATA DIVISION.
       WORKING-STORAGE SECTION.
       01  WS-User-Action              PIC X(10).
           88  UA-ADD                              VALUE "Add".
           88  UA-DELETE                           VALUE "Delete".
           88  UA-END                              VALUE "End".


       PROCEDURE DIVISION.

      ****************************************************************
      ****************************************************************
      *    Method readAction gets the system user's command to be    *
      *    processed.                                                *
      ****************************************************************
       IDENTIFICATION DIVISION.
       METHOD-ID. "readAction".

       DATA DIVISION.
       LOCAL-STORAGE SECTION.
       01  LSS-Edit-Flag               PIC X.

      ****************************************************************
      *    Define the linkage attributes.                            *
      ****************************************************************
       LINKAGE SECTION.
       01  LS-Action                   PIC X(10).


       PROCEDURE DIVISION              RETURNING   LS-Action.

           MOVE LOW-VALUE TO LSS-Edit-Flag.
           PERFORM UNTIL LSS-Edit-Flag NOT = LOW-VALUE
               DISPLAY "Enter the action desired:  add, delete, end: "
               ACCEPT WS-User-Action                  FROM SYSIN
               MOVE FUNCTION UPPER-CASE (WS-User-Action)
                                      TO WS-User-Action
               MOVE WS-User-Action                    TO LS-Action

               EVALUATE WS-User-Action (1:3)
                   WHEN "ADD"
                       MOVE HIGH-VALUE TO LSS-Edit-Flag
                   WHEN "DEL"
                       MOVE HIGH-VALUE TO LSS-Edit-Flag
                   WHEN "END"
                       MOVE HIGH-VALUE TO LSS-Edit-Flag
                   WHEN OTHER
                       DISPLAY "Requested action was " WS-User-Action
                       DISPLAY "Try again, fumblefingers!!!"
               END-EVALUATE
           END-PERFORM.
           EXIT METHOD.
       END METHOD "readAction".



      ****************************************************************
      ****************************************************************
      *    Method readNumber gets the Number of item to be processed. *
      ****************************************************************
       IDENTIFICATION DIVISION.
       METHOD-ID. "readNumber".

       DATA DIVISION.
       LOCAL-STORAGE SECTION.
       01  LSS-Number                  PIC X(80).

      ****************************************************************
      *    Define the linkage attributes.                            *
      ****************************************************************
       LINKAGE SECTION.
       01  LS-Number                   PIC X(10).

       PROCEDURE DIVISION              RETURNING   LS-Number.

           DISPLAY "Enter the item: ".
           ACCEPT LSS-Number                  FROM SYSIN.
           MOVE FUNCTION UPPER-CASE (LSS-Number) TO LSS-Number.
           MOVE   LSS-Number (1:20)           TO   LS-Number.
           EXIT METHOD.
       END METHOD "readNumber".



      ****************************************************************
      ****************************************************************
      *    Method readCost gets the cost of the item to be processed.*
      ****************************************************************
       IDENTIFICATION DIVISION.
       METHOD-ID. "readCost".

       DATA DIVISION.
       LOCAL-STORAGE SECTION.
       01  LSS-Edit-Flag               PIC X.
       01  LSS-Cost-Work               PIC X(6).

      ****************************************************************
      *    Define the linkage attributes.                            *
      ****************************************************************
       LINKAGE SECTION.
       01  LS-Cost                     PIC 999V99.


       PROCEDURE DIVISION              RETURNING   LS-Cost.

           MOVE LOW-VALUE TO LSS-Edit-Flag.
           PERFORM UNTIL LSS-Edit-Flag = HIGH-VALUE
               DISPLAY "Enter the cost: "
               ACCEPT LSS-Cost-Work                  FROM SYSIN
               COMPUTE LS-Cost = FUNCTION NUMVAL (LSS-Cost-Work)
               IF LS-Cost NUMERIC
                   THEN MOVE HIGH-VALUE TO LSS-Edit-Flag
               ELSE
                   DISPLAY "Cost is not numeric - try again "
                   DISPLAY "and get it right this time!!! "
               END-IF
           END-PERFORM.
           EXIT METHOD.
       END METHOD "readCost".



      ****************************************************************
      ****************************************************************
      *    Method writeMessage lets the system user know if the      *
      *    requested action was successful.                          *
      ****************************************************************
       IDENTIFICATION DIVISION.
       METHOD-ID. "writeMessage".

       DATA DIVISION.

      ****************************************************************
      *    Define the linkage attributes.                            *
      ****************************************************************
       LINKAGE SECTION.
       01  LS-Flag                     PIC X.


       PROCEDURE DIVISION              USING       LS-Flag.

           IF LS-Flag = "0"
               THEN DISPLAY WS-User-Action "successful "
           ELSE
               DISPLAY WS-User-Action "failed "
           END-IF.
           EXIT METHOD.
       END METHOD "writeMessage".



      ****************************************************************
      ****************************************************************
      *    Method writeOutput displays the order number and cost     *
      *    to the system user.                                       *
      ****************************************************************
       IDENTIFICATION DIVISION.
       METHOD-ID. "writeOutput".

       DATA DIVISION.
       LOCAL-STORAGE SECTION.
       01  LSS-Formatted-Cost          PIC $Z,ZZZ,ZZ9.99.

      ****************************************************************
      *    Define the linkage attributes.                            *
      ****************************************************************
       LINKAGE SECTION.
       01  LS-Total-Cost               PIC 9(7)V99.
       01  LS-Order-Number             PIC 9(5).
       01  LS-Order-Date               PIC X(8).


       PROCEDURE DIVISION              USING       LS-Total-Cost
                                                   LS-Order-Number
                                                   LS-Order-Date.

           MOVE LS-Total-Cost TO LSS-Formatted-Cost.
           DISPLAY "Your order number "  LS-Order-Number
                   " placed on "         LS-Order-Date
                   " costs "             LSS-Formatted-Cost.
           EXIT METHOD.
       END METHOD "writeOutput".



      ****************************************************************
      ****************************************************************
      *    Method writeItem displays the attributes of items         *
      *    that have been collected in the order.                    *
      ****************************************************************
       IDENTIFICATION DIVISION.
       METHOD-ID. "writeItem".

       DATA DIVISION.

       LOCAL-STORAGE SECTION.

       01  LSS-Formatted-Count         PIC ZZZ9.
       01  LSS-Formatted-Cost          PIC ZZ9.99.

      ****************************************************************
      *    Define the linkage attributes.                            *
      ****************************************************************
       LINKAGE SECTION.
       01  LS-Items.
           05  LS-Item-Count           PIC S9(4).
           05  LS-Item             OCCURS 1 TO 64 TIMES
                                   DEPENDING ON LS-Item-Count
                                   INDEXED BY   LS-Index.
               10  LS-Number           PIC X(10).
               10  LS-Cost             PIC 999V99.


       PROCEDURE DIVISION              USING       LS-Items.

           MOVE LS-Item-Count TO LSS-Formatted-Count.
           DISPLAY "Contains " LSS-Formatted-Count " items".
           PERFORM VARYING LS-Index FROM 1 BY 1
                   UNTIL   LS-Index > LS-Item-Count
               MOVE    LS-Cost (LS-Index) TO   LSS-Formatted-Cost
               DISPLAY LS-Number (LS-Index)" @ " LSS-Formatted-Cost
           END-PERFORM.
           EXIT METHOD.
       END METHOD "writeItem".



      ****************************************************************
      *    End object definition and class UserInterface.            *
      ****************************************************************
       END CLASS "UserInterface".
