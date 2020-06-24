/*Archivo Stack.h 
En este archivo esta definida la clase Stack (Pila). 
Esta clase es una plantilla que permite almacenar variada cantidad de tipos*/
#ifndef STACK__H
#define STACK__H

#include <iostream>
#include <initializer_list>

using namespace std;
// Implementación del tipo de dato abstracto Pila


template <typename T>
class Stack{
private:
  
  // Esta clase define los nodos que contendran a los items del Stack
	class nodo{
		
		friend class Stack;

		T entry_;
		nodo * next_;
	  	
	public:

	  	nodo();
		nodo(T & item);
		nodo(const T & item);
		nodo(nodo & item);
		~nodo();
	};
	
	nodo * top_;
	size_t size_;
	
public:
	
	Stack(); // Crea un Stack vacío
	Stack(const Stack & ); // Crea y copia una pila a partir de otra
	Stack(const initializer_list<T> l); //crea un stack a partir de una lista de inicializadores
	~Stack(); // Elimina un stack y todo su contenido


	// Metodos del stack

	bool empty() const; // Verifica si un stack está vacío o no
	void push(T & item); // Inserta un nuevo item en el tope de la pila
	void push(const T & item);
	void pop(); // Elimina el elemento que esta en el tope del stack 
	T &  top(); // Devuelve una referencia al elemento que está en el tope del stack
	T const & top() const; // Idem anterior, solo que no se puede modificar
	void invert(); // Invierte un stack
	void clear(); // Borra el stack y lo deja vacio
	size_t size()const; // Devuelve el tamaño del stack

	//Operadores

	void operator=(const Stack & inp);



};

// Nodo

template <typename T>
Stack<T>::nodo::nodo(): next_(nullptr)
{
}

template <typename T>
Stack<T>::nodo::nodo(T & item):  entry_(item), next_(nullptr)
{
}

template <typename T>
Stack<T>::nodo::nodo(const T & item):  entry_(item), next_(nullptr)
{
}

template <typename T>
Stack<T>::nodo::nodo(nodo & item):  entry_(item.entry_), next_(nullptr)
{
}

template <typename T>
Stack<T>::nodo::~nodo()
{
}

// Stack

template <typename T>
Stack<T>::Stack() : top_(nullptr), size_(0)
{
	/*Crea e inizializa un stack vacio*/
}


template <typename T>
Stack<T>::Stack(const Stack & in ) : top_(nullptr), size_(0)
{
//Crea un stack y copia el contenido de in


	if(!(in.empty()) ) // Si el stack a copiar no está vacío
	{ 

		nodo * actual_in = in.top_;
		nodo * last = nullptr;

		while (actual_in != nullptr)
		{
			nodo * copy = new nodo(*actual_in); // Copia un nodo del stack in

			if (top_ == nullptr) // Si es la primer pasada
			{	
				top_ = copy;	
			}
			else
			{
				last->next_ = copy;
			}

			last = copy;
			actual_in = actual_in->next_;

			size_++;

		}



	}
	

}


template <typename T>
Stack<T>::Stack(const initializer_list<T> l): top_(nullptr), size_(0)
{

	nodo * copy;
	nodo * last;
	for(T i: l) // Para cada elemento de la lista de inicializadores
	{	
		copy = new nodo(i); // Crea un nodo del stack

		if(top_ == nullptr)
		{
			top_ = copy;
		}
		else
		{
			last -> next_ = copy;
		}

		last = copy;

		size_++;
	}
}

template <typename T>
Stack<T>::~Stack()
{
	if(!(this->empty())) // Si el stack no esta vacio
	{
		nodo * actual = top_;
		nodo * next;
		while(actual != nullptr) // Mientras no se alcance el fondo del stack
		{
			next = actual-> next_;
			delete actual;
			actual = next;
		}
	}
}

/*METODOS*/

template <typename T>
bool Stack<T>::empty()const 
{
	return (top_ == nullptr)? true:false; // Si en el tope del stack no hay nada entonces está vacío
}


/*Este metodo inserta un elemento en el tope de la pila
PRECONDICIONES: La pila esta creada
POSTCONDICIONES: En el tope de la pila tiene insertado el elemento T*/
template <typename T>
void Stack<T>::push(T &item){
	
	nodo * newTop = new nodo(item); // Crea un nodo inicializado con item

	if (!(this->empty())) // si el stack no está vacio 
	{
		newTop->next_ = top_; 
	}

	top_ = newTop;
	size_++;
}

/*Este metodo inserta un elemento en el tope de la pila
PRECONDICIONES: La pila esta creada
POSTCONDICIONES: En el tope de la pila tiene insertado el elemento T*/
template <typename T>
void Stack<T>::push(const T &item){
	
	nodo * newTop = new nodo(item); // Crea un nodo inicializado con item

	if (!(this->empty())) // si el stack no está vacio 
	{
		newTop->next_ = top_; 
	}

	top_ = newTop;
	size_++;
}

/*Elimina el elemento que esta en el tope del stack
PRECONDICIONES:Ninguna
POSTCONDICIONES: Se borra el tope del stack*/
template <typename T>
void Stack<T>::pop()
{
	if(!(this->empty()))
	{
		nodo * next = top_-> next_;
		delete top_;
		top_ = next;
		size_ --;
	}
}

/*Este metodo invierte el stack
PRECONDICIONES: NINGUNA
POSTCONDICIONES: El stack esta invertido*/
template<typename T>
void Stack<T>::invert(){

	if(!this->empty()) // Si el stack no esta vacío
	{	
		nodo * top = nullptr; // Stack 
		nodo * prev = nullptr;

		// Desapila el stack y lo reapila en un stack 
		// cuyo tope es top
		while(top_ != nullptr)  
		{
			
			top = top_;
			top_ = top_->next_;
			top-> next_ = prev;
			prev = top;	
			
		}

		top_ = top; // Asigna el stack invertido
	}
}


/*Este metodo retorna una refencia al elemento que esta
en el tope de la pila.
PRECONDICIONES: La pila no debe estar vacia
POSTCONDICIONES: Se devuelve una referencia al elemento que esta en el 
tope. El elemento se puede modificar*/
template <typename T>
T & Stack<T>::top()
{
	return top_ -> entry_;
}


/*Este metodo retorna una refencia al elemento que esta
en el tope de la pila.
PRECONDICIONES: La pila no debe estar vacia
POSTCONDICIONES: Se devuelve una referencia al elemento que esta en el 
tope. El elemento NO se puede modificar*/
template <typename T>
T const & Stack<T>::top() const
{
	return top_ -> entry_;
}

template <typename T>
void Stack<T>::clear(){
	if (!(this->empty())) // Si el stack no esta vacio
	{
		nodo * actual = top_;
		nodo * next;

		while (actual != nullptr) // Mientras no se llegue al fondo del stack
		{
			next = actual ->next_;

			delete actual;

			actual = next;
		}

		size_ = 0;

		top_= nullptr;
	}
}

/*Esta funcion devuelve el tamaño del stack
PRECONDICIONES: Ninguna
POSTCONDICIONES: Devuelve el tamaño del stack */
template <typename T>
size_t Stack<T>::size()const{
	return size_;
}

template <typename T>
void Stack<T>::operator=(const Stack & in){

	if(&in != this) // Si son stacks distintos
	{
		this->clear(); //Borra el stack

		if(!(in.empty()) ) // Si el stack a copiar no está vacío
		{ 

			nodo * actual_in = in.top_;
			nodo * last = nullptr;

			while (actual_in != nullptr)
			{
				nodo * copy = new nodo(*actual_in); // Copia un nodo del stack in

				if (top_ == nullptr) // Si es la primer pasada
				{	
					top_ = copy;	
				}
				else
				{
					last->next_ = copy;
				}

				last = copy;
				actual_in = actual_in->next_;

				size_++;

			}

		}
	}
}
#endif
