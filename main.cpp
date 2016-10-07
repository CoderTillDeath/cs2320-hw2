#include <iostream>
#include <fstream>

using namespace std;


// Helper Methods
bool contains(string s1, string s2)
{
    return (s1.find(s2)< s1.length());
}

string before(string s, string b)
{
    return s.substr(0,s.find(b));
}

string after(string s, string b)
{
    return s.substr(s.find(b)+1);
}

bool wildCompare(string s1, string s2)
{
    return (s1.compare(s2) == 0 || s1.compare("G:#wild#") == 0 || s2.compare("G:#wild#") == 0);
}



		// Email Node and Methods
		struct emailNode {
			string name;
			string subject;
			string body;
			emailNode *next;
			emailNode *prev;
		};
		
void deleteEmail(emailNode * n)
{
	if(n->prev != NULL)
	{
		n->prev->next = n->next;
	}
	
	if(n->next != NULL)
	{
		n->next->prev = n->prev;
	}
}

// Comparable
bool equals(emailNode * n1, emailNode * n2)
{
    return wildCompare(n1->name,n2->name) && wildCompare(n1->subject,n2->subject) && wildCompare(n1->body,n2->body);
}
// toString
void printEmail(emailNode * node)
{
	cout << node->name << "\t" << node->subject << "\t" << node->body << endl;
}
// Constructor
emailNode * newEmail(string name, string subject, string body)
{
	emailNode * node = new emailNode;
	node->name = name;
	node->subject = subject;
	node->body = body;
	node->next = NULL;
	node->prev = NULL;
	return node;
}


		// Queue structure for emails
		struct emailList {
			emailNode *first;
			emailNode *last;
			emailList *next;
		};

bool contains(emailNode * current, emailNode * node)
{
	if(current == NULL) return false;
	
	if(equals(current,node))
	{
		return true;
	}
	else
	{
		return contains(current->next, node);
	}
}

void printList(emailNode * current)
{
	if(current == NULL) return;
	
	printf("%s\t%s\t%s\n", current->name.c_str(), current->subject.c_str(), current->body.c_str());
	printList(current->next);
}

// Construcutor
emailList * newEmailList()
{
	emailList * list = new emailList;
	list->first = NULL;
	list->last = NULL;
	list->next = NULL;
	return list;
}

void remove(bool all, emailNode * list, emailNode * n)
{
	if(list == NULL) return;
	
	if(equals(list, n))
	{
		deleteEmail(list);
		if(all)
		{
			remove(all,list->next, n);
		}
		else
		{
			return;
		}
	}
	
	remove(all,list->next, n);
}

void remove(bool all, emailList * list, emailNode * n)
{
	if(list->first == NULL) return;
	
	if(equals(list->first, n))
	{
		deleteEmail(list->first);
		list->first = list->first->next;
		
		if(!all)
		{
			return;
		}
		else
		{
			remove(all,list, n);
		}
	}
	else
	{
		remove(all, list->first->next, n);
	}
}

void remove(emailList * list, emailNode * n)
{
    remove(false,list,n);
}

void removeAll(emailList * list, emailNode * n)
{
    remove(true,list,n);
}

void insert(emailList * list, string name, string subject, string body)
{
    emailNode *n = newEmail(name,subject,body);

    if(list->first == NULL)
    {
		list->first = n;
        list->last = n;
    }
    else
    {
		if(!contains(list->first, n))
		{
			if(contains(subject,"(Cancelled)"))
			{
				emailNode * remNode = newEmail(name,before(subject,"(Cancelled)"),body);

				remove(list, remNode);
    	    }
			else {
				if(subject.compare("Purchase-order-cancellation") == 0) 
				{
					emailNode * remNode = newEmail("G:#wild#","G:#wild#",body);
		    
					removeAll(list,remNode);
				}

                if(list->first == NULL)
                {
                    list->first = n;
                    list->last = n;
                }
                else
                {
					n->prev = list->last;
				    list->last->next = n;
				    list->last = n;
                }
			}
		}
    }
}

string getBody(emailNode * comparator, emailNode * current)
{
	if(current == NULL) return "";
	
	string name = comparator->name;
	string subject = comparator->subject;
	
	if(current->name.compare(name) == 0 && current->subject.compare(subject) == 0)
	{
		deleteEmail(current);
		
		return ", " + current->body + getBody(comparator, current->next);
	}
	else
	{
		return getBody(comparator, current->next);
	}
}

void addAllLike(emailList * list, emailNode * node)
{
    emailNode * current = node;
    string name = current->name;
    string subject = current->subject;

    string ALL = current->body + getBody(node,current->next);
    
    insert(list,name,subject,ALL);
}

void getGrouped (emailList * newlist, emailNode * current)
{
	if(current == NULL) return;
    
	addAllLike(newlist,current);
    getGrouped(newlist,current->next);
}

		struct fileList {
			emailList *first;
			emailList *last;
		};
		

void insert(fileList * list, emailList * l)
{
    if(list->first == NULL)
    {
		list->first = l;
		list->last = l;
	}
    else 
    {
		list->last->next = l;
		list->last = l;
    }
}
		
fileList * newFileList()
{
	fileList * list = new fileList;
	list->first = NULL;
	list->last = NULL;
	
	return list;
}

void insertLists(emailNode ** arr, emailList * current, int i)
{
	if(current == NULL) return;
	arr[i] = current->first;
	insertLists(arr, current->next, i+1);
}

bool addIter(emailList * finalList, emailNode ** list, int i, int max)
{
	if(i == max) return false;
	
	bool result = false;
	
	if(list[i])
	{
		result = true;
		insert(finalList,list[i]->name,list[i]->subject,list[i]->body);
		list[i] = list[i]->next;
	}
	
	return result || addIter(finalList, list,i+1,max);
}

emailList * printInterleaved(fileList * l, int size)
{
	emailList * finalList = newEmailList();
	
	emailNode ** list = new emailNode*[size];
	
	insertLists(list, l->first, 0);
	
	bool repeat = false;
	
	do
	{
		repeat = false;
		for(int i = 0; i < size; i++)
		{
			if(list[i])
			{
				repeat = true;
				insert(finalList,list[i]->name,list[i]->subject,list[i]->body);
				list[i] = list[i]->next;
			}
		}
	}
	while(repeat);
	
	return finalList;
}

void readFile(ifstream * file, emailList * e)
{
	string line = "";
	if(getline(*file, line))
	{
		if(line.length() != 0 && line.at(0) != '#')
		{
			string name = before(line,"\t");
			string subject = before(after(line,"\t"), "\t");
			string body = after(after(line, "\t"),"\t");
            insert(e, name, subject, body);
		}
		readFile(file,e);
	}
}

int readFiles(string input, fileList * f, int i)
{
	ifstream file (input + "." + to_string(i) + ".txt");

    if(!file.is_open()) 
    {
		i--;
		return i;
	}

	emailList * e = newEmailList();

	readFile(&file,e);
		
    emailList * list = newEmailList();
    getGrouped(list,e->first);
    
	insert(f,list);
	
	return readFiles(input,f,i+1);
}


int main(int argc, const char * argv[])
{
    string input = argv[1];
	input = input.substr(6, input.length() - 12);
	
    fileList * f = newFileList();

    int i = readFiles(input,f,1);
    
    printList(printInterleaved(f,i)->first);
}
