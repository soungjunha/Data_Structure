#include "TitleBST.h"

// Constructor: Initializes the BST with null pointers.
TitleBST::TitleBST():root(nullptr),parent(nullptr),data(""),target(nullptr) {

}

// Destructor: Frees all memory allocated for the nodes.
TitleBST::~TitleBST() {
    clear(this->root);
}

// Inserts a new music node into the BST, ordered by title.
// It traverses the tree to find the correct position.
// If the title already exists, it adds the artist to that node.
// If not, it creates a new node for the title.
void TitleBST::insert(const MusicQueueNode* node) {
    // If the tree is empty, create a new root.
    if (root==nullptr)
    {
        root=new TitleBSTNode(node);
    }
    else
    {
        // Start traversal from the root.
        target=root;
        while (true) {
            try
            {
                // Case 1: The title already exists in the tree.
                if (node->get_title() == target->get_title()) {
                    target->set(node->get_artist(), node->get_run_time());
                    return; // Insertion complete.
                }
                // Case 2: The new title is lexicographically smaller.
                else if (node->get_title() < target->get_title()) {
                    if (target->get_left() == nullptr) {
                        // Found the correct empty spot. Insert new node.
                        target->set_left(new TitleBSTNode(node));
                        return;
                    }
                    // Continue traversing to the left.
                    target = target->get_left();
                }
                // Case 3: The new title is lexicographically larger.
                else {
                    if (target->get_right() == nullptr) {
                        // Found the correct empty spot. Insert new node.
                        target->set_right(new TitleBSTNode(node));
                        return;
                    }
                    // Continue traversing to the right.
                    target = target->get_right();
                }
            }
            catch(const char* errorMessage)
            {
                // Catch the exception for duplicate music and re-throw it.
                throw "same music";
            }
        }
    }
}

// Searches for a title and retrieves a list of artists who performed it.
// @param title The name of the title to find.
// @return A vector of pairs, each containing an artist name and the formatted run time.
// @throws A const char* exception if the title is not found.
vector<pair<string,string>> TitleBST::search(const string& title) {
    // Start traversal from the root.
    target=root;

    while (target!=nullptr)
    {
        // Case 1: Title found.
        if (title == target->get_title()) {
            vector<pair<string, string>> result;
            auto const& artists = target->get_artist();
            auto const& rts = target->get_rt(); // Run times in seconds.
            // Iterate through all artists for this title.
            for (int i = 0; i < target->get_artist().size(); i++)
            {
                // Convert seconds back to MM:SS format.
                int min = rts[i] / 60;
                int sec = rts[i] % 60;
                string tmp=to_string(min)+':'+(sec < 10 ? "0" : "")+to_string(sec);
                result.push_back(pair<string, string>(artists[i],tmp));
            }
            return result;
        }
        // Traverse left if the target title is smaller.
        else if (title < target->get_title()) {
            target = target->get_left();
        }
        // Traverse right if the target title is larger.
        else {
            target = target->get_right();
        }
    }
    // If the loop finishes, the title was not found.
    throw "not found";
}

// Public interface to print the tree's contents to a file.
// Performs an in-order traversal, sorting by title.
void TitleBST::print(ofstream& flog) {
    if(empty()) throw "BST is empty";
    flog << "========Print========" << endl;
	flog << "TitleBST" << endl;
    print(flog,root); // Call the recursive helper function.
    flog << "======================" << endl;
}

// Recursive helper function to print the tree using in-order traversal.
void TitleBST::print(ofstream& flog,const TitleBSTNode* node) {
    if(node!=nullptr)
    {
        // 1. Traverse left subtree.
        print(flog,node->get_left());
        
        // 2. Process (print) the current node's data.
        for (int i = 0; i < node->get_artist().size(); i++)
        {
            // Format run time from seconds to MM:SS.
            int min = node->get_rt()[i] / 60;
            int sec = node->get_rt()[i] % 60;
            string run_time=to_string(min)+':'+(sec < 10 ? "0" : "")+to_string(sec);
            flog<<node->get_artist()[i]<<'/'<<node->get_title()<<'/'<<run_time<<endl;
        }
        
        // 3. Traverse right subtree.
        print(flog,node->get_right());
    }
}

// Deletes a node (a title) from the BST.
void TitleBST::delete_node(const string& title) {
    if (root == nullptr) {
        throw "Cannot delete from an empty BST.";
    }
    // Call the recursive helper to find and delete the node.
    root = delete_node_recursive(root, title);
}

// Recursive helper to perform the deletion of a node.
TitleBSTNode* TitleBST::delete_node_recursive(TitleBSTNode* node, const string& title) {
    if (node == nullptr) {
        throw "Title not found in the BST.";
    }

    // Traverse to find the node.
    if (title < node->get_title()) {
        node->set_left(delete_node_recursive(node->get_left(), title));
    } else if (title > node->get_title()) {
        node->set_right(delete_node_recursive(node->get_right(), title));
    } else {
        // Node found. Handle the three deletion cases.
        // Case 1: Node has no left child or one child.
        if (node->get_left() == nullptr) {
            TitleBSTNode* temp = node->get_right();
            delete node;
            return temp;
        } else if (node->get_right() == nullptr) {
            TitleBSTNode* temp = node->get_left();
            delete node;
            return temp;
        }
        // Case 2: Node has two children.
        // Find the in-order successor (smallest node in the right subtree).
        TitleBSTNode* temp = find_min_node(node->get_right());
        // Copy the successor's data to this node.
        node->copy_data_from(temp);
        // Delete the in-order successor.
        node->set_right(delete_node_recursive(node->get_right(), temp->get_title()));
    }
    return node;
}

// Finds the node with the minimum value in a subtree (the leftmost node).
TitleBSTNode* TitleBST::find_min_node(TitleBSTNode* node) {
    TitleBSTNode* current = node;
    while (current && current->get_left() != nullptr) {
        current = current->get_left();
    }
    return current;
}

// Deletes all songs by a specific artist across the entire BST.
void TitleBST::delete_artist(const string& artist) {
    if (root == nullptr) {
        throw "Cannot delete from an empty BST.";
    }

    bool artist_was_ever_found = false;

    // Loop to find and delete all songs by the artist.
    while (true) {
        bool found_this_iteration = false;
        string title_to_delete = "";
        TitleBSTNode* node_to_modify = nullptr;

        // Search for the first song by the artist.
        find_first_song_by_artist(root, artist, found_this_iteration, title_to_delete, node_to_modify);

        if (found_this_iteration) {
            artist_was_ever_found = true;
            // If the artist was the only one for a given title, delete the entire title node.
            if (!title_to_delete.empty()) {
                delete_node(title_to_delete);
            } 
            // If other artists also have a song with this title, just remove this artist.
            else if (node_to_modify != nullptr) {
                node_to_modify->remove_artist(artist);
            }
        } else {
            // No more songs by this artist found, exit the loop.
            break;
        }
    }

    if (!artist_was_ever_found) {
        throw "Artist not found in the BST.";
    }
}

// Recursive helper (in-order traversal) to find the first node containing a given artist.
void TitleBST::find_first_song_by_artist(TitleBSTNode* node, const string& artist, bool& found, string& title_to_delete, TitleBSTNode*& node_to_modify) {
    // Base case: current node is null or an instance has already been found.
    if (node == nullptr || found) {
        return;
    }

    // 1. Traverse left.
    find_first_song_by_artist(node->get_left(), artist, found, title_to_delete, node_to_modify);

    if (found) return; // Stop if found in the left subtree.

    // 2. Process current node.
    if (node->search(artist) != -1) {
        found = true;
        // If this artist is the only one for this title, mark the title for deletion.
        if (node->get_artist().size() == 1) {
            title_to_delete = node->get_title();
        } else {
            // Otherwise, mark the node for artist removal.
            node_to_modify = node;
        }
    }

    if (found) return; // Stop if found in the current node.
    
    // 3. Traverse right.
    find_first_song_by_artist(node->get_right(), artist, found, title_to_delete, node_to_modify);
}

// Finds a node by song title.
TitleBSTNode* TitleBST::find_node(const string& title) {
    TitleBSTNode* current = root;
    while (current != nullptr) {
        if (title == current->get_title()) {
            return current; // Found
        } else if (title < current->get_title()) {
            current = current->get_left(); // Go left
        } else {
            current = current->get_right(); // Go right
        }
    }
    return nullptr; // Not found
}

// Deletes a specific song (one artist's version of a title).
void TitleBST::delete_song(const string& artist, const string& title) {
    // Find the node corresponding to the title.
    TitleBSTNode* target_node = find_node(title);

    if (target_node == nullptr) {
        throw "Song title not found.";
    }

    // Search for the artist within the title's artist list.
    int artist_index = target_node->search(artist);

    if (artist_index == -1) {
        throw "Artist for the specified song not found.";
    }

    // If this is the only artist for the title, delete the entire title node.
    if (target_node->get_artist().size() == 1) {
        delete_node(title);
    } 
    // Otherwise, just remove the artist from the title's list.
    else {
        target_node->remove_artist(artist);
    }
}

// Recursively deletes all nodes in the tree (post-order traversal).
void TitleBST::clear(TitleBSTNode* node) {
    if (node == nullptr) {
        return;
    }

    // 1. Clear left subtree.
    clear(node->get_left());
    
    // 2. Clear right subtree.
    clear(node->get_right());

    // 3. Delete the current node.
    delete node;
}