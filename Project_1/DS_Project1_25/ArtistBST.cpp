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

vector<pair<string,string>> ArtistBST::search(const string& artist) {

    target=root;

    while (target!=nullptr)
    {
        
        if (artist == target->get_artist()) {
            vector<pair<string, string>> result;
            auto const& titles = target->get_title();
            auto const& rts = target->get_rt();
            for (int i = 0; i < target->get_title().size(); i++)
            {
                int min = rts[i] / 60;
                int sec = rts[i] % 60;
                string tmp=to_string(min)+':'+(sec < 10 ? "0" : "")+to_string(sec);
                result.push_back(pair<string, string>(titles[i],tmp));
            }
            return result;
        }
        else if (artist < target->get_artist()) {
            target = target->get_left();
        }
        else {
            target = target->get_right();
        }
        
    }
    throw "not found";

}

void ArtistBST::print(ofstream& flog) {
    if(empty()) throw "BST is empty";
    flog << "========Print========" << endl;
	flog << "ArtistBST" << endl;
    print(flog,root);
    flog << "======================" << endl;
}

void ArtistBST::print(ofstream& flog,const ArtistBSTNode* node) {
    if(node!=nullptr)
    {
        print(flog,node->get_left());
        for (int i = 0; i < node->get_title().size(); i++)
        {
            int min = node->get_rt()[i] / 60;
            int sec = node->get_rt()[i] % 60;
            string run_time=to_string(min)+':'+(sec < 10 ? "0" : "")+to_string(sec);
            flog<<node->get_artist()<<'/'<<node->get_title()[i]<<'/'<<run_time<<endl;
        }
        print(flog,node->get_right());
    }
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