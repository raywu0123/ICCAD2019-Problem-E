#include "BipGraph.h"


// Returns size of maximum matching
int BipGraph::hopcroftKarp()
{
    // pairU[u] stores pair of u in matching where u
    // is a vertex on left side of Bipartite Graph.
    // If u doesn't have any pair, then pairU[u] is NIL
    pairU = new int[m+1];

    // pairV[v] stores pair of v in matching. If v
    // doesn't have any pair, then pairU[v] is NIL
    pairV = new int[n+1];

    // dist[u] stores distance of left side vertices
    // dist[u] is one more than dist[u'] if u is next
    // to u'in augmenting path
    dist = new int[m+1];

    // Initialize NIL as pair of all vertices
    for (int u=0; u<=m; u++)
        pairU[u] = NIL;
    for (int v=0; v<=n; v++)
        pairV[v] = NIL;

    // Initialize result
    int result = 0;

    // Keep updating the result while there is an
    // augmenting path.
    while (bfs())
    {
        // Find a free vertex
        for (int u=1; u<=m; u++)

            // If current vertex is free and there is
            // an augmenting path from current vertex
            if (pairU[u]==NIL && dfs(u))
                result++;
    }
    return result;
}

// Returns true if there is an augmenting path, else returns
// false
bool BipGraph::bfs()
{
    queue<int> Q; //an integer queue

    // First layer of vertices (set distance as 0)
    for (int u=1; u<=m; u++)
    {
        // If this is a free vertex, add it to queue
        if (pairU[u]==NIL)
        {
            // u is not matched
            dist[u] = 0;
            Q.push(u);
        }

            // Else set distance as infinite so that this vertex
            // is considered next time
        else dist[u] = INF;
    }

    // Initialize distance to NIL as infinite
    dist[NIL] = INF;

    // Q is going to contain vertices of left side only.
    while (!Q.empty())
    {
        // Dequeue a vertex
        int u = Q.front();
        Q.pop();

        // If this node is not NIL and can provide a shorter path to NIL
        if (dist[u] < dist[NIL])
        {
            // Get all adjacent vertices of the dequeued vertex u
            list<int>::iterator i;
            for (i=adj[u].begin(); i!=adj[u].end(); ++i)
            {
                int v = *i;

                // If pair of v is not considered so far
                // (v, pairV[V]) is not yet explored edge.
                if (dist[pairV[v]] == INF)
                {
                    // Consider the pair and add it to queue
                    dist[pairV[v]] = dist[u] + 1;
                    Q.push(pairV[v]);
                }
            }
        }
    }

    // If we could come back to NIL using alternating path of distinct
    // vertices then there is an augmenting path
    return (dist[NIL] != INF);
}

// Returns true if there is an augmenting path beginning with free vertex u
bool BipGraph::dfs(int u)
{
    if (u != NIL)
    {
        list<int>::iterator i;
        for (i=adj[u].begin(); i!=adj[u].end(); ++i)
        {
            // Adjacent to u
            int v = *i;

            // Follow the distances set by BFS
            if (dist[pairV[v]] == dist[u]+1)
            {
                // If dfs for pair of v also returns
                // true
                if (dfs(pairV[v]) == true)
                {
                    pairV[v] = u;
                    pairU[u] = v;
                    return true;
                }
            }
        }

        // If there is no augmenting path beginning with u.
        dist[u] = INF;
        return false;
    }
    return true;
}

// Constructor
BipGraph::BipGraph(int m, int n)
{
    this->m = m;
    this->n = n;
    adj = new list<int>[m+1];
}

// To add edge from u to v and v to u
void BipGraph::addEdge(int u, int v)
{
    adj[u].push_back(v); // Add u to v’s list.
}

void BipGraph::maximum_independent_set() {
    if (!matched) {
        hopcroftKarp();
    }

    // build adjacent list of v nodes
    adj_v = new list<int>[n+1];
    for(int i=1; i<=m; i++) {
        const list<int>& adj_list= adj[i];
        list<int>::const_iterator it;
        for(it=adj_list.begin(); it != adj_list.end(); it++) {
            adj_v[*it].push_back(i);
        }
    }
    // init visited flags
    visited_u = new bool[m + 1];
    visited_v = new bool[n + 1];
    for(int i=0; i<=m; i++)
        visited_u[i] = false;
    for(int i=0; i<=n; i++)
        visited_v[i] = false;

    for(int i=1; i<=m; i++) {
        if(pairU[i] == NIL) {
            myDFS(i, true, 0);
        }
    }
    for(int i=1; i<=n; i++) {
        if(pairV[i] == NIL) {
            myDFS(i, false, 0);
        }
    }
    for(int i=1; i<=m; i++) {
        if(pairU[i] != NIL) {
            myDFS(i, true, 0);
        }
    }
    for(int i=1; i<=n; i++) {
        if(pairV[i] != NIL) {
            myDFS(i, false, 0);
        }
    }
}


void BipGraph::myDFS(int idx, bool in_U, int level) {
    assert(idx != 0);
    if(in_U and visited_u[idx]) return;
    if(not in_U and visited_v[idx]) return;

    if(in_U) visited_u[idx] = true;
    else visited_v[idx] = true;

    if(level % 2 == 0) {
        if(in_U) {
            U_ind.push_back(idx);
            list<int>::iterator it;
            for(it=adj[idx].begin(); it != adj[idx].end(); ++it) {
                if(*it == 0) continue;
                myDFS(*it, !in_U, level+1);
            }
        }
        else {
            V_ind.push_back(idx);
            list<int>::iterator it;
            for(it=adj_v[idx].begin(); it != adj_v[idx].end(); ++it) {
                if(*it == 0) continue;
                myDFS(*it, !in_U, level+1);
            }
        }
    } else {
        if(in_U and pairU[idx] != NIL) myDFS(pairU[idx], !in_U, level+1);
        else if(pairV[idx] != NIL) myDFS(pairV[idx], !in_U, level+1);
    }
}
