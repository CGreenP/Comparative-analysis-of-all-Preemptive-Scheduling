#include <iostream>
#include <stdio.h>
#include <graphics.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
using namespace std;

struct Process {
    int pid; // Process ID
    int bt; // Burst Time
    int art; // Arrival Time
    int priority; // Priority
    int responsetime;
    int outtime;
    int intime;
};

//function to set graph scale
int set_scale(float a, float b, float c)
{
    int i,flag=0;
    float n;
    float scale[12]={0.1,0.5,1.0,2.0,5.0,10.0,20.0,50.0,100.0,200.0,500.0,1000.0};
    if((a >= b) && (a >= c))
        {
            n= a;
        }
    else if ((b >= a) && (b >= c))
        {
            n= b;
        }
    else
        {
            n= c;
        }
    for(i=0;i<12;i++)
    {
        if (n < scale[i])
        {
            flag=i;
            break;
        }
    }
    return flag;
}

void insertt(Process Heap[], Process value, int* heapsize,int* currentTime)
{
    int start = *heapsize, i;
    Heap[*heapsize] = value;
    if (Heap[*heapsize].intime == -1)
    {
        Heap[*heapsize].intime = *currentTime;
    }
    ++(*heapsize);

    while (start != 0 && Heap[(start - 1) / 2].priority > Heap[start].priority)
    {
        Process temp = Heap[(start - 1) / 2];
        Heap[(start - 1) / 2] = Heap[start];
        Heap[start] = temp;
        start = (start - 1) / 2;
    }
}

void order(Process Heap[], int* heapsize, int start)
{
    int smallest = start;
    int left = 2 * start + 1;
    int right = 2 * start + 2;
    if (left < *heapsize && Heap[left].priority <Heap[smallest].priority)
    {
        smallest = left;
    }
     if (right < *heapsize && Heap[right].priority < Heap[smallest].priority)
    {
        smallest = right;
    }
    if (smallest != start)
    {
        Process temp = Heap[smallest];
        Heap[smallest] = Heap[start];
        Heap[start] = temp;
        order(Heap, heapsize, smallest);
    }
}

Process extractminimum(Process Heap[], int* heapsize,int* currentTime)
{
    Process minn = Heap[0];
    if (minn.responsetime == -1)
    {
        minn.responsetime = *currentTime - minn.art;
    }
    --(*heapsize);
    if (*heapsize >= 1)
    {
        Heap[0] = Heap[*heapsize];
        order(Heap, heapsize, 0);
    }
    return minn;
}

void scheduling(Process Heap[], Process proc[], int n,int* heapsize, int* currentTime)
{
    if (heapsize == 0)
    {
        return;
    }
    Process minn = extractminimum(Heap, heapsize, currentTime);
    minn.outtime = *currentTime + 1;
    --minn.bt;
    if (minn.bt > 0)
    {
        insertt(Heap, minn, heapsize, currentTime);
        return;
    }

    for (int i = 0; i < n; i++)
        if (proc[i].pid == minn.pid)
        {
            proc[i] = minn;
            break;
        }
}

//Function to calculate Preemptive Priority
float preemptive_priority(Process proc[],int n,int tempburstTime[],int totalbursttime)
{
    int totalwaitingtime=0,i,insertedprocess=0,heapsize=0,currentTime=proc[0].art,totalresponsetime=0,start;
    Process Heap[4 * n];
    do
    {
        if (insertedprocess != n)
        {
            for (i = 0; i < n; i++)
            {
                if (proc[i].art == currentTime)
                {
                    ++insertedprocess;
                    proc[i].intime = -1;
                    proc[i].responsetime = -1;
                    insertt(Heap, proc[i], &heapsize, &currentTime);
                }
            }
        }
        scheduling(Heap, proc, n, &heapsize, &currentTime);
        ++currentTime;
        if (heapsize == 0 && insertedprocess == n)
        {
            break;
        }
    }while (1);
    for (int i = 0; i < n; i++)
    {
        totalwaitingtime += (proc[i].outtime - proc[i].intime -tempburstTime[i]);
    }
    return ((float)totalwaitingtime / (float)n);
}

//Function to calculate Preemptive SJF
float preemptive_sjf(Process proc[], int n, int rt[])
{
    int wt[n], tat[n], total_wt = 0;
    int complete = 0, t = 0, minm = INT_MAX;
    int shortest = 0, finish_time;
    bool check = false;
    while (complete != n)
    {
        for (int j = 0; j < n; j++)
        {
            if ((proc[j].art <= t) && (rt[j] < minm) && rt[j] > 0)
            {
                minm = rt[j];
                shortest = j;
                check = true;
            }
        }
        if (check == false)
        {
            t++;
            continue;
        }
        rt[shortest]--;
        minm = rt[shortest];
        if (minm == 0)
            {
                minm = INT_MAX;
            }

        if (rt[shortest] == 0)
            {
                complete++;
                check = false;
                finish_time = t + 1;
                wt[shortest] = finish_time - proc[shortest].bt - proc[shortest].art;
                if (wt[shortest] < 0)
                    {
                        wt[shortest] = 0;
                    }
            }
        t++;
    }
    for (int i = 0; i < n; i++)
        {
            total_wt = total_wt + wt[i];
        }
    return ((float)total_wt / (float)n);
}

//Function to calculate Round Robin
float round_robin(struct Process p[],int n,int time_quantum,int rt[])
{
    int remain=n,time,cnt,flag=0,wait_time=0;
    for(time=0,cnt=0;remain!=0;)
    {
        if(rt[cnt]<=time_quantum && rt[cnt]>0)
        {
            time+=rt[cnt];
            rt[cnt]=0;
            flag=1;
        }
        else if(rt[cnt]>0)
        {
            rt[cnt]-=time_quantum;
            time+=time_quantum;
        }
        if(rt[cnt]==0 && flag==1)
        {
            remain--;
            wait_time+=time-p[cnt].art-p[cnt].bt;
            flag=0;
        }
        if(cnt==n-1)
        {
            cnt=0;
        }
        else if(p[cnt+1].art<=time)
        {
            cnt++;
        }
        else
        {
            cnt=0;
        }
    }
    return(((float)(wait_time))/(float)n);
}

int main()
{
    int n,tq,i,c,d,pos,temp,totalbursttime=0;
    float avgwait_time_round_robin,avgwait_time_sjf,avgwait_time_priority;

    printf("Enter number of process: ");
    scanf("%d",&n);
    printf("\nEnter the value of Time Quantum: ");
    scanf("%d",&tq);

    struct Process proc[n];
    int rt[n],rt2[n],rt3[n];

    //Taking input from user
    for(i=0;i<n;i++)
    {
        proc[i].pid=i+1;
        printf("\nEnter The Process details of %d:\n",proc[i].pid);
        printf("Arrival Time: ");
        scanf("%d",&proc[i].art);
        printf("\nBurst Time: ");
        scanf("%d",&proc[i].bt);
        printf("\nPriority (smaller numbers denote higher priority) : ");
        scanf("%d",&proc[i].priority);
    }

    //Sorting according to arrival time
    for (c = 0; c < (n - 1); c++)
    {
        pos = c;
        for (d = c + 1; d < n; d++)
        {
            if (proc[pos].art > proc[d].art)
            pos = d;
        }
        if (pos != c)
        {
            temp = proc[c].art;
            proc[c].art = proc[pos].art;
            proc[pos].art = temp;

            temp = proc[c].pid;
            proc[c].pid = proc[pos].pid;
            proc[pos].pid = temp;

            temp = proc[c].bt;
            proc[c].bt = proc[pos].bt;
            proc[pos].bt = temp;

            temp = proc[c].priority;
            proc[c].priority = proc[pos].priority;
            proc[pos].priority = temp;
        }
    }

    for(i=0;i<n;i++)
    {
        rt[i]=proc[i].bt;
        rt2[i]=proc[i].bt;
        rt3[i]=proc[i].bt;
        totalbursttime += proc[i].bt;
    }

    avgwait_time_round_robin=round_robin(proc,n,tq,rt2);
    avgwait_time_sjf=preemptive_sjf(proc, n, rt);
    avgwait_time_priority=preemptive_priority(proc,n,rt3,totalbursttime);

    printf("\n\nAverage waiting time in Round Robin= %f",avgwait_time_round_robin);
    printf("\n\nAverage waiting time in Preemptive SJF= %f",avgwait_time_sjf);
    printf("\n\nAverage waiting time in Preemptive Priority= %f",avgwait_time_priority);

    printf("\n--------------------------------------------------------------------------------");
    printf("\n\t\t\tPresenting Visual Representation");

    int graphicdriver=DETECT,graphicmode,h_rr1=0,h_sjf2=0,h_pp3=0,y=0,flag=(-1);

    int f_rr1=(int)round(avgwait_time_round_robin*100),f_sjf2=(int)round(avgwait_time_sjf*100), f_pp3= (int)round(avgwait_time_priority*100);

	flag= set_scale(avgwait_time_round_robin,avgwait_time_sjf,avgwait_time_priority);

	initgraph(&graphicdriver,&graphicmode,"");

    setcolor(YELLOW);
    rectangle(0,25,639,454);
    settextstyle(SANS_SERIF_FONT,HORIZ_DIR,2);
    setcolor(WHITE);
    outtextxy(277,0,"Bar Graph");

    settextstyle(SANS_SERIF_FONT,HORIZ_DIR,1);
    outtextxy(427,77,"- Round Robin");
    outtextxy(427,127,"- Preemptive SJF");
    outtextxy(427,177,"- Preemptive Priority");

    setlinestyle(SOLID_LINE,0,2);
    line(100,434,100,60);
    line(100,434,600,434);
    line(90,70,100,60);
    line(110,70,100,60);
    line(590,424,600,434);
    line(590,444,600,434);

    settextstyle(SANS_SERIF_FONT,HORIZ_DIR,2);
    outtextxy(75,35,"TIME");
    outtextxy(65,420,"0  -");

    settextstyle(SANS_SERIF_FONT,HORIZ_DIR,2);
    setcolor(WHITE);

    char buffer1[10],buffer2[10],buffer3[10];
    switch (flag)
    {
    case 0:
        outtextxy(45, 120,"0.10 -");
        outtextxy(45, 150,"0.09 -");
        outtextxy(45, 180,"0.08 -");
        outtextxy(45, 210,"0.07 -");
        outtextxy(45, 240,"0.06 -");
        outtextxy(45, 270,"0.05 -");
        outtextxy(45, 300,"0.04 -");
        outtextxy(45, 330,"0.03 -");
        outtextxy(45, 360,"0.02 -");
        outtextxy(45, 390,"0.01 -");

        h_rr1= 433-(30*f_rr1);
        h_sjf2= 433-(30*f_sjf2);
        h_pp3= 433-(30*f_pp3);

        sprintf(buffer1,"%0.2f",((float)f_rr1)/100);
        outtextxy(160,h_rr1-35,buffer1);
        sprintf(buffer2,"%0.2f",((float)f_sjf2)/100);
        outtextxy(235,h_sjf2-35,buffer2);
        sprintf(buffer3,"%0.2f",((float)f_pp3)/100);
        outtextxy(310,h_pp3-35,buffer3);
        break;
    case 1:
        outtextxy(45, 120,"0.50 -");
        outtextxy(45, 150,"0.45 -");
        outtextxy(45, 180,"0.40 -");
        outtextxy(45, 210,"0.35 -");
        outtextxy(45, 240,"0.30 -");
        outtextxy(45, 270,"0.25 -");
        outtextxy(45, 300,"0.20 -");
        outtextxy(45, 330,"0.15 -");
        outtextxy(45, 360,"0.10 -");
        outtextxy(45, 390,"0.05 -");

        h_rr1= 433-(6*f_rr1);
        h_sjf2= 433-(6*f_sjf2);
        h_pp3= 433-(6*f_pp3);

        sprintf(buffer1,"%0.2f",((float)f_rr1)/100);
        outtextxy(160,h_rr1-35,buffer1);
        sprintf(buffer2,"%0.2f",((float)f_sjf2)/100);
        outtextxy(235,h_sjf2-35,buffer2);
        sprintf(buffer3,"%0.2f",((float)f_pp3)/100);
        outtextxy(310,h_pp3-35,buffer3);
        break;
    case 2:
        outtextxy(45, 120,"1.00 -");
        outtextxy(45, 150,"0.90 -");
        outtextxy(45, 180,"0.80 -");
        outtextxy(45, 210,"0.70 -");
        outtextxy(45, 240,"0.60 -");
        outtextxy(45, 270,"0.50 -");
        outtextxy(45, 300,"0.40 -");
        outtextxy(45, 330,"0.30 -");
        outtextxy(45, 360,"0.20 -");
        outtextxy(45, 390,"0.10 -");

        h_rr1= 433-(3*f_rr1);
        h_sjf2= 433-(3*f_sjf2);
        h_pp3= 433-(3*f_pp3);

        sprintf(buffer1,"%0.2f",((float)f_rr1)/100);
        outtextxy(160,h_rr1-35,buffer1);
        sprintf(buffer2,"%0.2f",((float)f_sjf2)/100);
        outtextxy(235,h_sjf2-35,buffer2);
        sprintf(buffer3,"%0.2f",((float)f_pp3)/100);
        outtextxy(310,h_pp3-35,buffer3);
        break;
    case 3:
        outtextxy(45, 120,"2.00 -");
        outtextxy(45, 150,"1.80 -");
        outtextxy(45, 180,"1.60 -");
        outtextxy(45, 210,"1.40 -");
        outtextxy(45, 240,"1.20 -");
        outtextxy(45, 270,"1.00 -");
        outtextxy(45, 300,"0.80 -");
        outtextxy(45, 330,"0.60 -");
        outtextxy(45, 360,"0.40 -");
        outtextxy(45, 390,"0.20 -");

        h_rr1= 433-(3*(f_rr1/2));
        h_sjf2= 433-(3*(f_sjf2/2));
        h_pp3= 433-(3*(f_pp3/2));

        sprintf(buffer1,"%0.2f",((float)f_rr1)/100);
        outtextxy(160,h_rr1-35,buffer1);
        sprintf(buffer2,"%0.2f",((float)f_sjf2)/100);
        outtextxy(235,h_sjf2-35,buffer2);
        sprintf(buffer3,"%0.2f",((float)f_pp3)/100);
        outtextxy(310,h_pp3-35,buffer3);
        break;
    case 4:
        outtextxy(45, 120,"5.00 -");
        outtextxy(45, 150,"4.50 -");
        outtextxy(45, 180,"4.00 -");
        outtextxy(45, 210,"3.50 -");
        outtextxy(45, 240,"3.00 -");
        outtextxy(45, 270,"2.50 -");
        outtextxy(45, 300,"2.00 -");
        outtextxy(45, 330,"1.50 -");
        outtextxy(45, 360,"1.00 -");
        outtextxy(45, 390,"0.50 -");

        h_rr1= 433-(3*(f_rr1/5));
        h_sjf2= 433-(3*(f_sjf2/5));
        h_pp3= 433-(3*(f_pp3/5));

        sprintf(buffer1,"%0.2f",((float)f_rr1)/100);
        outtextxy(160,h_rr1-35,buffer1);
        sprintf(buffer2,"%0.2f",((float)f_sjf2)/100);
        outtextxy(235,h_sjf2-35,buffer2);
        sprintf(buffer3,"%0.2f",((float)f_pp3)/100);
        outtextxy(310,h_pp3-35,buffer3);
        break;
    case 5:
        outtextxy(45, 120,"10.0 -");
        outtextxy(45, 150,"9.00 -");
        outtextxy(45, 180,"8.00 -");
        outtextxy(45, 210,"7.00 -");
        outtextxy(45, 240,"6.00 -");
        outtextxy(45, 270,"5.00 -");
        outtextxy(45, 300,"4.00 -");
        outtextxy(45, 330,"3.00 -");
        outtextxy(45, 360,"2.00 -");
        outtextxy(45, 390,"1.00 -");

        h_rr1= 433-(3*(f_rr1/10));
        h_sjf2= 433-(3*(f_sjf2/10));
        h_pp3= 433-(3*(f_pp3/10));

        sprintf(buffer1,"%0.2f",((float)f_rr1)/100);
        outtextxy(160,h_rr1-35,buffer1);
        sprintf(buffer2,"%0.2f",((float)f_sjf2)/100);
        outtextxy(235,h_sjf2-35,buffer2);
        sprintf(buffer3,"%0.2f",((float)f_pp3)/100);
        outtextxy(310,h_pp3-35,buffer3);
        break;
    case 6:
        outtextxy(45, 120,"20.0 -");
        outtextxy(45, 150,"18.0 -");
        outtextxy(45, 180,"16.0 -");
        outtextxy(45, 210,"14.0 -");
        outtextxy(45, 240,"12.0 -");
        outtextxy(45, 270,"10.0 -");
        outtextxy(45, 300,"8.00 -");
        outtextxy(45, 330,"6.00 -");
        outtextxy(45, 360,"4.00 -");
        outtextxy(45, 390,"2.00 -");

        h_rr1= 433-(3*(f_rr1/20));
        h_sjf2= 433-(3*(f_sjf2/20));
        h_pp3= 433-(3*(f_pp3/20));

        sprintf(buffer1,"%0.2f",((float)f_rr1)/100);
        outtextxy(160,h_rr1-35,buffer1);
        sprintf(buffer2,"%0.2f",((float)f_sjf2)/100);
        outtextxy(235,h_sjf2-35,buffer2);
        sprintf(buffer3,"%0.2f",((float)f_pp3)/100);
        outtextxy(310,h_pp3-35,buffer3);
        break;
    case 7:
        outtextxy(45, 120,"50.0 -");
        outtextxy(45, 150,"45.0 -");
        outtextxy(45, 180,"40.0 -");
        outtextxy(45, 210,"35.0 -");
        outtextxy(45, 240,"30.0 -");
        outtextxy(45, 270,"25.0 -");
        outtextxy(45, 300,"20.0 -");
        outtextxy(45, 330,"15.0 -");
        outtextxy(45, 360,"10.0 -");
        outtextxy(45, 390,"05.0 -");

        h_rr1= 433-(3*(f_rr1/50));
        h_sjf2= 433-(3*(f_sjf2/50));
        h_pp3= 433-(3*(f_pp3/50));

        sprintf(buffer1,"%0.2f",((float)f_rr1)/100);
        outtextxy(160,h_rr1-35,buffer1);
        sprintf(buffer2,"%0.2f",((float)f_sjf2)/100);
        outtextxy(235,h_sjf2-35,buffer2);
        sprintf(buffer3,"%0.2f",((float)f_pp3)/100);
        outtextxy(310,h_pp3-35,buffer3);
        break;
    case 8:
        outtextxy(35, 120,"100.0 -");
        outtextxy(45, 150,"90.0 -");
        outtextxy(45, 180,"80.0 -");
        outtextxy(45, 210,"70.0 -");
        outtextxy(45, 240,"60.0 -");
        outtextxy(45, 270,"50.0 -");
        outtextxy(45, 300,"40.0 -");
        outtextxy(45, 330,"30.0 -");
        outtextxy(45, 360,"20.0 -");
        outtextxy(45, 390,"10.0 -");

        h_rr1= 433-(3*(f_rr1/100));
        h_sjf2= 433-(3*(f_sjf2/100));
        h_pp3= 433-(3*(f_pp3/100));

        sprintf(buffer1,"%0.2f",((float)f_rr1)/100);
        outtextxy(160,h_rr1-35,buffer1);
        sprintf(buffer2,"%0.2f",((float)f_sjf2)/100);
        outtextxy(235,h_sjf2-35,buffer2);
        sprintf(buffer3,"%0.2f",((float)f_pp3)/100);
        outtextxy(310,h_pp3-35,buffer3);
        break;
    case 9:
        outtextxy(45, 120,"200 -");
        outtextxy(45, 150,"180 -");
        outtextxy(45, 180,"160 -");
        outtextxy(45, 210,"140 -");
        outtextxy(45, 240,"120 -");
        outtextxy(45, 270,"100 -");
        outtextxy(45, 300,"80  -");
        outtextxy(45, 330,"60  -");
        outtextxy(45, 360,"40  -");
        outtextxy(45, 390,"20  -");

        h_rr1= 433-(3*(f_rr1/200));
        h_sjf2= 433-(3*(f_sjf2/200));
        h_pp3= 433-(3*(f_pp3/200));

        sprintf(buffer1,"%0.2f",((float)f_rr1)/100);
        outtextxy(160,h_rr1-35,buffer1);
        sprintf(buffer2,"%0.2f",((float)f_sjf2)/100);
        outtextxy(235,h_sjf2-35,buffer2);
        sprintf(buffer3,"%0.2f",((float)f_pp3)/100);
        outtextxy(310,h_pp3-35,buffer3);
        break;
    case 10:
        outtextxy(45, 120,"500 -");
        outtextxy(45, 150,"450 -");
        outtextxy(45, 180,"400 -");
        outtextxy(45, 210,"350 -");
        outtextxy(45, 240,"300 -");
        outtextxy(45, 270,"250 -");
        outtextxy(45, 300,"200 -");
        outtextxy(45, 330,"150 -");
        outtextxy(45, 360,"100 -");
        outtextxy(45, 390,"50  -");

        h_rr1= 433-(3*(f_rr1/500));
        h_sjf2= 433-(3*(f_sjf2/500));
        h_pp3= 433-(3*(f_pp3/500));

        sprintf(buffer1,"%0.2f",((float)f_rr1)/100);
        outtextxy(160,h_rr1-35,buffer1);
        sprintf(buffer2,"%0.2f",((float)f_sjf2)/100);
        outtextxy(235,h_sjf2-35,buffer2);
        sprintf(buffer3,"%0.2f",((float)f_pp3)/100);
        outtextxy(310,h_pp3-35,buffer3);
        break;
    case 11:
        outtextxy(35, 120,"1000 -");
        outtextxy(45, 150,"900 -");
        outtextxy(45, 180,"800 -");
        outtextxy(45, 210,"700 -");
        outtextxy(45, 240,"600 -");
        outtextxy(45, 270,"500 -");
        outtextxy(45, 300,"400 -");
        outtextxy(45, 330,"300 -");
        outtextxy(45, 360,"200 -");
        outtextxy(45, 390,"100 -");

        h_rr1= 433-(3*(f_rr1/1000));
        h_sjf2= 433-(3*(f_sjf2/1000));
        h_pp3= 433-(3*(f_pp3/1000));

        sprintf(buffer1,"%0.2f",((float)f_rr1)/100);
        outtextxy(160,h_rr1-35,buffer1);
        sprintf(buffer2,"%0.2f",((float)f_sjf2)/100);
        outtextxy(235,h_sjf2-35,buffer2);
        sprintf(buffer3,"%0.2f",((float)f_pp3)/100);
        outtextxy(310,h_pp3-35,buffer3);

        break;
    }

    setfillstyle(SOLID_FILL,RED);
    setcolor(RED);
    bar3d(150,h_rr1,200,433,10,1);
    bar(400,75,425,100);

    setfillstyle(SOLID_FILL,GREEN);
    setcolor(GREEN);
    bar3d(225,h_sjf2,275,433,10,1);
    bar(400,125,425,150);

    setfillstyle(SOLID_FILL,WHITE);
    setcolor(WHITE);
    bar3d(300,h_pp3,350,433,10,1);
    bar(400,175,425,200);

    getch();
    closegraph();
	return 0;
}
