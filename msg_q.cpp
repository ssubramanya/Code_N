#include <algorithm> 
#include <iostream>
#include <vector>  
#include <map>

using namespace std;

typedef struct {
    int clientId;
    int window;
} client_s;

typedef struct  {
  int clientId;
  int msgId;
  int msgAge;
} msg_s;


map<int,client_s *> client_list;
map<int,vector<msg_s *>*> msg_list;
map<int,map<int,msg_s *>> msg_map;

bool CompareClientFn(msg_s* l1,msg_s* l2) {
  return l1->msgId > l2->msgId;
}

void send_message(int clientId, int msgId, int msgAge) {
    cout<<clientId<<"\t"<<msgId<<"\t"<<msgAge<<"\n";
}

vector<msg_s* >* getVector() {
    vector<msg_s *>* v= new vector<msg_s *>;
    return v;
}
void register_client(int clientId, int window) {
    client_s* client  =  (client_s *)malloc(sizeof(client_s));
    client->clientId = clientId;
    client->window   = window;
    client_list.insert(pair<int, client_s *>(clientId,client));
    msg_list.insert(pair<int,vector<msg_s *>*>(clientId,getVector()));
}

void adjust_window(int clientId, int window) {
    client_s* client = client_list[clientId];
    client->window  = window;
    map<int,vector<msg_s *>*> :: iterator it;
    it = msg_list.find(clientId);
    vector <msg_s *>* temp1 = it->second;
    while(client->window > 0 && temp1->size() > 0) {
      msg_s* temp = it->second->front();
      pop_heap (temp1->begin(),temp1->end(),CompareClientFn);
      it->second->pop_back();
      map<int,map<int, msg_s *>>:: iterator itr = msg_map.find(clientId);
      map<int, msg_s *>::iterator it_inner = itr->second.find(temp->msgId);
      itr->second.erase(it_inner);
      send_message(temp->clientId,temp->msgId,temp->msgAge);
      client->window--;
    }

}


void new_message(int clientId, int msgId, int msgAge) {
    map<int, client_s *>:: iterator itr;
    itr = client_list.find(clientId);
    if(itr != client_list.end()){
       if(itr->second->window > 0) {
           send_message(clientId,msgId,msgAge);
           itr->second--;
       }
       else {
          map<int,map<int, msg_s *>>:: iterator itr = msg_map.find(clientId);
          map<int, msg_s *>::iterator it_inner = itr->second.find(msgId);
          if(it_inner != itr->second.end()) {
            it_inner->second->msgAge=msgAge;
          }
          else {
                msg_s* temp = (msg_s *)malloc(sizeof(msg_s));
                temp->clientId = clientId;
                temp->msgId    = msgId;
                temp->msgAge   = msgAge;
                map<int, vector<msg_s *>*>::iterator it;
                it = msg_list.find(clientId);
                it->second->push_back(temp);
                push_heap(it->second->begin(),it->second->end(),CompareClientFn);
                msg_map[clientId][msgId] = temp;
            }
       }
    } else {
        cout<<"The client has not registered"<<endl;
    }
}
int main() {
    register_client(10,1);
    new_message(10,1,1);
    new_message(10,2,1);
    new_message(10,3,2);
    adjust_window(10,3);
    new_message(10,1,2);
    new_message(10,5,2);
    new_message(10,4,1);
    new_message(10,5,3);
    adjust_window(10,3);
    new_message(20,2,2);
    register_client(20,2);
    new_message(20,1,2);
}
