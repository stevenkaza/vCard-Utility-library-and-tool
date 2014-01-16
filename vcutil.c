#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vcutil.h"
#include "helper.h"

/*
 vcStatusReadVcFILE( FILE * const, vcf, VcFile * const file) ;

/* Precondition: vcf has been opened for reading,
 and is assumed to be positioned at beginning-of-life.
 filep  points to the VcFile structure to be filled in (its current contents is ignored) */

/* Post condition
   If status=OK, the file was read to normal EOF and is available in memory for access via the filep pointer. VcStatus line numbers give the total lines read from the file.  The caller is respnsibile for eventually
freeing the contents of filep  with the help of freeVcFile. If status is not OK, VcStatus contains both the error code and the line numbers in the file where the error was detectedm, and the VcFile contents (*cardp) has already been fired.             */

/*Create a vcard in readVcFile  */
/*Read vcard will know if the struct has been filled in */
VcStatus readVcFile (FILE *const vcf, VcFile *const filep)
{

        filep->ncards = 0;
        filep->cardp = NULL;
        VcStatus newStatus;


        if (vcf==NULL)
        {
                printf("file pointer NULL \n");
                newStatus.code =  IOERR;
        }

        newStatus = readVcard(vcf,filep->cardp);
        if (newStatus.code == OK)
            printf("File was parsed \n");
        else
            printf("Houston, we have a problem \n");

        return newStatus;


        /*Now we have to figure out what we do with each vcard and the flow of the program */

}

/* Precondition: 
   vcf is open for reading. cardp points to 
   a variable where the address of a newly allocated Vcard will be returned.

   Postcondition:  
    If status=OK, and * cardp is non-NULL,
    the Vcard structure has been filled in. OK status and NULL *
    cardp means normal EOF, and VcStatus line numbers are the total lines read from the file.
    If error status, * cardp contents are undefined (but no additional storage remains allocated). */

VcStatus readVcard( FILE * const vcf, Vcard **const cardp)
{
    VcStatus newStatus; 
    char * buff;
    int state = 0; 
    
    if (vcf==NULL)
        newStatus.code = IOERR; 
     /*Checks for begin, and version  */ 
    while (state==0)
    {
        newStatus=getUnfolded(vcf,&buff);
        if (newStatus.code == OK)
        {
            /*Send the buff for parsing */
            printf("Parsing Buffer %s \n",buff); 

        }
        else if (newStatus.code == BEGEND)
            printf("Beggining not found \n");
        if (buff=='\0')
        {
            state =1; 
            printf("BROKEN\n");
            break;
        }
    }
    return newStatus;

}

VcStatus getUnfolded ( FILE * const vcf, char **const buff )
{
    /* For each line in the vcard, unfold */
    /*Its only returning the line that begins with the whitespace.... Why? */ 
    /* Should only assign to the buff once it has been unfolded */ 
    int position; 
    char lineAhead[200];
    static char staticLine[200];
    char  currentLine[200]; 
    char lineBefore[100];

    static char nextLine[75];
    int result = 0; 
    int foldedState;  /* 0 if line is unfolded ok, 1 if needs to be folded */ 
                char *pos;

    VcStatus newStatus; 
    FILE  * copy; 
    /* The first time the program runs, ideally we call fgets twice, 
    for currentLine and for lineAhead
    
       I then check to see if the lineAhead is a folded one.
       If it is, remove spaces, strcat it to the current line, 
       assign it to buff, and return it. Also I grab the next line and assign
       it to staticLine. This method is unique so thats why It checks to see
        that staticLine has a strlen at zero at first.

      In the event that the program is called and staticLine has a strlen greater than zero,
      a similar thing takes place. The next line is grabbed and checked to see if it is folded, if it is wrap it around, otherwise, 
      return it as as to buff */

        if (strlen(staticLine)>0)
        {
           if(fgets(lineAhead,75,vcf)==NULL)
           {
             *buff = NULL;
             return newStatus; 
           }
            if (isFolded(lineAhead)==1)
            {
                printf("wow = %d",removeNewLine(staticLine));
             //   if ((pos=strchr(staticLine, '\r')) != NULL)
                //    *pos = '\0';
                staticLine[strlen(staticLine)-1] = '\0';

                removeSpaces(lineAhead);

                strncat(staticLine,lineAhead,strlen(lineAhead)-1);
            }
        }

        else /*First time function is called */ 
        {
            if(fgets(staticLine,75,vcf)==NULL)
            {
                *buff = NULL;
                return newStatus; 
            }
            if(fgets(lineAhead,75,vcf)==NULL)
            {
                *buff = NULL;
                return newStatus; 
            }
            if (isFolded(lineAhead)==1)
            {
              printf("wow = %d",removeNewLine(staticLine));

               staticLine[strlen(staticLine)] = '\0';
                removeSpaces(lineAhead);
                strncat(staticLine,lineAhead,strlen(lineAhead)-1);
            }
        }

        *buff = (char*)calloc(strlen(staticLine),sizeof(char));
        strcpy(*buff,staticLine);
        staticLine[0]='\0';
        strcpy(staticLine,lineAhead);
        newStatus.code= OK;
        return newStatus;
   
    








/*








   
    if (foldedState==1)
    {
        if (strlen(staticLine>0))
            strcpy(*buff,staticLine); /*This Might Leak 
        
        foldedState=0; /* Buff should now  be unfolded, but the next line also might hav a white space  

    }

    while (foldedState!=1 || *buff!=NULL)
    {
        /* Scanning for unfolded *
        fgets(*buff,100,vcf);
        strcpy(staticLine,*buff);
        copy = vcf; 
        /* If line is OK, return it 
        /* how do we know if a line is OK */
        /* If the next one doesnt start with a whitespace :P 
        fgets(*buff,75,copy);
        /* Checking the next line 
        if (*buff[0]==' ')
        {
            printf("This line is a folder! ");
            foldedState = 1; 
        }
    }

    if (foldedState==1)
    {
        removeSpaces(*buff);
        /* Copying buff into a static line, meaning its time to fuck around with *buff and set
        it back to static line  in a recursive call 
        strncat(staticLine,buff,sizeof(buff));
        /*EXIT INDICATING THAT A LINE NEEDS TO BE FOLDED */ 
        /* Recursive call to do the folding 
        return VcStatus;

    }
    
*/



    

}



VcError parseVcProp ( const char * buff, VcProp * const propp)
{
    /* String Matching */ 
}

void freeVcFile ( VcFile * const filep)
{
}

/* int Contains(char * string, char pattern);
 * Returns: position where pattern was found 
 * 0 if nothing found                   */ 
int Contains(char * string, char  pattern)
{
    int i =0;
    if (string=='\0')
        return 0;
    for (i=0;i<strlen(string);i++)
    {
        if (string[i]==pattern)
        {
            return i; 
        } 
    }
    return 0; 
}
/* Returns a 1 if a pattern is found at 
  a certain location */
int checkPosition(char * string, int position)
{
    int i = 0; 
    if ((string[position+1] == '\n'))// && (string[position+2]==' '))
    {
        return 1; 
    }
    return 0; 
}


void removeSpaces(char * string)  /*Function to remove spaces, found from  http://roseindia.net/c-tutorials/c-string-remove-spaces.shtml */ 
{
        

  char *p1 = string;
  char *p2 = string;
  p1 = string;
 while(*p1 != 0) 
  {
         
  if(isspace(*p1)) /*if a space is found, rearange the pointers so the spaces are ignored */ 
  {
    ++p1;
  }
  else
   *p2++ = *p1++; 
  }
  *p2 = 0; 


}

int isFolded(char * string)
{
    if ((string[0]==' ') || (string[0] == '\t'))
        return 1; 
    return 0; 
}

int removeNewLine(char * string)
{
    int i = 0; 
    for (i=0;i<=strlen(string);i++)
    {
        if (string[i]=='\n')
        {
            string[i]='\0';
            return 1; 
        }

    }
    return 0; 
}