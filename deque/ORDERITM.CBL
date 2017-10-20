 process pgmname(mixed) test notypechk
       IDENTIFICATION DIVISION.

      ****************************************************************
      *    Class OrderItem : Inherits from somf_MCollectible         *
      *                       in the SOM Class Library.              *
      ****************************************************************

       CLASS-ID.   "OrderItem"     INHERITS somf-MCollectible.

      ****************************************************************
      *    Class OrderItem contains the following methods:           *
      *    somfIsEqual    -    Determines if two OrderItem objects   *
      *                        are equivalent.                       *
      *    setCost        -    Sets the Cost of a OrderItem object   *
      *                        based on a given object reference.    *
      *    setNumber      -    Sets the Number of a OrderItem object *
      *                        based on a given object reference.    *
      *    getCost        -    Retrieves the Cost of a OrderItem     *
      *                        object based on a given object        *
      *                        reference.                            *
      *    getNumber      -    Retrieves the Number of a OrderItem   *
      *                        object based on a given object        *
      *                        reference.                            *
      ****************************************************************

       ENVIRONMENT DIVISION.

      ****************************************************************
      *    Define which classes will be used by the methods in       *
      *    this class.                                               *
      ****************************************************************

       CONFIGURATION SECTION.
       REPOSITORY.
           CLASS OrderItem            IS "OrderItem"
           CLASS somf-MCollectible    IS "somf_MCollectible".

      ****************************************************************
      *    Define the OrderItem Object.                              *
      ****************************************************************

       DATA DIVISION.
       WORKING-STORAGE SECTION.

      ****************************************************************
      *    Define the instance data of the OrderItem Object.         *
      ****************************************************************
       01  OrderItem-Object.
           05  Item-Number             PIC X(10).
           05  Item-Cost               PIC 999V99.



       PROCEDURE DIVISION.

      ****************************************************************
      ****************************************************************
      *    Method somfIsEqual determines if two OrderItem objects    *
      *    are equivalent.  If their Number and Cost attributes      *
      *    are the same, they are considered equal.                  *
      ****************************************************************
       IDENTIFICATION DIVISION.
       METHOD-ID. "somfIsEqual"        OVERRIDE.

       DATA DIVISION.
       LOCAL-STORAGE SECTION.
       01  ItemNumber                  PIC X(10).
       01  ItemCost                    PIC 999V99.

       LINKAGE SECTION.
       01  LS-EV                   USAGE POINTER.
       01  theOrderItem            USAGE OBJECT REFERENCE OrderItem.
       01  theEqualFlag                PIC X.


       PROCEDURE DIVISION     USING
                                         BY VALUE LS-EV
                                         BY VALUE theOrderItem
                               RETURNING          theEqualFlag.

      ****************************************************************
      *    Get the Number and Cost of the order object               *
      ****************************************************************

           INVOKE theOrderItem "getNumber" RETURNING ItemNumber.
           INVOKE theOrderItem "getCost"   RETURNING ItemCost.

      ****************************************************************
      *    Get those just obtained to the attributes of this         *
      *    instance.  If they are equal, set the equality flag       *
      *    to a binary 1, else set it to a zero.                     *
      ****************************************************************
           IF (Item-Number = ItemNumber) AND
              (Item-Cost = ItemCost)
               THEN MOVE X"01" TO theEqualFlag
           ELSE
               MOVE X"00" TO theEqualFlag.

           EXIT METHOD.
       END METHOD "somfIsEqual".



      ****************************************************************
      ****************************************************************
      *    Method getNumber gets the Number of a OrderItem based on  *
      *    the object reference of the OrderItem.                    *
      ****************************************************************
       IDENTIFICATION DIVISION.
       METHOD-ID. "getNumber".

       DATA DIVISION.
       WORKING-STORAGE SECTION.

      ****************************************************************
      *    Define the linkage attributes.                            *
      ****************************************************************
       LINKAGE SECTION.
       01  LS-Number                   PIC X(10).


       PROCEDURE DIVISION              RETURNING   LS-Number.

      ****************************************************************
      *    Move data to the LINKAGE SECTION.                         *
      ****************************************************************
           MOVE Item-Number TO LS-Number.

           EXIT METHOD.
       END METHOD "getNumber".



      ****************************************************************
      ****************************************************************
      *    Method getCost Gets the Cost of a OrderItem based on the *
      *    object reference of the OrderItem.                        *
      ****************************************************************
       IDENTIFICATION DIVISION.
       METHOD-ID. "getCost".

       DATA DIVISION.
       WORKING-STORAGE SECTION.

      ****************************************************************
      *    Define the linkage attributes.                            *
      ****************************************************************
       LINKAGE SECTION.
       01  LS-Cost                     PIC 999V99.


       PROCEDURE DIVISION              RETURNING   LS-Cost.

      ****************************************************************
      *    Move data to the LINKAGE SECTION.                         *
      ****************************************************************
           MOVE Item-Cost TO LS-Cost.

           EXIT METHOD.
       END METHOD "getCost".



      ****************************************************************
      ****************************************************************
      *    Method setNumber Sets the Number of a OrderItem based on the *
      *    object reference of the OrderItem.                        *
      ****************************************************************
       IDENTIFICATION DIVISION.
       METHOD-ID. "setNumber".

       DATA DIVISION.
       WORKING-STORAGE SECTION.

      ****************************************************************
      *    Define the linkage attributes.                            *
      ****************************************************************
       LINKAGE SECTION.
       01  LS-Number                   PIC X(10).


       PROCEDURE DIVISION              USING       LS-Number.

      ****************************************************************
      *    Move data to the LINKAGE SECTION.                         *
      ****************************************************************
           MOVE LS-Number TO Item-Number.

           EXIT METHOD.
       END METHOD "setNumber".



      ****************************************************************
      ****************************************************************
      *    Method setCost Sets the Cost of a OrderItem based on the *
      *    object reference of the OrderItem.                        *
      ****************************************************************
       IDENTIFICATION DIVISION.
       METHOD-ID. "setCost".

       DATA DIVISION.
       WORKING-STORAGE SECTION.

      ****************************************************************
      *    Define the linkage attributes.                            *
      ****************************************************************
       LINKAGE SECTION.
       01  LS-Cost                     PIC 999V99.


       PROCEDURE DIVISION              USING       LS-Cost.

      ****************************************************************
      *    Move data to the LINKAGE SECTION.                         *
      ****************************************************************
           MOVE LS-Cost TO Item-Cost.

           EXIT METHOD.
       END METHOD "setCost".



      ****************************************************************
      *    End object definition and class OrderItem.                *
      ****************************************************************
       END CLASS "OrderItem".
