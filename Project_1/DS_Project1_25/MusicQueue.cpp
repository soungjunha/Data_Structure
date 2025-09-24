#include "MusicQueue.h"

MusicQueue::MusicQueue() {

}

MusicQueue::~MusicQueue() {

}

bool MusicQueue::empty() {
    return 0;
}

bool MusicQueue::exist() {
    return 0;
}

void MusicQueue::push() {
    if(size<=100){
        throw "queue is full";
    }

}

MusicQueueNode* MusicQueue::pop(){

}

MusicQueueNode* MusicQueue::front() {

}