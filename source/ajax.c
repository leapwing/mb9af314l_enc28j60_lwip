/******************************************************************************/
/*               (C) Fujitsu Semiconductor Europe GmbH (FSEU)                */
/*                                                                           */
/* The following software deliverable is intended for and must only be       */
/* used for reference and in an evaluation laboratory environment.           */
/* It is provided on an as-is basis without charge and is subject to         */
/* alterations.                                                              */
/* It is the user’s obligation to fully test the software in its             */
/* environment and to ensure proper functionality, qualification and         */
/* compliance with component specifications.                                 */
/*                                                                           */
/* In the event the software deliverable includes the use of open            */
/* source components, the provisions of the governing open source            */
/* license agreement shall apply with respect to such software               */
/* deliverable.                                                              */
/* FSEU does not warrant that the deliverables do not infringe any           */
/* third party intellectual property right (IPR). In the event that          */
/* the deliverables infringe a third party IPR it is the sole                */
/* responsibility of the customer to obtain necessary licenses to            */
/* continue the usage of the deliverable.                                    */
/*                                                                           */
/* To the maximum extent permitted by applicable law FSEU disclaims all      */
/* warranties, whether express or implied, in particular, but not            */
/* limited to, warranties of merchantability and fitness for a               */
/* particular purpose for which the deliverable is not designated.           */
/*                                                                           */
/* To the maximum extent permitted by applicable law, FSEU’s liability       */
/* is restricted to intentional misconduct and gross negligence.             */
/* FSEU is not liable for consequential damages.                             */
/*                                                                           */
/* (V1.5)                                                                    */
/******************************************************************************/
/** \file ajax.c
 **
 ** Handler functions to generate dynamic content for AJAX-enabled website
 **
 ** History:
 ** 2012-06-12 V0.1 CNo initial version
 ** 2012-10-01 V0.1 CNo Improved documentation
 ******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "httpd.h"
#include "ajax.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// defined in tasks.h
extern uint16_t u16ADCData;
extern uint32_t u32KeyData;
extern uint16_t u16LedVal;

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

///  Content of JSON file for AJAX operation. If you alter it, check output with http://jsonlint.com/ !
#define JSONSTRING "{\
\"adc\":\"%d\",\
\"key\":\"%d\",\
\"led\":\"%d\"\
}" , u16ADCData, u32KeyData, u16LedVal


const uint8_t * pu8SSITags[] =
{
  (const uint8_t *)"AJAXdata",
  (const uint8_t *)"ADC",    // The last three SSI Tags are not used for AJAX but the simple site
  (const uint8_t *)"KEY",
  (const uint8_t *)"LED"
};

tCGI CGIHandlers[] =
{
  {"/input.cgi", &Ajax_SetLEDsCGIHandler},
  {"/inputsimple.cgi", &Ajax_SetLEDsCGIHandlerSimple},
};

tCGI POSTHandlers[] =
{
	{"/inputpost.cgi", &Ajax_SetLEDsCGIHandler},
	{"/inputsimplepost.cgi", &SetLEDsPOSTHandlerSimplePost}
};

/**
 *****************************************************************************
 ** \brief Initialize CGI and SSI handlers for AJAX operation
 **
 *****************************************************************************/
void Ajax_Init(void){
  http_set_ssi_handler( &Ajax_GenerateData, (const char **) pu8SSITags, sizeof(pu8SSITags)/sizeof (uint8_t *));
  http_set_cgi_handlers(CGIHandlers, sizeof(CGIHandlers)/sizeof(tCGI));
  http_set_post_handlers(POSTHandlers, sizeof(POSTHandlers)/sizeof(tCGI));
}

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/
/**
 *****************************************************************************
 ** \brief Turn SSI tags into actual values
 **
 ** params and return value: See LwIP documentation
 **
 *****************************************************************************/
static uint16_t
Ajax_GenerateData(int iIndex, char *pcInsert, int iInsertLen)
{
  switch(iIndex)
  {
  case 0: // AJAX
    return (snprintf(pcInsert, iInsertLen, JSONSTRING));
    //break;
  case 1: // ADC
    return (snprintf(pcInsert, iInsertLen, "%d", u16ADCData));
    //break;
  case 2: // KEY
    return (snprintf(pcInsert, iInsertLen, "%d", u32KeyData));
    //break;
  case 3: // LED
    return (snprintf(pcInsert, iInsertLen, "%d", u16LedVal));
    //break;
  default:
    // SSI tag not supported
    return 0;
  }
}

/**
 *****************************************************************************
 ** \brief Iterate through the CGI parameters and set LED display value if a param name matches "ledval"
 **
 ** This function is bound to file index.html
 **
 ** params and return value: See LwIP documentation
 **
 *****************************************************************************/
const char * Ajax_SetLEDsCGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
  int32_t i = 0;
	static const uint8_t cgiretval[] = "/index.html";

  for(i=0; i < iNumParams; ++i)
  {
    if (0 == strcmp(pcParam[i], "ledval"))
    {
      u16LedVal = atoi(pcValue[i]);
    }
  }
  return (const char *) cgiretval;
}

/**
 *****************************************************************************
 ** \brief Iterate through the CGI parameters and set LED display value if a param name matches "ledval"
 **
 ** This function is bound to file simpe.html
 **
 ** params and return value: See LwIP documentation
 **
 *****************************************************************************/
const char * Ajax_SetLEDsCGIHandlerSimple(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
  int32_t i = 0;
	static const uint8_t cgiretval[] = "/simple.shtml";
  // Iterate through the parameters and set LED display value if a param name matches "ledval"
  for(i=0; i < iNumParams; ++i)
  {
    if (0 == strcmp(pcParam[i], "ledval"))
    {
      u16LedVal = atoi(pcValue[i]);
    }
  }
  return (const char *) cgiretval;
}

const char * SetLEDsPOSTHandlerSimplePost(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
  int32_t i = 0;
	static const uint8_t cgiretval[] = "/simple.shtml";
  // Iterate through the parameters and set LED display value if a param name matches "ledval"
  for(i=0; i < iNumParams; ++i)
  {
    if (0 == strcmp(pcParam[i], "ledval_post"))
    {
      u16LedVal = atoi(pcValue[i]);
    }
  }
  return (const char *) cgiretval;
}

