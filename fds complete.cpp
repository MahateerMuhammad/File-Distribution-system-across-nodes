#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <graphics.h>
#include <conio.h> 
#include <chrono>
#include <iomanip>
#include <ctime>

using namespace std;
using namespace chrono;

class Node 
{
	private:
	    int Id;
	    unordered_map<string, string> files; 
	    Node* next;
	
	public:
	    Node(int Id) 
		{
	        this->Id = Id;
	        next = nullptr;
	    }
	
	    int getId() 
		{ 
			return Id; 
		}
	
	    void setId(int Id) 
		{ 
			this->Id = Id; 
		}
	
	    void setNext(Node* a) 
		{ 	
			next = a; 
		}
	
	    Node* getNext() 
		{ 	
			return next; 
		}
	
	    unordered_map<string, string>& getFiles() 
		{ 
			return files; 
		}
		
};


class Distribution_in_Nodes 
{
	private:
	    Node* head;
	    int Nodescount;
	
	public:
	    Distribution_in_Nodes() 
		{
	        head = nullptr;
	        Nodescount = 0;
	    }
	
	    int hashFunction(string& fileName) 
		{
	        unsigned long hashValue = 0;
	        int prime = 31;
	
	        for (char c : fileName) 
			{
	            hashValue = (hashValue * prime + c) % 100000;
	        }
	
	        if (Nodescount == 0) 
			{
	            return 0;
	        }
	        return hashValue % Nodescount;
	    }
	
	    Node* getNode(int index) 
		{
	        Node* temp = head;
	        for (int i = 0; i < index && temp; ++i) 
			{
	            temp = temp->getNext();
	        }
	        return temp;
	    }
	
	    void addNode() 
		{
	        Node* newNode = new Node(Nodescount);
	        if (!head) 
			{
	            head = newNode;
	        } 
			else 
			{
	            Node* current = head;
	            while (current->getNext()) 
				{
	                current = current->getNext();
	            }
	            current->setNext(newNode);
	        }
	
	        Nodescount++;
	
	       
	        visualizeNodes();
	    }
	
		string getCurrentTimestamp() 
		{
		    auto now = system_clock::now();
		    time_t now_c = system_clock::to_time_t(now);
		    tm* now_tm = localtime(&now_c);
		
		    stringstream ss;
		    ss << put_time(now_tm, "%Y-%m-%d %H:%M:%S");
		    return ss.str();
		}
	
	  
	    void searchFile(string& fileName) 
		{
		    Node* current = head;
		    bool found = false;
		
		    while (current) 
			{
		        if (current->getFiles().find(fileName) != current->getFiles().end()) 
				{
		            cout << "File '" << fileName << "' found in Node " << current->getId() << " with content:\n";
		            cout << current->getFiles()[fileName] << endl;
		            found = true;
		            break;
		        }
		        current = current->getNext();
		    }
		
		    if (!found) {
		        cout << "File '" << fileName << "' not found in any node!" << endl;
		    }
		}
	
		void storeFile(string& fileName, string& content) 
		{
		    
		    string timestamp = getCurrentTimestamp();
		
		   
		    Node* current = head;
		    while (current) 
			{
		        if (current->getFiles().find(fileName) != current->getFiles().end()) 
				{
		            cout << "[" << timestamp << "] File with name '" << fileName << "' already exists in Node " << current->getId() << "!" << endl;
		            return;
		        }
		        current = current->getNext();
		    }
		
		    int nodeIndex = hashFunction(fileName);
		    Node* primaryNode = getNode(nodeIndex);
		
		    if (primaryNode) 
			{
		        primaryNode->getFiles()[fileName] = content;
		        cout << "[" << timestamp << "] File '" << fileName << "' stored in Node " << primaryNode->getId() << endl;
		    } 
			else 
			{
		        cout << "[" << timestamp << "] Node not found for storing file '" << fileName << "'!" << endl;
		    }
		}
	
		void retrieveFile(string& fileName) 
		{
		    string timestamp = getCurrentTimestamp();
		
		    Node* current = head;
		    bool found = false;
		
		    while (current) 
			{
		        if (current->getFiles().find(fileName) != current->getFiles().end()) 
				{
		            cout << "[" << timestamp << "] File '" << fileName << "' found in Node " << current->getId() << " with content:\n";
		            cout << current->getFiles()[fileName] << endl;
		            found = true;
		            break;
		        }
		        current = current->getNext();
		    }
		
		    if (!found) 
			{
		        cout << "[" << timestamp << "] File '" << fileName << "' not found in any node!" << endl;
		    }
		}
	
		void deleteFile(string& fileName) 
		{
		    string timestamp = getCurrentTimestamp();
		
		    Node* current = head;
		    bool fileDeleted = false;
		
		    while (current) 
			{
		        if (current->getFiles().find(fileName) != current->getFiles().end()) 
				{
					string file = fileName + ".txt";
					if(remove(file.c_str()) == 0)
					{
						cout<<"File "<<file<<" deleted successfully "<<endl;
					}
					else
					{
						cout<<"Error deleting "<<file <<" Permission denied or not found "<<endl;
						
					}
		            current->getFiles().erase(fileName);
		            cout << "[" << timestamp << "] File '" << fileName << "' deleted from Node " << current->getId() << endl;
		            fileDeleted = true;
		        }
		        current = current->getNext();
		    }
		
		    if (!fileDeleted) 
			{
		        cout << "[" << timestamp << "] File '" << fileName << "' not found for deletion!" << endl;
		    }
		}
	
		void modifyFile(string& fileName, string& newContent) 
		{
		    string timestamp = getCurrentTimestamp();
		
		    int nodeIndex = hashFunction(fileName);
		    Node* primaryNode = getNode(nodeIndex);
		
		    if (primaryNode && primaryNode->getFiles().find(fileName) != primaryNode->getFiles().end()) 
			{
		        primaryNode->getFiles()[fileName] = newContent;
		        cout << "[" << timestamp << "] File '" << fileName << "' modified in Node " << primaryNode->getId() << endl;
		    } 
			else 
			{
		        cout << "[" << timestamp << "] File '" << fileName << "' not found for modification!" << endl;
		    }
		}
	
	
	
	   void backupSystem(string backupFileName) 
		{
		    ofstream backupFile(backupFileName);
		
		    if (!backupFile.is_open()) 
		    {
		        cout << "Failed to create backup file!" << endl;
		        return;
		    }
		
		    Node* current = head;
		    while (current) 
		    {
		        for (const auto& file : current->getFiles()) 
		        {
		            backupFile << file.first << "\n" << file.second << "\n\n";
		        }
		        current = current->getNext();
		    }
		
		    backupFile.close();
		    cout << "System successfully backed up to " << backupFileName << endl;
		}


	
	
		void deleteNode(int nodeId) 
		{
		    if (!head) 
		    {
		        cout << "No nodes available to delete!" << endl;
		        return;
		    }
		
		    Node* current = head;
		    Node* prev = nullptr;
		
		   
		    while (current && current->getId() != nodeId) 
		    {
		        prev = current;
		        current = current->getNext();
		    }
		
		    if (!current) 
		    {
		        cout << "Node with ID " << nodeId << " not found!" << endl;
		        return;
		    }
		
		   
		    int choice;
		    cout << "Do you want to:\n";
		    cout << "1. Delete the node and all its files.\n";
		    cout << "2. Redistribute the files to other nodes.\n";
		    cout << "Enter your choice: ";
		    cin >> choice;
		
		    if (choice == 2) 
		    {
		        if (Nodescount <= 1) 
		        {
		            cout << "Cannot redistribute files as there are no other nodes!" << endl;
		            return;
		        }
		
		       
		        for (pair<string,string> file : current->getFiles()) 
		        {
		            string fileName = file.first;
		            string content = file.second;
		
		            
		            int newNodeIndex = hashFunction(fileName);
		            if (newNodeIndex == nodeId) 
		            {
		                newNodeIndex = (newNodeIndex + 1) % Nodescount; 
		            }
		
		            Node* newNode = getNode(newNodeIndex);
		            if (newNode) 
		            {
		                newNode->getFiles()[fileName] = content;
		                cout << "File '" << fileName << "' moved to Node " << newNode->getId() << endl;
		            }
		        }
		    } 
		    else 
		    {
		       
		        for (const pair<const string,const string> file : current->getFiles()) 
		        {
		            string fileName = file.first+".txt";
		
		           
		            if (remove(fileName.c_str()) == 0) 
		            {
		                cout << "File '" << fileName << "' deleted successfully." << endl;
		            } 
		            else 
		            {
		                cout<<"Error deleting file"<<fileName<<endl;
		            }
		        }
		
		        current->getFiles().clear(); 
		        cout << "All files deleted successfully from Node " << nodeId << "." << endl;
		    }
		
		   
		    if (current == head) 
		    {
		        head = current->getNext();
		    } 
		    else 
		    {
		        prev->setNext(current->getNext());
		    }
		
		    delete current; 
		    Nodescount--;
		
		   
		    int newId = 0;
		    current = head;
		    while (current) 
		    {
		        current->setId(newId++);
		        current = current->getNext();
		    }
		
		    cout << "Node " << nodeId << " deleted successfully." << endl;
		
		   
		   
		}


	    
		void restoreSystem(string backupFileName) 
		{
		    ifstream backupFile(backupFileName);
		
		    if (!backupFile.is_open()) 
		    {
		        cout << "Failed to open backup file!" << endl;
		        return;
		    }
		
		    string fileName, fileContent;
		
		    while (getline(backupFile, fileName)) 
		    {
		        if (!fileName.empty()) 
		        {
		            if (getline(backupFile, fileContent)) 
		            {
		                int nodeIndex = hashFunction(fileName);
		                Node* primaryNode = getNode(nodeIndex);
		
		                if (primaryNode) 
		                {
		                    if (primaryNode->getFiles().find(fileName) == primaryNode->getFiles().end()) 
		                    {
		                        primaryNode->getFiles()[fileName] = fileContent;
		                        cout << "File '" << fileName << "' restored to Node " << primaryNode->getId() << endl;
		                    } 
		                    else 
		                    {
		                        cout << "File '" << fileName << "' already exists in Node " << primaryNode->getId() << "!" << endl;
		                    }
		                } 
		                else 
		                {
		                    cout << "Node not found for restoring file '" << fileName << "'!" << endl;
		                }
		            } 
		            else 
		            {
		                cout << "Invalid backup file format!" << endl;
		                break;
		            }
		        }
		    }
		
		    backupFile.close();
		    cout << "System successfully restored from " << backupFileName << endl;
		}


	
	    void displayFiles() 
		{
	        Node* current = head;
	        while (current) 
			{
	            cout << "Node " << current->getId() << " files:" << endl;
	            for (pair<string,string> file : current->getFiles()) 
				{
	                cout << "  " << file.first << ": " << file.second << endl;
	            }
	            current = current->getNext();
	    	}
		}
	
	    void visualizeNodes() 
		{
		    int gd = DETECT, gm;
		    initgraph(&gd, &gm, (char*)"");
		
		    Node* current = head;
		    int x = 50, y = 50;  
		    int nodeWidth = 150, nodeHeight = 70;  
		    int fileBoxHeight = 20;  
		    int spacing = 200;  
		
		    int totalWidth = x + (Nodescount * (nodeWidth + spacing)) + 150;
		    int maxHeight = y + nodeHeight + (fileBoxHeight + 5) * 5 + 50;
		
		    if (totalWidth > getmaxx() || maxHeight > getmaxy()) {
		        int width = max(totalWidth, getmaxx());
		        int height = max(maxHeight, getmaxy());
		        initwindow(width, height, "Node Visualization");
		    }
		
		    cleardevice();
		
		    while (current) 
		    {
		        rectangle(x, y, x + nodeWidth, y + nodeHeight);
		
		        int divideX = x + (nodeWidth * 3) / 4; 
		        line(divideX, y, divideX, y + nodeHeight);
		
		        string nodeLabel = "Node " + to_string(current->getId());
		        outtextxy(x + 10, y + nodeHeight / 2 - 10, (char*)nodeLabel.c_str());
		
		        
		        int fileX = x + 10, fileY = y + nodeHeight + 10;
		        for (const auto& file : current->getFiles()) 
		        {
		            outtextxy(fileX, fileY, (char*)file.first.c_str());
		            fileY += fileBoxHeight;
		        }
		
		        if (current->getNext()) 
		        {
		            int arrowStartX = x + nodeWidth;
		            int arrowEndX = x + nodeWidth + spacing - 10;
		            int arrowY = y + nodeHeight / 2;
		            line(arrowStartX, arrowY, arrowEndX, arrowY);
		            line(arrowEndX - 10, arrowY - 5, arrowEndX, arrowY); 
		            line(arrowEndX - 10, arrowY + 5, arrowEndX, arrowY);
		        } 
		        else 
		        {
		        	
		            outtextxy(x + nodeWidth + 20, y + nodeHeight / 2 - 10, "NULL");
		        }
		
		        x += nodeWidth + spacing;
		        current = current->getNext();
		    }
		
		    getch();  
		    closegraph();  
		}

	
	    ~Distribution_in_Nodes() 
		{
	        Node* current = head;
	        while (current) 
			{
	            Node* nextNode = current->getNext();
	            delete current;
	            current = nextNode;
	        }
	    }
};

int main() 
{
    Distribution_in_Nodes fileSystem; 
    int choice;
    string fileName, content;
    fstream file;
    string line;

    fileSystem.addNode();
    fileSystem.addNode();
    fileSystem.addNode();

    do 
	{
        cout << "\n\t\t========================================" << endl;
        cout << "\t\t|        File Distribution System      |" << endl;
        cout << "\t\t========================================" << endl;
        cout << "\t\t| 1. Store File                        |" << endl;
        cout << "\t\t| 2. Retrieve File                     |" << endl;
        cout << "\t\t| 3. Delete File                       |" << endl;
        cout << "\t\t| 4. Display All Files                 |" << endl;
        cout << "\t\t| 5. Exit                              |" << endl;
        cout << "\t\t| 6. Delete Node                       |" << endl;
        cout << "\t\t| 7. Visualize Nodes                   |" << endl;
        cout << "\t\t| 8. Modify File                       |" << endl;
        cout << "\t\t| 9. search  File                      |" << endl;
        cout << "\t\t| 10. Backup System                    |" << endl;
        cout << "\t\t| 11. Restore System                   |" << endl;

        cout << "\t\t========================================" << endl;
        cout << "\t\t| Enter your choice: ";
        cin >> choice;
        while(cin.fail())
        {
        	cout<<"\n\t\t Dont enter string or invalid charcaters."<<endl;
        	cin.clear();
        	cin.ignore(numeric_limits<streamsize>::max(), '\n');
        	cout<<"\t\tEnter your choice again ";
        	cin>>choice;
		}

        switch (choice) 
		{
            case 1:
                system("cls");
                cout << "Enter file name (without extension): ";
                cin >> fileName;

                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                file.open(fileName + ".txt", ios::out);
                if (!file.is_open()) 
				{
                    cout << "Failed to create file '" << fileName << "'!" << endl;
                    break;
                }

                cout << "Enter content for the file (type 'END' to finish):\n";
                content.clear();
                while (true) 
				{
                    getline(cin, line);
                    if (line == "END")
                        break;
                    content += line + "\n";
                }

                file << content;
                file.close();

                fileSystem.storeFile(fileName, content);
                
                break;
                
            case 2:
                system("cls");
                cout << "Enter file name to retrieve: ";
                cin >> fileName;
                fileSystem.retrieveFile(fileName);
                break;
                
            case 3:
                system("cls");
                cout << "Enter file name to delete: ";
                cin >> fileName;
                fileSystem.deleteFile(fileName);
                break;
                
            case 4:
                system("cls");
                fileSystem.displayFiles();
                break;
                
            case 5:
                cout << "Exiting system..." << endl;
                break;
                
            case 6:
                system("cls");
                int nodeId;
                cout << "Enter Node ID to delete: ";
                cin >> nodeId;
                fileSystem.deleteNode(nodeId);
                fileSystem.visualizeNodes();
                break;
                
            case 7:
                fileSystem.visualizeNodes();
                break;
                
            case 8:
                system("cls");
                cout << "Enter file name to modify: ";
                cin >> fileName;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter new content for the file (type 'END' to finish):\n";
                content.clear();
                while (true) 
				{
                    getline(cin, line);
                    if (line == "END")
                        break;
                    content += line + "\n";
                }
                fileSystem.modifyFile(fileName, content);
                break;
                
            case 9:
                 system("cls");
                  cout << "Enter file name to search: ";
                 cin >> fileName;
                 fileSystem.searchFile(fileName);
    			break;
    			
			case 10:
			    system("cls");
			    fileSystem.backupSystem("backup.txt");
			    break;
			    
			case 11:
			    system("cls");
			    fileSystem.restoreSystem("backup.txt");
			    break;
			    
			default:
			    cout << "Invalid choice! Please try again." << endl;
			    system("cls");
			    break;
			    
		}
	} while (choice != 5);

    return 0;
}
