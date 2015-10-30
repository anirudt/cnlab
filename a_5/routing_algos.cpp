#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <cstdint>
#define INT_MAX 2147483647
using namespace std;

/* 
 * Gets the index of the smallest distance from the dist_array
 */
int get_small( int*dist,int v)
{
    int small=INT_MAX,i,index=0;
    for(i=1;i<=v;i++)
    {
        if(dist[i]<small)
           {
               small=dist[i];
               index=i;
           }
    }
    return (index);
}

//..............................................................................................................................................//
 
int *bellman_ford (int *G,int s,int v) 
{ 
    int *dist,i,row,col; 
    dist = new int[v+1]; 
    for(i=0;i<(v+1);i++) 
        { 
            dist[i]=1000; 
        } 
    dist[s]=0; 
     for (i=0;i<v;i++) 
     { 
         for(row=1;row<=v;row++) 
            for(col=1;col<=v;col++) 
         { 
             if(G[v*(row-1)+(col-1)]!=0&&G[v*(row-1)+(col-1)]!=INT_MAX)    //relaxing 
              if((dist[col]>dist[row]+G[v*(row-1)+(col-1)])) 
                dist[col]=dist[row]+G[v*(row-1)+(col-1)]; 
 
 
         } 
     } 
 
     //checking negative cycles 
 
     for(row=1;row<=v;row++) 
            for(col=1;col<=v;col++) 
         { 
             if(G[v*(row-1)+(col-1)]!=0&&G[v*(row-1)+(col-1)]!=INT_MAX) 
              if((dist[col]>dist[row]+G[v*(row-1)+(col-1)])) 
                {cout<<"Negative cycle detected."; 
                exit(0); 
                } 
 
 
         } 
    return dist; 
 
 
} 
 
//.................................................................................................................................................// 
 
int* dikstra (int *G,int s,int v) 
{ 
    int *dist,i,smallest_index,*finale,j,cont,*temp; 
    dist = new int[v+1]; 
    finale= new int[v+1]; 
    temp= new int[v+1]; 
    for(i=0;i<(v+1);i++) 
        { 
            dist[i]=INT_MAX; 
            temp[i]=INT_MAX; 
        } 
    dist[s]=0; 
    finale[s]=0; 
    temp[s]=0; 
    cont=v; 
    while(cont>0) 
    {   smallest_index=get_small(temp,v); 
        for(j=1;j<=v;j++) 
        { 
        if(G[v*(smallest_index-1)+(j-1)]!=0&&G[v*(smallest_index-1)+(j-1)]!=INT_MAX)	//to get adjacent vertices 
        { 
            if(dist[j]>dist[smallest_index]+G[v*(smallest_index-1)+(j-1)])              // relaxing 
              { 
                dist[j]=dist[smallest_index]+G[v*(smallest_index-1)+(j-1)]; 
                temp[j]=dist[j]; 
              } 
        } 
        } 
        finale[smallest_index]=dist[smallest_index]; 
        temp[smallest_index]=INT_MAX; 
        cont--; 
    } 
    return finale; 
} 
 
int main() 
{ 
    int v,row,col,i,j,s,times,graph_choice,algorithm; 
    static int *G= NULL,*output; 
    static int sample[25]; 
    char choice; 
 
    cout<<"\nENTER A CHOICE:\n\n1.To select sample graph press 1.\n\n2.To enter your own graph press 2.\n"; 
graph_choice_again: 
    cin>>graph_choice; 
    if(graph_choice!=1&&graph_choice!=2) 
    { 
      cout<<"\n\t\tPlease enter a valid choice.\n\n"; 
      goto graph_choice_again; 
    } 

    if(graph_choice==2)  // entering user defined graph 
    { 

      cout<<"\n\nYou have chosen to enter your own graph, to continue..."<<endl; 
      cout<<"Enter the total number of vertices in your graph:"<<endl; 
      cin>>v; 

      G = new int [v*v]; 

      for(i=0;i<v;i++) 
        for(j=0;j<v;j++) 
        { 
          if(i!=j) 
          { 
            G[v*i+j]=INT_MAX; 
          } 
        } 
      times=0; 
      cout<<"Enter the elements of adjacency matrix"<<endl; 
      do{ 
        times++; 
        cout<<endl<<"Enter index values for your graph:"<<endl; 
again: cout<<endl<<"row number= "; 
       cin>>row; 

       if(row>v) 
       {cout<<"Wrong Choice enter again..."<<endl; 
         goto again;} 

       cout<<endl<<"column number= "; 
       cin>>col; 

       if(col>v) 
       {cout<<"Wrong Choice enter again..."<<endl; 
         goto again;} 

       cout<<"\nNow, Enter the weight of the entered edge ("<<row<<","<<col<<") :"; 
       cin>>G[v*(row-1)+(col-1)]; 
       cout<<endl<<"Want to enter more edges with corresponding weights ? \nPress Y for Yes or N for No : "; 
       cin>>choice; 

      }while((choice=='y'||choice=='Y')&&times<=v*v); 

      cout<<"\nYour entered inputs are as..."<<endl; 


    } 

    else if (graph_choice==1)        // creating sample graph 
    {     v=5; 

      for(i=0;i<v;i++) 
        for(j=0;j<v;j++) 
          if(i!=j) 
            sample[v*i+j]=INT_MAX; 



      sample[1]=10; 
      sample[3]=5; 
      sample[5*1+3]=2; 
      sample[5*1+2]=1; 
      sample[5*2+4]=4; 
      sample[5*3+1]=3; 
      sample[5*3+2]=9; 
      sample[5*3+4]=2; 
      sample[5*4+2]=6; 
      sample[5*4]=7; 
      cout<<"\nThe sample graph contains 5 vertices.\n"; 
      cout<<"\nAdjacency matrix for the graph is... :"<<endl; 

    } 

    for(i=0;i<v;i++) // to display the adjacency matrix 
    { 

      cout<<endl<<endl; 
      cout<<"\t\t\t"; 
      for(j=0;j<v;j++) 
      { 
        if(i==j) 
          cout<<"  0  "; 
        else if (graph_choice==2) 
        { 
          if (G[v*i+j]==INT_MAX) 
            cout<<"  ~  "; 

          else if(graph_choice==2) 
            cout<<"  "<<G[v*i+j]<<"  "; 

        } 

        else if (graph_choice==1) 
        { 
          if(sample[v*i+j]==INT_MAX) 
            cout<<"  ~  "; 

          else if(graph_choice==1) 
            cout<<"  "<<sample[v*i+j]<<"  "; 

        } 

      } 

    } 

    cout<<"\n\n  Select an algorithm to use:\n\n  1.Dikstra's Algorithm\n\n  2.Bellman-Ford algorithm\n"; 
    cin>>algorithm; 

    cout<<endl<<"\nEnter your source vertex ( from 1 to "<<v<<" )";  
    cin>>s; 

    if(graph_choice==1&&algorithm==2)                // calling appropriate function as per choices. 
      output=bellman_ford(sample,s,v); 

    else if (graph_choice==1&&algorithm==1) 
      output=dikstra(sample,s,v); 

    else if(graph_choice==2&&algorithm==1) 
      output=dikstra(G,s,v); 

    else if(graph_choice==2&&algorithm==2) 
      output=bellman_ford(G,s,v); 

    cout<<endl;
    cout<<"\tSource\t\tDestination\t\tShortest Distance\n" ; 
    for(i=1;i<=v;i++) 
    { 
      if(output[i]<100)
      {
        cout<<"\n\t"<<s<<"\t\t\t"<<i<<"\t\t\t"<<output[i] <<endl;
      } 
      else 
        cout<<"\n\t"<<s<<"\t\t\t"<<i<<"\t\t\tINFINITE!! "<<endl ; 

    } 



} 
