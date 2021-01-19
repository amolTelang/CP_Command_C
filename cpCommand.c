//c program to implement cp command in c


// header files
#include<stdio.h>//standard library
#include<string.h>//string functions
#include<stdlib.h>//exit() system call
#include<fcntl.h>//open() system call
#include<unistd.h>//gives access to posix api // unix stanadrd api


//function prototypes
void copy();		//copy() function to copy files
FILE *src,*dest;//file poimter to source and destination

//main function
int main(int argc,char *argv[])
{
	char ch,response[50];		//character array to hold the response of user
	int fd,fileExist;			//flags to hold file descriptor and fileExist flags
			
	if(argc<3)					//less than 3 arguments are given in command line		
	{
		printf("insufficient argument\n");
		printf("\nUSAGE ./a.out src_file dest_file\n");
		printf("\nUSAGE ./a.out -i src_file dest_file\n");
		printf("\nUSAGE ./a.out -n src_file dest_file\n");
		printf("\nUSAGE ./a.out --remove-destination src_file dest_file\n");
		printf("\nUSAGE ./a.out -f src_file dest_file\n");
		printf("\nUSAGE ./a.out -v src_file dest_file\n");
		printf("\nUSAGE ./a.out -l src_file dest_file\n");
		exit(0);	
	}
	//end if

	if(argc==3)					//if arguments passed is 3
	{
		src=fopen(argv[1],"r");			//open source file in read only mode
		if(src==NULL)					//if source file does not exist
		{
			printf("cp:cannot stat '%s':NO such file or directory\n",argv[1] );
			exit(0);
		}
		//end if
		fd=open(argv[2],O_WRONLY);		//open file in read only mode and return file discriptor
		if(fd==-1)
		{
			printf("destination is not a file\n");
			exit(0);
		}
		//end if
		else
		{
			dest=fopen(argv[2],"w");		//destination file opened in write mode
			copy();//function call to copy the contents of src_file to dest_file
		}
		//end else
	
	}
	//end if

	if(argc==4)						//if no of argument passed is 4
	{
		src=fopen(argv[2],"r");		//open source file in read only mode
		if(src==NULL)				//if no soucrce file does not exist
		{	

			printf("cp:cannot stat '%s':No such file or directory\n",argv[2]);
			exit(0);
		}
		//end if
		if(strcmp(argv[1],"-i")==0 || strcmp(argv[1],"--interactive")==0)	//if -i option is passed
		{	
			fileExist=access(argv[3],F_OK); //check if the file exists or not
			if(fileExist==0)					//if file exists ask whether to overwrite destination file or not
			{
				printf("cp:overwrite '%s'?\n",argv[3]);		//asking the input 
				scanf("%s",response);						//taking thr input
				if(response[0]=='y' || response[0]=='Y')			//if response is yes
				{
					dest=fopen(argv[3],"w");
					copy();			//fucntion to copy the file
				}
				//end if
				else
				{
					printf("process ended");
					exit(0);
				}
				//end else
			}
			//end if
			else		//if file does not exist create new file and copy the source file
			{
				dest=fopen(argv[3],"w");	//create and open the file in write mode
				copy();		//fucntion to copy the file
			}
			//end else
		}
		//end if
		else if(strcmp(argv[1],"-n")==0 || strcmp(argv[1],"--no-clobber")==0) 		//if -n option is given as input
		{
			fileExist=access(argv[3],F_OK);
			printf(fileExist);
			if(fileExist==-1)			//if destination file does not exist then create and write to that file
			{
				dest=fopen(argv[3],"w");		//function to create file and open it in write mode
				copy();		//fucntion to copy the file to destination
			}
			//end if
			else
			{
				exit(0);
			}
			//end else
		}
		//end else if
		else if(strcmp(argv[1],"--remove-destination")==0)		//if remove opetion is passed
		{
			remove(argv[3]);	//remove dest_file
			dest=fopen(argv[3],"w");	//create and open file in write mode
			copy();		//fucntion too copy
		}
		//end else if
		else if(strcmp(argv[1],"-v")==0 || strcmp(argv[1],"-verbose")==0)		//if -v option is passed
		{
			dest=fopen(argv[3],"w");	//create dest_file and open in write mode
			printf("'%s'->'%s'\n",argv[2],argv[3] );//printing source and destination files
			copy();//function to copy  the src to destinaion
		}
		//end else if
		else if(strcmp(argv[1],"-f")==0 || strcmp(argv[1],"--force")==0)	//if -f option is passed
		{
			fd=open(argv[3],O_WRONLY);	//opens the file in write only mode
			if(fd==-1)			//if file cannot be opened
			{
				remove(argv[3]);	//remove destinaton file
				dest=fopen(argv[3],"w");	//create and open file in write only mode
				copy();//fucntion to copy fles
			}
			//end if
			else	//if file can be opened
			{
				dest=fopen(argv[3],"w");	//open file in write mode
				copy();
			}
			//end else
		}
		//end else if
		else if(strcmp(argv[1],"-l")==0 || strcmp(argv[1],"--link")==0)
		{	//if -l "link" option is passed
			fileExist=access(argv[3],F_OK);	//returns if file can be accessde or not
			if(fileExist==-1)	//if file cannot be accessed
			{
				if(link(argv[2],argv[3])==-1)		//if link cannot be created
				{
					printf("%s and %s cannot be linked\n",argv[2],argv[3] );
					exit(0);
				}
				//end if
				else
				{
					printf("cp:cannot create hard link '%s' and '%s':File exists\n",argv[3],argv[2] );	//if file exists but link cannot be created
					exit(0);
				}
				//end else
			}	
			//end if
			else
			{
				exit(0);
			}
		}
		//end else if
	}
	//end if
}
//main end



//function to copy files from source to destination
//arguments:NULL
//return type:void
void copy()
{
	char ch;
	while((ch=fgetc(src))!=EOF)	//loop runs from the begining until EOF is encountered
	{
		//copying src to dest file
		fputc(ch,dest);
	}
	fclose(src);		//closing src file after copying is completed
	fclose(dest);		//closing dest file after copying is completed
}
//end copy()