#include "TitleBST.h"


TitleBST::TitleBST():root(nullptr),parent(nullptr),data(""),target(nullptr) {

}
TitleBST::~TitleBST() {
    clear(this->root);
}

void TitleBST::insert(const MusicQueueNode* node) {
    if (root==nullptr)
    {
        root=new TitleBSTNode(node);
    }
    else
    {
        target=root;
        while (true) {
            try
            {
                if (node->get_title() == target->get_title()) {
                    target->set(node->get_artist(), node->get_run_time());
                    return;
                }
                else if (node->get_title() < target->get_title()) {
                    if (target->get_left() == nullptr) {
                        target->set_left(new TitleBSTNode(node));
                        return;
                    }
                    target = target->get_left();
                }
                else {
                    if (target->get_right() == nullptr) {
                        target->set_right(new TitleBSTNode(node));
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

void TitleBST::search(const string& title) {

}

void TitleBST::print(const string& title) {

}

void TitleBST::delete_node(const string& title) {

}

void TitleBST::clear(TitleBSTNode* node) {
    if (node == nullptr) {
        return;
    }

    clear(node->get_left());
    
    clear(node->get_right());

    delete node;
}