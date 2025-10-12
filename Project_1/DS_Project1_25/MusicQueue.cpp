#include "MusicQueue.h"

// Constructor: Initializes an empty queue by setting pointers to null and size to 0.
MusicQueue::MusicQueue(): head(nullptr),rear(nullptr),size(0){
}

// Destructor: Iterates through the queue from the head and deletes each node
// to prevent memory leaks.
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

// Checks if the queue is empty.
bool MusicQueue::empty() {
    return size==0;
}

// Checks if a specific song is already in the queue.
// Traverses the list and compares the artist, title, and run_time.
// Returns 'false' if a match is found (it exists), and 'true' otherwise.
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

// Adds a new song to the end (rear) of the queue.
void MusicQueue::push(string artist,string title,string run_time) {
    // Enforce the maximum queue size.
    if(size>=100){
        throw "queue is full";
    }
    // If the queue is empty, the new node is both the head and the rear.
    if(head==nullptr){
        head=rear=new MusicQueueNode(artist,title,run_time);
    }
    else
    {
        // Check for duplicates before adding.
        if(exist(artist,title,run_time))
        {
            try
            {
                // Insert the new node after the current rear node.
                rear->insert(artist,title,run_time);
            }
            catch(const char* errorMessage)
            {
                throw "node is not exist";
            }
            // Update the rear pointer to the newly added node.
            rear=rear->get_next();
        }
        else
        {
            // Throw an exception if the song is a duplicate.
            throw "same music";
        }
        
    }
    size++; // Increment the size of the queue.
}

// Removes the song from the front (head) of the queue.
void MusicQueue::pop(){
    if (empty()) {
        throw "queue is empty";
    }
    // Temporarily store the current head.
    MusicQueueNode* tmp = head;
    // Move the head pointer to the next node.
    head = head->get_next();
    if (head != nullptr) {
        // The new head has no previous node.
        head->set_prev(nullptr);
    }
    // Delete the old head node.
    delete tmp;
    size--;
    // If pop made the queue empty, the rear must also be null.
    if (head == nullptr) {
        rear = nullptr;
    }
}

// Returns a pointer to the front node of the queue.
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