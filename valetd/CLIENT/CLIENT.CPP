//
// Client.CPP
// Test Driver for Valet Parking
//
// Author: Rick Weaver
// Date: 09/05/1995
//
// Purpose:  Reads the script from the file CAR.SCR and based on the
// script, it will either park the car or retrieve the car from the
// parking lot.
//
// The file format of CAR.SCR should be as follows:  (Note the '|' is used
//                                                    as a delimiter)
//        P|Blue|Buick|LeSabre
//        P|Black|Pontiac|Trans AM
//        R|1
//        R|0
//
// The P record (park a car) is organized as follows:
//     P|color|make|model
//
// The R record (retrieve a car) is organized as follows:
//     R|slot number
//
// Ideally, this program should hold onto all tickets returned when a
// car is parked, but for testing purposes, this works better.
// The parking slot number value is set in a dummy parking ticket and then
// passed as an argument to the retrieveCar method on the Valet object.
//

#include "valet.xh"
#include "car.xh"

//Following line added for DSOM
#include <somd.xh>

#include <fstream.h>
#include <string.h>
#include <stdlib.h>

long check_ev(Environment *);  //Used to check SOM exceptions

main(int argc, char *argv[], char *envp[])
{
 fstream infile;
 char buf[80];   /*buffer for holding record from file*/
 char *token=0;    /*used when reading input file*/
 long tNum=0;

 Environment *ev = somGetGlobalEnvironment();

//Following lines added for DSOM
 SOMD_Init(ev);
 ValetNewClass (0,0);
 CarNewClass (0,0);
 TicketNewClass (0,0);

//Following change made for DSOM
// Valet *v = new Valet();
 Valet *v = (Valet *) SOMD_ObjectMgr->somdNewObject(ev, "Valet", "");
 Ticket *t;

/*Create a valet object*/
 v->_set_valetName(ev,"Roy Bigg");
 v->_set_valetID(ev,1);

 infile.open("car.scr",ios::in);  // *** open car.scr for input

 while (!infile.eof())
    {
     buf[0]='\0';
     infile.getline(buf,sizeof(buf),'\n');
     if (strlen(buf)!=0)         // *** if it is a valid record
       {
       //Following change made for DSOM
       // Car *c = new Car();
        Car *c = (Car *) SOMD_ObjectMgr->somdNewObject(ev, "Car", "");

        token=strtok(buf,"|");
        if (*token=='P')  //Park a car.
          {
           token=strtok(NULL,"|");
           c->_set_color(ev,token);
           token = strtok(NULL,"|");
           c->_set_make(ev,token);
           token = strtok(NULL,"|");
           c->_set_model(ev,token);
           t = v->parkCar(ev,c);
           cout << "==> The "<<c->_get_color(ev)<<" "<<c->_get_make(ev)<<" "<<c->_get_model(ev)<<
                   " was parked in slot ";
           cout <<t->_get_parkingSlotNum(ev)<<endl;

           //Changed for DSOM
           //delete t; //Telete ticket.  (For testing)
           SOMFree (t);

          }
         else  //Retrieve a car.
          {
          //Following change made for DSOM
          //Ticket *nT = new Ticket;
           Ticket *nT = (Ticket *) SOMD_ObjectMgr->somdNewObject(ev, "Ticket", "");

           token=strtok(NULL,"|");
           tNum=atol(token);
           nT->_set_parkingSlotNum(ev,tNum);
           c = v->retrieveCar(ev,nT);
           if (!check_ev(ev))
             cout << "<== The "<<c->_get_color(ev) << " " << c->_get_make(ev) << " " << c->_get_model(ev)<<
                     " was returned from slot "<< nT->_get_parkingSlotNum(ev)<<endl;

          //Following changes made for DSOM
          //delete nT;
          //delete c;  //delete car object returned.
           SOMFree (nT);
           SOMFree (c);
          }  /*else*/
       } /*if..then*/
   } /*while*/
 infile.close();

//Following change made for DSOM
// delete v;
 SOMFree (v);
 SOMD_Uninit(ev);

}

long check_ev(Environment *ev)
 {
  long rc=0;
  CarMissing *cm;

  switch (ev->_major)
  {
   case SYSTEM_EXCEPTION:
      cout << "System Exception occurred" << endl;
      rc=1;
      break;
   case USER_EXCEPTION:
      cm=(CarMissing *) somExceptionValue(ev);
      cout << "   ***   " <<cm->reason<<" in slot " << cm->errorCode<<endl;
      somExceptionFree(ev);
      rc=1;
      break;
   case NO_EXCEPTION:
      rc=0;
      break;
  }
  return rc;
 }

