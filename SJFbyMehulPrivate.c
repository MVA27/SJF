#include<stdio.h>
#include<stdlib.h>

struct node {
int Process;
int ArrivalTime;
int BurstTime;
int TurnAroundTime;
int WaitingTime;
int CompletionTime;
struct node *next;
};
struct node *start = NULL;   //Pointer to the main List

int globalTime = 0;
int totalBurstTime;         //sum of BurstTime of all processes

int minimumArrivalTime;    //minimum arrival time among all the processes

int startTime = 0;		   //Time at which Execution starts
int currentTime = 0;	   //Time at which a process is finished executing

int recoredrAray[100];				//Array that records the number of processes that have arrived for each second.
struct node *listQueue = NULL;		//the processes from 'recoredrAray[]' is taken and converted into  a list. *listQueue points to that list. (it acts like a ready Queue)

/*A process once fully executed must be removed from the Queue, these functions are used for that purpose*/
void deletionAtTheEnd()
{
    struct node *temp = listQueue;
    struct node *previousNode;

    while(temp->next != NULL)
    {
        previousNode = temp;
        temp = temp->next;
    }
    previousNode->next=NULL;
    free(temp);
}

void deletionOfSpecificValue(int value)
{
    struct node *temp = listQueue;
    struct node *nextNode;
    while(temp->next->Process != value )
    {
        temp = temp->next;
    }
    nextNode = temp->next;
    temp->next = nextNode->next;
    nextNode->next=NULL;
    free(nextNode);
}

/*This function is core of this program. It performs:- convrion of [1]array to list [2]initialising current time and other values like Waiting time etc. */
void core(int arrayIndex){

    int i,m;
    int sizeOfArray = 0;
    struct node *check = start;

	//Whatever Processes are selected add them into a List like a Ready Queue (in our case its *listQueue)
    for(i=0;i<arrayIndex;i++)
    {
        while(check != NULL)
        {
            if(check->Process == recoredrAray[i])
            {
                struct node *p = (struct node *)malloc(sizeof(struct node));
                p->Process = check->Process;
                p->ArrivalTime = check->ArrivalTime;
                p->BurstTime = check->BurstTime;
                p->TurnAroundTime = 0;
                p->WaitingTime = 0;
                p->CompletionTime = 0;
                p->next=NULL;

                if(listQueue == NULL){
                    listQueue = p;
                }

                else
                {
                    struct node *temp = listQueue;
                    while(temp->next != NULL)
                    {
                        temp = temp->next;
                    }
                    temp->next = p;
                }

            }
            check = check->next;
        }
        check = start;
    }


	/*At this point we have a Queue of Processes ready to be executed */


    //From newly created listQueue find the Process with lowest BurstTime, execute it and remove it from the Queue (ie listQueue)
    check = listQueue;
    int minimumBurstTime = check->BurstTime;
    int minimumBurstTimeProcessNo = check->Process;
    while(check != NULL)
    {
        if(check->BurstTime < minimumBurstTime)
        {
            minimumBurstTime = check->BurstTime;
            minimumBurstTimeProcessNo = check->Process;
        }
        check = check->next;
    }

    //NOTE : minimumBurstTimeProcessNo is the process which is just EXECUTED.

    //Set time for globalTime counter = to Burst Time of Executing Process
	/*If the TIME-LOOP is at 1 second BUT the BurstTime is 3 second.
	That means For a single second, This loop have been executed for 3 second. Which is not possible.
	Hence the TIME-LOOP must also be executed for 3 second. Therefore decrement this counter 'globalTime' one by one and till then
	dont call core() function just keep gathering the processes in the recoredrAray.
	Once the time is synchronized, call the core() function again with new set of process in the recoredrAray
	*/
    globalTime = minimumBurstTime;

    //Set CompletionTime, TurnAroundTime , WaitingTime and update currentTime
    check = start;
    while(check != NULL)
    {
        if(check->Process == minimumBurstTimeProcessNo)
        {
            currentTime = currentTime + check->BurstTime;
            check->CompletionTime = currentTime;
            check->TurnAroundTime = check->CompletionTime - check->ArrivalTime;
            check->WaitingTime = check->TurnAroundTime - check->BurstTime;
        }
        check = check->next;
    }

    printf(" < %d > %d ",minimumBurstTimeProcessNo,currentTime);

    //Now delete the Executed Process ie minimumBurstTimeProcessNo
    check = listQueue;
    while(check != NULL)
    {
        if(check->Process == minimumBurstTimeProcessNo)
        {
            if(check == listQueue && check->next == NULL)
            {
                //THIS IS FIRST NODE AND LAST NODE
                struct node *deletionPointer;
                deletionPointer = check;
                listQueue = NULL;
                free(deletionPointer);
            }

            else if(check == listQueue)
            {
                //THIS IS FIRST NODE
                struct node *deletionPointer;
                deletionPointer = check;
                listQueue = listQueue->next;
                free(deletionPointer);
            }

            else if(check->next == NULL)
            {
                //THIS IS LAST NODE
                deletionAtTheEnd();
            }

            else{
                //THIS IS INTERMEDIATE NODE
                deletionOfSpecificValue(check->Process);
            }

        }
        check = check->next;
    }

}

/*This function is used to create node*/
void createNode()
{
    struct node *temp = start;
    struct node *p = (struct node *)malloc(sizeof(struct node));
    printf("\nEnter Process Number : ");
    scanf("%d",&p->Process);

    printf("\nEnter Arrival Time : ");
    scanf("%d",&p->ArrivalTime);

    printf("\nEnter Burst Time : ");
    scanf("%d",&p->BurstTime);

    p->TurnAroundTime = 0;
    p->WaitingTime = 0;
    p->CompletionTime = 0;
    p->next=NULL;

    if(start == NULL){
        start = p;
    }

    else{
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = p;
    }
}


void main()
{
    int n,i;
    printf("\nENTER THE NUMBER OF PROCESS : ");
    scanf("%d",&n);

    for(i=1;i<=n;i++){
        createNode();
    }

    printf("\nTOTAL PROCESSES IN THE SYSTEM : ");
    struct node *x = start;
    while(x != NULL){
        printf(" %d ",x->Process);
        totalBurstTime = totalBurstTime + x->BurstTime;
        x = x->next;
    }

    //Find the minimum ArrivalTime
    struct node *m =start;
    minimumArrivalTime = start->ArrivalTime;
    while(m != NULL)
    {
        if(m->ArrivalTime < minimumArrivalTime)
            {
                minimumArrivalTime = m->ArrivalTime;
            }
            m = m->next;
    }
    printf("\nMINIMUM Arrival Time is = %d \n",minimumArrivalTime);

    printf(" NOTE : Output format is : TIME <PROCESS NO> TIME <PROCESS NO.> ..... \n");


    //Main Logic
	int arrayIndex = 0;					//To keep track of index of the recoredrAray
    startTime = minimumArrivalTime;
    currentTime = startTime;


    printf("\n %d ",startTime);			//Tp print startTime on screen

	//Run a loop for each second (aasume name of this loop to be :- TIME-LOOP)
    for(i=minimumArrivalTime;i<totalBurstTime;i++)
    {
        x=start;

		//For each second check how many processes have arrived
        while(x != NULL)
        {
            if(x->ArrivalTime == i)
            {
                recoredrAray[arrayIndex] = x->Process;  			//Record the number of processes arrived for ith second in an array
                arrayIndex++;
            }

            else{
                //NO PROCESS FOUND
            }

            x = x -> next;
        }

        //To Synchronize the core time and for-loop(TIME-LOOP) time add if-statement
        if(globalTime == 0)
        {
            core(arrayIndex);

            //After the core is executed, reset the arrayIndex to get new set of processes
            arrayIndex = 0 ;
        }

        //Reduce the globalTime by 1 second for every iteration to Synchronize with outer-loop time
		//Used if-statement such that globalTime dosent go in negative for 1st itteration
        if(globalTime != 0){
            globalTime--;
        }
    }

    printf("\n DETAILS OF EACH PROCESS : ");
    int totalTurnAroundTime = 0;
    int totalWaitingTime = 0;
    m =start;
    while(m != NULL)
    {
        printf("\n Process No = %d : Arrival Time = %d : Burst Time = %d : Turn Around Time = %d : Waiting Time = %d : Completion Time = %d ",m->Process,m->ArrivalTime,m->BurstTime,m->TurnAroundTime,m->WaitingTime,m->CompletionTime);
        totalTurnAroundTime = totalTurnAroundTime + m->TurnAroundTime;
        totalWaitingTime = totalWaitingTime + m->WaitingTime;
        m = m->next;
    }

    printf("\n average waiting time = %f ",(float)totalWaitingTime/n);
    printf("\n average turn around time = %f ",(float)totalTurnAroundTime/n);
}
