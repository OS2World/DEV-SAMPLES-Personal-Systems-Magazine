 process pgmname(mixed) test notypechk
       IDENTIFICATION DIVISION.

      ****************************************************************
      *    Class Order : Inherits from SOMObject                     *
      *                      in the SOM Class Library.               *
      ****************************************************************

       CLASS-ID.   "Order" INHERITS SOMObject.

      ****************************************************************
      *    Class Order contains the following methods:               *
      *    somDefaultInit -    Initializes an Order Object.          *
      *    somFree        -    Frees orderitems, collection,         *
      *                        and order.                            *
      *    setOrderNumber -    Sets the number of a Order Object     *
      *                        based on a given Object reference.    *
      *    setOrderDate   -    Sets the date of a Order Object       *
      *                        based on a given Object reference.    *
      *    addOrderItem   -    Adds a OrderItem Object to the order  *
      *    removeOrderItem -   Removes a OrderItem Object from the   *
      *                        order.                                *
      *    calculateCost  -    Computes the cost of the OrderItem    *
      *                        Objects in the order.                 *
      *    describeOrder  -    Lists the contents of the OrderItems  *
      *                        collected in the order.               *
      *    getOrderNumber -    Retrieves the number of a Order       *
      *                        Object.                               *
      *    getOrderDate   -    Retrieves the date of a Order         *
      *                        Object.                               *
      ****************************************************************

       ENVIRONMENT DIVISION.

      ****************************************************************
      *    Define which classes will be used by the methods in       *
      *    this class.                                               *
      ****************************************************************

       CONFIGURATION SECTION.
       REPOSITORY.
           CLASS SOMObject            IS "SOMObject"
           CLASS SOMCollection        IS "somf_TDeque"
           CLASS SOMIterator          IS "somf_TDequeIterator"
           CLASS OrderItem            IS "OrderItem".

      ****************************************************************
      *    Define the Order Object.                                  *
      ****************************************************************

       DATA DIVISION.
       WORKING-STORAGE SECTION.
       01  WS-EV                   USAGE POINTER.

      ****************************************************************
      *    Define the instance data of the Order Object.             *
      ****************************************************************
       01  Order-Object.
           05  Order-Number            PIC X(5).
           05  Order-Date              PIC X(8).
           05  Order-Collection USAGE OBJECT REFERENCE SOMCollection.

      ****************************************************************
      *    Define an iterator for use on the Order-Collection.       *
      ****************************************************************
       01  Order-Iterator      USAGE OBJECT REFERENCE SOMIterator.



       PROCEDURE DIVISION.

      ****************************************************************
      ****************************************************************
      *    The overridden method somDefaultInit initializes the      *
      *    Order instance, and creates the collection to be          *
      *    used in the order.                                        *
      ****************************************************************
       IDENTIFICATION DIVISION.
       METHOD-ID. "somDefaultInit"     OVERRIDE.

       DATA DIVISION.

       PROCEDURE DIVISION.

      ****************************************************************
      *    Initialize the SOM global environment variable.           *
      ****************************************************************
           CALL "somGetGlobalEnvironment" RETURNING WS-EV.
      ****************************************************************
      *    Now initialize an empty collection for us to add OrderItems *
      *    into with the addOrderItem method.                        *
      ****************************************************************
           INVOKE SOMCollection "somNew"
                                RETURNING Order-Collection.

      ****************************************************************
      *    Instantiate an iterator object and associate it with the  *
      *    collection.                                               *
      ****************************************************************
           INVOKE Order-Collection     "somfCreateSequenceIterator"
                                       USING     BY VALUE WS-EV
                                       RETURNING Order-Iterator.

      ****************************************************************
      *    EXIT and END the method.                                  *
      ****************************************************************
           EXIT METHOD.
       END METHOD "somDefaultInit".



      ****************************************************************
      ****************************************************************
      *    The overridden method somFree     destroys the OrderItem  *
      *    objects created, the collection object, and the order     *
      *    object.                                                   *
      ****************************************************************
       IDENTIFICATION DIVISION.
       METHOD-ID. "somFree"            OVERRIDE.

       DATA DIVISION.
       WORKING-STORAGE SECTION.


       PROCEDURE DIVISION.

      ****************************************************************
      *    Get the collected objects.                                *
      ****************************************************************
           INVOKE Order-Collection     "somfDeleteAll"
                                       USING     BY VALUE WS-EV.


      ****************************************************************
      *    Free the collection and iterator Objects                  *
      ****************************************************************
           INVOKE Order-Iterator   "somFree".

           INVOKE Order-Collection "somFree".

      ****************************************************************
      *    Free thyself...Use SUPER so we don't recurse back into    *
      *    this method.                                              *
      ****************************************************************
           INVOKE SUPER "somFree".

      ****************************************************************
      *    EXIT and END the method.                                  *
      ****************************************************************
           EXIT METHOD.
       END METHOD "somFree".



      ****************************************************************
      ****************************************************************
      *    Method getOrderNumber gets the number of Order based      *
      *    on the object reference of the Order.                     *
      ****************************************************************
       IDENTIFICATION DIVISION.
       METHOD-ID. "getOrderNumber".

       DATA DIVISION.

      ****************************************************************
      *    Define the linkage attributes.                            *
      ****************************************************************
       LINKAGE SECTION.
       01  LS-OrderNumber              PIC X(5).


       PROCEDURE DIVISION              RETURNING   LS-OrderNumber.

      ****************************************************************
      *    Move data to the LINKAGE SECTION.                         *
      ****************************************************************
           MOVE Order-Number TO LS-OrderNumber.

           EXIT METHOD.
       END METHOD "getOrderNumber".



      ****************************************************************
      ****************************************************************
      *    Method getOrderDate gets the date of a Order based        *
      *    on the Object reference of the Order.                     *
      ****************************************************************
       IDENTIFICATION DIVISION.
       METHOD-ID. "getOrderDate".

       DATA DIVISION.

      ****************************************************************
      *    Define the linkage attributes.                            *
      ****************************************************************
       LINKAGE SECTION.
       01  LS-OrderDate                PIC X(8).


       PROCEDURE DIVISION              RETURNING   LS-OrderDate.

      ****************************************************************
      *    Move data to the LINKAGE SECTION.                         *
      ****************************************************************
           MOVE Order-Date TO LS-OrderDate.

           EXIT METHOD.
       END METHOD "getOrderDate".



      ****************************************************************
      ****************************************************************
      *    Method setOrderNumber sets the number of Order based      *
      *    on the Object reference of the Order.                     *
      ****************************************************************
       IDENTIFICATION DIVISION.
       METHOD-ID. "setOrderNumber".

       DATA DIVISION.

      ****************************************************************
      *    Define the linkage attributes.                            *
      ****************************************************************
       LINKAGE SECTION.
       01  LS-OrderNumber              PIC X(5).


       PROCEDURE DIVISION              USING       LS-OrderNumber.

      ****************************************************************
      *    Move data from the LINKAGE SECTION.                       *
      ****************************************************************
           MOVE LS-OrderNumber TO Order-Number.

           EXIT METHOD.
       END METHOD "setOrderNumber".



      ****************************************************************
      ****************************************************************
      *    Method setOrderDate   sets the date of a Order based      *
      *    on the Object reference of the Order.                     *
      ****************************************************************
       IDENTIFICATION DIVISION.
       METHOD-ID. "setOrderDate".

       DATA DIVISION.

      ****************************************************************
      *    Define the linkage attributes.                            *
      ****************************************************************
       LINKAGE SECTION.
       01  LS-OrderDate                PIC X(8).


       PROCEDURE DIVISION              USING       LS-OrderDate.

      ****************************************************************
      *    Move data from the LINKAGE SECTION.                       *
      ****************************************************************
           MOVE LS-OrderDate TO Order-Date.

           EXIT METHOD.
       END METHOD "setOrderDate".



      ****************************************************************
      ****************************************************************
      *    Method describeOrder describes the order contents.        *
      ****************************************************************
       IDENTIFICATION DIVISION.
       METHOD-ID. "describeOrder".

       DATA DIVISION.

       LOCAL-STORAGE SECTION.
       01  LSS-CollectedOrderItem  USAGE OBJECT REFERENCE OrderItem.
       01  LSS-Number                  PIC X(10).
       01  LSS-Cost                    PIC 999V99.
       01  LSS-Item-Count              PIC S9(8)   COMP.

       LINKAGE SECTION.
       01  LS-Items.
           05  LS-Item-Count           PIC S9(4).
           05  LS-Item             OCCURS 1 TO 64 TIMES
                                   DEPENDING ON LS-Item-Count
                                   INDEXED BY   LS-Index.
               10  LS-Number           PIC X(10).
               10  LS-Cost             PIC 999V99.


       PROCEDURE DIVISION          RETURNING LS-Items.

      ****************************************************************
      *    Get the count of the number of items in the collection.   *
      ****************************************************************
           INVOKE Order-Collection     "somfCount"
                                       USING     BY VALUE WS-EV
                                       RETURNING      LSS-Item-Count.
           MOVE LSS-Item-Count TO LS-Item-Count.

      ****************************************************************
      *    Get the first one in the collection.                      *
      ****************************************************************
           IF LSS-Item-Count > 0
               THEN SET LS-Index TO 1
                    INVOKE Order-Iterator "somfFirst"
                                   USING     BY VALUE  WS-EV
                                   RETURNING LSS-CollectedOrderItem
                    PERFORM GET-Number-N-COST
           END-IF.

      ****************************************************************
      *    Get the rest...                                           *
      ****************************************************************
           SUBTRACT 1 FROM LSS-Item-Count.
           IF LSS-Item-Count > 0
               THEN PERFORM LSS-Item-Count TIMES
                        SET LS-Index UP BY 1
                        INVOKE Order-Iterator "somfNext"
                                   USING     BY VALUE WS-EV
                                   RETURNING LSS-CollectedOrderItem
                        PERFORM GET-Number-N-COST
                    END-PERFORM
           END-IF.

      ****************************************************************
      *    Exit and end the method.                                  *
      ****************************************************************
           EXIT METHOD.

      ****************************************************************
      *    Invoke the getNumber and getcost methods on the OrderItem *
      *    object and move the returned attributes to the table.     *
      ****************************************************************
       GET-Number-N-COST.
           INVOKE LSS-CollectedOrderItem "getNumber"
                                   RETURNING LSS-Number.
           MOVE LSS-Number TO        LS-Number (LS-Index).
           INVOKE LSS-CollectedOrderItem "getCost"
                                   RETURNING LSS-Cost.
           MOVE LSS-Cost   TO        LS-Cost (LS-Index).

       END METHOD "describeOrder".



      ****************************************************************
      ****************************************************************
      *    Method calculateCost computes the cost of the order.      *
      ****************************************************************
       IDENTIFICATION DIVISION.
       METHOD-ID. "calculateCost".

       DATA DIVISION.
       LOCAL-STORAGE SECTION.
       01  LSS-CollectedOrderItem  USAGE Object REFERENCE OrderItem.
       01  LSS-Item-Count              PIC S9(8)   COMP.
       01  LSS-Cost                    PIC 999V99.

      ****************************************************************
      *    Define the linkage attributes.                            *
      ****************************************************************
       LINKAGE SECTION.
       01  LS-Cost                     PIC 9(7)V99.


       PROCEDURE DIVISION              RETURNING   LS-Cost.

      ****************************************************************
      *    Initialize the accumulator for the total cost.            *
      ****************************************************************
           MOVE ZERO TO LS-Cost.

      ****************************************************************
      *    Get the count of the number of items in the collection.   *
      ****************************************************************
           INVOKE Order-Collection     "somfCount"
                                       USING     BY VALUE WS-EV
                                       RETURNING LSS-Item-Count.

      ****************************************************************
      *    Get the first one in the collection.                      *
      ****************************************************************
           IF LSS-Item-Count > 0
                    INVOKE Order-Iterator "somfFirst"
                                    USING     BY VALUE WS-EV
                                    RETURNING LSS-CollectedOrderItem
                    PERFORM GET-COST
           END-IF.

      ****************************************************************
      *    Get the rest...                                           *
      ****************************************************************
           SUBTRACT 1 FROM LSS-Item-Count.
           IF LSS-Item-Count > 0
               THEN PERFORM LSS-Item-Count TIMES
                        INVOKE Order-Iterator "somfNext"
                                   USING     BY VALUE WS-EV
                                   RETURNING LSS-CollectedOrderItem
                        PERFORM GET-COST
                    END-PERFORM
           END-IF.

      ****************************************************************
      *    EXIT the method and return.                               *
      ****************************************************************
           EXIT METHOD.

      ****************************************************************
      *    Invoke the getCost method on the OrderItem Object and     *
      *    accumulate the cost.                                      *
      ****************************************************************
       GET-COST.
           INVOKE LSS-CollectedOrderItem "getCost"
                                   RETURNING LSS-Cost.
           ADD LSS-Cost TO LS-Cost.

       END METHOD "calculateCost".



      ****************************************************************
      ****************************************************************
      *    Method addOrderItem adds a OrderItem to the OrderItem     *
      *    collection in the order.                                  *
      ****************************************************************
       IDENTIFICATION DIVISION.
       METHOD-ID. "addOrderItem".

       DATA DIVISION.
       LOCAL-STORAGE SECTION.
       01  LSS-Before-Count            PIC S9(8)    COMP.
       01  LSS-After-Count             PIC S9(8)    COMP.
       01  LSS-CollectedOrderItem  USAGE Object REFERENCE OrderItem.

       01  LSS-Item-Count              PIC S9(8)   COMP.

      ****************************************************************
      *    Define the linkage attributes.                            *
      ****************************************************************
       LINKAGE SECTION.
       01  LS-OrderItem        USAGE OBJECT REFERENCE OrderItem.
       01  LS-Parms.
           05  LS-Item-Count           PIC S9(8)   COMP.
           05  LS-Flag                 PIC X.

       PROCEDURE DIVISION              USING       LS-OrderItem
                                       RETURNING   LS-Parms.

      ****************************************************************
      *    Get the count of items before adding the object.          *
      ****************************************************************
           INVOKE Order-Collection     "somfCount"
                                       USING     BY VALUE WS-EV
                                       RETURNING LSS-Before-Count.

           INVOKE Order-Collection "somfAdd"
                                       USING BY VALUE WS-EV
                                             BY VALUE LS-OrderItem.

      ****************************************************************
      *    Get the count of items after adding the Object.           *
      ****************************************************************
           INVOKE Order-Collection     "somfCount"
                                       USING     BY VALUE WS-EV
                                       RETURNING LSS-After-Count.
           MOVE LSS-After-Count   TO LS-Item-Count.

      ****************************************************************
      *    If the counts are the same the add failed.                *
      ****************************************************************
           IF LSS-Before-Count = LSS-After-Count
               THEN MOVE "1" TO LS-Flag
           ELSE
               MOVE "0" TO LS-Flag
           END-IF.

      ****************************************************************
      *    EXIT the method and return.                               *
      ****************************************************************
           EXIT METHOD.

       END METHOD "addOrderItem".


      ****************************************************************
      ****************************************************************
      *    Method removeOrderItem removes a OrderItem from the       *
      *    collection in the order.                                  *
      ****************************************************************
       IDENTIFICATION DIVISION.
       METHOD-ID. "removeOrderItem".

       DATA DIVISION.
       LOCAL-STORAGE SECTION.
       01  LSS-Before-Count            PIC S9(8)     COMP.
       01  LSS-After-Count             PIC S9(8)     COMP.
       01  LSS-CollectedOrderItem  USAGE Object REFERENCE OrderItem.
       01  LSS-theEqualFlag            PIC X.
       01  LSS-Item-Count              PIC S9(8)   COMP.
       01  LSS-Loop-Count              PIC S9(8)   COMP.

      ****************************************************************
      *    Define the linkage attributes.                            *
      ****************************************************************
       LINKAGE SECTION.
       01  LS-OrderItem        USAGE Object REFERENCE OrderItem.
       01  LS-Parms.
           05  LS-Item-Count           PIC S9(8)   COMP.
           05  LS-Flag                 PIC X.

       PROCEDURE DIVISION              USING       LS-OrderItem
                                       RETURNING   LS-Parms.


      ****************************************************************
      *    Get the count of items before the delete.                 *
      ****************************************************************
           INVOKE Order-Collection     "somfCount"
                                       USING     BY VALUE WS-EV
                                       RETURNING LSS-Before-Count.
           MOVE LSS-Before-Count  TO LSS-Item-Count.

      ****************************************************************
      *    Get the first one in the collection.                      *
      ****************************************************************
           IF LSS-Item-Count NOT = 0
               THEN INVOKE Order-Iterator "somfFirst"
                                    USING     BY VALUE  WS-EV
                                    RETURNING LSS-CollectedOrderItem
                    PERFORM CHECK-EQUAL-N-REMOVE
           END-IF.

      ****************************************************************
      *    Get the rest...                                           *
      ****************************************************************
           SUBTRACT 1 FROM LSS-Item-Count.
           IF LSS-Item-Count > 0 AND
              LSS-theEqualFlag = X"00"
               THEN PERFORM VARYING LSS-Loop-Count
                            FROM 1 BY 1
                            UNTIL LSS-Loop-Count   > LSS-Item-Count
                               OR LSS-theEqualFlag = X"01"
                        INVOKE Order-Iterator "somfNext"
                                   USING     BY VALUE WS-EV
                                   RETURNING LSS-CollectedOrderItem
                        PERFORM CHECK-EQUAL-N-REMOVE
                    END-PERFORM
           END-IF.

      ****************************************************************
      *    Get the count of items after the delete.                  *
      ****************************************************************
           INVOKE Order-Collection     "somfCount"
                                       USING     BY VALUE WS-EV
                                       RETURNING LSS-After-Count.
           MOVE LSS-After-Count   TO LS-Item-Count.

      ****************************************************************
      *    If the counts are the same the delete failed.             *
      ****************************************************************
           IF LSS-Before-Count = LSS-After-Count
               THEN MOVE "1" TO LS-Flag
           ELSE
               MOVE "0" TO LS-Flag
           END-IF.

      ****************************************************************
      *    EXIT the method and return.                               *
      ****************************************************************
           EXIT METHOD.

       CHECK-EQUAL-N-REMOVE.
           INVOKE LSS-CollectedOrderItem "somfIsEqual"
                               USING   BY VALUE  WS-EV
                                       BY VALUE  LS-OrderItem
                               RETURNING         LSS-theEqualFlag.
      ****************************************************************
      *    If we find one, remove it from the collection.            *
      ****************************************************************
           IF LSS-theEqualFlag = X"01"
               THEN INVOKE Order-Collection "somfRemove"
                               USING BY VALUE WS-EV
                                     BY VALUE LSS-CollectedOrderItem
                    INVOKE LSS-CollectedOrderItem "somFree".

       END METHOD "removeOrderItem".


      ****************************************************************
      *    End object definition and class Order.                    *
      ****************************************************************
       END CLASS "Order".
