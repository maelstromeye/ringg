#include <iostream>
template<typename Key> class Ring
{
	private:
		struct Node
		{
			Node* prev, *next;
			Key key;
			Node(Key key1)
			{
				key=key1;
				next=NULL;
				prev=NULL;
			}
		};
		Node* head;
		int size;
	public:
		class Iterator
		{
			private:
				Node* curr;
			public:
				Iterator() {curr=NULL;}
				Iterator(Node* nodeptr){curr=nodeptr;}
				Iterator operator++(){curr=curr->next; return *this;}
				Iterator operator--(){curr=curr->prev; return *this;}
				Key operator*(){return curr->key;}
				void reset(){curr=head;}
				void clear(){delete curr; curr=NULL;}
		};
		Ring()
		{
			size=0;
			head=NULL;
		}
		Ring(Key arr[], int x)
		{
			size=0;
			if(x<=0) 
			{
				head=NULL;
				return;	
			}
			head=new Node(arr[0]);
			head->next=head;
			head->prev=head;
			size++;
			for(int i=1; i<x; i++)
			{
				head->prev->next=new Node(arr[i]);
				head->prev->next->prev=head->prev;
				head->prev=head->prev->next;
				head->prev->next=head;
				size++;
			}
		}
		template<typename K>
		Ring(Ring<K> &ring)
		{
			size=0;
			if(ring.isempty())
			{
				head=NULL;
				return;
			}
			head=new Node(ring.head->key);
			head->next=head;
			head->prev=head;
			size++;
			Iterator iterator(ring.head->next);
			for(int i=1; i<ring.size; i++, ++iterator)
			{
				head->prev->next=new Node(*iterator);
				head->prev->next->prev=head->prev;
				head->prev=head->prev->next;
				head->prev->next=head;
				size++;
			}
		}
		template<typename K>
		Ring<Key> operator=(Ring<K> &ring)
		{
			if(*this==ring) return *this;
			size=0;
			if(ring.isempty())
			{
				clear();
				head=NULL;
				return *this;
			}
			head=new Node(ring.head->key);
			head->next=head;
			head->prev=head;
			size++;
			Iterator iterator(ring.head->next);
			for(int i=1; i<ring.size; i++, ++iterator)
			{
				head->prev->next=new Node(*iterator);
				head->prev->next->prev=head->prev;
				head->prev=head->prev->next;
				head->prev->next=head;
				size++;
			}
			return *this;			
		}
		Ring<Key> operator+(Ring<Key> &ring)
		{
			if(ring.isempty()) return *this;
			if(this->isempty()) return ring;
			Iterator iterator(ring.head);
			Ring r1(*this);
			for(int i=0; i<ring.size; i++, ++iterator)
			{
				r1.head->prev->next=new Node(*iterator);
				r1.head->prev->next->prev=r1.head->prev;
				r1.head->prev=r1.head->prev->next;
				r1.head->prev->next=r1.head;
				r1.size++;
			}
			return r1;
		}
		Ring<Key> operator+=(Ring &ring)
		{
			if(ring.isempty()) return *this;
			int i=0;
			Iterator iterator(ring.head);
			if(isempty())
			{
				head=new Node(ring.head->key);
				head->next=head;
				head->prev=head;
				size++;
				i=1;
				++iterator;
			}
			for(; i<ring.size; i++, ++iterator)
			{
				head->prev->next=new Node(*iterator);
				head->prev->next->prev=head->prev;
				head->prev=head->prev->next;
				head->prev->next=head;
				size++;
			}
			return *this;
		}
		~Ring()
		{
			clear();
		}
		bool operator==(Ring<Key> ring)
		{
			if(size!=ring.size) return false;	//CHECK EMPTY
			Iterator it1(head), it2(ring.head);
			for(int i=0; i<size; i++, ++it1, ++it2)
			{
				if(*it1!=*it2) return false;
			}
			return true;
		}
		int search(Key k)
		{
			if(isempty()) return 0;
			int i=0, j=0;
			for(Iterator iterator(head);j<size; ++iterator, j++)
			{
				if(*iterator==k) i++;
			}
			return i;
		}
		Key operator[](int i)
		{
			if(i<0) return 0;
			if(isempty) return 0;
			Iterator iterator(head);
			for(int j=0; j<i; ++iterator, j++);
			return *iterator;
		}
		void print()
		{
			if(isempty()) return;
			int i=0;
			for(Iterator iterator(head); i<size; ++iterator, i++)
			{
				std::cout<<"Key at: "<<i<<std::endl<<*iterator<<std::endl;	
			}
		}
		void revprint()
		{
			if(isempty()) return;
			int i=0;
			for(Iterator iterator(head); i<size; --iterator, i++)
			{
				std::cout<<*iterator<<std::endl;	
			}
		}
		void putFront(Key k)
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
		int remove(Key k)	//popfront recursively
		{
			if(isempty()) return 0;
			if(head->key==k)
			{
				popFront();
				return remove(k)+1;
			}
			Node* nodeptr=head->next, *dlt;
			int sum=0;
			for(int i=1; i<size; i++, nodeptr=nodeptr->next)
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
		void popFront()
		{
			if(isempty()) return;
			Node* nodeptr;
			head->prev->next=head->next;
			head->next->prev=head->prev;
			nodeptr=head;
			head=head->next;
			delete nodeptr;
			size--;
		}
		void clear()
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
			if(i<0) return;
			if(!i) 
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
			for(int j=1; j<i; j++, nodeptr=nodeptr->next);
			nodeptr->next->prev=new Node(k);
			nodeptr->next->prev->next=nodeptr->next;
			nodeptr->next=nodeptr->next->prev;
			nodeptr->next->prev=nodeptr;
			size++;
		}
		bool multinsert(Key k, Key where)
		{
			if(isempty()) return false;
			Node* nodeptr=head;
			bool flag=false;
			for(int i=0; i<size; i++, nodeptr=nodeptr->next)
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
		int getsize(){return size;}	
		bool isempty() {return size==0;}
		Key front() {return head->key;}
};

int main ()
{
	int arr1[8]={2,3,4,5,1,2,1,9};
	int arr2[1]={8};
	int arr3[9]={2,2,2,1,2,2,1,2,2};
	Ring<int> ring1(arr1, 8);
	Ring<int> ring2(arr2, 1);
	Ring<int> ring3(arr1, 0);
	Ring<int> ring5(arr3, 9);
	ring5.print();
	ring5.multinsert(69, 2);
	ring5.print();
	ring5.multinsert(2, 2);
	ring5.print();
	std::cout<<ring5.remove(2)<<std::endl;
	ring5.print();
//	ring1.print();
//	std::cout<<(ring1==ring1)<<std::endl;
//	std::cout<<(ring3==ring3)<<std::endl;
//	std::cout<<(ring2==ring1)<<std::endl;
//	Ring<int> ring4(ring1);
//	ring4.print();
//	ring1.print();
//	(ring4+ring1).print();
//	(ring1+ring4).print();
//	(ring2+ring4).print();
//	ring4=(ring1+ring2);
//	std::cout<<"keyyyyek"<<std::endl;
//	ring4.clear();
//	std::cout<<ring4.isempty()<<std::endl;
//	ring4.print();
//	ring3=ring4;
//	ring4+=ring1;
//	std::cout<<(ring4==(ring3+ring1))<<std::endl;
//	ring4.print();
//	ring4+=ring2;
//	ring4.insert(29420, 0);
//	ring4.insert(98, 12);
//	ring4.insert(11, 2);
//	ring4.print();
//	std::cout<<ring4.search(1)<<std::endl<<ring4.search(8)<<std::endl<<ring4.isempty()<<std::endl;
	return 0;
}

