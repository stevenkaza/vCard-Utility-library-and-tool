VcStatus readVcard( FILE * const vcf, Vcard **const cardp)
{
    VcStatus newStatus; 
    char buff1[222];
    char  *buff;
    int nameFlag=0;
    int fnFlag = 0;
    int nFlag = 0;  
    int endFlag;
    char * verNum; 
    VcError error; 
    int versionFlag =0; /*If version flag stays 0, then we know no version was found */ 
    int tempIndex =0; 

    int i = 0; 
    VcProp * tempProp = NULL;
    VcProp * proppp=NULL; 
    int beginFlag=0; 

    /* How do access the individual vcards in here */ 

    if (vcf==NULL)
    {
        newStatus.code = IOERR; 
    }
     /*Checks for begin, and version  */ 
    do
    {
       // printf("%s\n",buff );
        /*do this check outside? */ 
         while (feof(vcf)!=0)
         {
            goto check;
         }

        newStatus=getUnfolded(vcf,&buff);
        /* If we see another begin before an end, return an error */ 
                                        
        if (beginFlag==1) 
        {
            if (strcmp("BEGIN:VCARD",buff)==0) 
            {
                newStatus.code =BEGEND; 
                goto end;
            }
        }
        /*Ensuring line begins with BEGIN */ 
        /* If not, set an error */ 
        if (beginFlag==0) 
        {
            if (strcmp("BEGIN:VCARD",buff)==0)
            {
                beginFlag=1;
            }
            else
            {
                newStatus.code = BEGEND;
                return newStatus;
                goto end;
            }
        }
        /* Checking for FN / N */ 

        if (buff[0]=='F')
        {
          if (buff[1]=='N')
          {
          //  printf("%s\n", buff);
            fnFlag=1; 
          }
        }
        if (buff[0]=='N')
        {
          if (buff[1]==';' || buff[1] == ':')
             nFlag=1; 
        }
        
       /* if we reach the end of the vcard, go to the  flag checks */ 
       if ((strcmp("END:VCARD",buff) ==0) && (beginFlag==1))
       {
        endFlag=1;
        goto check;
       }
       /* If the buff contains version, check that its the proper version number */ 

       if (strcasestr(buff,"VERSION:")!=NULL)
       {
         versionFlag=1;
         if ((buff[8] != '3') ||  (buff[9] != '.') || (buff[10]!='0'))
         {
           newStatus.code = BADVER; 
           return newStatus;
         }
       }
       if ((newStatus.code == OK) && (strcmp("END:VCARD",buff)!=0) &&
        (strcmp("BEGIN:VCARD",buff)!=0) && (strcmp("VERSION:3.0",buff)!=0))
       {
            /*Send the buff for parsing */
            buff[strlen(buff)]='\0';
            tempProp=malloc(sizeof(VcProp));
            error=parseVcProp(buff,tempProp);
            if (error!=OK)
            {
              printf("wtf\n");
              printf("cSPENCER CODE = %d\n",newStatus.code);
              printf("coder = %d\n",error );
              newStatus.code = error;
              return newStatus;

            }
            /* If we are on the first prop, we simply alloc for 1*/ 
            if (i==0)
            {
              (*cardp)=malloc(sizeof(Vcard)+sizeof(VcProp));
	            (*cardp)->nprops=1;	
            } 
            else
              (*cardp)=realloc((*cardp),sizeof(Vcard)+(sizeof(VcProp)*(i+1)));

            (*cardp)->prop[i]=*tempProp;

            i=i+1;
            free(buff);
         if (newStatus.code == BEGEND)
         {
            printf("Beggining not found \n");
            goto end; 
         }
      if (buff=='\0')
        {
            printf("BROKEN\n");
            break;
        }
       (*cardp)->nprops=i;

   }
 }while (strcmp("END:VCARD",buff)!=0);
      /* If we went through the entire vcard and couldnt find a FN, error */ 
    printf("fnFlag=%d\n",fnFlag );

    /* FLAG CHECKING FOR ERRORS */ 
    check:
    if (fnFlag ==0 || nFlag==0)
    {
      printf("fn not found\n");
      newStatus.code=NO;
    }

    if (endFlag==0 || beginFlag==0)
    {
      printf("end not found\n");
      newStatus.code=4;
    }
    if (versionFlag==0 ) /* No version was found */ 
    {
      newStatus.code =6;
    }
      
    //*buff=NULL;
    end:
    printf("right before, code = %d\n",newStatus.code );
    return newStatus;

}
