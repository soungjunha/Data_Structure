#include "ArtistBST.h"


ArtistBST::ArtistBST():root(nullptr),parent(nullptr),data(""),target(nullptr) {

}
ArtistBST::~ArtistBST() {
    clear(this->root);
}

void ArtistBST::insert(const MusicQueueNode* node) {
    if (root==nullptr)
    {
        root=new ArtistBSTNode(node);
    }
    else
    {
        target=root;
        while (true) {
            try
            {
                if (node->get_artist() == target->get_artist()) {
                    target->set(node->get_title(), node->get_run_time());
                    return;
                }
                else if (node->get_artist() < target->get_artist()) {
                    if (target->get_left() == nullptr) {
                        target->set_left(new ArtistBSTNode(node));
                        return;
                    }
                    target = target->get_left();
                }
                else {
                    if (target->get_right() == nullptr) {
                        target->set_right(new ArtistBSTNode(node));
                        return;
                    }
                    target = target->get_right();
                }
            }
            catch(const char* errorMessage)
            {
                throw "same music";
            }
        }

    }
}

void ArtistBST::search(const string& artist) {

}

void ArtistBST::print(const string& artist) {

}

void ArtistBST::delete_node(const string& artist) {

}

void ArtistBST::clear(ArtistBSTNode* node) {
    if (node == nullptr) {
        return;
    }

    clear(node->get_left());
    
    clear(node->get_right());

    delete node;
}