/*********************************************************Readme**************************
This program implements PSOApriori approach that combines Apriori and Particle Swarm Optimization for solving FIM (Frequent 
Itemsets Mining) problem, it employs Apriori heuristics in exploring the solution's space by the the particles. 
It exisits some basic functions like: 
population_initialization, and generation for the PSO. We also developed other intelligent functions such as 
Ipopulation_initialization, and Igeneration for PSO-Apriori algorithm. There is other functions for FIM problem, 
such as fitness1, support, confidence. Finally, there is some functions to manage input and output data like: 
copy, read_trans, display_dataset, display_pop and display_solution.

This  program is developed by Dr. Djenouri Youcef  and supervised by Pr. Marco Comuzzi under the project 
funded by UNIST University, Ulsan, South Korea 
Contact: y.djenouri@gmail.com 
**************************************************************************************************/
/********************************************Improtant Note:********************************************
 Since the size() of a vector is an int, which is a 32-bit signed integer, 
 the maximum number of elements is 2**31-1 or roughly 2.1 billion elements. Thus, I suggest who want to use this program, 
 that respect this constraint, that means the maximum number of transactions that could be handled are 
 2.1 billion transactions with limited number of items. Otherwise, 
 you have to use more appropriate structure for handling with big transactional database
 *******************************************************************************************************/
 
#include <iostream>
#include <vector>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <sys/time.h>
#include <unistd.h>
using namespace std;
/*****Input program******/
# define N 40// number of items
# define M 1000 //umber of transactions
# define MinSup 0.20 
#define  nb_particules 100 // represents the number of particles on the given swarm
int IMAX=8;
typedef struct {int trans[N]; int nb;} ligne;
typedef struct {int solution[N]; float cost;} particle;
vector<ligne> dataset(M);
vector<particle> pop(nb_particules);
vector<particle> best_local(nb_particules);
//particle pop[P];
vector<particle> bestlocal(nb_particules);
particle best_globale;
int vitesse[nb_particules];
/****************************prototype declaration of PSO********************/
void population_initialization(); // this function allows to initialize population
void Ipopulation_initialization(); // this function allows to initialize population intelligently
void generation(); // update the position of the particles 
void Igeneration(); // update the position of the particles intelligently 
void reproduction(); // reproduize the new population
float average_fitness(); // average fitness of the current population
/**************prototype declaration of ARM problem*******/
void read_trans();// this function allows to read the transactional data base et insert it into the dataset vector
float support_rule(int s[]); // this function calculates the support of the entire solution s
float support_antecedent(int s[]); // this function computes the support of the antecedent of the solution s
float confidence(int s[]); // it calculates the confidence of the rule
float fitness1(int s[]); // computes the fitness of a given solution s
void copy(int t[], int v[]); // it copies the vector t in the vector v
void display_dataset(); //this function allows to display the transactional data base
void display_solution(particle S); // this function display the current solution with its cost
void display_pop(); // display solutions

/****************************principal program*********/
int main(void)
{
int i,j;
float moy=0;

read_trans();
//long topcrono = System.currentTimeMillis();
//long x=topcrono;
	time_t start,end; 
	 clock_t t1,t2;
      //start = time(NULL);
     t1=clock();
     Ipopulation_initialization();
     //display_pop();
     i=0;
     while(i<IMAX)
     {
     Igeneration();
     //display_pop();
     //printf("\nthe next generation\n");
     //reproduction();
     i++;    
     }
     //printf("the average fitness is: %f", average_fitness());
     //display_pop();
     t2=clock();
      float diff ((float)t2-(float)t1);
	  float seconds = diff / CLOCKS_PER_SEC;
      printf("\npopsize is %d the average fitness is: %f", nb_particules, average_fitness());
      printf("\nthe exexution time is%f\n",seconds);
 int x; scanf("%d",&x);
 return 0;
}
/****************PSO*******/
void population_initialization()
{

for (int i=0;i<nb_particules;i++)
{
   for (int j=0;j<N;j++)
 {
  pop[i].solution[j]=rand()%2;
  bestlocal[i].solution[j]=pop[i].solution[j];
 }
 pop[i].cost=fitness1(pop[i].solution);
bestlocal[i].cost=pop[i].cost;
}

 for (int j=0;j<nb_particules;j++)
 {     
  vitesse[j]=rand()%N;
 }
}

void Ipopulation_initialization()
{

for (int i=0;i<nb_particules;i++)
{
    int index=rand()%N;
    pop[i].solution[index]=1;
    bestlocal[i].solution[index]=1;
   for (int j=0;j<N && j!=index;j++)
 {     
  pop[i].solution[j]=0;
  bestlocal[i].solution[j]=0;
 }
pop[i].cost=fitness1(pop[i].solution);
bestlocal[i].cost=pop[i].cost;
}
}

/*********************************/
void best()
{
float cost=pop[0].cost;
int indice=0;   
for (int i=1; i<nb_particules;i++)
{
if (cost<pop[i].cost)
{
indice=i;
cost=pop[i].cost;                     
}        
}
for (int j=0;j<N;j++)
{
best_globale.solution[j]=pop[indice].solution[j];
}
best_globale.cost=pop[indice].cost; 
}
/********************************/
void generation()
{
 for (int i=0;i<nb_particules;i++)
  {
   int indice=vitesse[i];
   pop[i].solution[indice]=best_local[i].solution[indice];  
   indice=(indice*2)%N;
   pop[i].solution[indice]=best_globale.solution[indice];
   pop[i].cost=fitness1(pop[i].solution);
   if (best_local[i].cost<pop[i].cost)
     {
        for (int j=0;j<N;j++)
         {
          best_local[i].solution[j]=pop[i].solution[j];        
         }
       best_local[i].cost=pop[i].cost;                    
     } 
     
  }
  best();
   
}

void combinaison1(particle A, particle B, particle C)
{
int index; 
bool trouve=false;
 for (int i=0;i<N; i++)     
  {
  C.solution[i]=A.solution[i];
  if (B.solution[i]==1 && A.solution[i]==0 && trouve==false)
  {
  index=i;
  trouve=true;                     
  }        
  }
C.solution[index]=1;     
}
void combinaison2(particle A, particle B)
{
int index; 
bool trouve=false;
int i=0;
while (i<N && trouve==false)     
  {
       if (A.solution[i]==0 && B.solution[i]==1)  
             {
             trouve=true;
             index=i;               
             }
       i++;      
  }
  
A.solution[i]=1;

trouve=false;
i=0;
while (i<N && trouve==false)     
  {
       if (A.solution[i]==1)  
             {
             trouve=true;
             A.solution[i]=0;               
             }
       i++;      
  }
 for (i=0; i<N; i++)
 {
 B.solution[i]=A.solution[i];    
 }  
    B.cost=fitness1(B.solution);
       
}

void Igeneration()
{
     particle C;
       best();
 for (int i=0;i<nb_particules;i++)
  {
   combinaison1(best_globale, best_local[i], C);  
   combinaison2(C, pop[i]);
     
  } 
}

float fitness1(int s[])
{
	return support_rule(s);
}

float average_fitness()
{
 float sum=0;
 for (int i=0;i<nb_particules;i++)
  {
       sum=sum+pop[i].cost;                  
  }          
  return(sum/nb_particules);   
}
/***********ARM problem*****************/
/*******************display dataset***********************/
void display_dataset()
{
  for (vector<ligne>::size_type i = 0; i < dataset.size(); i++)
    {
                               
              for ( int j=0;j<dataset[i].nb;j++)                
              printf ("%d ",dataset[i].trans[j]);
              printf("\n");
   }    
}
/**********************copry t in v********/
void copy(int t[], int v[])
{
for (int i=0;i<N; i++)
{
v[i]=t[i]; 
}     
}
/**************************display_solution*****************/
void display_solution(particle S)
{
for (int i=0;i<N;i++)
{
    printf("%d ", S.solution[i]);
}
//printf ("cost is:%f",S.cost);
printf("\n");
}
/*******read transactional data bass and insert it in the data set structure********************************/
void read_trans()
{
int c ;
int val;
char t[10];
int j;
int i=0;
int l=0;
FILE *f=NULL;
f=fopen("C://dataset.txt","r");
if (f!=NULL) {printf("the file is succefully opened\n");} 
j=0;
while (c!=EOF)
{
   
        c=fgetc(f); 
        if (c==' ') {t[j]='\0';dataset[i].trans[l]=atoi(t);l++;/*printf("\n\t%d",val);*/j=0;}
        if (c=='\n'){dataset[i].nb=l;l=0;i++;j=0;}
        if (c!=' ' && c!='\n'){t[j]=c;j++;}
}    
printf("\n");
fclose(f); 
}
/*************************compute the support of the solution s**********/
float support_rule(int s[])
{
float compt=0;
		for (int i=0; i<M; i++)
		{
		bool appartient=true;
		
		int j=0;
		while (j<N)
		{
		 if (s[j]!=0)
		{
			int l=0; 
			bool existe=false;
			while (l< dataset[i].nb && existe==false)
			{
				if (dataset[i].trans[l]==j+1)
				{existe=true;}
			l++;
			}
			if (existe==false){appartient=false;}
		}
		j++;	
		}
		if (appartient==true) {compt++;}
		}
       //printf("support rule is:%f\n",compt);
       compt=compt/M;

	return compt;
}
/*****************************support antecedent computing*****************************/
float support_antecedent(int s[])
	{
		float compt=0;
		
		for (int i=0; i<M; i++)
		{
		bool appartient=true;
		int j=0;
		while (j<N)
		{
		 if (s[j]==1)
		{
			int l=0; 
			bool existe=false;
			while (l< dataset[i].nb && existe==false)
			{
					if (dataset[i].trans[l]==j+1)
				        {existe=true;}
			l++;
			}
			if (existe==false){appartient=false;}
		}
		j++;	
		}
		if (appartient==true) {compt++;}
		}
		 //printf("support antecedente is:%f\n",compt);
	   compt=compt/M;
	//if(compt!=0)System.out.println("antecedent"+compt);
	   return compt;
}
/****************************condifence computing**************************/
float confidence(int s[])
{
	float conf=0;
	if (support_antecedent(s)!=0)
	conf=support_rule(s)/support_antecedent(s);
	else conf=0;
	//nf=support_rule(sol);
	//if (conf==1){display_sol_SAT(s);}
	//if (conf==0){conf=1;}
    return conf;
}
/***********************evaluation of the solution s******/

bool verify(int t[], int v[])
{
bool exist=true;
int i=0;
while (i<N && exist==true)
{
  if (t[i]!=v[i]){exist=false;}
  i++;    
}
return exist;  
}
/*****************************display T_dance************/
void display_pop()
{
//f=fopen("C://Users//Dzcomp//Desktop//benchmark//dataset.txt","a");
//if (f==NULL) {printf("the file is succefully opened\n");} 
for (int i=0;i<nb_particules;i++)
{
    for (int j=0;j<N;j++)
    {
    printf ("%d-",pop[i].solution[j]);   
    }
    printf (" %f\n",pop[i].cost); 
}
/*printf("\ncrossover step \n");
for (int i=0;i<pop_size;i++)
{
    for (int j=0;j<N;j++)
    {
    printf ("%d-",pop[i].solution[j]);   
    }
    printf (" %f\n",new_pop[i].cost); 
}
*/
}
