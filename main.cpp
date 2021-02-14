#include <iostream>
#include <unordered_set>
#include <vector>

using namespace std;


int max(int a, int b){
    if (a > b)
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
        head = new ListNode(0);
        tail = new ListNode(0);
        head->next = tail;
        tail->prev = head;
    }
    ~LinkedList(){
        ListNode* tmp;
        tmp = head->next;
        while (tmp != nullptr){
            free(head);
            tmp = tmp->next;
        }
        free(tail);
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

        free(to_del_node);
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
    SimpleGraph* graph;
    ListNode** elements_array;
    LexBFSstructure(){
        classes = new LinkedList();
        elements = new LinkedList();

        classes->head->other_list = elements->head;
        elements->head->other_list = classes->head;
        classes->tail->other_list = elements->tail;
        elements->tail->other_list = classes->tail;
    }
    ~LexBFSstructure(){
        delete classes;
        delete elements;
        delete elements_array;
    }
    void add_initial_elements(int size){
        elements_array = new ListNode*[size];
        //gen array
        int arr[size];
        for (int i=0; i<size; i++)
            arr[i] = i;



        ListNode* new_node;
        for(int i=0; i< size; i++){
            new_node = new ListNode(arr[i]);
            elements->add_node_before_tail(new_node);
            elements_array[arr[i]] = new_node;
        }
    }
    void add_initial_class(){
        ListNode* first_class = new ListNode(0);
        classes->add_node_before_tail(first_class);
        ListNode* tmp = elements->head->next;
        first_class->other_list = tmp;
        while (tmp->next) {
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
        last_elem = last_elem_node->val;
        if(last_class_node->other_list == last_elem_node)
            classes->del_node_before_tail();
        elements->del_node_before_tail();
        elements_array[last_elem] = nullptr;
        return last_elem;
    }

    void add_elem_to_existing_class(ListNode* class_node, ListNode* elem_node){
        //add as first element
        ListNode* first_elem_of_class = class_node->other_list;
        elements->add_node_before(elem_node, first_elem_of_class);
        class_node->other_list = elem_node;
        elem_node->other_list = class_node;

    }
    int partition(vector<int> neighbors, int time){
        ListNode* tmp_elem_node;
        ListNode* tmp_class_node, *tmp_next_class_node;
        ListNode* new_class_node;
        int neighbor;
        int tmp_clique = 1;

        for (int i=0; i < neighbors.size(); i++) {
            neighbor = neighbors[i];
            if(elements_array[neighbor] == nullptr) {
                tmp_clique++;
                continue;
            }

            tmp_elem_node = elements_array[neighbor];

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

        }
        return tmp_clique;
    }
    void print_elements(){
        ListNode* tmp = elements->head->next;
        while (tmp->next){
            cout << tmp->val<< " ";
            tmp = tmp->next;
        }
        cout<<endl;
    }
    void print_corresponding_classes(){
        ListNode* tmp = elements->head->next;
        while (tmp->next){
            cout << tmp->other_list->other_list->val << " ";
            tmp = tmp->next;
        }
        cout<<endl;
    }
    void print_classes(){
        ListNode* tmp = classes->head->next;
        while (tmp->next){
            cout << tmp->other_list->val<< " ";
            tmp = tmp->next;
        }
        cout<<endl;
    }
    void print_corresponding_elements(){
        ListNode* tmp = classes->head->next;
        while (tmp->next){
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
        cout<<endl;

        return results;
    }

    int get_max_possible_clique(){
        int* counter;
        counter = new int[graph->size];
        for(int i=0; i< graph->size; i++){
            counter[i] = 0;
        }
        for(int i=0; i< graph->size; i++){
            counter[graph->vertices[i].size()]  ++;
        }
        for(int i=graph->size - 2 ; i>= 0; i--){
            counter[i] += counter[i+1];
            if( counter[i] >= i + 1) {
                free(counter);
                return i + 1;
            }
        }


    }

    int get_max_clique(){

        int pivot,max_cl=0,tmp_cl,max_pos_cl;
        max_pos_cl = get_max_possible_clique();

        for(int i=0; i<graph->size; i++){
            pivot = get_pivot();

            tmp_cl = partition(graph->vertices[pivot], i+1);
            max_cl = max(max_cl,tmp_cl);
            if(max_cl == max_pos_cl) {
                return max_cl;
            }
        }

        return max_cl;
    }
};

int max_clique_algorithm(SimpleGraph* H){
    int max_cl = 0;
    LexBFSstructure* lexBfSstructure = new LexBFSstructure();
    lexBfSstructure->initialize_structure(H);

    max_cl = lexBfSstructure->get_max_clique();

    return max_cl;
}

int main() {
    int Z;
    scanf("%d", &Z);
    int N, M;
    int first, second;
    bool tree, clique;
    SimpleGraph* H;
    while (Z--){
        scanf("%d", &N);
        scanf("%d", &M);

        tree = false;
        clique = false;
        if(M == N - 1) {
            tree = true;
        }else if(M == N*(N-1)/2){
            clique = true;
        }
        else{
            H = new SimpleGraph(N);
        }

        for(int i = 0; i < M; i++){
            scanf("%d", &first);
            scanf("%d", &second);

            if(!tree && !clique)
                H->addEdge(first - 1, second - 1);
        }
        if(tree)
            cout<<2<<endl;
        else if(clique)
            cout<<N-1<<endl;
        else
            cout << max_clique_algorithm(H) - 1 << endl;

        delete H;

    }
    return 0;
}