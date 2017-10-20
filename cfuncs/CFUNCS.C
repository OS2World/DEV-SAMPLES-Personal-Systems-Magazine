
//********************************************************************************
//*  Progammer:   Greg Piamonte
//*  Date Written:  24 Aug 1995.
//********************************************************************************

/*-------------------------------------------------------------------*/
/* CFuncs.c Source file for a C DLL                             */
/*-------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CFuncs.h"

/*********************************************************************/
/*  Function Name:  convertFahrenToCelsius                           */
/*  Description:    Convert Fahrenheit temperature to Celsius        */
/*  Parameters                                                       */
/*                                                                   */
/*********************************************************************/
int _Optlink convertFahrenToCelsius( int fahrenheit )
{
    int celsius;

    celsius = ((fahrenheit -32) * 5) / 9;

    return( celsius );
}

/* If we are statically linking to the runtime libraries, we should */
/* register the exception handler for each function.                */
#ifdef   STATIC_LINK
#pragma  handler( convertFahrenToCelsius )
#endif

/*********************************************************************/
/*  Function Name:  convertCelsiusToFahren                           */
/*  Description:    Convert Celsius temperature to Fahrenheit        */
/*  Parameters                                                       */
/*                                                                   */
/*********************************************************************/
int _Optlink convertCelsiusToFahren( int celsius )
{
   int fahrenheit;

   fahrenheit = ((celsius * 9) / 5) + 32;

   return( fahrenheit );
}

/* If we are statically linking to the runtime libraries, we should */
/* register the exception handler for each function.                */
#ifdef   STATIC_LINK
#pragma  handler( convertCelsiusToFahren )
#endif



