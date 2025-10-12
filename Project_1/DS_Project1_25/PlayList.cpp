#include "PlayList.h"

// Constructor: Initializes an empty playlist.
PlayList::PlayList():head(nullptr),cursor(nullptr),count(0),time(0),data("") {

}

// Destructor: Safely deletes all nodes in the circular linked list.
PlayList::~PlayList() {
    if (head == nullptr) {
        return; // Nothing to delete.
    }

    // Break the circular link to treat it as a standard linear list for deletion.
    PlayListNode* current = head;
    head->get_prev()->set_next(nullptr); 

    // Traverse the now-linear list and delete each node.
    while (current != nullptr) {
        PlayListNode* next_node = current->get_next();
        delete current;
        current = next_node;
    }
    head = nullptr; // Ensure the head is null after deletion.
}

// Inserts a new song into the circular doubly linked list.
void PlayList::insert_node(const string& artist,const string& title,const int& runtime) {

    // Do not insert if the song already exists or the playlist is full.
    if (!exist(artist,title)||full())
    {
        throw "not exist";
    }

    PlayListNode* new_node=new PlayListNode(artist,title,runtime);

    // Case 1: The list is empty.
    if (head==nullptr)
    {
        head=new_node;
        // The single node points to itself.
        head->set_next(head);
        head->set_prev(head);
    }
    // Case 2: The list is not empty.
    else
    {
        // Insert the new node at the end of the list (before the head).
        cursor=head->get_prev(); // Get the last node.
        new_node->set_next(head);
        new_node->set_prev(cursor);
        head->set_prev(new_node);
        cursor->set_next(new_node);
    }
    count++;
    time+=runtime;
}

// Deletes all songs where either the artist or title matches the 'data' string.
void PlayList::delete_node(const string& data) {
    if (head == nullptr) {
        return; // List is empty.
    }

    PlayListNode* node_to_delete = nullptr;
    int initial_count = count;
    cursor = head; // Start traversal from the head.

    // Iterate through the original number of nodes to check each one.
    for (int i = 0; i < initial_count; ++i) {
        // If a match is found...
        if (cursor->get_artist() == data || cursor->get_title() == data) {
            node_to_delete = cursor;
            cursor = cursor->get_next(); // Move cursor before deleting.

            // Case 1: Deleting the only node.
            if (count == 1) {
                head = nullptr;
            } 
            // Case 2: Deleting a node from a multi-node list.
            else {
                // Unlink the node from the list.
                node_to_delete->get_prev()->set_next(node_to_delete->get_next());
                node_to_delete->get_next()->set_prev(node_to_delete->get_prev());

                // If the head node was deleted, update the head pointer.
                if (node_to_delete == head) {
                    head = cursor;
                }
            }
            // Update stats and free memory.
            time -= node_to_delete->get_runtime();
            delete node_to_delete;
            count--;

        } else {
            // No match, move to the next node.
            cursor = cursor->get_next();
        }
    }
}

// Deletes a single, specific song identified by both artist and title.
void PlayList::delete_node(bool only,const string& artist,const string& title) {
    if (head == nullptr) return; // Cannot delete from an empty list.

    cursor=head;
    bool found = false;

    // Traverse the circular list to find the matching node.
    do {
		if (cursor->get_artist() == artist && cursor->get_title() == title) {
			found = true;
			break;
		}
		cursor = cursor->get_next();
    } while (cursor != head);
    
    // If the song was not found in the list.
    if (!found) {
        if (only)
        {
            // The 'only' flag seems to indicate that an error should be thrown if not found.
            throw "notfound";
        }
        else{
            return; // Otherwise, do nothing.
        }
    }

    // Unlink and delete the found node.
    // Case 1: It's the only node in the list.
    if (count == 1) {
		head = nullptr;
    }
    // Case 2: It's one of several nodes.
    else {
        cursor->get_prev()->set_next(cursor->get_next());
        cursor->get_next()->set_prev(cursor->get_prev());

        // If we're deleting the head node, update the head to the next one.
        if (cursor == head) {
            head = cursor->get_next();
        }
    }

    // Update stats and free memory.
    time -= cursor->get_runtime();
    delete cursor;
    count--;
}

// Returns true if the playlist is empty.
bool PlayList::empty() {
    return head==nullptr;
}

// Returns true if the playlist has reached its capacity.
bool PlayList::full() {
    return count>=10;
}

// Checks for the existence of a specific song.
// Returns true if the song does NOT exist (i.e., it can be added).
// Returns false if the song already exists (is a duplicate).
bool PlayList::exist(const string& artist, const string& title) {
    if (head == nullptr) {
        return true; // List is empty, so it can be added.
    }
    cursor=head;
    // Traverse the circular list.
    do{
        if (cursor->get_artist()==artist&&cursor->get_title()==title)
        {
            return false; // Found a duplicate.
        }
        cursor=cursor->get_next();
    }while (cursor!=head);
    
    return true; // No duplicate found.
}

// Prints all songs in the playlist to the log file.
void PlayList::print(ofstream& flog) {

    if (head == nullptr) return;

    cursor=head;
    // Traverse the circular list and print each song's details.
    do{
        // Format the runtime from seconds to MM:SS.
        int min = cursor->get_runtime() / 60;
        int sec = cursor->get_runtime() % 60;
        string runtime=to_string(min)+':'+(sec < 10 ? "0" : "")+to_string(sec);
        flog<<cursor->get_artist()<<'/'<<cursor->get_title()<<'/'<<runtime<<endl;
        cursor=cursor->get_next();
    }while (cursor!=head);
    
    // Print summary statistics.
    flog<<"Count :"<<count<<"/10"<<endl;
    flog<<"Time :"<<run_time()<<endl;
}

// Helper function to format the total playlist time.
string PlayList::run_time() {
    int min = time / 60;
    int sec = time % 60;

    return to_string(min)+"min "+(sec < 10 ? "0" : "")+to_string(sec)+"sec";
}