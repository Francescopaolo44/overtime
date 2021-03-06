//Informatic project Dellaquila #1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//---------------------------constant--------------------------
const int number_of_workers = 10;
const int days_of_works = 28;
const int compulsory_hours_week = 40;
const int compulsory_hours_month = 160;

//---------------------------structure--------------------------
typedef struct{
	int id; /*AUTO INCREMENT */
	char name[100],surname[100],date_of_birth[100],residence[100];
}worker;

//-------------------------function-----------------------------------------

//-------------------------DBMS---------------------------------------------
//function that add a new workers to "database"
void DBMS_add(FILE *config){
	worker database;
	
	//worker's database file 
	FILE *workers_list;
	
	//worker's badge file
	FILE *workers_badge;
	
	char data[100],complex_path[100];
	int check_load=0,workers=0,response=1;
	
	
	//open in "a" mode to add workers
	workers_list=fopen("workers_database/workers_list/workers_list.txt","a");
	
	//open config.ini
	config=fopen("settings/config.ini","r");

	//read number of workers from setting
	while(fscanf(config,"%s",data) > 0){
		if(strcmp("workers:",data) == 0){
			fscanf(config,"%d",&workers);
		}
	}
	
	//set autoincrement id to default value of settings.ini
	database.id = workers;
	
	//close config
	fclose(config);
	
	do{
		if(workers < number_of_workers){
			check_load=1;
		}else{
			printf("You have reached max number of workers.");
		}
		if(check_load==1){
			//divider
			fprintf(workers_list,"---------------------------------------\n");
			
			//id
			fprintf(workers_list,"%d\n",database.id);
			
			//------------create badge file for all workers in DB by ID--------
			//create file path with string concatenation of int and string
			sprintf(complex_path,"workers_database/work_hours/%d.txt",database.id);
			workers_badge=fopen(complex_path,"w");
			//initialize worker's day;
			fprintf(workers_badge,"Worker's day: 1\n");
			fclose(workers_badge);
			//-------------------------------------------------------------------
			 
			printf("Insert worker's surname: ");
			scanf("%s",database.surname);
			//surname	
			fprintf(workers_list,"%s\n",database.surname);
			
			printf("Insert worker's name: ");
			scanf("%s",database.name);
			//name	
			fprintf(workers_list,"%s\n",database.name);
				
			printf("Insert worker's date of birth: [gg mm aa] ");
			
			//empty get for send button
			getchar();
			
			gets(database.date_of_birth);
			//date of birthday	
			fprintf(workers_list,"%s\n",database.date_of_birth);
				
			printf("Insert workers' residence: ");
			scanf("%s",database.residence);
			//residence	
			fprintf(workers_list,"%s\n",database.residence);
				
			workers++;
			check_load=0;
			database.id++;
		}	
		printf("Add another workers?:(S=1/N=0) ");
		scanf("%d",&response);
	}while(response!=0);
	
	//open config.ini
	config=fopen("settings/config.ini","r+");
	//move near number
	fseek(config,93,0);
	//update
	fprintf(config,"%d",workers);
	fprintf(config,"\n");
	//close config && workers_list
	fclose(config);
	fclose(workers_list);
}

//function that search a user
void DBMS_search(){
	char surname[100],read_from[100];
	int i,check_workers=0;
	FILE *workers_list;
	
	//open in "r" mode to read workers
	workers_list=fopen("workers_database/workers_list/workers_list.txt","r");
	
	if(workers_list == NULL){
		printf("You have never added a workers to database");
	}else{
		printf("Insert workers surname: ");
		scanf("%s",surname);
		while(fscanf(workers_list,"%s",read_from) > 0){
			if(strcmp(surname,read_from) == 0){
				printf("\nWorker's data\n");
				for(i=0;i<5;i++){
					fscanf(workers_list,"%s",read_from);
					printf("\n%s\n",read_from);
					check_workers=1;
				}
			}
		}
		if(check_workers==0){
			printf("\nThere'isnt workers with this surname");
		}
	}
	//close file
	fclose(workers_list);
}

//function that print all workers
void DBMS_print(){
	FILE *workers_list;
	char workers[100];
	
	//open in "r" mode to read workers
	workers_list=fopen("workers_database/workers_list/workers_list.txt","r");
	
	if(workers_list == NULL){
		printf("You have never added a workers to database");
	}else{
		while(fscanf(workers_list,"%s",workers) > 0){
			printf("%s\n",workers);
		}
	}
}

//function that simulate a DBMS (Database Management System) to manage all workers in industry
void DBMS_menu(){
	
	int response,choice=1,check_data=0,i;
	char username[100],password[100],read_file[100];
	
	//load config.ini
	FILE *config;
	
	//open config.ini
	config = fopen("settings/config.ini","r");
			
	if(config == NULL){
		printf("\nOps....you delete or move config.ini file. Please restart the program in the same folder of settings/config.ini");
	}else{
		//check if user is admin
		printf("\nInsert admin Username: ");
		scanf("%s",username);
		printf("\nInsert admin Password: ");
		scanf("%s",password);
		while(fscanf(config,"%s",read_file) > 0){
			if(strcmp(username,read_file) == 0){
				for(i=0;i<3;i++){
					fscanf(config,"%s",read_file);
					if(strcmp(password,read_file) == 0){
						check_data=1;
					}
				}
			}
		}
		
		//close config.ini
		fclose(config);
			
		if(check_data!=1){
			printf("\nUsername or password is incorrect.");
		}else{
			while(choice!=0){
				//clean screen
				system("cls");
				printf("\nWelcome Admin\n\n");
				printf("\nChoose an action: \n");
				printf("|1)Add Workers      |\n");
				printf("|2)Search Workers   |\n");
				printf("|3)Print  Workers   |\n");	
				printf("\nInsert action: ");
				scanf("%d",&response);
				
				switch (response){
				
					case 1: DBMS_add(config);
					break;
					
					case 2: DBMS_search();
					break;
					
					case 3: DBMS_print();
					break;
					
					default: printf("wrong action");
				}
				printf("\n\nContinue with DBMS?:(S=1/N=0) ");
				scanf("%d",&choice);
			}
		}
	}
}
//-------------------------END DBMS-----------------------------------------------

//-------------------------BADGE--------------------------------------------------
//function that transform entrance and exit value in minute of work
int calculate_minute_work (int entrance_H){
	int minute=0;
	
	//calculate minute
	minute = entrance_H * 60;
	
	return minute;
}
//function that transform minute value in hour
float calculate_hour_work (int minute){
	float hour;
	
	//calculate hour
	hour = minute / 60.0;
	
	return hour;
}
//function that to specific id file entrance and exit
void Badge(){
	int id,day=0,check_day=0;
	char complex_path[100];
	int check_entrance=0,check_exit=0;
	int entrance_H=0,entrance_M=0,exit_H=0,exit_M=0;
	int total_m_entrance=0,total_m_exit=0,minute_H=0,total_m=0;
	
	FILE *workers_badge;
	
	printf("Insert badge ID: ");
	scanf("%d",&id);
	sprintf(complex_path,"workers_database/work_hours/%d.txt",id);
	
	//open badge file fo read
	workers_badge=fopen(complex_path,"r");
	
	//read worker's day
	fseek(workers_badge,14,0);
	fscanf(workers_badge,"%d",&day);
	
	if(workers_badge == NULL){
		printf("\nFile doesn't exist.Have you ever add this worker in the database?");
	}else{
		//close file in 'r' mode"
		fclose(workers_badge);
		
		//open file for appendin hour and update day
		workers_badge=fopen(complex_path,"a");
		
		if(day <= days_of_works){
		
			//print day
			if(day == 1 || day == 8 || day == 15 || day == 22){
				fprintf(workers_badge,"\n\nLunedi: ");
				check_day=1;
			}else if(day == 2 || day == 9 || day == 16 || day == 23){
				fprintf(workers_badge,"\n\nMarted�: ");
				check_day=1;
			}else if(day == 3 || day == 10 || day == 17 || day == 24){
				fprintf(workers_badge,"\n\nMercoled�: ");
				check_day=1;
			}else if(day == 4 || day == 11 || day == 18 || day == 25){
				fprintf(workers_badge,"\n\nGioved�: ");
				check_day=1;				
			}else if(day == 5 || day == 12 || day == 19 || day == 26){
				fprintf(workers_badge,"\n\nVenerd�: ");
				check_day=1;				
			}else if(day == 6 || day == 13 || day == 20 || day == 27){
				fprintf(workers_badge,"\n\nSabato:-");
			}else{
				fprintf(workers_badge,"\n\nDomenica:-\n");
			}
			if(check_day==1){
				
				//check correct hour for entrance and exit
				do{
					//set green				
					system("color 2F");
					
					//entrance hour
					printf("Insert Entrance Hour: [hh]");
					scanf("%d",&entrance_H);
					
					//entrance minute
					printf("Insert Entrance Minute: [mm]");
					scanf("%d",&entrance_M);
					
					//check right hour
					if((entrance_H>=0 && entrance_H<=23) && (entrance_M>=0 && entrance_M<=59)){
						check_entrance=1;
					}else{
						printf("Invalid hour\n");
					}
					
					//set red				
					system("color 4F");
					
					//exit hour
					printf("Insert Exit Hour: [hh]");
					scanf("%d",&exit_H);
					
					//exit minute
					printf("Insert Exit Minute: [mm]");
					scanf("%d",&exit_M);
					
					//check right hour
					if((exit_H>=0 && exit_H<=23) && (exit_M>=0 && exit_M<=59)){
						check_exit=1;
					}else{
						printf("invalid minute\n");
					}
					
				}while(check_entrance!=1 && check_exit!=1);
				
				//transformation from hour to minute
				minute_H = calculate_minute_work(entrance_H);
				//total
				total_m_entrance = minute_H+ entrance_M;
				
				//print entrance
				fprintf(workers_badge,"%d",entrance_H);
				fprintf(workers_badge,".");
				fprintf(workers_badge,"%d",entrance_M);
				
				//space
				fprintf(workers_badge," ");
				fprintf(workers_badge,"/");
				fprintf(workers_badge," ");		
				
				//transformation from hour to minute
				minute_H = calculate_minute_work(exit_H);
				//total
				total_m_exit = minute_H+ exit_M;
				
				//print exit
				fprintf(workers_badge,"%d",exit_H);
				fprintf(workers_badge,".");
				fprintf(workers_badge,"%d\n",exit_M);
				
				//calculate total of minute
				total_m = total_m_exit - total_m_entrance;
				
				//print minute of work
				fprintf(workers_badge,"Minute of work: ");
				fprintf(workers_badge,"%d\n",total_m);
				
				day++;
				
			}else{
				printf("Weekend man!");
				day++;
			}
		}else{
			printf("You have worked all 20 days.C'mon man take a little vacation. ");
		}
	}
	//close badge in append mode
	fclose(workers_badge);
	
	//update days of works
	workers_badge=fopen(complex_path,"r+");
	fseek(workers_badge,14,0);
	fprintf(workers_badge,"%d",day);
	
	//close badge in append mode
	fclose(workers_badge);
	
	//set default				
	system("color 3F");
}

//function that print all worker's hour
void Badge_print(){
	int id;
	char complex_path[100],read_hour[100];
	
	FILE *workers_badge;
	
	printf("Insert badge ID: ");
	scanf("%d",&id);
	sprintf(complex_path,"workers_database/work_hours/%d.txt",id);
	
	//open badge file fo read
	workers_badge=fopen(complex_path,"r");
	
	if(workers_badge == NULL){
		printf("\nFile doesn't exist.Have you ever add this worker in the database?");
	}else{
		while(fscanf(workers_badge,"%s",read_hour) > 0){
			printf("%s\n",read_hour);
		}
	}
	//close file
	fclose(workers_badge);
}

//function that simulate an Electronic Badge
void Badge_menu(){
	int choice=1,response=0;
	
	while(choice!=0){
		//clean screen
		system("cls");
		printf("\nWelcome Worker\n\n");
		printf("\nChoose an action: \n");
		printf("|1)Badge                 |\n");
		printf("|2)Print Workers Badge   |\n");	
		printf("\nInsert action: ");
		scanf("%d",&response);
				
		switch (response){
				
			case 1: Badge();
					break;
			
			case 2: Badge_print();
					break;
					
			default: printf("wrong action");
		}
		printf("\n\nContinue with Badge?:(S=1/N=0) ");
		scanf("%d",&choice);
	}
}

//overtime of all workers
void Overtime_all(){
	int workers=0,i,e,week=0,workers_days=0;
	int minute=0,total=0,total_minute[4],overtime=0;
	int compulsory_week=0,compulsory_month=0,extra=0;
	float extra_hour=0;
	char complex_path[100],garbage[100];
	
	//transform constant hour of work in minutes
	//week
	compulsory_week=calculate_minute_work(compulsory_hours_week);
	//month
	compulsory_month=calculate_minute_work(compulsory_hours_month);

	//load worker 
	FILE *workers_hour;
	
	//load config.ini
	FILE *config;
	
	//open config.ini
	config = fopen("settings/config.ini","r");

			
	if(config == NULL){
		printf("\nOps....you delete or move config.ini file. Please restart the program in the same folder of settings/config.ini");
	}else{
		//read number of workers
		fseek(config,93,0);
		fscanf(config,"%d",&workers);
		
		//check 0 workers
		if(workers==0){
			printf("No workers. Have you ever add to the database?");
		}else{
			//close config
			fclose(config);
			
			//open correct file for each workers
			for(i=0;i<workers;i++){
				
				//initialize component for calculation
				week=0;
				total=0;
				total_minute[0,0,0,0];
				overtime=0;
				
				//open workers_hours
				sprintf(complex_path,"workers_database/work_hours/%d.txt",i);
		
				//open badge file fo read
				workers_hour=fopen(complex_path,"r");
				
				//read worker days
				fseek(workers_hour,14,0);
				fscanf(workers_hour,"%d",&workers_days);
			
				//close workers_hour
				fclose(workers_hour);
				
				if(workers_days<29){
					printf("\nThe software can't calculate overtime for this workers because he doesn't work all 20 days.\n");
				}else{
					//open badge file fo read
					workers_hour=fopen(complex_path,"r");
					
					fseek(workers_hour,18,0);
					
					for(e=0;e<workers_days-1;e++){
						if(e==5 || e==6 || e==12 || e==13 || e==19 || e==20 || e==26 || e==27){
							//skip name of day
							fscanf(workers_hour,"%s",garbage);
						}else{
							//---------parser start
							//skip name of day
							fscanf(workers_hour,"%s",garbage);
							//skip entrance
							fscanf(workers_hour,"%s",garbage);
							//skip divider
							fscanf(workers_hour,"%s",garbage);
							//skip exit
							fscanf(workers_hour,"%s",garbage);
							//skip declaration of minute
							fscanf(workers_hour,"%s",garbage);
							//skip declaration of minute
							fscanf(workers_hour,"%s",garbage);
							//skip declaration of minute
							fscanf(workers_hour,"%s",garbage);	
							//---------parser finish
							//get total minute value
							fscanf(workers_hour,"%d",&minute);
							total+=minute;
							if(e==4 || e==11 || e==18 || e==25){
								if(total>=compulsory_week){
									total_minute[week]=total;
								}else{
									total_minute[week]=0;
								}
								total=0;
								week++;
							}
						}							
					}													
					for(week=0;week<4;week++){
						overtime += total_minute[week];
					}
					//check overtime
					if(overtime < compulsory_month){
						printf("\nThis worker, id:%d, work fewer hours than 160.\n",i);
					}else if(overtime == compulsory_month){
						printf("\nThis worker, id:%d, work 160 hours.\n",i);
					}else{
						extra = overtime - compulsory_month;
						extra_hour = calculate_hour_work(extra);
						printf("\nFor this worker,id:%d, You must pay about %0.2f hour of overtime.\n",i,extra_hour);
					}					
				}
			}			
		}
	}
}

//function that calculate overtime of workers{the main program}
void Overtime_menu(){
	int choice=1,response;
	
	while(choice!=0){
		//clean screen
		system("cls");
		printf("\nWelcome Worker\n\n");
		printf("\nChoose an action: \n");
		printf("|1)Overtime [ALL] workers |\n");
		printf("\nInsert action: ");
		scanf("%d",&response);
				
		switch (response){
				
			case 1: Overtime_all();
					break;
					
			default: printf("wrong action");
		}
		printf("\n\nContinue with Overtime?:(S=1/N=0) ");
		scanf("%d",&choice);
	}
}

void menu (){
	
	int response,choice=1;
	
	while(choice!=0){
		//clean screen
		system("cls");
		
		printf("\n\nChoose an action: \n");
		printf("---------------------|\n");
		printf("|1)DBMS              |\n");
		printf("|2)BADGE             |\n");
		printf("---------------------|\n");
		printf("|3)OVERTIME          |\n");
		printf("\nInsert action: ");
		scanf("%d",&response);
		
		switch (response){
		
			case 1: DBMS_menu();
			break;
			
			case 2: Badge_menu();
			break;
			
			case 3: Overtime_menu();
			break;
			
			default: printf("wrong action");
		}
		printf("\n\nContinue with Menu?:(S=1/N=0) ");
		scanf("%d",&choice);
	}
}

//launch screen
void launch_screen(){
	system("color 1A");
	int i;
	printf("                            @####     \n");       
    printf("                       @#####         \n");    
    printf("                      ###             \n");   
    printf("                     @##              \n");     
    printf("                  @@###   @@@@@       \n");     
    printf("                 @####:   #####@      \n");     
    printf("                     ##;      @#@     \n");     
    printf("                      ##'      @#@    \n");     
    printf("                       ##@@@@   @#@@@@\n");     
    printf("                        #####   ,###@ \n");     
    printf("                               ,##    \n");     
    printf("                              ;##     \n");     
	printf("                         @@@@##       \n");    
    printf("                          #####     \n\n");
	printf(" _____            _   _      _     _____           _                 \n");                
	printf("/  __ |          | | | |    | |   /  ___|         | |                \n");             
	printf("| /  |/_ __ _   _| |_| | ___| |_  | `--.__   _ ___| |_ ___ _ __ ___  \n");  
	printf("| |   | '__| | | |      / _ | __| `--. |  | | / __| __/ _ | '_ ` _  |\n");  
	printf("| |__/| |  | |_| | ||  | |__/ |_  /|__/ _ |_| |__ | ||  __/ | | | | |\n"); 
	printf("|____/|_|  |__,  |_||__/|___||__| |____/ |__, |___/___|___|_| |_| |_|\n"); 
	printf("            __/ |                        __/ |                       \n");                       
	printf("            |___/                        |___/                       \n\n");             
	
	printf("Crynet System version 1.0\n\n");
	printf("Copyright Dellaquila Francesco Paolo\n");
	printf("                       {Don't touch the window until the loading finish or reload program}\n\n");
	printf("                                                   Loading");
	
	for(i=0;i<5;i++){
		printf(".");
		//sleep
		sleep(1);
	}        
}
//---------------------------main--------------------------------
int main(){
	
	//initialize design
	system("title Overtime Project by D.F.P");

	//loading screen
	launch_screen();
	
	//default color
	system("color 3F");
	
	//call menu
	menu();
	
}
