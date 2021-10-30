#include<bits/stdc++.h>
using namespace std;
class Edge
{
   public:
   int x,y,w;
   Edge(int first,int second,int weight){
       x=first;
       y=second;
       w=weight;
   }
   bool operator<(const Edge a) const
   {
       return w<a.w;
   }
};
class Graph
{
    int nvertices,nedges;
    vector<pair<int,int> > *adjList;
    bool is_directed;
    vector<Edge> Edgelist;
    
    public:
      void initialise(int nvertex,int nedge,bool directed=false,bool weighted=false)
      {
       nvertices=nvertex;
       nedges=nedge;
       is_directed=directed;
       adjList=new vector<pair<int,int> > [nvertices+1];
       for (int i=0;i<nedges;i++)
       {
           int x,y,w=1;
           cin>>x>>y;
           if(weighted)cin>>w;
           adjList[x].push_back({y,w});
           if(directed==false) adjList[y].push_back({x,w});
       }
       
      }
       void dfsUtil(int source,vector<bool> &visited, vector<int> &result)
       {
            visited[source]=true;
            result.push_back(source);
            for(auto node:adjList[source])
            {
                if(visited[node.first]==false)
                dfsUtil(node.first,visited,result);
            }
          
       }
       vector<int> dfs(int source)
       {
           vector<int> result; //initialise the vector for storing the result 
           vector<bool> visited(nvertices+1,false); //initialise the visited array 
           for (int i=1;i<=nvertices;i++)
           {
               if(visited[i]==false)
               dfsUtil(i,visited,result);
           }
           return result;
       }
       void dfsUtil(int source,vector<bool> &visited)
       {
            visited[source]=true;
            for(auto node:adjList[source])
             if(!visited[node.first])
              dfsUtil(node.first,visited);
            
       }
       int dfs()
       {
           int num_of_cc=0;
           vector<bool> visited(nvertices+1,false);
           for(int i=1;i<=nvertices;i++)
           if(visited[i]==false)
           dfsUtil(i,visited) , num_of_cc++;
           return num_of_cc;
       }
       vector<int> bfs(int source)
       {
           vector<int> result;
           queue<int> q;
           vector<bool> visited(nvertices+1,false);
           q.push(source);
           visited[source]=true;
           while(!q.empty())
           {
               int current=q.front();
               result.push_back(current);
               q.pop();
               for(auto node:adjList[current])
               {
                   if(visited[node.first]==false)
                   {
                       q.push(node.first);
                       visited[node.first]=true;
                   }
               }
           }
           return result;
       }
       bool detectCycleUtil(int source,vector<bool> &visited,int par=-1)
       {
           visited[source]=true;
           for(auto node:adjList[source])
           {
              if(node.first == par)continue;
              else if (!visited[node.first])
              {
                  if (detectCycleUtil(node.first , visited , source))return true;
              }
              else if (node.first != par)return true;
              
           }
           return false;
       }
       bool detectCycleUtil(int source,vector<bool> &visited,vector<bool> &dfsVisited,int par=-1)
       {
           visited[source]=true;
           dfsVisited[source]=true;
           for(auto node:adjList[source])
           {
              if (!visited[node.first])
              {
                  if (detectCycleUtil(node.first , visited ,dfsVisited, source))return true;
              }
              else if (dfsVisited[node.first])return true;
            }
           dfsVisited[source]=false;
           return false;
       }
       bool detectCycle(int source)
       {
           vector<bool> visited(nvertices+1,false);
           vector<bool> dfsVisited(nvertices+1,false);
           if(!is_directed)
           {
               for(int i=1;i<=nvertices;i++)
               if (!visited[i])
               if(detectCycleUtil(i,visited))return true;
               return false;
           }
           else{
               for(int i=1;i<=nvertices;i++)
               if (!visited[i])
               if(detectCycleUtil(i,visited,dfsVisited))return true;
               return false;
           }
           
       }
       int findParent(int node,vector<int> &parent)
       {
           if(parent[node]==node)return node;
           return parent[node]=findParent(parent[node],parent);
           
       }
       void UnionNodes(int node1,int node2,vector<int> &parent)
       {
           int x=findParent(node1,parent);
           int y=findParent(node2,parent);
           parent[x]=y;
       }
       void ReadEdgeList(int nvertices,int nedges)
       {
           for(int i=0;i<nedges;i++)
           {
               int x,y,w;
               cin>>x>>y>>w;
               Edgelist.push_back(Edge(x,y,w));
           }
       }
       int MST()
       {
           //Kruskal's algorithm
           int ans=0;
           
           //the edgelist has been created successfully 
           sort(Edgelist.begin(),Edgelist.end());

           vector<int> parent(nvertices+1);//initialise the parent array first
           for(int i=1;i<=nvertices;i++)parent[i]=i;
           for(int i=0;i<Edgelist.size();i++)
           {
               Edge e=Edgelist[i];
               int first_node=e.x;
               int second_node=e.y;
               int weight=e.w;
               int parent_first=findParent(first_node,parent);
               int parent_second=findParent(second_node,parent);
               if(parent_first!=parent_second)
               {
                   ans+=weight;
                   UnionNodes(first_node,second_node,parent);
               }

           }
           return ans;
       }
       int numberOfCC()
       {
          int number_of_cc=0;
          return dfs();
          
       }
       //Number of Articulated Points
       void dfsFindArticulationPoints(int current_node,int par,int &timer,set<int> &AP,vector<int> &inTime,vector<int> &lowTime,vector<bool> &visited)
       {
           inTime[current_node]=lowTime[current_node]=timer++;
           visited[current_node]=true;
           int rootChildren=0;
           for(auto node : adjList[current_node])
           {
               if(node.first==par)continue;
               else if(visited[node.first])
               {
                   lowTime[current_node]=min(inTime[node.first],lowTime[current_node]);
               }
               else
               {
                   dfsFindArticulationPoints(node.first,current_node,timer,AP,inTime,lowTime,visited);
                   lowTime[node.first]=min(lowTime[node.first],lowTime[current_node]);
                   
                   if (par!=-1 && lowTime[node.first] >= inTime[current_node])
                   AP.insert(current_node);

                   rootChildren++;

               }
           }
           if(par==-1 && rootChildren>1)AP.insert(current_node);
          

       }
       set<int> findArticulationPoints()
       {
           vector<int> lowTime(nvertices+1,0) , inTime(nvertices+1,0);
           vector<bool> visited(nvertices+1,0);
           int timer=0;
           set<int> result;
           dfsFindArticulationPoints(1,-1,timer,result,inTime,lowTime,visited);
           return result;
       }

       //Number of Bridges
       void dfsFindBridges(int current_node,int par,int &timer,vector<pair<int,int> > &bridges,vector<int> &inTime,vector<int> &lowTime,vector<bool> &visited)
       {
           inTime[current_node]=lowTime[current_node]=timer++;
           visited[current_node]=true;
           for(auto node : adjList[current_node])
           {
               if(node.first==par)continue;
               else if(visited[node.first])
               {
                   lowTime[current_node]=min(inTime[node.first],lowTime[current_node]);
               }
               else
               {
                   dfsFindBridges(node.first,current_node,timer,bridges,inTime,lowTime,visited);
                   if (lowTime[node.first] > inTime[current_node])
                   bridges.push_back({node.first,current_node});

                   lowTime[node.first]=min(lowTime[node.first],lowTime[current_node]);

               }
           }
          

       }
       vector<pair<int,int> > findBridges()
       {
           vector<int> lowTime(nvertices+1,0) , inTime(nvertices+1,0);
           vector<bool> visited(nvertices+1,0);
           int timer=0;
           vector<pair<int,int> > result;
           dfsFindBridges(1,-1,timer,result,inTime,lowTime,visited);
           return result;
       }
       ~Graph()
       {
           delete(adjList);
        //    cout<<"The memory relesed\n";
       }

};
int main()
{
    int nvertices,nedges;
    cin>>nvertices;
    cin>>nedges;
    Graph G;

    //Uncomment any one of the below initialisations as per your Graph requirement

    G.initialise(nvertices,nedges) ;                    // for undirected-unweighted
    //G.initialise(nvertices,nedges,false,true)   ;       // for undirected-unweighted
    //G.initialise(nvertices,nedges,true)    ;            // for directed-unweighted
    //G.initialise(nvertices,nedges,true,true)  ;         // for directed-weighted
    // G.ReadEdgeList(nvertices,nedges);                   //for reading Edge-List 

    // G.initialise(nvertices,nedges); //initialise the graph

    
    //Uncomment below line for dfs
    vector<int> dfsresult=G.dfs(1);
    
    //Uncomment below line for bfs
    //vector<int> bfsresult=G.bfs(1);

    
    //Uncomment below line for Cycle detection
    //cout<<G.detectCycle(1)<<"\n";


    //Uncomment below line for number of connected components 
    //cout<<G.numberOfCC()<<"\n";
    
    //Uncomment below line for printing the cost for  minimal spanning tree 
    //cout<<G.MST()<<"\n";


    //Uncomment below line for getting the bridges lists
    //vector<pair<int,int> > bridges=G.findBridges();
    

    //Uncomment below line for getting the set of articulation points in the graph
    //set<int> articulationPoints=G.findArticulationPoints();

    

    return 0;

}