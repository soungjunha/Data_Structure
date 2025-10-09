#include "PlayList.h"

PlayList::PlayList():head(nullptr),cursor(nullptr),count(0),time(0),data("") {

}

PlayList::~PlayList() {
    while (head!=nullptr)
    {
        PlayListNode* tmp=head;
        head=head->get_next();
        delete tmp;
    }
    count=0;
    time=0;
}

void PlayList::insert_node(const string& artist,const string& title,const int& runtime) {

    if (!exist(artist,title)&&full())
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

string PlayList::print() {
    return 0;
}

int PlayList::run_time() {
    return 0;
}