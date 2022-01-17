struct Node {
  int id        = 0,
      requests  = 0,
      responses = 0;
  
  Node() {}
  Node(int id) : id(id) {}
};

// number of nodes to keep track of
#define SIZE 50
// number of requests we allow with no response before removing node
#define LIMIT 5

class NodeManager {
  Node nodes[SIZE];

  bool full() {
    return nodes[SIZE-1].id != 0;
  }

  int find(int id) {
    if(nodes[id-1].id == id) return id-1;
    for(int i=0; i<SIZE; ++i)
      if(nodes[i].id == id) return i;
    return -1; // return invalid index if not found
  }

  bool insert(int i, Node n) {
    if(full()) return false;

    for(; i<SIZE; ++i) {
      if(nodes[i].id == 0) {
        nodes[i] = n;
        return true;
      }
      Node tmp = nodes[i];
      nodes[i] = n;
      n = tmp;
    }
    return false; // never hit - appease compiler
  }

  void remove(int id) {
    int i = find(id);
    if(i > -1) {
      for (; i<SIZE; ++i) {
        if(nodes[i].id == 0) return;
        nodes[i] = nodes[i+1];
      }
      nodes[SIZE-1] = Node(); // last element is undefined - replace it
    }
  }

  // unused - may be useful in future (but probably not)
  bool push(Node n) {
    if(full()) return false;

    for(int i=0; i<SIZE; ++i) {
      if(nodes[i].id == 0) {
        nodes[i] = n;
        return true;
      }
    }
    return false; // never hit - appease compiler
  }

public:
  NodeManager(){}

  // exposed just in case the programmer wants to use this, but there is no need
  void prune() {
    for(int i=0; i<SIZE; ++i) {
      if(nodes[i].requests >= LIMIT && nodes[i].responses == 0)
        remove(nodes[i].id);
      nodes[i].requests  = 0;
      nodes[i].responses = 0;
    }
  }

  // Returns the id it should assign to a new node
  int add() {
    prune(); // free space before adding new node
    if(full()) return 0; // cannot assign valid id because Node array is full

    for(int i=0; i<SIZE; ++i) {
      if(nodes[i].id == 0) {
        insert(i, Node(i+1));
        return i+1; // ex: at index 0, id is 1
      }
      if(nodes[i+1].id && nodes[i].id+1 < nodes[i+1].id) {
        insert(i+1, Node(i+2));
        return i+2;
      }
    }
    return 0; // should never get hit
  }

  // Use this function to track # requests sent to a node
  void request(int id) {
    int i = find(id);
    if(i > -1)
      ++nodes[i].requests;
  }

  // Use this function to track # responses recieved from a node
  void response(int id) {
    int i = find(id);
    if(i > -1)
      ++nodes[i].responses;
  }
};
