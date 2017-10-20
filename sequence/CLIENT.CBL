 process pgmname(mixed) test notypechk
       IDENTIFICATION DIVISION.
       PROGRAM-ID.     "Client".

      ****************************************************************
      *                                                              *
      *    The client program of this application does the           *
      *    following tasks:                                          *
      *    - Instantiates the UserInterface and Order objects.       *
      *    - Tells the UserInterface object to read the user's       *
      *      request.                                                *
      *    - Tells the Order object to process the user's request    *
      *      and tells the UserInterface object to get another       *
      *      request until the user signals the end of the order.    *
      *      If the request is an add or delete, sends the           *
      *      appropriate message to the Userinterface object to get  *
      *      the item cost and Number, as required by the user's     *
      *      processing request.                                     *
      *    - Tells the Order object to compute the order cost.       *
      *    - Tells the Order object to get the order number.         *
      *    - Tells the UserInterface object to write order cost.     *
      *    - Tells the order to describe itself.                     *
      *    - Frees the objects it instantiated.                      *
      *    - Terminates.                                             *
      *                                                              *
      ****************************************************************

       ENVIRONMENT DIVISION.
       CONFIGURATION SECTION.
       REPOSITORY.
           CLASS TheOrder              IS "Order"
           CLASS OrderItem             IS "OrderItem"
           CLASS UserInterface         IS "UserInterface".

       DATA DIVISION.
       WORKING-STORAGE SECTION.

      *    OBJECTS:
       01  orderObj             USAGE OBJECT REFERENCE TheOrder.
       01  userObj              USAGE OBJECT REFERENCE UserInterface.
       01  orderItemObj         USAGE OBJECT REFERENCE OrderItem.

      *    DATA ITEMS:
       01  Action                      PIC X(10).
       01  Item-Number                 PIC X(10).
       01  Item-Cost                   PIC 999V99.
       01  Max-Items                   PIC 9(4)        COMP VALUE 64.
       01  WS-Parms.
           05  Item-Count              PIC S9(8)       COMP.
           05  WS-Flag                 PIC X.
               88  SUCCESSFUL                      VALUE "0".
               88  FAILURE                         VALUE "1".
       01  Order-Number                PIC 9(5).

       01  Order-Date                  PIC X(8).
       01  WS-Random-Value             PIC 9V9(5).

       01  Total-Cost                  PIC 9(7)V99.
       01  WS-Items.
           05  WS-Count                PIC S9(4).
           05  WS-Item             OCCURS 1 TO 64 TIMES
                                   DEPENDING ON WS-Count
                                   INDEXED BY WS-Index.
               10  WS-Number           PIC X(10).
               10  WS-Cost             PIC 999V99.


       PROCEDURE DIVISION.

      ****************************************************************
      *    Invoke the UserInterface class with the inherited somNew  *
      *    method to instantiate a UserInterface object.             *
      *    somNew is inherited from SOMObject.                       *
      ****************************************************************
           INVOKE UserInterface "somNew"    RETURNING userObj.

      ****************************************************************
      *    We will simply use the system date for the order date     *
      *    and generate a random number for the order number.        *
      *    Also we will initialize the item-count field, which will  *
      *    control how many items are placed in the order.           *
      ****************************************************************
           MOVE FUNCTION CURRENT-DATE TO Order-Date.

           COMPUTE WS-Random-Value = FUNCTION RANDOM.
           COMPUTE Order-Number    = WS-Random-Value * 10000.

           MOVE    ZERO       TO   Item-Count.

      ****************************************************************
      *    Invoke the Order class with the inherited method somNew   *
      *    to instantiate an order object.                           *
      ****************************************************************
           INVOKE TheOrder    "somNew"       RETURNING   orderObj.

      ****************************************************************
      *    Invoke the setOrderNumber and setOrderDate methods to     *
      *    set the order's date and number.                          *
      ****************************************************************
           INVOKE orderObj  "setOrderNumber" USING ORDER-NUMBER.
           INVOKE orderObj  "setOrderDate"   USING ORDER-DATE.

      ****************************************************************
      *    Invoke the UserInterface object with readAction method.   *
      ****************************************************************
           INVOKE userObj "readAction" RETURNING   ACTION.

      ****************************************************************
      *    Loop until the user signals the end of the order.         *
      ****************************************************************
           PERFORM UNTIL Action = "END"
                      OR Item-Count = Max-Items

               EVALUATE Action (1:3)
                   WHEN "ADD"
      *                get the Number and cost from the user interface
                       INVOKE userObj "readNumber" RETURNING Item-Number
                       INVOKE userObj "readCost"   RETURNING Item-Cost

      *                instantiate an OrderItem with those attributes
                       INVOKE OrderItem "somNew"
                                           RETURNING orderItemObj
                       INVOKE orderItemObj "setNumber" USING Item-Number
                       INVOKE orderItemObj "setCost"  USING Item-Cost

      *                add it to the collection in the order
                       INVOKE orderObj "addOrderItem"
                                               USING     orderItemObj
                                               RETURNING WS-PARMS

      *                if the add failed, destroy the object just
      *                created because we can't do anything with it
      *                and it's not in the collection.
                       IF WS-Flag = "1"
                           THEN INVOKE orderItemObj   "somFree"
                       END-IF

      *                send appropriate msg via the user interface
                       INVOKE userObj   "writeMessage" USING WS-Flag

                   WHEN "DEL"
      *                get the Number and cost from the user interface
                       INVOKE userObj "readNumber" RETURNING Item-Number
                       INVOKE userObj "readCost"   RETURNING Item-Cost

      *                create a bottle with those attributes
                       INVOKE OrderItem "somNew"
                                           RETURNING orderItemObj
                       INVOKE orderItemObj "setNumber"
                                           USING     Item-Number
                       INVOKE orderItemObj "setCost"
                                           USING     Item-Cost

      *                delete copies of it from the collection
                       INVOKE orderObj "removeOrderItem"
                                           USING      orderItemObj
                                           RETURNING  WS-Parms

      *                destroy the order item just created
                       INVOKE orderItemObj   "somFree"

      *                send appropriate msg via the user interface
                       INVOKE userObj   "writeMessage" USING WS-Flag

                   WHEN OTHER
                       CONTINUE
               END-EVALUATE
               INVOKE userObj "readAction"   RETURNING Action
           END-PERFORM.
      ****************************************************************
      *    End of loop.                                              *
      ****************************************************************

      ****************************************************************
      ****************************************************************
      *    Close-out processing follows.                             *
      ****************************************************************
      ****************************************************************

      ****************************************************************
      *    If no items were ordered, end the process here.           *
      ****************************************************************
           IF Item-Count = 0
               THEN GOBACK.

      ****************************************************************
      *    Invoke the order object with the calculate cost method.   *
      ****************************************************************
           INVOKE orderObj "calculateCost"     RETURNING Total-Cost.

      ****************************************************************
      *    Invoke the Order object with the getOrdernumber           *
      *    and the getOrderdate methods.                             *
      ****************************************************************
           INVOKE orderObj "getOrderNumber"    RETURNING Order-Number.
           INVOKE orderObj "getOrderDate"      RETURNING Order-Date.

      ****************************************************************
      *    Invoke the UserInterface object with writeOutput method.  *
      ****************************************************************
           INVOKE userObj  "writeOutput"       USING Total-Cost
                                                     Order-Number
                                                     Order-Date.

      ****************************************************************
      *    Invoke the order object with the describeOrder method.    *
      ****************************************************************
           INVOKE orderObj "describeOrder" RETURNING WS-Items.

      ****************************************************************
      *    Invoke the userinterface object with writeItem method.    *
      ****************************************************************
           INVOKE userObj  "writeItem"         USING WS-Items.


      ****************************************************************
      *    Invoke the instantiated objects with the inherited        *
      *    somDestruct method.                                       *
      ****************************************************************
           INVOKE userObj  "somFree".

           INVOKE orderObj "somFree".

      ****************************************************************
      *    We're outta here...                                       *
      ****************************************************************
           GOBACK.
       END PROGRAM "Client".
