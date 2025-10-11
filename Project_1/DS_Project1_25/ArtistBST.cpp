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
    if (root == nullptr) {
        throw "Cannot delete from an empty BST.";
    }
    root = delete_node_recursive(root, artist);
}

ArtistBSTNode* ArtistBST::delete_node_recursive(ArtistBSTNode* node, const string& artist) {
    if (node == nullptr) {
        throw "Artist not found in the BST.";
    }

    if (artist < node->get_artist()) {
        node->set_left(delete_node_recursive(node->get_left(), artist));
    } else if (artist > node->get_artist()) {
        node->set_right(delete_node_recursive(node->get_right(), artist));
    } else {
        if (node->get_left() == nullptr) {
            ArtistBSTNode* temp = node->get_right();
            delete node;
            return temp;
        } else if (node->get_right() == nullptr) {
            ArtistBSTNode* temp = node->get_left();
            delete node;
            return temp;
        }
        ArtistBSTNode* temp = find_min_node(node->get_right());
        node->copy_data_from(temp);
        node->set_right(delete_node_recursive(node->get_right(), temp->get_artist()));
    }
    return node;
}

ArtistBSTNode* ArtistBST::find_min_node(ArtistBSTNode* node) {
    ArtistBSTNode* current = node;
    while (current && current->get_left() != nullptr) {
        current = current->get_left();
    }
    return current;
}

void ArtistBST::delete_title(const string& title) {
    if (root == nullptr) {
        throw "Cannot delete from an empty BST.";
    }

    vector<string> artists_to_delete;
    vector<ArtistBSTNode*> nodes_to_modify;
    
    collect_title_songs(root, title, artists_to_delete, nodes_to_modify);

    if (artists_to_delete.empty() && nodes_to_modify.empty()) {
        throw "Title not found in the BST.";
    }

    for (ArtistBSTNode* node : nodes_to_modify) {
        node->remove_title(title);
    }
    for (const string& artist : artists_to_delete) {
        delete_node(artist);
    }
}

void ArtistBST::collect_title_songs(ArtistBSTNode* node, const string& title,vector<string>& artists_to_delete, vector<ArtistBSTNode*>& nodes_to_modify) {
    if (node == nullptr) return;

    collect_title_songs(node->get_left(), title, artists_to_delete, nodes_to_modify);

    if (node->search(title) != -1) {
        if (node->get_artist().size() == 1) {
            artists_to_delete.push_back(node->get_artist());
        } else {
            nodes_to_modify.push_back(node);
        }
    }
    
    collect_title_songs(node->get_right(), title, artists_to_delete, nodes_to_modify);
}

ArtistBSTNode* ArtistBST::find_node(const string& artist) {
    ArtistBSTNode* current = root;
    while (current != nullptr) {
        if (artist == current->get_artist()) {
            return current;
        } else if (artist < current->get_artist()) {
            current = current->get_left();
        } else {
            current = current->get_right();
        }
    }
    return nullptr;
}

void ArtistBST::delete_song(const string& artist, const string& title) {
    ArtistBSTNode* target_node = find_node(artist);

    if (target_node == nullptr) {
        throw "Song title not found.";
    }

    int title_index = target_node->search(title);

    if (title_index == -1) {
        throw "title for the specified song not found.";
    }

    if (target_node->get_artist().size() == 1) {
        delete_node(artist);
    } else {
        target_node->remove_title(title);
    }
}

void ArtistBST::clear(ArtistBSTNode* node) {
    if (node == nullptr) {
        return;
    }

    clear(node->get_left());
    
    clear(node->get_right());

    delete node;
}