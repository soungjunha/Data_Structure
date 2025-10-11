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

    bool title_was_ever_found = false;

    while (true) {
        bool found_this_iteration = false;
        string artist_to_delete = "";
        ArtistBSTNode* node_to_modify = nullptr;
        
        // 매번 트리의 처음(root)부터 다시 검색해서 해당 제목의 노래를 '하나만' 찾음
        find_first_artist_by_title(root, title, found_this_iteration, artist_to_delete, node_to_modify);

        if (found_this_iteration) {
            title_was_ever_found = true; // 노래를 한 번이라도 찾았음을 기록

            if (!artist_to_delete.empty()) {
                // 사례 1: 해당 아티스트가 부른 노래가 이것 하나뿐인 경우, 아티스트 노드 전체를 삭제
                delete_node(artist_to_delete);
            }
            else if (node_to_modify != nullptr) {
                // 사례 2: 해당 아티스트가 다른 노래도 부른 경우, 이 노래 제목만 제거
                node_to_modify->remove_title(title);
            }
        } else {
            // 이번 탐색에서 노래를 더 이상 찾지 못했다면, 루프 종료
            break;
        }
    }

    // 루프가 끝날 때까지 노래를 한 번도 찾지 못했다면 예외 발생
    if (!title_was_ever_found) {
        throw "Title not found in the BST.";
    }
}

void ArtistBST::find_first_artist_by_title(ArtistBSTNode* node, const string& title, bool& found, string& artist_to_delete, ArtistBSTNode*& node_to_modify) {
    if (node == nullptr || found) {
        return; // 노드가 없거나, 이미 다른 곳에서 찾았다면 즉시 중단
    }

    // 왼쪽 서브트리 탐색
    find_first_artist_by_title(node->get_left(), title, found, artist_to_delete, node_to_modify);

    if (found) return; // 왼쪽에서 찾았다면 더 이상 진행하지 않고 중단

    // 현재 노드 확인
    if (node->search(title) != -1) {
        found = true; // 찾았음을 표시
        if (node->get_title().size() == 1) { // 이 아티스트의 유일한 곡이라면
            artist_to_delete = node->get_artist();
        } else {
            node_to_modify = node;
        }
    }

    if (found) return; // 현재 노드에서 찾았다면 더 이상 진행하지 않고 중단

    // 오른쪽 서브트리 탐색
    find_first_artist_by_title(node->get_right(), title, found, artist_to_delete, node_to_modify);
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