/*
=====================
||PROBLEM STATMENT:||
=====================
    It is required to simulate some of paging replacement algorithms. The required algorithms to
    be implemented are as follows:
    -Optimal
    -First In First Out (FIFO)
    -Least Recently Used (LRU)
    -Clock

===================
||INPUT / OUTPUT:||
===================
    Your input will be from standard input. Your output will be from standard output. The input
    will contain the number of pages allocated to the process, the simulated algorithm
    (OPTIMAL, FIFO, LRU, or CLOCK), and then a sequence of page references like below:

        3
        FIFO
        5
        12
        5
        2
        4
        2
        5
        -1

    Note: The last line in the input is -1 (and is ignored)
    For each run, you should print the following:
        1. A trace recording page faults for each memory reference in the sequence.
        2. Counter recording total page faults.


    We will have the following results (notice the two-digit page numbers):

        Replacement Policy = FIFO
        -------------------------------------
        Page   Content of Frames
        ----   -----------------
        05     05 
        12     05 12 
        05     05 12 
        02     05 12 02 
        04 F   04 12 02 
        02     04 12 02 
        05 F   04 05 02 
        -------------------------------------
        Number of page faults = 2

===============
||SAMPLE RUN:||
===============
    the source program will be compiled by typing make, then will be tested by
    running the following command, which should produce nothing in case of successful
    test:

        >> make
        >> gcc lab4.c -o lab4
        >> cat inputLRU.txt | ./lab4 | diff outputLRU.txt -
        >> cat inputLRU2.txt | ./lab4 | diff outputLRU2.txt -
        >> cat inputFIFO.txt | ./lab4 | diff outputFIFO.txt -
        >> cat inputFIFO2.txt | ./lab4 | diff outputFIFO2.txt -
        >> cat inputCLOCK.txt | ./lab4 | diff outputCLOCK.txt -
        >> cat inputCLOCK2.txt | ./lab4 | diff outputCLOCK2.txt -
        >> cat inputOPTIMAL.txt | ./lab4 | diff outputOPTIMAL.txt -
        >> cat inputOPTIMAL2.txt | ./lab4 | diff outputOPTIMAL2.txt -
        >> 

*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>


//max size of queue
#define MAX 1000

//number of pages allocated to the process
int pages = 0;
//content of frames
int *page;
//simulated algorithm (OPTIMAL, FIFO, LRU, or CLOCK)
char replacment_type[8];
//incase read_file() function
char input_name[32];
//counter recording total page faults
int fault_number = 0;


//implementation of queue
int intArray[MAX];
int front = 0;
int rear = -1;
int itemCount = 0;


bool isEmpty()
{
    return itemCount == 0;
}


bool isFull()
{
    return itemCount == MAX;
}


int size()
{
    return itemCount;
}


void insert(char data)
{

    if (!isFull())
    {

        if (rear == MAX - 1)
        {
            rear = -1;
        }

        intArray[++rear] = data;
        itemCount++;
    }
}


int removeData()
{
    int data = intArray[front++];

    if (front == MAX)
    {
        front = 0;
    }

    itemCount--;
    return data;
}


//initializing inputs in case of STANDARD INPUT
void input(){
    //take the sequence of page refrences
    int num;
    scanf("%d",&pages);
    scanf("%s",replacment_type);
    scanf("%d",&num);
    //take inputs till reaching the lastline
    while(num != -1){
        insert(num);
        scanf("%d",&num);
    }
    //allocating memory to frames
    page = malloc(sizeof(int) * pages);
    for (int i = 0; i < pages; i++)
        page[i] = -1;
}


//initializing inputs in case of input FILE
void read_file()
{
    printf("Input File Name: ");
    scanf("%s", input_name);
    //take the sequence of page refrences
    int c;
    FILE *fptr;
    fptr = fopen(input_name, "r");
    if (fptr == NULL)
    {
        printf("Could not open file %s", input_name);
    }
    fscanf(fptr, "%d", &pages);
    fscanf(fptr, "%s", replacment_type);
    //fgets(replacment_type, sizeof(replacment_type),fptr);
    fscanf(fptr, "%d", &c);
    //take inputs till reaching the lastline
    while (c != -1)
    {
        insert(c);
        fscanf(fptr, "%d", &c);
    }
    fclose(fptr);
    //allocating memory to frames
    page = malloc(sizeof(int) * pages);
    for (int i = 0; i < pages; i++)
        page[i] = -1;
}


/*
check if page(num) is already in a frame
& return its position
*/
int check(int num)
{
    for (int i = 0; i < pages; i++)
        if (num == page[i])
            return i + 1;
    return 0;
}


/*
select the page for which the time to the
last refrence is the longest
*/
int old_helper(){
    //collect the time for each page in frame
    int old[pages]; 

    //initialize all pages to -1 time
    for (int i = 0; i < pages; i++) 
        old[i] = -1;

    //find the recent reference for each frame
    for (int i = 0; i < pages; i++){
        for(int j = 0; j < front; j++){
            if(page[i]==intArray[j]){
                if(old[i]<j){
                    //far[i] = rear-j;
                    old[i] = j;
                }
            }
        }
    }

    //find the LRU refrence position
    int min = old[0];
    int min_position = 0;
    for (int i = 1; i < pages; i++){
        if(old[i] < min){
            min = old[i];
            min_position = i;
        }
    }
    return min_position;
}


/*
select the page for which the time to the
next refrence is the longest
*/
int far_helper(){
    //collect the time for each page in frame
    int far[pages]; 
    //collect remaining pages from quueue
    int count = itemCount;
    int check[count];

    //initialize all pages to infinity time
    for (int i = 0; i < pages; i++) 
        far[i] = INT_MAX;

    //make an array with the remaining pages in the queue
    for (int i = 0; i < count; i++){
        check[i]=removeData();
        insert(check[i]);
    }
    //find the first reference for each frame if exists in check[]
    for (int i = 0; i < pages; i++){
        for(int j = 0; j < count; j++){
            if(page[i]==check[j]){
                if(far[i]>j){
                    //far[i] = rear-j;
                    far[i] = j;
                }
            }
        }
    }
    /*
    for (int i = 0; i < pages; i++) 
        printf("f%d:%d ",i,far[i]);
    */

    //find the farthest refrence position
    int max = far[0];
    int max_position = 0;
    for (int i = 1; i < pages; i++){
        if(far[i] > max){
            max = far[i];
            max_position = i;
        }
    }
    return max_position;
}


//print the content of the frame at a giving point
void frame_content()
{
    for (int i = 0; i < pages; i++)
    {
        if (page[i] == -1)
            break;
        printf("%02d ", page[i]);
    }
}


//FIFO replacement policy implementation
void FIFO_replacement()
{
    //next page
    int num;
    //point to which position to place a page in the frames
    int arrow = 0;
    /*
    to check if there is an empty place to allocate to a page
    before applying the replacement policy
    */
    int flag = 0;
    
    while (!isEmpty())
    {
        num = removeData();
        printf("%02d ", num);
        //page exists in a frame
        if (check(num))
        {
            printf(" ");
        }
        else
        {
            //no need to apply the replacement policy
            if (flag < pages)
            {
                page[arrow] = num;
                arrow = (arrow + 1) % pages;
                flag++;
                printf(" ");
            }
            //apply the replacement policy
            else
            {
                //replace page pointed by arrow to the first page in the queue
                page[arrow] = num;
                //make the arrow point to the next frame
                arrow = (arrow + 1) % pages;
                
                fault_number++;
                printf("F");
            }
        }
        printf("   ");
        //print content of the frame
        frame_content();
        printf("\n");
    }
}


//CLOCK replacement policy implementation
void CLOCK_replacement()
{
    //next page
    int num;
    //point to which position to place a page in the frames
    int arrow = 0;
    /*
    to check if there is an empty place to allocate to a page
    before applying the replacement policy
    */
    int flag = 0;
    /*
    when a page is first loaded in memory or refrenced,
    the use bit is set to 1
    */
    int use_bit[pages];
    for (int i = 0; i < pages; i++) 
        use_bit[i]=0;
    while (!isEmpty())
    {
        num = removeData();
        printf("%02d ", num);
        //page exists in a frame
        if (check(num))
        {
            //page refrenced, change its use_bit to 1
            use_bit[check(num)-1] = 1;
            printf(" ");
        }
        else
        {
            //no need to apply the replacement policy
            if (flag < pages)
            {
                page[arrow] = num;
                //page first loaded in memory, use_bit = 1
                use_bit[arrow] = 1;
                arrow = (arrow + 1) % pages;
                flag++;
                printf(" ");
            }
            //apply the replacement policy
            else
            {
                //check for a frame to replace (use_bit=0)
                while(1){
                    //found a frame to replace
                    if(use_bit[arrow]==0){
                        break;
                    }
                    //pass over frames changing their use_bit to 0
                    else{
                        use_bit[arrow]=0;
                        arrow = (arrow + 1) % pages;
                    }
                }
                //replace page pointed by arrow to the first page in the queue
                page[arrow] = num;
                //page first loaded in memory, use_bit = 1
                use_bit[arrow] = 1;
                //make the arrow point to the next frame
                arrow = (arrow + 1) % pages;
                fault_number++;
                printf("F");
            }
        }
        printf("   ");
        //print content of the frame
        frame_content();
        printf("\n");
    }
}


//--------------------------
//NOT WORKING BEYOND 3 PAGES
//--------------------------
// //LRU replacement policy implementation
// void LRU_replacement(){
//     //next page
//     int num;
//     //point to which position to place a page in the frames
//     int arrow = 0;
//     /*
//     to check if there is an empty place to allocate to a page
//     before applying the replacement policy
//     */
//     int flag = 0;
//     //contains the least recently used pages
//     int old[pages];
//     while (!isEmpty())
//     {
//         num = removeData();
//         printf("%02d ", num);
//         //page exists in a frame
//         if (check(num))
//         {    
//             //add it to recently used pages
//             old[arrow] = num;
//             //point to the new least recently used page
//             arrow = (arrow + 1) % pages;
//             printf(" ");
//         }
//         else
//         {
//             //no need to apply the replacement policy
//             if (flag < pages)
//             {
//                 page[flag] = num;
//                 //add it to recently used pages
//                 old[arrow] = num;
//                 //point to the new least recently used page
//                 arrow = (arrow + 1) % pages;
//                 flag++;
//                 printf(" ");
//             }
//             //apply the replacement policy
//             else
//             {
//                 //find the least recently used frame
//                 int old_position = check(old[arrow]) - 1;
//                 //replace LRU frame with the new page
//                 page[old_position] = num;
//                 //add new page to recently used pages
//                 old[arrow] = num;
//                 //point to the new least recently used page
//                 arrow = (arrow + 1) % pages;
//                 fault_number++;
//                 printf("F");
//             }
//         }
//         printf("   ");
//         frame_content();
//         printf("\n");
//     }
// }


//LRU replacement policy implementation
void LRU_replacement(){
    //next page
    int num;
    /*
    to check if there is an empty place to allocate to a page
    before applying the replacement policy
    */
    int flag = 0;
    while (!isEmpty())
    {
        num = removeData();
        printf("%02d ", num);
        //page exists in a frame
        if (check(num))
        {
            printf(" ");
        }
        else
        {
            //no need to apply the replacement policy
            if (flag < pages)
            {
                page[flag] = num;
                flag++;
                printf(" ");
            }
            //apply the replacement policy
            else
            {
                //apply the replacement policy
                int old_position = old_helper();
                //replace LRU frame with the new page
                page[old_position] = num;
                fault_number++;
                printf("F");
            }
        }
        printf("   ");
        frame_content();
        printf("\n");
    }
}


//OPTIMAL replacement policy implementation
void OPTIMAL_replacement(){
    //next page
    int num;
    /*
    to check if there is an empty place to allocate to a page
    before applying the replacement policy
    */
    int flag = 0;
    while (!isEmpty())
    {
        //printf("f:%d ,r:%d ,c:%d ",front,rear,itemCount);
        num = removeData();
        printf("%02d ", num);
        //page exists in a frame
        if (check(num))
        {
            printf(" ");
        }
        else
        {
            //no need to apply the replacement policy
            if (flag < pages)
            {
                page[flag] = num;
                flag++;
                printf(" ");
            }
            //apply the replacement policy
            else
            {
                //find the page whose next use will occur farthest in the future 
                int far_position = far_helper();
                //replace farthest page with the new page
                page[far_position] = num;
                fault_number++;
                printf("F");
            }
        }
        printf("   ");
        frame_content();
        printf("\n");
    }
}


int main()
{
    //read_file();
    input();

    printf("Replacement Policy = %s\n", replacment_type);
    printf("-------------------------------------\n");
    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");

    if (strcmp(replacment_type, "FIFO") == 0)
    {
        FIFO_replacement();
    }
    else if (strcmp(replacment_type, "CLOCK") == 0)
    {
        CLOCK_replacement();
    }
    else if (strcmp(replacment_type, "LRU") == 0)
    {
        LRU_replacement();
    }
    else if (strcmp(replacment_type, "OPTIMAL") == 0)
    {
        OPTIMAL_replacement();
    }

    printf("-------------------------------------\n");
    printf("Number of page faults = %d\n", fault_number);
    return 0;
}