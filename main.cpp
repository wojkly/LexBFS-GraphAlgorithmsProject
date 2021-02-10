#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <queue>
#include <limits.h>

using namespace std;


int max(int a, int b){
    if (a > b)
        return a;
    return b;
}
int min(int a, int b){
    if (a < b)
        return a;
    return b;
}

struct ListNode{
    int val;
    ListNode* other_list = nullptr;
    ListNode* prev = nullptr;
    ListNode* next = nullptr;
    ListNode(int num){
        val = num;
    }
};
struct LinkedList{
    ListNode* head;
    ListNode* tail;
    LinkedList(){
        head = new ListNode(-1);
        tail = new ListNode(-1);
        head->next = tail;
        tail->prev = head;
    }
    void add_node_before(ListNode* new_node, ListNode* next_node){
        ListNode* prev_node = next_node->prev;

        prev_node->next = new_node;
        next_node->prev = new_node;

        new_node->prev = prev_node;
        new_node->next = next_node;
    }
    void add_node_before_tail(ListNode* new_node){
        add_node_before(new_node, tail);
    }
    void del_node_before(ListNode* next_node){
        ListNode* to_del_node = next_node->prev;
        if(to_del_node == head)
            return;
        ListNode* prev_node = to_del_node-> prev;

        prev_node->next = next_node;
        next_node->prev = prev_node;

        delete to_del_node;
    }
    void del_node_before_tail(){
        del_node_before(tail);
    }
    void del_from_list(ListNode* node){
        ListNode* prev_node = node->prev;
        ListNode* next_node = node->next;
        prev_node->next = next_node;
        next_node->prev = prev_node;
    }
};

struct SimpleGraph {
    int size;
    vector<int> *vertices;

    SimpleGraph(int size) {
        this->size = size;
        vertices = new vector<int>[size];
    }
    void addEdge(int a, int b){
        vertices[a].push_back(b);
        vertices[b].push_back(a);
    }
    void print(){
        for(int i=0; i< this->size; i++){
            cout<<i<<" : ";
            for(int j=0;j < this->vertices[i].size(); j++)
                cout<<this->vertices[i][j]<<" ";
            cout<<endl;
        }
    }
};


struct LexBFSstructure{
    LinkedList* classes;
    LinkedList* elements;
    SimpleGraph* graph = nullptr;
    unordered_map<int, ListNode*> elements_map;
    LexBFSstructure(){
        classes = new LinkedList();
        elements = new LinkedList();

        classes->head->other_list = elements->head;
        elements->head->other_list = classes->head;
        classes->tail->other_list = elements->tail;
        elements->tail->other_list = classes->tail;
    }
    void add_initial_elements(int size){
        for(int i=0; i< size; i++){
            ListNode* new_node = new ListNode(i);
            elements->add_node_before_tail(new_node);
            elements_map[i] = new_node;
        }
    }
    void add_initial_class(){
        ListNode* first_class = new ListNode(0);
        classes->add_node_before_tail(first_class);
        ListNode* tmp = elements->head->next;
        first_class->other_list = tmp;
        while (tmp->val != -1) {
            tmp->other_list = first_class;
            tmp = tmp->next;
        }
    }
    void initialize_structure(SimpleGraph* G){
        graph = G;
        add_initial_elements(G->size);
        add_initial_class();
    }

    int get_pivot(){
        int last_elem;
        ListNode* last_elem_node = elements->tail->prev;
        ListNode* last_class_node = classes->tail->prev;
        if(last_elem_node == elements->head)
            return -1;
        last_elem = last_elem_node->val;
        if(last_class_node->other_list == last_elem_node)
            classes->del_node_before_tail();
        elements->del_node_before_tail();
        elements_map.erase(last_elem);
        return last_elem;
    }

    void add_elem_to_existing_class(ListNode* class_node, ListNode* elem_node){
        ListNode* first_elem_of_class = class_node->other_list;

        elements->add_node_before(elem_node, first_elem_of_class);
        class_node->other_list = elem_node;
        elem_node->other_list = class_node;
    }
    void partition(vector<int> neighbors, int time){
        ListNode* tmp_elem_node;
        ListNode* tmp_class_node, *tmp_next_class_node;
        ListNode* new_class_node;
        unordered_map<int, ListNode*>::const_iterator it;
        int neighbor;

        for (int i=0; i < neighbors.size(); i++) {
            neighbor = neighbors[i];
            it = elements_map.find(neighbor);
            if(it == elements_map.end())
                continue;
            tmp_elem_node = it->second;

            tmp_class_node = tmp_elem_node->other_list;
            tmp_next_class_node = tmp_class_node->next;

            if(tmp_class_node->other_list != tmp_elem_node){
                //dont remove this set
                elements->del_from_list(tmp_elem_node);

                if(tmp_next_class_node->val == time){
                    //if next set was born from last set in this iteration
                    //simply add to it
                    add_elem_to_existing_class(tmp_next_class_node, tmp_elem_node);
                }else{
                    //we need to make a new class

                    new_class_node = new ListNode(time);
                    classes->add_node_before(new_class_node, tmp_next_class_node);

                    elements->add_node_before(tmp_elem_node, tmp_next_class_node->other_list);

                    new_class_node->other_list = tmp_elem_node;
                    tmp_elem_node->other_list = new_class_node;
                }
            }else{
                //first element of the class

                //if not last elem in class
                if(tmp_elem_node->next->other_list == tmp_class_node){
                    elements->del_from_list(tmp_elem_node);
                    //connect class to next element
                    tmp_class_node->other_list = tmp_elem_node->next;

                    //if new class was added before
                    if(tmp_next_class_node->val == time){
                        //add to next class
                        add_elem_to_existing_class(tmp_next_class_node, tmp_elem_node);

                    }else{
                        //make new class
                        new_class_node = new ListNode(time);
                        classes->add_node_before(new_class_node, tmp_next_class_node);

                        elements->add_node_before(tmp_elem_node, tmp_next_class_node->other_list);

                        new_class_node->other_list = tmp_elem_node;
                        tmp_elem_node->other_list = new_class_node;
                    }
                }else{
                    //last elem in this class

                    //if new class exists
                    if(tmp_next_class_node->val == time){
                        //remove this class
                        elements->del_from_list(tmp_elem_node);
                        classes->del_node_before(tmp_next_class_node);
                        //add to next class
                        add_elem_to_existing_class(tmp_next_class_node, tmp_elem_node);
                        //reset time to 0
                        tmp_next_class_node->val = 0;
                    }else {
                        //else() -> last and only elem of this class, dont change anything

                        //reset time to 0
                        tmp_class_node->val = 0;
                    }
                }

            }


//            cout<<"after iteration "<<time<<" neigbor was: "<< neighbor <<endl;
//            print();
        }
    }
    void print_elements(){
        ListNode* tmp = elements->head->next;
        while (tmp->val != -1){
            cout << tmp->val<< " ";
            tmp = tmp->next;
        }
        cout<<endl;
    }
    void print_corresponding_classes(){
        ListNode* tmp = elements->head->next;
        while (tmp->val != -1){
            cout << tmp->other_list->other_list->val << " ";
            tmp = tmp->next;
        }
        cout<<endl;
    }
    void print_classes(){
        ListNode* tmp = classes->head->next;
        while (tmp->val != -1){
            cout << tmp->other_list->val<< " ";
            tmp = tmp->next;
        }
        cout<<endl;
    }
    void print_corresponding_elements(){
        ListNode* tmp = classes->head->next;
        while (tmp->val != -1){
            cout << tmp->other_list->val << " ";
            tmp = tmp->next;
        }
        cout<<endl;
    }
    void print(){
        cout<<"----------------------"<<endl;
        cout<<"elements:"<<endl;
        print_elements();
        cout<<"corresponding_classes:"<<endl;
        print_corresponding_classes();
        cout<<"classes:"<<endl;
        print_classes();
        cout<<"corresponding_elements:"<<endl;
        print_corresponding_elements();
        cout<<"----------------------"<<endl;
    }

    int* get_lex_BFS(){
        int* results = new int[graph->size];

        int pivot;
        for(int i=0; i<graph->size; i++){
            pivot = get_pivot();
            results[i] = pivot;

            partition(graph->vertices[pivot], i+1);
        }
//        cout<<"LEX: ";
//        for(int i=0; i<graph->size; i++){
//            cout<<results[i]<<" ";
//        }
//        cout<<endl;

        return results;
    }
};

int max_clique(SimpleGraph* H){
    int max_cl = 0, tmp_cl;
    LexBFSstructure* lexBfSstructure = new LexBFSstructure();
    lexBfSstructure->initialize_structure(H);
//    lexBfSstructure->print();

    int* peo = lexBfSstructure->get_lex_BFS();

    unordered_set<int> peo_set;

    for(int i=1; i< H->size; i++){
        tmp_cl = 1;
        peo_set.insert(peo[i-1]);

        for(int j=0; j<H->vertices[peo[i]].size(); j++)
            if (peo_set.find(H->vertices[peo[i]][j]) != peo_set.end())
                tmp_cl ++;
//        cout<<" TCL: "<<tmp_cl;
        max_cl = max(max_cl, tmp_cl);
    }
//    cout<<endl;
    return max_cl;
}

int main() {
    int Z;
    scanf("%d", &Z);
    int N, M;
    int first, second;
    bool not_tree;
    SimpleGraph* H;
    while (Z--){
        scanf("%d", &N);
        scanf("%d", &M);

        not_tree = true;
        if(M == N - 1) {
            not_tree = false;
        }else {
            H = new SimpleGraph(N);
        }

        for(int i = 0; i < M; i++){
            scanf("%d", &first);
            scanf("%d", &second);

            if(not_tree) {
                H->addEdge(first - 1, second - 1);
            }
        }
        if(not_tree) {
            cout << max_clique(H) - 1 << endl;
        }
        else
            cout<<2<<endl;

    }
    return 0;
}