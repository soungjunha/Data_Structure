#include "PlayList.h"

PlayList::PlayList():head(nullptr),cursor(nullptr),count(0),time(0),data("") {

}

PlayList::~PlayList() {
    if (head == nullptr) {
        return;
    }

    PlayListNode* current = head;
    head->get_prev()->set_next(nullptr); 

    while (current != nullptr) {
        PlayListNode* next_node = current->get_next();
        delete current;
        current = next_node;
    }
    head = nullptr;
}

void PlayList::insert_node(const string& artist,const string& title,const int& runtime) {

    if (!exist(artist,title)||full())
    {
        throw "not exist";
    }

    PlayListNode* new_node=new PlayListNode(artist,title,runtime);

    if (head==nullptr)
    {
        head=new_node;
        head->set_next(head);
        head->set_prev(head);
    }
    else
    {
        cursor=head->get_prev();
        new_node->set_next(head);
        new_node->set_prev(cursor);
        head->set_prev(new_node);
        cursor->set_next(new_node);
    }
    count++;
    time+=runtime;
}

void PlayList::delete_node(const string& data) {
    if (head == nullptr) {
        return;
    }

    PlayListNode* node_to_delete = nullptr;
    int initial_count = count;

    for (int i = 0; i < initial_count; ++i) {
        if (cursor->get_artist() == data || cursor->get_title() == data) {
            node_to_delete = cursor;
            cursor = cursor->get_next();

            if (count == 1) {
                head = nullptr;
            } else {
                node_to_delete->get_prev()->set_next(node_to_delete->get_next());
                node_to_delete->get_next()->set_prev(node_to_delete->get_prev());

                if (node_to_delete == head) {
                    head = cursor;
                }
            }
            time -= node_to_delete->get_runtime();
            delete node_to_delete;
            count--;

        } else {
            cursor = cursor->get_next();
        }
    }

}

void PlayList::delete_node(bool only,const string& artist,const string& title) {
    cursor=head;
    bool found = false;

    do {
    if (cursor->get_artist() == artist && cursor->get_title() == title) {
        found = true;
        break;
    }
    cursor = cursor->get_next();
    } while (cursor != head);
    
    if (!found) {
        if (only)
        {
            throw "notfound";
        }
        else{
            return;
        }
        
    }

    if (count == 1) {
    head = nullptr;
    }
    else {
        cursor->get_prev()->set_next(cursor->get_next());
        cursor->get_next()->set_prev(cursor->get_prev());

        if (cursor == head) {
            head = cursor->get_next();
        }
    }

    time -= cursor->get_runtime();
    delete cursor;
    count--;

}

bool PlayList::empty() {
    return head==nullptr;
   
}

bool PlayList::full() {
    return count>=10;
}

bool PlayList::exist(const string& artist, const string& title) {
    if (head == nullptr) {
        return true;
    }
    cursor=head;
    do{
        if (cursor->get_artist()==artist&&cursor->get_title()==title)
        {
            return false;
        }
        cursor=cursor->get_next();
    }while (cursor!=head);
    
    return true;
}

void PlayList::print(ofstream& flog) {

    cursor=head;
    do{
        int min = cursor->get_runtime() / 60;
        int sec = cursor->get_runtime() % 60;
        string runtime=to_string(min)+':'+(sec < 10 ? "0" : "")+to_string(sec);
        flog<<cursor->get_artist()<<'/'<<cursor->get_title()<<'/'<<runtime<<endl;
        cursor=cursor->get_next();
    }while (cursor!=head);
    flog<<"Count :"<<count<<"/10"<<endl;
    flog<<"Time :"<<run_time()<<endl;
}

string PlayList::run_time() {

    int min = time / 60;
    int sec = time % 60;

    return to_string(min)+"min "+(sec < 10 ? "0" : "")+to_string(sec)+"sec";
}