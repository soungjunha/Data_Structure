#include "MusicQueue.h"

MusicQueue::MusicQueue(): head(nullptr),rear(nullptr),size(0){
}

MusicQueue::~MusicQueue() {
    while (head!=nullptr)
    {
        MusicQueueNode* tmp=head;
        head=head->get_next();
        delete tmp;
    }
    rear=nullptr;
    size=0;
}

bool MusicQueue::empty() {
    return size==0;
}

bool MusicQueue::exist(string artist,string title,string run_time) {
    MusicQueueNode* tmp=head;
    while (tmp!=nullptr)
    {
        if (tmp->get_artist()==artist&&tmp->get_title()==title&&tmp->get_run_time()==run_time)
        {
            return false;
        }
        tmp=tmp->get_next();
    }
    
    return true;
}

void MusicQueue::push(string artist,string title,string run_time) {
    if(size>=100){
        throw "queue is full";
    }
    if(head==nullptr){
        head=rear=new MusicQueueNode(artist,title,run_time);
    }
    else
    {
        if(exist(artist,title,run_time))
        {
            try
            {
                rear->insert(artist,title,run_time);
            }
            catch(const char* errorMessage)
            {
                throw "node is not exist";
            }
            rear=rear->get_next();
        }
        else
        {
            throw "same music";
        }
        
    }
    size++;

}

void MusicQueue::pop(){
    if (empty()) {
        throw "queue is empty";
    }
    MusicQueueNode* tmp = head;
    head = head->get_next();
    if (head != nullptr) {
        head->set_prev(nullptr);
    }
    delete tmp;
    size--;
    if (head == nullptr) {
        rear = nullptr;
    }
    
}

MusicQueueNode* MusicQueue::front() {
    if (!empty())
    {
        return head;
    }
    else
    {
        throw "queue is empty";
    }

}