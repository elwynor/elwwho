/*****************************************************************************
 *   ELWWHO.C   V0.1                                              Who's On   *
 *                                                                           *
 *   Copyright (C) 2025 Rick Hadsall.  All Rights Reserved.                  *
 *                                                                           *
 *   Description:       Who's Online Global - With Module Filtering          *
 *                      Requested by Brian Morgan from The Penalty Box BBS   *
 *                                                                           *
 *                                                   - R. Hadsall 01/29/2025 *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify      * 
 * it under the terms of the GNU Affero General Public License as published  *
 * by the Free Software Foundation, either version 3 of the License, or      *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU Affero General Public License for more details.                       *
 *                                                                           *
 * You should have received a copy of the GNU Affero General Public License  *
 * along with this program. If not, see <https://www.gnu.org/licenses/>.     *
 *                                                                           *
 * Additional Terms for Contributors:                                        *
 * 1. By contributing to this project, you agree to assign all right, title, *
 *    and interest, including all copyrights, in and to your contributions   *
 *    to Rick Hadsall and Elwynor Technologies.                              *
 * 2. You grant Rick Hadsall and Elwynor Technologies a non-exclusive,       *
 *    royalty-free, worldwide license to use, reproduce, prepare derivative  *
 *    works of, publicly display, publicly perform, sublicense, and          *
 *    distribute your contributions                                          *
 * 3. You represent that you have the legal right to make your contributions *
 *    and that the contributions do not infringe any third-party rights.     *
 * 4. Rick Hadsall and Elwynor Technologies are not obligated to incorporate *
 *    any contributions into the project.                                    *
 * 5. This project is licensed under the AGPL v3, and any derivative works   *
 *    must also be licensed under the AGPL v3.                               *
 * 6. If you create an entirely new project (a fork) based on this work, it  *
 *    must also be licensed under the AGPL v3, you assign all right, title,  *
 *    and interest, including all copyrights, in and to your contributions   *
 *    to Rick Hadsall and Elwynor Technologies, and you must include these   *
 *    additional terms in your project's LICENSE file(s).                    *
 *                                                                           *
 * By contributing to this project, you agree to these terms.                *
 *                                                                           *
 *****************************************************************************/

#include "gcomm.h"
#include "majorbbs.h"
#include "elwwho.h"

/* function declarations */
VOID EXPORT init__elwwho(VOID);
GBOOL whoLogon(VOID);
VOID whoShutdown(VOID);
static GBOOL whoMain(VOID);

VOID showWhosOnGlobalHelp(VOID);
VOID showWhosOnGlobal(VOID);
INT elwwhoGlobals(VOID);
VOID showWhosOn(SHORT which, SHORT mode, CHAR *filterText);

GBOOL containsSubstring(const char *str,const char *sub);
GBOOL containsDefinedFilters(const char *str);
VOID trim(CHAR *str);
VOID setOtherUserPointers(VOID);

CHAR *getDayOfWeek(VOID);
CHAR *getCurrentDate(VOID);
CHAR *getCurrentTime(VOID);
CHAR *getBaud(VOID);
CHAR *getFlagChars(VOID);
CHAR *getFlagString(VOID);
CHAR *getModuleIn(VOID);
CHAR *getTimeOnline(VOID);
CHAR *getAverageAgeVALUE(SHORT filter);
CHAR *getAverageAgeFiltered(VOID);
CHAR *getAverageAgeAll(VOID);
CHAR *getLinesInUse(VOID);
INT calculateAge(CHAR *birthdate);

VOID setUserTextVariables(VOID);
VOID setSystemTextVariables(VOID);

CHAR *tv_userid(VOID);   
CHAR *tv_channel(VOID);    
CHAR *tv_age(VOID);
CHAR *tv_genderchar(VOID);
CHAR *tv_genderfull(VOID);
CHAR *tv_timeonline(VOID);
CHAR *tv_location(VOID);
CHAR *tv_flagchar(VOID);
CHAR *tv_flagstring(VOID);
CHAR *tv_modulein(VOID);
CHAR *tv_glochar(VOID);
CHAR *tv_baud(VOID);
CHAR *tv_class(VOID);
CHAR *tv_dayname(VOID);
CHAR *tv_curdate(VOID);
CHAR *tv_curtime(VOID);
CHAR *tv_linesinuse(VOID);
CHAR *tv_avgage(VOID);
CHAR *tv_avgageall(VOID);
CHAR *tv_sysload(VOID);

VOID EXPORT end_elwwho(VOID);

#define VERSION "0.1"

static INT elwwhostt;        /* module state                         */
struct module elwwho={       /* module interface block               */
     "",                      /*    name used to refer to this module */
     whoLogon,                /*    user logon supplemental routine   */
     whoMain,                 /*    input routine if selected         */
     NULL,                    /*    status-input routine if selected  */
     NULL,                    /*    "injoth" routine for this module  */
     NULL,                    /*    user logoff supplemental routine  */
     NULL,                    /*    hangup (lost carrier) routine     */
     NULL,                    /*    midnight cleanup routine          */
     NULL,                    /*    delete-account routine            */
     whoShutdown              /*    finish-up (sys shutdown) routine  */
};

/* Level 4 CNF Options */          
GBOOL      showatlogon;       /* Display Who's On at logon?           */
CHAR       glochr;            /* Global command character             */
SHORT      logonlistnum;      /* Which of the lists at logon          */

CHAR       *filterstring1,    /* Filter-out string #1                 */
           *filterstring2,    /* Filter-out string #2                 */
           *filterstring3,    /* Filter-out string #3                 */
           *filterstring4,    /* Filter-out string #4                 */
           *filterstring5;    /* Filter-out string #5                 */

/* end CNF options */

// define MSG and DAT file variables
static HMCVFILE elwwhomsg;       /* file variable for elwwhoo.msg        */

/* text variables for message/email/log file use */
CHAR *tvUserid,
     *tvChannel,
     *tvAge,
     *tvGenderChar,
     *tvGenderFull,
     *tvTimeOnline,
     *tvLocation,
     *tvFlagChar,
     *tvFlagString,
     *tvModuleIn,
     *tvBaud,
     *tvClass,
     *tvDayName,
     *tvCurDate,
     *tvCurTime,
     *tvLinesInUse,
     *tvAvgAge,
     *tvAvgAgeAll,
     *tvSysLoad,
     *tvGloChar;

// define mode macros     
#define NORMAL 0     // normal mode - filtered by SysOp, no extended filter
#define EXTEND 1     // extended mode - filtered by SysOp PLUS user filter
#define REPLACE 2    // replacement mode - filtered only by user filter
#define UNFILTERED 3 // unfiltered mode - show everyone, no filters

// other declarations
CHAR *flag_vals[] = {"Sys","Bsy","Inv"};

CHAR *month[]={    
     "January",
     "February",
     "March",
     "April",
     "May",
     "June",
     "July",
     "August",
     "September",
     "October",
     "November",
     "December"
};

CHAR *dayname[] = {
     "Sunday",
     "Monday",
     "Tuesday",
     "Wednesday",
     "Thursday",
     "Friday",
     "Saturday"
};

// #define showable ((SHORT *)vdaptr) // use to see if the channel is displayable or not (used for aggregate functions) - Removed as usage of VDA not required
static SHORT showable_array[256];
#define showable showable_array

//
// end declaration section, begin code
//

VOID EXPORT init__elwwho(VOID)
{
  
  /* open message file */
  elwwhomsg=opnmsg("elwwho.mcv"); /* message file */

  stzcpy(elwwho.descrp,gmdnam("ELWWHO.MDF"),MNMSIZ);
  elwwhostt=register_module(&elwwho);
  shocst(spr("ELW Who's On v%s",VERSION),"(C) Copyright 2025 Elwynor - www.elwynor.com");

  /* setup variables tied to CNF options */
  
  showatlogon  =ynopt(SHOWLON);         /* Do we want to show the list at logon?*/
  logonlistnum =(SHORT)numopt(WHICHLST,1,4);   /* Which logon list do we want to show? */
  glochr       =(CHAR)chropt(GLOCHR);          /* Global command character             */
  filterstring1=stgopt(FILTER1);        /* Filter string #1                     */
  filterstring2=stgopt(FILTER2);        /* Filter string #2                     */
  filterstring3=stgopt(FILTER3);        /* Filter string #3                     */
  filterstring4=stgopt(FILTER4);        /* Filter string #4                     */
  filterstring5=stgopt(FILTER5);        /* Filter string #5                     */
  trim(filterstring1);                  /* Remove extraneous front/back spaces  */
  trim(filterstring2);                  /* Remove extraneous front/back spaces  */
  trim(filterstring3);                  /* Remove extraneous front/back spaces  */
  trim(filterstring4);                  /* Remove extraneous front/back spaces  */
  trim(filterstring5);                  /* Remove extraneous front/back spaces  */

  globalcmd(elwwhoGlobals);             /* hook the global routine in           */

  /* register text variables */
  register_textvar("WHO_USERID",tv_userid);
  register_textvar("WHO_CHANNEL",tv_channel);
  register_textvar("WHO_AGE",tv_age);
  register_textvar("WHO_GENDERCHAR",tv_genderchar);
  register_textvar("WHO_GENDERFULL",tv_genderfull);
  register_textvar("WHO_TIMEONLINE",tv_timeonline);
  register_textvar("WHO_LOCATION",tv_location);
  register_textvar("WHO_FLAGCHAR",tv_flagchar);
  register_textvar("WHO_FLAGSTRING",tv_flagstring);
  register_textvar("WHO_MODULEIN",tv_modulein);
  register_textvar("WHO_BAUD",tv_baud);
  register_textvar("WHO_CLASS",tv_class);
  register_textvar("WHO_DAYNAME",tv_dayname);
  register_textvar("WHO_CURDATE",tv_curdate);
  register_textvar("WHO_CURTIME",tv_curtime);
  register_textvar("WHO_LINESINUSE",tv_linesinuse);
  register_textvar("WHO_AVGAGE",tv_avgage);
  register_textvar("WHO_AVGAGEALL",tv_avgageall);
  register_textvar("WHO_SYSLOAD",tv_sysload);
  register_textvar("WHO_GLOCHAR",tv_glochar);
  tvUserid        =(CHAR *)alczer(UIDSIZ);
  tvChannel       =(CHAR *)alczer(3);
  tvAge           =(CHAR *)alczer(4);
  tvGenderChar    =(CHAR *)alczer(2);
  tvGenderFull    =(CHAR *)alczer(7);
  tvTimeOnline    =(CHAR *)alczer(6);
  tvLocation      =(CHAR *)alczer(NADSIZ);
  tvFlagChar      =(CHAR *)alczer(5); /* SBI */
  tvFlagString    =(CHAR *)alczer(12);  /* Sys,Bsy,Inv */
  tvModuleIn      =(CHAR *)alczer(MNMSIZ);
  tvBaud          =(CHAR *)alczer(10);
  tvClass         =(CHAR *)alczer(KEYSIZ);
  tvDayName       =(CHAR *)alczer(10); // Wednesday
  tvCurDate       =(CHAR *)alczer(19); // September ##, ####
  tvCurTime       =(CHAR *)alczer(12); // hh:mm:ss am
  tvLinesInUse    =(CHAR *)alczer(5);
  tvAvgAge        =(CHAR *)alczer(4);
  tvAvgAgeAll     =(CHAR *)alczer(4);
  tvSysLoad       =(CHAR *)alczer(5);  
  tvGloChar       =(CHAR *)alczer(2);
 
  strcpy(tvGloChar,spr("%c",glochr));
  // end initialization routine
}

/* text variable routines */
CHAR *tv_userid(VOID)       { return(tvUserid);     }
CHAR *tv_channel(VOID)      { return(tvChannel);    }
CHAR *tv_age(VOID)          { return(tvAge);        }
CHAR *tv_genderchar(VOID)   { return(tvGenderChar); }
CHAR *tv_genderfull(VOID)   { return(tvGenderFull); }
CHAR *tv_timeonline(VOID)   { return(tvTimeOnline); }
CHAR *tv_location(VOID)     { return(tvLocation);   }
CHAR *tv_flagchar(VOID)     { return(tvFlagChar);   }
CHAR *tv_flagstring(VOID)   { return(tvFlagString); }
CHAR *tv_modulein(VOID)     { return(tvModuleIn);   }
CHAR *tv_baud(VOID)         { return(tvBaud);       }
CHAR *tv_class(VOID)        { return(tvClass);      }
CHAR *tv_dayname(VOID)      { return(tvDayName);    }
CHAR *tv_curdate(VOID)      { return(tvCurDate);    }
CHAR *tv_curtime(VOID)      { return(tvCurTime);    }
CHAR *tv_linesinuse(VOID)   { return(tvLinesInUse); }
CHAR *tv_avgage(VOID)       { return(tvAvgAge);     }
CHAR *tv_avgageall(VOID)    { return(tvAvgAgeAll);  }
CHAR *tv_sysload(VOID)      { return(tvSysLoad);    }
CHAR *tv_glochar(VOID)      { return(tvGloChar);    }

// Module Routines

// whoLogon - user logon method                   
GBOOL EXPORT whoLogon(VOID)
{
  if (showatlogon) showWhosOn(logonlistnum,NORMAL,NULL);
  return(0);
}

//whoShutdown(void) - bbs shutdown routine - close open files
VOID  EXPORT whoShutdown(VOID)
{
  clsmsg(elwwhomsg);
}

//  main input handler for the module
static GBOOL EXPORT whoMain(VOID)                                     
{
  showWhosOn(1,NORMAL,NULL);
  return(0);
}


// Text Variable and Utility Routines

// remove leading and trailing spaces
VOID trim(CHAR *str) {
    CHAR *start, *end;

    /* NULL check */
    if (str == NULL || *str == '\0') {
        return;
    }

    /* Move start to first non-whitespace character */
    start = str;
    while (*start && (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r')) {
        start++;
    }

    /* If the entire string was whitespace, return an empty string */
    if (*start == '\0') {
        *str = '\0';
        return;
    }

    /* Move end to the last non-whitespace character */
    end = start;
    while (*end) {
        end++;
    }
    end--;  /* Step back to the last valid character */

    while (end > start && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        end--;
    }

    /* Null-terminate the trimmed string */
    *(end + 1) = '\0';

    /* Shift the trimmed string to the original buffer */
    if (start != str) {
        char *dst = str;
        while (*start) {
          *dst = *start;
          dst++;
          start++;
        }
        *dst = '\0'; /* Ensure proper null termination */
    }
}

// does a particular string contain passed substring?
GBOOL containsSubstring(const char *str,const char *sub) {
    int i, strLen, subLen;

    if (str == NULL) return 0;
    if (str == 0) return 0;
      
    if (sub == NULL) return 0;
    if (sub == 0) return 0;

    strLen = strlen(str);
    subLen = strlen(sub);
    
    if ((strLen == 0) || (subLen == 0)) return 0;
      
    for (i = 0; i <= strLen - subLen; i++) {
        int j;
        for (j = 0; j < subLen; j++) {
            if (str[i + j] != sub[j]) {
                break;
            }
        }
        if (j == subLen) {
            return 1; // Substring found
        }
    }

    return 0; // Substring not found
}

INT EXPORT calculateAge(CHAR *birthdate)
{
#if defined(IS30) || defined(__BUILDV10MODULE)
    return calcage(birthdate);
#else
  int m, d, y, today_date, retval;

  today_date = today();
  sscanf(birthdate,"%d/%d/%d",&m,&d,&y);
  retval = ((ddyear(today_date)-1900)-y)-
           ((ddmon(today_date) < m || (ddmon(today()) == m &&
             ddday(today_date) < d)) ? 1 : 0);
  return (retval > 0) ? retval : 0;
#endif
}

CHAR * EXPORT getFlagChars(VOID)
{
  CHAR *tmp= (CHAR *)alczer(4); //0;
  tmp[0] = '\0';

  stzcpy(tmp, "---", 4);
  if (othusp->flags&MASTER) tmp[0] = *flag_vals[0]; // S for SysOp
  if (othusp->flags&NOINJO) tmp[1] = *flag_vals[1]; // B for Busy
  if (othusp->flags&INVISB) tmp[2] = *flag_vals[2]; // I for Invisible
   
  return tmp;
}

CHAR * EXPORT getFlagString(VOID)
{
  CHAR *tmp = (CHAR *)alczer(12);
  tmp[0] = '\0';

  if (othusp->flags&MASTER) strcat(tmp, flag_vals[0]);
  if (othusp->flags&NOINJO) strcat(tmp, (*tmp == 0) ? flag_vals[1] : spr(",%s",flag_vals[1]));
  if (othusp->flags&INVISB) strcat(tmp, (*tmp == 0) ? flag_vals[2] : spr(",%s",flag_vals[2]));

  return tmp;
}

CHAR * EXPORT getModuleIn(VOID)
{
  CHAR *tmp=(CHAR *)alczer(MNMSIZ);
  tmp[0] = '\0';
  
  switch(othusp->usrcls) {
        case VACANT:
             stzcpy(tmp,"VACANT",MNMSIZ);
             break;
        case ONLINE:
             stzcpy(tmp,"(log-on)",MNMSIZ);
             break;
        case BBSPRV:
             stzcpy(tmp,"(private)",MNMSIZ);
             break;
        case SUPIPG:
             stzcpy(tmp,"(sign-up)",MNMSIZ);
             break;
        case SUPLON:
             stzcpy(tmp,"(log-on)",MNMSIZ);
             break;
        case SUPLOF:
             stzcpy(tmp,"(log-off)",MNMSIZ);
             break;
        case ACTUSR:
             stzcpy(tmp, module[othusp->state]->descrp,MNMSIZ);
             break;
        default:
             stzcpy(tmp,"(unknown)",MNMSIZ);
             break;
   }

   return tmp;
}

CHAR * EXPORT getTimeOnline(VOID)
{
  return(spr("%d",(othusp->minut4+2)/4));
}

CHAR * EXPORT getCurrentDate(VOID)
{
    static char dateBuffer[32]; 
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);

    if (tm_info) {
#ifdef __BUILDV10MODULE
        snprintf(dateBuffer, sizeof(dateBuffer), "%s %d, %d", month[tm_info->tm_mon], tm_info->tm_mday, tm_info->tm_year + 1900);
#else
        spr(dateBuffer,"%s %d, %d", month[tm_info->tm_mon], tm_info->tm_mday, tm_info->tm_year + 1900);
#endif
    }
    else {
#ifdef __BUILDV10MODULE
        snprintf(dateBuffer, sizeof(dateBuffer), "Unknown Date");
#else
        spr(dateBuffer, "Unknown Date");
#endif
    }

    return dateBuffer;

/*  struct date d;
  
  getdate(&d);

  if ((d.da_mon > 0) && (d.da_mon < 13)) {
    return spr("%s %d, %d", month[d.da_mon-1], d.da_day, d.da_year);
  }
  
  return "";    */

}

CHAR * EXPORT getCurrentTime(VOID)
{
    static char timeBuffer[16];
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);

    if (tm_info) {
        int hour = tm_info->tm_hour;
        int isAM = (hour < 12);

        if (hour == 0) hour  = 12; 
        if (hour > 12) hour -= 12;

#ifdef __BUILDV10MODULE
        snprintf(timeBuffer, sizeof(timeBuffer), "%02d:%02d:%02d %s", hour, tm_info->tm_min, tm_info->tm_sec, (isAM) ? "AM" : "PM");
#else
        spr(timeBuffer, "%02d:%02d:%02d %s", hour, tm_info->tm_min, tm_info->tm_sec, (isAM) ? "AM" : "PM");
#endif
    } else {
#ifdef __BUILDV10MODULE
        snprintf(timeBuffer, sizeof(timeBuffer), "Unknown Time");
#else
        spr(timeBuffer, "Unknown Time");
#endif
    }

    return timeBuffer;
/*
  struct time t;
  SHORT isAM;
  INT hour;

  gettime(&t);
  isAM = ((hour = t.ti_hour) < 12);

  if (hour == 0) hour = 12;
  if (hour > 12) hour -=12;

  return spr("%02d:%02d:%02d %s",hour,t.ti_min,t.ti_sec,(isAM) ? "am":"pm");
*/
}

CHAR * EXPORT getDayOfWeek(VOID)
{
  SHORT day=daytoday();
  
  if (day >= 0 && day < 7) {
    return dayname[day];
  } 
  
  return "";
}

CHAR * EXPORT getBaud(VOID)
{
#if defined(IS30) || defined(__BUILDV10MODULE)
  return l2as(othusp->baud);
#else
  return l2as(othexp->baud);
#endif
}

// return the average age of all signed-in users 
// if filter=TRUE, it will only calc for in-scope users
CHAR * EXPORT getAverageAgeValue(SHORT filter)
{
  int i;
  int ageSum=0, 
      userCount=0;

  for (i=0; i<nterms; i++) {
    if (usroff(i)->usrcls>ONLINE)
    {
      if ( (showable[i] == TRUE) || (filter == FALSE) ) {
        if (uacoff(i)->age > 0) {
          ageSum += uacoff(i)->age;
          userCount++;
        }         
      }
    }
  }
  
  if ((ageSum == 0) || (userCount==0)) {
    return "0";
  }
  
  return spr("%d", ageSum/userCount);
}

CHAR * EXPORT getAverageAgeFiltered(VOID)
{
   return getAverageAgeValue(TRUE);
}

CHAR * EXPORT getAverageAgeAll(VOID)
{
   return getAverageAgeValue(FALSE);
}

// returns total number of lines in use, regardless of filter
CHAR * EXPORT getLinesInUse(VOID)
{
  int i, linesActive=0;

  for (i=0; i<nterms; i++) {
    if (usroff(i)->usrcls!=VACANT)
    {
      linesActive++;
    }
  }
  
  return spr("%d",linesActive);
}

VOID EXPORT setSystemTextVariables(VOID)
{
   // this should be called each time the global is invoked, once
   stzcpy(tvDayName   ,getDayOfWeek()  ,10);
   stzcpy(tvCurDate   ,getCurrentDate(),19);
   stzcpy(tvCurTime   ,getCurrentTime(),12);
   stzcpy(tvLinesInUse,getLinesInUse() ,5);
   stzcpy(tvAvgAge    ,getAverageAgeFiltered() ,4);
   stzcpy(tvSysLoad   ,spr("%u%",syslod),5);
}

VOID EXPORT setUserTextVariables(VOID)
{

   // for each user this should be called
   stzcpy(tvUserid    ,othuap->userid   ,UIDSIZ);
   stzcpy(tvChannel   ,strupr(spr("%02x",channel[othusn])),3);
   stzcpy(tvAge       ,spr("%d",calculateAge(othuap->birthd)),3);
   stzcpy(tvGenderChar,spr("%c",othuap->sex),2);
   stzcpy(tvGenderFull,othuap->sex == 'M' ? "Male\0" : othuap->sex == 'F' ? "Female\0" : "Other\0",7);
   stzcpy(tvTimeOnline,getTimeOnline() ,6);
   stzcpy(tvLocation  ,othuap->usrad3  ,NADSIZ);
   stzcpy(tvFlagChar  ,getFlagChars()  ,4); 
   stzcpy(tvFlagString,getFlagString() ,12);
   stzcpy(tvModuleIn  ,getModuleIn()   ,MNMSIZ);
   stzcpy(tvBaud      ,getBaud()       ,10);
   stzcpy(tvClass     ,othuap->curcls  ,KEYSIZ);
}

// Global Command Routines

GBOOL containsDefinedFilters(const char *str) {
   
   return containsSubstring(str,filterstring1) || containsSubstring(str,filterstring2) || containsSubstring(str,filterstring3) || containsSubstring(str,filterstring4) || containsSubstring(str,filterstring5);
}

GBOOL EXPORT okayToShow(SHORT mode, CHAR *filterText) 
{

  switch (othusp->usrcls) {
    case VACANT:
    case BBSPRV:
    case SUPLON:
    case SUPLOF:
      return FALSE;    // we don't want to show anything for vacant channels, private system logins (majorlink, web, etc.) or supplemental login/logoff processes
  }
  
  if ( (othusp->flags&INVISB) && (!(usrptr->flags&MASTER)) ) return FALSE; // if they're invisible, you can't see them, unless you're master

  if (mode == NORMAL)  return !containsDefinedFilters(module[othusp->state]->descrp);

  if (mode == REPLACE) return      !containsSubstring(module[othusp->state]->descrp, filterText);

  if (mode == EXTEND)  return      !containsSubstring(module[othusp->state]->descrp, filterText) && 
                              !containsDefinedFilters(module[othusp->state]->descrp); 
  
  return TRUE; // this should be unreachable
}

VOID EXPORT setOtherUserPointers(VOID) 
{
#if defined(IS30) || defined(__BUILDV10MODULE)
  othusp = usroff(othusn);
#else
  othusp = &user[othusn];
  othexp = extoff(othusn);
#endif
  othuap = uacoff(othusn);
}

VOID EXPORT showWhosOn(SHORT which,SHORT mode,CHAR *filterText) 
{
  if ( (which < 1) || (which > 4) ) which=1; // 1-4 is the valid set for which, so #1 is default

  setmbk(elwwhomsg);               // set the message block to ELWWHO.MSG

  // Missing the filter? If so, rebuff and exit
  if ( ( (mode == EXTEND) || (mode == REPLACE) ) && ( NULL == filterText ) ) {
    prfmsg(NOFILTER);
    outprf(usrnum);
    rstmbk();
    return;
  } 

  // reset the channel's vda showable array
  //memset(showable, 0, nterms * sizeof(SHORT));
  memset(showable_array, 0, sizeof(showable_array));

  // repopulate the array  
  for (othusn=0 ; othusn < nterms ; othusn++ ) {
    setOtherUserPointers();
    showable[othusn] = okayToShow(mode, filterText);    
  }
  
  setSystemTextVariables();

  prfmlt(LISTHDR1+(which-1));
  
  for (othusn=0 ; othusn < nterms ; othusn++ ) {
    if (showable[othusn] == TRUE) {
      setOtherUserPointers();
      setUserTextVariables();
      prfmlt(LISTLIN1+(which-1));        
    }
  }
  
  prfmlt(LISTFTR1+(which-1));  
  outprf(usrnum);
  rstmbk();   
}


// print the help for globals
VOID  EXPORT showGlobalHelp(VOID) 
{
   setmbk(elwwhomsg);
   prfmlt(WHOHELP);
   outprf(usrnum);
   rstmbk();
}

// implements all the globals - redirects to routines for each
INT  EXPORT elwwhoGlobals(VOID) 
{

  if (sameas(margv[0], spr("%c?",glochr))) {
		showGlobalHelp();
		return(1);
  }		
  else if (sameas(margv[0], spr("%c",glochr))) {
		showGlobalHelp();
		return(1);
  }		
  else if (sameas(margv[0], spr("%c1",glochr))) {
		showWhosOn(1,NORMAL,NULL);
		return(1);
  }		
  else if (sameas(margv[0], spr("%c2",glochr))) {
		showWhosOn(2,NORMAL,NULL);
		return(1);
  }		
  else if (sameas(margv[0], spr("%c3",glochr))) {
		showWhosOn(3,NORMAL,NULL);
		return(1);
  }		
  else if (sameas(margv[0], spr("%c4",glochr))) {
		showWhosOn(4,NORMAL,NULL);
		return(1);
  }		
  else if (sameas(margv[0], spr("%cU1",glochr))) {
		showWhosOn(1,NORMAL,NULL);
		return(1);
  }		
  else if (sameas(margv[0], spr("%cU2",glochr))) {
		showWhosOn(2,NORMAL,NULL);
		return(1);
  }		
  else if (sameas(margv[0], spr("%cU3",glochr))) {
		showWhosOn(3,NORMAL,NULL);
		return(1);
  }		
  else if (sameas(margv[0], spr("%cU4",glochr))) {
		showWhosOn(4,NORMAL,NULL);
		return(1);
  }		
  else if (sameas(margv[0], spr("%cU",glochr))) {
		showWhosOn(1,NORMAL,NULL);
		return(1);
  }		
  else if (sameas(margv[0], spr("%cE1",glochr))) {
     if (margc < 2) {
		showGlobalHelp();
		return(1);
	  }
	  showWhosOn(1,EXTEND,margv[1]);
	  return(1);
  }		
  else if (sameas(margv[0], spr("%cE2",glochr))) {
     if (margc < 2) {
		showGlobalHelp();
		return(1);
	  }
	  showWhosOn(2,EXTEND,margv[1]);
	  return(1);
  }		
  else if (sameas(margv[0], spr("%cE3",glochr))) {
     if (margc < 2) {
		showGlobalHelp();
		return(1);
	  }
	  showWhosOn(3,EXTEND,margv[1]);
	  return(1);
  }		
  else if (sameas(margv[0], spr("%cE4",glochr))) {
     if (margc < 2) {
		showGlobalHelp();
		return(1);
	  }
	  showWhosOn(4,EXTEND,margv[1]);
	  return(1);
  }		
  else if (sameas(margv[0], spr("%cE",glochr))) {
     if (margc < 2) {
		showGlobalHelp();
		return(1);
	  }
	  showWhosOn(1,EXTEND,margv[1]);
	  return(1);
  }		
  else if (sameas(margv[0], spr("%cF1",glochr))) {
     if (margc < 2) {
		showGlobalHelp();
		return(1);
	  }
	  showWhosOn(1,REPLACE,margv[1]);
	  return(1);
  }		
  else if (sameas(margv[0], spr("%cF2",glochr))) {
     if (margc < 2) {
		showGlobalHelp();
		return(1);
	  }
	  showWhosOn(2,REPLACE,margv[1]);
	  return(1);
  }		
  else if (sameas(margv[0], spr("%cF3",glochr))) {
     if (margc < 2) {
		showGlobalHelp();
		return(1);
	  }
	  showWhosOn(3,REPLACE,margv[1]);
	  return(1);
  }		
  else if (sameas(margv[0], spr("%cF4",glochr))) {
     if (margc < 2) {
		showGlobalHelp();
		return(1);
	  }
	  showWhosOn(4,REPLACE,margv[1]);
	  return(1);
  }		
  else if (sameas(margv[0], spr("%cF",glochr))) {
     if (margc < 2) {
		showGlobalHelp();
		return(1);
	  }
	  showWhosOn(1,REPLACE,margv[1]);
	  return(1);
  }		
	
  return(0);	
}

VOID EXPORT end_elwwho(VOID) {}
