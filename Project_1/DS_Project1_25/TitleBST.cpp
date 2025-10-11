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

void TitleBST::print(ofstream& flog) {
    if(empty()) throw "BST is empty";
    flog << "========Print========" << endl;
	flog << "TitleBST" << endl;
    print(flog,root);
    flog << "======================" << endl;
}

void TitleBST::print(ofstream& flog,const TitleBSTNode* node) {
    if(node!=nullptr)
    {
        print(flog,node->get_left());
        for (int i = 0; i < node->get_artist().size(); i++)
        {
            int min = node->get_rt()[i] / 60;
            int sec = node->get_rt()[i] % 60;
            string run_time=to_string(min)+':'+(sec < 10 ? "0" : "")+to_string(sec);
            flog<<node->get_artist()[i]<<'/'<<node->get_title()<<'/'<<run_time<<endl;
        }
        print(flog,node->get_right());
    }
}

void TitleBST::delete_node(const string& title) {
    if (root == nullptr) {
        throw "Cannot delete from an empty BST.";
    }
    root = delete_node_recursive(root, title);
}

TitleBSTNode* TitleBST::delete_node_recursive(TitleBSTNode* node, const string& title) {
    if (node == nullptr) {
        throw "Title not found in the BST.456";
    }

    if (title < node->get_title()) {
        node->set_left(delete_node_recursive(node->get_left(), title));
    } else if (title > node->get_title()) {
        node->set_right(delete_node_recursive(node->get_right(), title));
    } else {
        if (node->get_left() == nullptr) {
            TitleBSTNode* temp = node->get_right();
            delete node;
            return temp;
        } else if (node->get_right() == nullptr) {
            TitleBSTNode* temp = node->get_left();
            delete node;
            return temp;
        }
        TitleBSTNode* temp = find_min_node(node->get_right());
        node->copy_data_from(temp);
        node->set_right(delete_node_recursive(node->get_right(), temp->get_title()));
    }
    return node;
}

TitleBSTNode* TitleBST::find_min_node(TitleBSTNode* node) {
    TitleBSTNode* current = node;
    while (current && current->get_left() != nullptr) {
        current = current->get_left();
    }
    return current;
}

void TitleBST::delete_artist(const string& artist) {
    if (root == nullptr) {
        throw "Cannot delete from an empty BST.";
    }

    bool artist_was_ever_found = false;

    while (true) {
        bool found_this_iteration = false;
        string title_to_delete = "";
        TitleBSTNode* node_to_modify = nullptr;

        find_first_song_by_artist(root, artist, found_this_iteration, title_to_delete, node_to_modify);

        if (found_this_iteration) {
            artist_was_ever_found = true;

            if (!title_to_delete.empty()) {
                delete_node(title_to_delete);
            } else if (node_to_modify != nullptr) {
                node_to_modify->remove_artist(artist);
            }
        } else {
            break;
        }
    }

    if (!artist_was_ever_found) {
        throw "Artist not found in the BST.";
    }
}

void TitleBST::find_first_song_by_artist(TitleBSTNode* node, const string& artist, bool& found, string& title_to_delete, TitleBSTNode*& node_to_modify) {
    if (node == nullptr || found) {
        return;
    }

    find_first_song_by_artist(node->get_left(), artist, found, title_to_delete, node_to_modify);

    if (found) return;

    if (node->search(artist) != -1) {
        found = true; // 찾았음을 표시
        if (node->get_artist().size() == 1) {
            title_to_delete = node->get_title();
        } else {
            node_to_modify = node;
        }
    }

    if (found) return;
    
    find_first_song_by_artist(node->get_right(), artist, found, title_to_delete, node_to_modify);
}


TitleBSTNode* TitleBST::find_node(const string& title) {
    TitleBSTNode* current = root;
    while (current != nullptr) {
        if (title == current->get_title()) {
            return current;
        } else if (title < current->get_title()) {
            current = current->get_left();
        } else {
            current = current->get_right();
        }
    }
    return nullptr;
}

void TitleBST::delete_song(const string& artist, const string& title) {
    TitleBSTNode* target_node = find_node(title);

    if (target_node == nullptr) {
        throw "Song title not found.";
    }

    int artist_index = target_node->search(artist);

    if (artist_index == -1) {
        throw "Artist for the specified song not found.";
    }

    if (target_node->get_artist().size() == 1) {
        delete_node(title);
    } else {
        target_node->remove_artist(artist);
    }
}

void TitleBST::clear(TitleBSTNode* node) {
    if (node == nullptr) {
        return;
    }

    clear(node->get_left());
    
    clear(node->get_right());

    delete node;
}