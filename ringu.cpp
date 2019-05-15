#include <iostream>
template<typename Key> class Ring
{
	private:
		struct Node									//node of a ring
		{
			Node* prev, *next;
			Key key;
			Node(Key k)		//construct node from a key
			{
				key=k;
				next=NULL;
				prev=NULL;
			}
		};
		Node* head;
		int size;		//size of a ring
	public:
		class Iterator				//ring iterator
		{
			private:
				Node* curr;			//current state
			public:
				Iterator() {curr=NULL;}			//default constructor
				Iterator(Node* nodeptr){curr=nodeptr;}	//starting point consturctor
				Iterator(Ring<Key> ring){curr=ring.head;}	//construct and assign to object
				~Iterator(){curr=NULL;}
				Iterator operator++(){curr=curr->next; return *this;}	//next element
				Iterator operator--(){curr=curr->prev; return *this;}	//previous element
				Key operator*(){if(curr!=NULL) return curr->key; else return 0;}	//access element
		};
		Ring()			//default constructor
		{
			size=0;
			head=NULL;
		}
		Ring(Key arr[], int x)	//constructor from array
		{
			size=0;			
			if(x<=0) 		//if wrong argument
			{
				head=NULL;
				return;	
			}
			head=new Node(arr[0]);	//first element
			head->next=head;
			head->prev=head;
			size++;
			for(int i=1; i<x; i++)	//other elements
			{
				putBack(arr[i]);
			}
		}
		template<typename K>
		Ring(Ring<K> &ring)		//copy constructor
		{
			size=0;
			if(ring.isempty())	//if wrong argument
			{
				head=NULL;
				return;
			}
			head=new Node(ring.head->key);		//first element
			head->next=head;
			head->prev=head;
			size++;
			Iterator iterator(ring.head->next);
			for(int i=1; i<ring.size; i++, ++iterator)	//other elements
			{
				putBack(*iterator);
			}
		}
		template<typename K>
		Ring<Key> operator=(Ring<K> &ring)	//assignment operator
		{
			if(*this==ring) return *this;	//if silly argument
			size=0;
			if(ring.isempty())	//clear if argument empty
			{
				clear();
				head=NULL;
				return *this;
			}
			head=new Node(ring.head->key);	//first element
			head->next=head;
			head->prev=head;
			size++;
			Iterator iterator(ring.head->next);
			for(int i=1; i<ring.size; i++, ++iterator)	//other elements
			{
				putBack(*iterator);
			}
			return *this;			
		}
		Ring<Key> operator+(Ring<Key> &ring)	//concatenation
		{
			if(ring.isempty()) return *this;
			if(this->isempty()) return ring;	//simple cases
			Iterator iterator(ring.head);	
			Ring r1(*this);	//we're returning this
			for(int i=0; i<ring.size; i++, ++iterator)		//add one ring to the other
			{
				r1.putBack(*iterator);
			}
			return r1;
		}
		Ring<Key> operator+=(Ring &ring)
		{
			if(ring.isempty()) return *this;	//simple case
			int i=0;
			Iterator iterator(ring.head);
			if(isempty())		//if was empty
			{
				head=new Node(ring.head->key);
				head->next=head;
				head->prev=head;
				size++;
				i=1;
				++iterator;
			}
			for(; i<ring.size; i++, ++iterator)	//add elements
			{
				putBack(*iterator);
			}
			return *this;
		}
		~Ring()	//destructor
		{
			clear();
		}
		bool operator==(Ring<Key> ring)
		{
			if(size!=ring.size) return false;	
			if(isempty()) return ring.isempty();	//simple cases
			Iterator it1(head), it2(ring.head);
			for(int i=0; i<size; i++, ++it1, ++it2)		//compare
			{
				if(*it1!=*it2) return false;
			}
			return true;
		}
		Key operator[](int i) const	//access key at index
		{
			if(i<0) return 0;	//wrong argument
			if(isempty()) return 0;
			Iterator iterator(head);
			for(int j=0; j<i; ++iterator, j++);	//iterate to index
			return *iterator;
		}
		int search(Key k)	//# of elements with Key k
		{
			if(isempty()) return 0;
			int i=0, j=0;
			for(Iterator iterator(head); j<size; ++iterator, j++)	//search
			{
				if(*iterator==k) i++;
			}
			return i;
		}
		void print()	//print to cout
		{
			if(isempty()) return;
			int i=0;
			for(Iterator iterator(head); i<size; ++iterator, i++)
			{
				std::cout<<"Key at: "<<i<<std::endl<<*iterator<<std::endl;	
			}
			std::cout<<"---------------------------------------"<<std::endl;
		}
		void revprint()	//reverse print to cout
		{
			if(isempty()) return;
			int i=0;
			for(Iterator iterator(head); i<size; --iterator, i++)
			{
				std::cout<<*iterator<<std::endl;	
			}
			std::cout<<"---------------------------------------"<<std::endl;
		}
		void putFront(Key k)	//insert element at head
		{
			if(isempty())
			{
				head=new Node(k);
				head->next=head;
				head->prev=head;
				size=1;
				return;
			}
			head->prev->next=new Node(k);
			head->prev->next->prev=head->prev;
			head->prev=head->prev->next;
			head->prev->next=head;
			head=head->prev;
			size++;
		}
		void putBack(Key k)	//insert before head
		{
			putFront(k);
			head=head->next;
		}
		int remove(Key k)	//remove all elements with key k, return #of elements removed
		{
			if(isempty()) return 0;
			if(head->key==k)
			{
				popFront();
				return remove(k)+1;	//recursively pop front until empty or nothing to remove
			}
			Node* nodeptr=head->next, *dlt;
			int sum=0;
			for(int i=1; i<size; i++, nodeptr=nodeptr->next)	//search
			{
				if(nodeptr->key==k)
				{
					nodeptr->prev->next=nodeptr->next;
					nodeptr->next->prev=nodeptr->prev;
					dlt=nodeptr;
					nodeptr=nodeptr->prev;
					delete dlt;
					i--;
					sum++;
					size--;
				}
			}
			return sum;
		}
		void popFront()	//pop head
		{
			if(isempty()) return;
			if(size==1){
				delete head;
				head = NULL;
				size =0;
				return;
			}
			Node* nodeptr;
			head->prev->next=head->next;
			head->next->prev=head->prev;
			nodeptr=head;
			head=head->next;
			delete nodeptr;
			size--;
		}
		void popBack()	//pop element before head
		{
			if(isempty()) return;
			if(size == 1){
				delete head;
				head = NULL;
				size=0;
				return;
			}
			head=head->prev;
			popFront();
		}
		void clear()	//clear the list
		{
			if(isempty()) return;
			Node* nodeptr;
			for(nodeptr=head->next; nodeptr!=head;)
			{
				nodeptr=nodeptr->next;
				delete nodeptr->prev;
			}
			delete head;
			size=0;
			head=NULL;
		}
		void insert(Key k, int i)	//insert at index
		{
			if(i<0) return;	//wrong argument
			if(!i) 	//insertion at 0
			{
				putFront(k);
				return;
			}
			if(isempty())
			{
				head=new Node(k);
				head->next=head;
				head->prev=head;
				size=1;
				return;
			}
			Node* nodeptr=head;
			for(int j=1; j<i; j++, nodeptr=nodeptr->next);	//search
			nodeptr->next->prev=new Node(k);
			nodeptr->next->prev->next=nodeptr->next;
			nodeptr->next=nodeptr->next->prev;
			nodeptr->next->prev=nodeptr;
			size++;
		}
		bool multinsert(Key k, Key where)	//insert after all "where" keys, return true if inserted
		{
			if(isempty()) return false;
			Node* nodeptr=head;
			bool flag=false;
			for(int i=0; i<size; i++, nodeptr=nodeptr->next)	//search
			{
				if(nodeptr->key==where)
				{
					nodeptr->next->prev=new Node(k);
					nodeptr->next->prev->next=nodeptr->next;
					nodeptr->next=nodeptr->next->prev;
					nodeptr->next->prev=nodeptr;
					size++;
					flag=true;
					nodeptr=nodeptr->next;
					i++;
				}
			}
			return flag;
		}
		int getsize(){return size;}	//size getter
		bool isempty() const {return size==0;}	//true if no elements, else false
		Key front() {if(isempty()) return 0; return head->key;}	//element at front
		Key back() {if(isempty()) return 0; return head->prev->key;}	//element at back
};
using namespace std;
template <typename Key>
void split(const Ring<Key> &source, Ring<Key> &result1, bool dir1, int len1, Ring<Key> &result2, bool dir2, int len2)	//split function
{
	if(source.isempty()) return;
	if((&result1==&source)||(&result2==&source)) return;
	cout<<"del1"<<endl;
	result1.clear();
	cout<<"del2"<<endl;	
	result2.clear();	//clear 2 outputs	//ring2 starts building at 2nd element
	cout<<"start build"<<endl;
	result1.putBack(1);
	cout<<"wtf"<<endl;
	cout<<"eh"<<endl;
	result1.clear();
	int j=0;
	if(dir1)	//ring1
	{
		for(int i=0; i<len1; i++, j+=2)	//clockwise
		{
			cout<<"insert1"<<endl;
			result1.putBack(source[j]);
		}
	}
	else
	{
		for(int i=0; i<len1; i++, j+=2)	//counter clockwise
		{
			result1.insert(source[j], 1);
		}
	}
	cout<<"finish1"<<endl;
	j=1;
	if(dir2)	//second ring
	{
		for(int i=0; i<len2; i++, j+=2)	//clockwise
		{
			result2.putBack(source[j]);
		}
	}
	else
	{
		for(int i=0; i<len2; i++, j+=2)	//counter clockwise
		{
			cout<<"insert2"<<endl;
			result2.insert(source[j], 1);
		}
	}
	cout<<"finsish2"<<endl;
}
void key()	//"Press enter to continue"
{
    cout<<"Press enter to continue"<<endl;
    cin.get();
    fseek(stdin,0,SEEK_END);
    return;
}
template<typename Key>
void stresstest(Ring<Key> &ring, Key r1, Key r2, Key r3, int rem1, int rem2, int rem3, Key k, Key where)	//this function tests all the methods
{																					//r1, r2, r3 - keys to be removed, rem1, rem2, rem3 - indeces that are going to be tested, k - such nodes will be inserted, where is argument of multinsert,
	int helper;																		
	cout<<"empty: "<<ring.isempty()<<endl<<"size: "<<ring.getsize()<<endl;
	cout<<"Ring:"<<endl;
	ring.print();
	cout<<"Removed elements with key: "<<r1<<" "<<r2<<" "<<r3<<endl;
	helper=ring.getsize();
	ring.remove(r1);
	ring.remove(r2);
	ring.remove(r3);
	if((ring.search(r1))||(ring.search(r2))||(ring.search(r3)))
	{
		cout<<"Removal fault, aborting"<<endl;
		key();
		return;
	}
	ring.print();
	key();
	cout<<"Copy constructor: "<<endl;
	Ring<Key> copy(ring);
	if(!(copy==ring))
	{
		cout<<"Copy constructor fault, aborting"<<endl;
		key();
		return;
	}
	copy.print();
	key();
	cout<<"Assignment operator:"<<endl;
	copy=ring;
	if(!(copy==ring))
	{
		cout<<"Assignment fault, aborting"<<endl;
		key();
		return;
	}
	copy.print();
	key();
	cout<<"Pop 2 first elements:"<<endl;
	helper=ring.getsize();
	ring.popFront();
	ring.popFront();
	if((ring.getsize()+2!=helper)&&(!ring.isempty()))
	{
		cout<<"Popfront fault, aborting"<<endl;
		key();
		return;
	}
	ring.print();
	key();
	cout<<"Concatenate with + with the previous copy:"<<endl;
	Ring <int> clone(ring+copy);
	(ring+copy).print();
	cout<<"Concatenate with +="<<endl;
	ring+=copy;
	if(!(clone==ring))
	{
		cout<<"Concatenation fault, aborting"<<endl;
		key();
		return;
	}
	ring.print();
	key();
	cout<<"Clear the copy:"<<endl;
	copy.clear();
	if(!(copy.isempty()))
	{
		cout<<"Clearing fault, aborting"<<endl;
		key();
		return;
	}
	copy.print();
	key();
	cout<<"Search for elements with key "<<k<<endl;
	cout<<ring.search(k)<<" elements found"<<endl;
	key();
	cout<<"Access key at: "<<rem1<<" "<<rem2<<" "<<rem3<<endl<<ring[rem1]<<" "<<ring[rem2]<<" "<<ring[rem3]<<endl<<"Full list:"<<endl;
	ring.print();
	cout<<"Key at front:"<<endl<<ring.front()<<endl<<"Creating a copy."<<endl;
	copy=ring;
	typename Ring<Key>::Iterator iterator(ring);
	if(!(*iterator==ring.front()))
	{
		cout<<"Iterator or front() fault, aborting"<<endl;
		key();
		return;
	}
	key();
	cout<<"Insert elements with key "<<k<<" at: "<<rem1<<" "<<rem2<<" "<<rem3<<" and at front"<<endl;
	ring.insert(k, rem1);
	if(!(k==ring[rem1]))
	{
		cout<<"Insertion fault, aborting"<<endl;
		key();
		return;
	}
	ring.insert(k, rem2);
	if(!(k==ring[rem2]))
	{
		cout<<"Insertion fault, aborting"<<endl;
		key();
		return;
	}
	ring.insert(k, rem3);
	if(!(k==ring[rem3]))
	{
		cout<<"Insertion fault, aborting"<<endl;
		key();
		return;
	}
	ring.putFront(k);
	if(!(k==ring.front()))
	{
		cout<<"Insertion fault, aborting"<<endl;
		key();
		return;
	}
	ring.print();
	key();
	cout<<"Insert elements with key "<<k<<" after elements with key "<<where<<endl;
	ring.multinsert(k, where);
	typename Ring<Key>::Iterator it(ring);
	helper=ring.getsize();
	for(int i=0; i<helper; i++, ++it)
	{
		if(*it==where)
		{
			++it;
			if(*it!=k)
			{
				cout<<"Multinsertion fault, aborting"<<endl;
				key();
				return;
			}
		}
	}
	ring.print();
	key();
	cout<<"End of test"<<endl;
	key();
}
template<typename Key>
void splittest(Ring<Key> &source, Ring<Key> &result1, bool dir1, int len1, Ring<Key> &result2, bool dir2, int len2)	//test split function
{
	if(source.isempty()) return;
	if((&result1==&source)||(&result2==&source))
	{
		cout<<"Wrong arguments"<<endl;
		return;
	}
	Ring<Key> ring(source);
	split(source, result1, dir1, len1, result2, dir2, len2);
	cout<<"Split function for ring: "<<endl;
	ring.print();
	int j=0;
	if(dir1)
	{
		for(int i=0; i<len1; j+=2, i++)
		{
			if(ring[j]!=result1[i])
			{
				result1.print();
				cout<<"Split function fault, aborting, at "<<i<<", "<<ring[j]<<" is not "<<result1[i]<<endl;
				key();
				return;
			}
		}
	}
	else
	{
		for(int i=0; i<len1; i++, j+=2)
		{
			if(ring[j]!=result1[result1.getsize()-i])
			{
				result1.print();
				cout<<"Split function fault, aborting, at "<<i<<", "<<ring[j]<<" is not "<<result1[i]<<endl;
				key();
				return;
			}
		}
	}
	result1.print();
	j=1;
	if(dir2)
	{
		for(int i=0; i<len2; j+=2, i++)
		{
			if(ring[j]!=result2[i])
			{
				result2.print();
				cout<<"Split function fault, aborting, at "<<i<<", "<<ring[j]<<" is not "<<result2[result2.getsize()-i]<<endl;
				key();
				return;
			}
		}
	}
	else
	{
		for(int i=0; i<len2; j+=2, i++)
		{
			if(ring[j]!=result2[result2.getsize()-i])
			{
				result2.print();
				cout<<"Split function fault, aborting, at "<<i<<", "<<ring[j]<<" is not "<<result2[result2.getsize()-i]<<endl;
				key();
				return;
			}
		}
	}
	result2.print();
	cout<<"End of test"<<endl;
	key();
}
int main ()
{
	int i=1;
	int arr1[10]={7, 6, 1, 3, 1, 1, 232, 344, 1, 4};
	int arr2[10]={2, 4, 5, 1 ,1, 0, 265, 353, 4 ,5};
	int arr3[9]={2,2,2,1,2,2,1,2,2};
	Ring<int> ring1(arr1, 10);
	Ring<int> ring2(arr2, 10);
	Ring<int> ring3(arr3, 9);
	Ring <int> blank;	//empty list
	splittest(ring1, ring2, true, 3, ring3, false, 7);
	splittest(ring2, blank, true, 3, ring2, false, 7);
	splittest(ring3, ring3, true, 3, ring1, false, 7);
	splittest(ring3, ring2, true, 10, ring1, true, 6);
	splittest(ring2, ring1, false, 14, ring3, false, 69);
	stresstest(ring1, 6, 1, 232, 1, 2, 0, 5, 232);
	stresstest(blank, 7, 7, 676, 3, 2, 3, 1, 7);	//test the lists
	stresstest(ring3, 2, 1, 8, 0, 10, 67, 89, 89);
	stresstest(ring2, 2, 1, 5, 7, 1, 0, 99, 2);
	return 0;
}

