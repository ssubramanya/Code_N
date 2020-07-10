#include <iostream>
#include <map>
using namespace std;

typedef struct node {
	int data;
	struct node* next;
	struct node* prev;
}node;

typedef struct s_lru {
	int size;
	map <int, node*> mp;
	node* head;
}s_lru;

node* getNode(int page) {
	node* temp = (node *)malloc(sizeof(node));
	temp->data=page;
	temp->next=temp;
	temp->prev=temp;
	return temp;
}

void ca_refer(int page, struct s_lru & lru) {
	map<int,node *>::iterator it = lru.mp.find(page);
	if(it!=lru.mp.end()) {
		node* temp=it->second;
		node* prev=temp->prev;
		prev->next = temp->next;
		temp->next->prev = prev;
		free(temp);
		temp = getNode(page);
		temp->next = lru.head;
		temp->prev=lru.head->prev;
		lru.head->prev->next=temp;
		lru.head->prev=temp;
		lru.head=temp;
		lru.mp[page]=temp;
	}
	else {
		 if(lru.mp.size() >= lru.size) {
		 	node* temp = lru.head->prev;
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;
			free(temp);
		 }
		node* temp = getNode(page);
		if(lru.mp.size() == 0) {
			lru.mp[page]=temp;
			lru.head=temp;
			return;
		}
		temp->next = lru.head;
		temp->prev	= lru.head->prev;
		lru.head->prev->next=temp;
		lru.head->prev = temp;
		lru.head	= temp;
		lru.mp[page]	= temp;
	}
}

void display(node* head) {
    node* temp = head;
	while(temp->next != head) {
		cout<<temp->data<<" ";
        temp=temp->next;
}
	cout<<temp->data<<endl;
}

int main() {
	struct s_lru s1;
	int n;
	cin>>n;
	s1.size=n;
	s1.head=NULL;
    ca_refer(1,s1); 
    ca_refer(2,s1); 
    ca_refer(3,s1); 
    ca_refer(1,s1); 
    ca_refer(4,s1); 
    ca_refer(5,s1); 
    display(s1.head); 
	return 0;
}