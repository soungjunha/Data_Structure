#include "ArtistBST.h"

// Constructor: Initializes the BST with null pointers.
ArtistBST::ArtistBST():root(nullptr),parent(nullptr),data(""),target(nullptr) {

}

// Destructor: Frees all memory allocated for the nodes.
ArtistBST::~ArtistBST() {
    clear(this->root);
}

// Inserts a new music node into the BST.
// It traverses the tree to find the correct position based on the artist's name.
// If the artist already exists, it adds the new song to that artist's node.
// If not, it creates a new node for the artist.
void ArtistBST::insert(const MusicQueueNode* node) {
    // If the tree is empty, create a new root.
    if (root==nullptr)
    {
        root=new ArtistBSTNode(node);
    }
    else
    {
        // Start traversal from the root.
        target=root;
        while (true) {
            try
            {
                // Case 1: The artist already exists in the tree.
                if (node->get_artist() == target->get_artist()) {
                    target->set(node->get_title(), node->get_run_time());
                    return; // Insertion complete.
                }
                // Case 2: The new artist's name is lexicographically smaller.
                else if (node->get_artist() < target->get_artist()) {
                    if (target->get_left() == nullptr) {
                        // Found the correct empty spot. Insert new node.
                        target->set_left(new ArtistBSTNode(node));
                        return;
                    }
                    // Continue traversing to the left.
                    target = target->get_left();
                }
                // Case 3: The new artist's name is lexicographically larger.
                else {
                    if (target->get_right() == nullptr) {
                        // Found the correct empty spot. Insert new node.
                        target->set_right(new ArtistBSTNode(node));
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

// Searches for an artist and retrieves their songs.
// @param artist The name of the artist to find.
// @return A vector of pairs, each containing a song title and its formatted run time.
// @throws A const char* exception if the artist is not found.
vector<pair<string,string>> ArtistBST::search(const string& artist) {
    // Start traversal from the root.
    target=root;

    while (target!=nullptr)
    {
        // Case 1: Artist found.
        if (artist == target->get_artist()) {
            vector<pair<string, string>> result;
            auto const& titles = target->get_title();
            auto const& rts = target->get_rt(); // Run times in seconds.
            // Iterate through all songs of the artist.
            for (int i = 0; i < target->get_title().size(); i++)
            {
                // Convert seconds back to MM:SS format.
                int min = rts[i] / 60;
                int sec = rts[i] % 60;
                string tmp=to_string(min)+':'+(sec < 10 ? "0" : "")+to_string(sec);
                result.push_back(pair<string, string>(titles[i],tmp));
            }
            return result;
        }
        // Traverse left if the target artist is smaller.
        else if (artist < target->get_artist()) {
            target = target->get_left();
        }
        // Traverse right if the target artist is larger.
        else {
            target = target->get_right();
        }
    }
    // If the loop finishes, the artist was not found.
    throw "not found";
}

// Public interface to print the tree's contents to a file.
// Performs an in-order traversal.
void ArtistBST::print(ofstream& flog) {
    if(empty()) throw "BST is empty";
    flog << "========Print========" << endl;
	flog << "ArtistBST" << endl;
    print(flog,root); // Call the recursive helper function.
    flog << "======================" << endl;
}

// Recursive helper function to print the tree using in-order traversal.
void ArtistBST::print(ofstream& flog,const ArtistBSTNode* node) {
    if(node!=nullptr)
    {
        // 1. Traverse left subtree.
        print(flog,node->get_left());
        
        // 2. Process (print) the current node's data.
        for (int i = 0; i < node->get_title().size(); i++)
        {
            // Format run time from seconds to MM:SS.
            int min = node->get_rt()[i] / 60;
            int sec = node->get_rt()[i] % 60;
            string run_time=to_string(min)+':'+(sec < 10 ? "0" : "")+to_string(sec);
            flog<<node->get_artist()<<'/'<<node->get_title()[i]<<'/'<<run_time<<endl;
        }
        
        // 3. Traverse right subtree.
        print(flog,node->get_right());
    }
}

// Deletes a node (an artist) from the BST.
void ArtistBST::delete_node(const string& artist) {
    if (root == nullptr) {
        throw "Cannot delete from an empty BST.";
    }
    // Call the recursive helper to find and delete the node.
    root = delete_node_recursive(root, artist);
}

// Recursive helper to perform the deletion of a node.
ArtistBSTNode* ArtistBST::delete_node_recursive(ArtistBSTNode* node, const string& artist) {
    if (node == nullptr) {
        throw "Artist not found in the BST.";
    }

    // Traverse to find the node.
    if (artist < node->get_artist()) {
        node->set_left(delete_node_recursive(node->get_left(), artist));
    } else if (artist > node->get_artist()) {
        node->set_right(delete_node_recursive(node->get_right(), artist));
    } else {
        // Node found. Handle the three deletion cases.
        // Case 1: Node has no left child or one child.
        if (node->get_left() == nullptr) {
            ArtistBSTNode* temp = node->get_right();
            delete node;
            return temp;
        } else if (node->get_right() == nullptr) {
            ArtistBSTNode* temp = node->get_left();
            delete node;
            return temp;
        }
        // Case 2: Node has two children.
        // Find the in-order successor (smallest node in the right subtree).
        ArtistBSTNode* temp = find_min_node(node->get_right());
        // Copy the successor's data to this node.
        node->copy_data_from(temp);
        // Delete the in-order successor.
        node->set_right(delete_node_recursive(node->get_right(), temp->get_artist()));
    }
    return node;
}

// Finds the node with the minimum value in a subtree (the leftmost node).
ArtistBSTNode* ArtistBST::find_min_node(ArtistBSTNode* node) {
    ArtistBSTNode* current = node;
    while (current && current->get_left() != nullptr) {
        current = current->get_left();
    }
    return current;
}

// Deletes all occurrences of a song by its title across the entire BST.
void ArtistBST::delete_title(const string& title) {
    if (root == nullptr) {
        throw "Cannot delete from an empty BST.";
    }

    bool title_was_ever_found = false;

    // Loop to find and delete all instances of the title, since the helper only finds one at a time.
    while (true) {
        bool found_this_iteration = false;
        string artist_to_delete = "";
        ArtistBSTNode* node_to_modify = nullptr;
        
        // Search for the first occurrence of the title.
        find_first_artist_by_title(root, title, found_this_iteration, artist_to_delete, node_to_modify);

        if (found_this_iteration) {
            title_was_ever_found = true;
            // If the artist had only this song, delete the entire artist node.
            if (!artist_to_delete.empty()) {
                delete_node(artist_to_delete);
            }
            // If the artist had other songs, just remove the specific title.
            else if (node_to_modify != nullptr) {
                node_to_modify->remove_title(title);
            }
        } else {
            // No more occurrences found, exit the loop.
            break;
        }
    }

    if (!title_was_ever_found) {
        throw "Title not found in the BST.";
    }
}

// Recursive helper (in-order traversal) to find the first node containing a given title.
void ArtistBST::find_first_artist_by_title(ArtistBSTNode* node, const string& title, bool& found, string& artist_to_delete, ArtistBSTNode*& node_to_modify) {
    // Base case: current node is null or an instance has already been found.
    if (node == nullptr || found) {
        return;
    }

    // 1. Traverse left.
    find_first_artist_by_title(node->get_left(), title, found, artist_to_delete, node_to_modify);

    if (found) return; // Stop if found in the left subtree.
    
    // 2. Process current node.
    if (node->search(title) != -1) {
        found = true;
        // If this is the artist's only song, mark the artist for deletion.
        if (node->get_title().size() == 1) {
            artist_to_delete = node->get_artist();
        } else {
            // Otherwise, mark the node for title removal.
            node_to_modify = node;
        }
    }

    if (found) return; // Stop if found in the current node.

    // 3. Traverse right.
    find_first_artist_by_title(node->get_right(), title, found, artist_to_delete, node_to_modify);
}

// Finds a node by artist name.
ArtistBSTNode* ArtistBST::find_node(const string& artist) {
    ArtistBSTNode* current = root;
    while (current != nullptr) {
        if (artist == current->get_artist()) {
            return current; // Found
        } else if (artist < current->get_artist()) {
            current = current->get_left(); // Go left
        } else {
            current = current->get_right(); // Go right
        }
    }
    return nullptr; // Not found
}

// Deletes a specific song from a specific artist.
void ArtistBST::delete_song(const string& artist, const string& title) {
    // Find the node corresponding to the artist.
    ArtistBSTNode* target_node = find_node(artist);

    if (target_node == nullptr) {
        throw "Artist for the specified song not found.";
    }

    // Search for the title within the artist's song list.
    int title_index = target_node->search(title);

    if (title_index == -1) {
        throw "Title for the specified song not found.";
    }

    // If this is the artist's only song, delete the entire artist node.
    if (target_node->get_title().size() == 1) {
        delete_node(artist);
    } 
    // Otherwise, just remove the song from the artist's list.
    else {
        target_node->remove_title(title);
    }
}

// Recursively deletes all nodes in the tree (post-order traversal).
void ArtistBST::clear(ArtistBSTNode* node) {
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