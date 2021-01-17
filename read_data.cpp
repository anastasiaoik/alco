// ανάγνωση δεδομένων από ένα αρχείο προβλήματος (με κατάληξη stu) που περιέχει
// πληροφορίες για τις εξετάσεις μαθημάτων στις οποίες είναι εγγεγραμμένος
// ο κάθε σπουδαστής

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <cmath>
#include <algorithm>
#include <list>

using namespace std;


int count_common_elements(set<int> s1, set<int> s2)
{
    int c = 0;
    for (int x : s1)
    {
        for (int y : s2)
        {
            if (x == y)
            {
                c=1;
                break;
            }
        }
    }
    return c;
}

class Graph
{
    int V;          // No. of vertices
    list<int> *adj; // A dynamic array of adjacency lists
public:
    // Constructor and destructor
    Graph(int V)
    {
        this->V = V;
        adj = new list<int>[V];
    }
    ~Graph() { delete[] adj; }

    // function to add an edge to graph
    void addEdge(int v, int w);

    // Prints greedy coloring of the vertices
    int greedyColoring();
};

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w);
    adj[w].push_back(v); // Note: the graph is undirected
}

int Graph::greedyColoring()
{
    int result[V];

    // Assign the first color to first vertex
    result[0] = 0;

    // Initialize remaining V-1 vertices as unassigned
    for (int u = 1; u < V; u++)
        result[u] = -1; // no color is assigned to u

    // A temporary array to store the available colors. True
    // value of available[cr] would mean that the color cr is
    // assigned to one of its adjacent vertices
    bool available[V];
    for (int cr = 0; cr < V; cr++)
        available[cr] = true;

    // Assign colors to remaining V-1 vertices
    for (int u = 1; u < V; u++)
    {
        // Process all adjacent vertices and flag their colors
        // as unavailable
        list<int>::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
            if (result[*i] != -1)
                available[result[*i]] = false;

        // Find the first available color
        int cr;
        for (cr = 0; cr < V; cr++)
            if (available[cr])
                break;

        result[u] = cr; // Assign the found color

        // Reset the values back to true for the next iteration
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
            if (result[*i] != -1)
                available[result[*i]] = true;
    }

    // print the result
    int max=result[0];
    for (int u = 0; u < V; u++){
             if(result[u]>max)
             max=result[u];
             }
    return(max+1);         
}

void read_data(string fn, int students, int exams)
{
    // ανάγνωση δεδομένων
    vector<set<int>> exam_students(exams + 1);
    fstream fs(fn);
    if (!fs.is_open())
    {
        cerr << "Could not open file " << fn << std::endl;
        exit(-1);
    }
    int student_id = 0;
    string line;
    while (getline(fs, line))
    {
        if (line.empty())
            continue;
        student_id++;
        istringstream iss(line);
        int exam_id;
        while (iss >> exam_id)
        {
            exam_students[exam_id].insert(student_id);
        }
    }
    fs.close();

    // δημιουργία πίνακα γειτνίασης
    int *adj_matrix = new int[exams * exams];
    for (int i = 0; i < exams; i++)
    {
        for (int j = 0; j < exams; j++)
        {
            if (i == j){
                adj_matrix[i * exams + j] = 0; // adj_matrix[i][j]
                continue;
            }
            int c = count_common_elements(exam_students[i + 1], exam_students[j + 1]);
            if (c > 0){
            adj_matrix[i * exams + j] = c;
            }
        }
    }

    // δημιουργία πίνακα κορυφων
    // Υπολογισμός συντελεστή πυκνότητας
    int *degrees = new int[exams];
    int c = 0;
    for (int i = 0; i < exams; i++)
    {
        degrees[i]=0;
        for (int j = 0; j < exams; j++)
        {
            if (adj_matrix[i * exams + j] > 0)
            {
                c++;
                degrees[i]+=1;
            }
        }
    }
    double cd = double(c) / double(exams * exams);
    
    sort(degrees,degrees+exams);
    double mean=0;
    for (int i = 0; i < exams; i++){
        mean=mean+degrees[i];
    }
    mean=mean/exams;

    double cv=0;
    double count=0;
    for (int i = 0; i < exams; i++){
        cv=cv+((degrees[i]-mean)*(degrees[i]-mean));
        count++;
    }
    cv=sqrt((1/count)*cv)/mean*100;

    Graph g1(exams);
    for (int i = 0; i < exams; i++){
        for (int j = i; j < exams; j++){
            if (adj_matrix[i * exams + j] > 0)
            {
                g1.addEdge(i,j);
            }
        }   
    }
    cout << "Name:" << fn;
    cout << "   |V|:" << exams;
    cout << "   Conflict Density:" << cd;
    cout << "   min:" << degrees[0];
    cout << "   median:" << degrees[(exams-1)/2];
    cout << "   max:" << degrees[exams-1];
    cout << "   mean:" << mean;
    cout << "   cv:" << cv;
    cout << "   greedy:" << g1.greedyColoring() << endl;
    delete[] adj_matrix,g1,degrees;
}

int main()
{
    read_data("datasets/hec-s-92.stu", 2823, 81);
    read_data("datasets/sta-f-83.stu", 611, 139);
    read_data("datasets/yor-f-83.stu", 941, 181);
    read_data("datasets/ute-s-92.stu", 2749, 184);
    read_data("datasets/ear-f-83.stu", 1125, 190);
    read_data("datasets/tre-s-92.stu", 4360, 261);
    read_data("datasets/lse-f-91.stu", 2726, 381);
    read_data("datasets/kfu-s-93.stu", 5349, 461);
    read_data("datasets/rye-s-93.stu", 11483, 486);
    read_data("datasets/car-f-92.stu", 18419, 543);
    read_data("datasets/uta-s-92.stu", 21266, 622);
    read_data("datasets/car-s-91.stu", 16925, 682);
    read_data("datasets/pur-s-93.stu", 30029, 2419);
}