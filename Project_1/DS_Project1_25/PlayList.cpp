#include "PlayList.h"

PlayList::PlayList():head(nullptr),cursor(nullptr),count(0),time(0),data("") {

}

PlayList::~PlayList() {
    if (head == nullptr) {
        return;
    }

    PlayListNode* tail = head->get_prev();
    tail->set_next(nullptr);

    while (head != nullptr) {
        PlayListNode* tmp = head;
        head = head->get_next();
        delete tmp;
    }
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

void PlayList::delete_node(const string& artist,const string& title) {

}

bool PlayList::empty() {
    return count==0;
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