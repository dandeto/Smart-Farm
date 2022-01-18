// IF YOU WANT TO RUN THIS TEST, MAKE `Node nodes[SIZE];` PUBLIC IN NODE.h
// THEN CHANGE `SIZE` TO 5 AND `LIMIT` TO 3
#include <iostream>
#include <iomanip>
#include "../LoRa_Recieve/Node.h"
using namespace std;

void print(NodeManager n) {
	cout << "ID | Requests | Responses \n";
	for(int i=0; i<SIZE; ++i)
		if(n.nodes[i].id != 0)
			cout << left
		       << setw(2) << n.nodes[i].id << " | "
			     << setw(8) << n.nodes[i].requests << " | "
			     << setw(9) << n.nodes[i].responses << "\n";
	cout << '\n';
}

int main() {
	NodeManager n;

	cout << "Get ID: " << n.next() << '\n';
	cout << "Get ID: " << n.next() << '\n';
	cout << "Add ID: " << n.add() << '\n';
	cout << "Add ID: " << n.add() << '\n';
	cout << "Add ID: " << n.add() << '\n';
	cout << "Add ID: " << n.add() << '\n';
	cout << "Add ID: " << n.add() << '\n';
	cout << "Add ID: " << n.add() << '\n';
	cout << '\n';

	n.request(1);
	n.response(1);
	n.request(4);
	n.request(4);
	n.request(4);
	n.response(4);
	n.request(3);
	n.response(3);
	n.request(2);
	n.request(2);
	n.request(2);
	n.request(5);

	cout << "Get ID: " << n.next() << '\n';
	cout << "Get ID: " << n.next() << '\n';

	print(n);
	
	cout << "Prune Before Trying To Add Another Node\n\n";
	n.prune();

	print(n);

	cout << "Get ID: " << n.next() << '\n';
	cout << "Get ID: " << n.next() << "\n\n";

	cout << "Add Another Node" << "\n";
	cout << "Add ID: " << n.add() << "\n\n";
	
	print(n);

	cout << "Get ID: " << n.next() << '\n';
	cout << "Get ID: " << n.next() << '\n';
	cout << "Get ID: " << n.next() << '\n';

	n.request(1);
	n.request(1);
	n.request(1);
	n.request(4);
	n.request(4);
	n.add();

	print(n);
}
