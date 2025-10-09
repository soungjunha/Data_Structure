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

vector<pair<string,string>> TitleBST::search(const string& title) {

    target=root;

    while (target!=nullptr)
    {
        
        if (title == target->get_title()) {
            vector<pair<string, string>> result;
            auto const& artists = target->get_artist();
            auto const& rts = target->get_rt();
            for (int i = 0; i < target->get_artist().size(); i++)
            {
                int min = rts[i] / 60;
                int sec = rts[i] % 60;
                string tmp=to_string(min)+':'+(sec < 10 ? "0" : "")+to_string(sec);
                result.push_back(pair<string, string>(artists[i],tmp));
            }
            return result;
        }
        else if (title < target->get_title()) {
            target = target->get_left();
        }
        else {
            target = target->get_right();
        }
        
    }
    throw "not found";

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