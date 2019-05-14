#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstdio>

#define outnode(nname) printf("nname: %d,%d,%d,%d - %d\n",nname.mask(),nname.content(),nname.next(),nname.another(),&nname);


class Node {
    char _mask;
    int _content;
    Node* _next;
    Node* _another;
  public:
    Node(char a, int b) {_mask=a; _content=b; _next=NULL; _another=NULL; }
    Node(char a, int b, Node* newNode) {_mask=a; _content=b; _next=NULL; _another=newNode; }
	Node(const Node &a){_mask=a._mask; _content=a._content; _next=a._next; _another=a._another; }
    char mask() {return _mask;}
    int content() {return _content;}
    Node* next() {return _next;}
    void snext(Node* newNode) {_next=newNode;}
    Node* another() {return _another;}
    void sanother(Node* newNode) {_another=newNode;}
    int excalc() { //tree calculation
        if (mask()==0) {return content();}
        if (!next()) {return 0;}
        if (mask()==1) {return 0-(next()->excalc());}
        if (!another()) {return 0;}
        if (mask()==2) {
            if (content()==0) {return (next()->excalc())+(another()->excalc());}
            else if (content()==1) {return (next()->excalc())-(another()->excalc());}
            else if (content()==2) {return (next()->excalc())*(another()->excalc());}
            else if (content()==3) {return (next()->excalc())/(another()->excalc());}
            else if (content()==4) {return (int)floor(pow((double)(next()->excalc()),(double)(another()->excalc())));}
        }
        return 0;
    }
};

class deque {
    Node* outputElement;
    public:
    deque(): outputElement(NULL) {}
    Node get() {
        if (!outputElement) {return Node(-1,0);}
        Node temp(*outputElement);
        delete outputElement;
        outputElement=temp.next();
        temp.snext(NULL);
        return temp;
    }
    Node show() {
        if (!outputElement) {return Node(-1,0);}
        Node temp(*outputElement);
        return temp;
    }
    void addFirstElement(Node* newNode) {
        newNode->snext(outputElement);
        outputElement=newNode;
    }
    void addLastElement(Node* newNode) {
        Node* cur = outputElement;
        if(!cur) {outputElement=newNode; return;}
        while (cur->next()) {cur=cur->next();}
        cur->snext(newNode);
    }
};

class tree {
    deque tempstack;
    public:
    tree() {}
    void addNode(Node* newNode) {
        for (int i=0; i<newNode->mask(); i++) {
            newNode->sanother(newNode->next());
            newNode->snext(tempstack.get().another());
        }
        Node* sNode = new Node(0, 0, newNode);
        tempstack.addFirstElement(sNode);
    }
    int calc() {
        return tempstack.get().another()->excalc();
    }
};

void excalc(char* expression)
{
    if (!*expression) {std::cout<<"No expression to calc\n"; exit(0);}
    deque stack;
    deque queue;
    char object[256];
    object[0]='\0';
    char i = 0;
    while (*expression) {
        if ((*expression>='0')&&(*expression<='9')) {
            object[i]=*expression;
            if (i==255) {std::cout<<"Too long number\n"; exit(1);}
            i++;
            object[i]='\0';
        } else if ((*expression=='-')||(*expression=='+')||(*expression=='*')||(*expression=='/')||(*expression=='^')||(*expression=='(')||(*expression=')')) {
            if ((i==0)&&(*expression!='-')&&(*expression!='(')&&(*(expression-1)!=')')) {
                std::cout<<"Unknown operation met\n";
                exit(1);
            } else if ((i==0)&&(*expression=='-')&&(*(expression-1)!=')')) {
                object[0]='-';
                i++;
                object[1]=='\0';
            } else if ((i==0)&&(*expression=='-')&&(*(expression-1)=')')) {
                while ((stack.show().mask()==2)) {
                    queue.addLastElement(new Node(stack.get()));
                }
                stack.addFirstElement(new Node(2,1));
            } else if ((i!=0)&&(*expression=='-')) {
                int a;
                a=atoi(object);
                i=0;
                object[0]='\0';
                queue.addLastElement(new Node(0,a));
                while ((stack.show().mask()==2)) {
                    queue.addLastElement(new Node(stack.get()));
                } 
                stack.addFirstElement(new Node(2,1));
            } else if ((i==0)&&(*expression=='(')) {
                if (*(expression-1)==')') {
                    while ((stack.show().mask()==2)&&(stack.show().content()>1)) {
                        queue.addLastElement(new Node(stack.get()));
                    } 
                    stack.addFirstElement(new Node(2,2));
                } 
                stack.addFirstElement(new Node(3,0));
            } else if ((i!=0)&&(*expression=='(')) {
                int a; 
                a=atoi(object); 
                i=0; 
                object[0]='\0'; 
                queue.addLastElement(new Node(0,a)); 
                while((stack.show().mask()==2)&&(stack.show().content()>1)) {
                    queue.addLastElement(new Node(stack.get()));
                } 
                stack.addFirstElement(new Node(2,2)); 
                stack.addFirstElement(new Node(3,0));
            } else if (*expression==')') {
                if (i!=0) {int a;
                a=atoi(object); 
                i=0; object[0]='\0'; 
                queue.addLastElement(new Node(0,a));}
                while ((stack.show().mask()==2)) {
                    queue.addLastElement(new Node(stack.get()));
                } 
                if (stack.get().mask()<3) {
                    std::cout<<"Incorrect expression to calc\n";
                    exit(1);
                }
            } else if (*expression=='+') {
                if (i!=0) {int a;
                a=atoi(object); 
                i=0; 
                object[0]='\0'; 
                queue.addLastElement(new Node(0,a));}
                while ((stack.show().mask()==2)) {
                    queue.addLastElement(new Node(stack.get()));
                } 
                stack.addFirstElement(new Node(2,0));
            } else if (*expression=='*') {
                if (i!=0) {int a;
                a=atoi(object); 
                i=0; 
                object[0]='\0'; 
                queue.addLastElement(new Node(0,a));}
                while ((stack.show().mask()==2)&&(stack.show().content()>1)) {
                    queue.addLastElement(new Node(stack.get()));
                } 
                stack.addFirstElement(new Node(2,2));
            } else if (*expression=='/') {
                if (i!=0) {int a;
                a=atoi(object);
                i=0;
                object[0]='\0';
                queue.addLastElement(new Node(0,a));}
                while ((stack.show().mask()==2)&&(stack.show().content()>1)) {
                    queue.addLastElement(new Node(stack.get()));
                } 
                stack.addFirstElement(new Node(2,3));
            } else if (*expression=='^') {
                if (i!=0) {int a;
                a=atoi(object);
                i=0; 
                object[0]='\0'; 
                queue.addLastElement(new Node(0,a)); }
                stack.addFirstElement(new Node(2,4));
            }
        } else {
            std::cout<<"Incorrect expression to calc\n";
            exit(1);
        }
        std::cout << *expression;
        expression++;	
    }
    std::cout<<"\n";
    if (i!=0) {
        int a;
        a=atoi(object);
        i=0;
        object[0]='\0';
        queue.addLastElement(new Node(0,a));
    }
    while (stack.show().mask()>=0) {
        queue.addLastElement(new Node(stack.get()));
    }
    tree calcer;
    do {
        Node out = queue.get();
        if (!out.mask()) {
            std::cout<<out.content();
        } else {
            if (out.content()==0) {
                std::cout<<"+";
            } else if (out.content()==1) {
                std::cout<<"-";
            } else if (out.content()==2) {
                std::cout<<"*";
            } else if (out.content()==3) {
                std::cout<<"/";
            } else if (out.content()==4) {
                std::cout<<"^";
            }
        }
        std::cout<<"|";
        calcer.addNode(new Node(out));
    } while (queue.show().mask()>=0);
    std::cout<<"\n";
    std::cout<< calcer.calc();
}

int main() {
    char *a, i=0;
	
	Node test1(1,2);
	Node test2(3,4,&test1);
	Node test3(5,6,&test2);
	test3.snext(&test1);
	Node test4(test3);
	Node test5(test1);
	
	printf("test1: %d,%d,%d,%d - %d\n",test1.mask(),test1.content(),test1.next(),test1.another(),&test1);
	printf("test2: %d,%d,%d,%d - %d\n",test2.mask(),test2.content(),test2.next(),test2.another(),&test2);
	outnode(test3);
	outnode(test4);
	outnode(test5);
	
    a=new char[1024];
    int it = 0;
    scanf("%c",&i);
    while ((i!='\n')&&(it<1023)) {
        a[it]=i;
        it++;
        a[it]='\0';
        scanf("%c",&i);
    }
    excalc(a);


}
