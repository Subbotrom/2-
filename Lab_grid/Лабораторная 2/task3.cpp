#include <iostream>

template <typename T, unsigned dim>
class Grid final{
public:
	using value_type = T;
	using size_type = unsigned;
private:
	T** data;
	size_type* size;
    bool child;

public:
    /*
    Grid<T>& operator=(Grid<T>& other){
        for(size_type i = 0, end = x_size*y_size; i != end; ++i) 
			delete data[i];
        delete[] data;
        data = new T*[other.y_size*other.x_size];
        y_size = other.y_size;
		x_size = other.x_size;
        for(size_type i = 0, end = x_size*y_size; i != end; ++i){
			data[i] = new T;
            *data[i] = *other.data[i];
	    }
        return *this;
    }
    Grid<T>& operator=(Grid<T>&&other){
        for(size_type i = 0, end = x_size*y_size; i != end; ++i) 
			delete data[i];
        delete[] data;
		data = other.data;
		y_size = other.y_size;
		x_size = other.x_size;
        other.data = nullptr;
        other.x_size = 0;
        other.y_size = 0;
		return *this;
	}
*/

    /*Вспомогательные функции*/

    template<typename... Args>
    size_type prod(size_type iter, size_type arg1, size_type arg2, Args... args) {
        if(iter < dim)
        return arg1*prod(iter+1, arg2, args...);
        else return 1;
    }
    size_type prod(size_type iter, size_type arg){
        if(iter < dim)
        return arg;
        else return 1;
    }
    size_type prod(size_type * s, size_type start) const{
        size_type res = 1;
        for(size_type i = start, end = dim; i != end; ++i){
            res*= s[i];
        }
        return res;
    }
    template<typename... Args>
    size_type sum(size_type iter, size_type arg1, size_type arg2, Args... args) {
        if(iter < dim)
        return arg1+sum(iter+1, arg2, args...);
        else return 1;
    }
    size_type sum(size_type iter, size_type arg){
        if(iter < dim)
        return arg;
        else return 0;
    }
    size_type sum(size_type* s){
        size_type res = 1;
        for(size_type i = 0, end = dim; i != end; ++i){
            res+= s[i];
        }
        return res;
    }
    template<typename... Args>
    T fill(size_type* dst, size_type iter, size_type size, Args... others){
        dst[0] = size;
        return fill(dst+1, iter+1, others...);
    }
    T fill(size_type* dst, size_type iter, T item){
        if(iter == dim)
        return item;
        else {dst[0] = item; return T();}
    }
    void fill(T** dst, T* src, size_type size){
        for(size_type i = 0, end = size; i != end; ++i){
            T* el = new T(src[i]);
            dst[i] = el;
        }
    }
    void fill(T** dst, size_type size){
        for(size_type i = 0, end = size; i != end; ++i){
            T* el = new T();
            dst[i] = el;
        }
    }
    void fill(T** dst, T const t, size_type size){
        for(size_type i = 0, end = size; i != end; ++i){
            T* el = new T(t);
            dst[i] = el;
        }
    }

    /*Конструкторы*/


    template<typename... Args>
	Grid(T *data_inp, Args... size_par):data(new T*[prod(0, size_par...)]), size(new size_type[sum(0, size_par...)]), child(false){
        fill(size, 0, size_par...);
        fill(data, data_inp, prod(0, size_par...));
    }
    
	template<typename... Args>
    Grid(Args... size_par):data(new T*[prod(0, size_par...)]), size(new size_type[sum(0, size_par...)]), child(false){
        T last = fill(size, 0, size_par...);
        fill(data, last, prod(0, size_par...));
    }

    Grid(T** data_arr, size_type* size_arr, bool child):data(data_arr),size(size_arr), child(child){}

    Grid(Grid<T, dim>& other):Grid(other.data, other.size, true){}

    Grid(Grid<T, dim> && other):Grid(other.data, other.size, true){other.data = nullptr; other.size = nullptr;}

    /*Методы*/

    Grid<T, dim>& operator=(Grid<T, dim> const & other){
        if(!child){
        for(size_type i = 0, end = prod(size, 0); i != end; ++i) 
			delete data[i];
        delete[] data;
        delete[] size;
        }
        else delete[] data;
        data = new T*[prod(other.size, 0)];
        for(size_type i = 0; i < prod(other.size, 0); ++i){
            T* el = new T(*other.data[i]);
            data[i] = el;
        }
        size = new size_type(dim);
        for(size_type i = 0; i < dim; ++i){
            size[i] = other.size[i];
        }
        return *this;
    }

    Grid<T, dim>& operator=(Grid<T, dim>&& other){
        if(!child){
        for(size_type i = 0, end = prod(size, 0); i != end; ++i) 
			delete data[i];
        delete[] data;
        delete[] size;
        }
        else delete[] data;
        data = new T*[prod(other.size, 0)];
        for(size_type i = 0; i < prod(other.size, 0); ++i){
            T* el = new T(*other.data[i]);
            data[i] = el;
            delete other.data[i];
        }
        size = new size_type(dim);
        for(size_type i = 0; i < dim; ++i){
            size[i] = other.size[i];
        }
        delete[] other.data;
        delete[] other.size;
        return *this;
    }

    Grid<T, dim>& operator=(T const t){
        for(size_type i = 0; i < prod(size, 0); ++i){
            *data[i] = t;
        }
        return *this;
    }

    template<typename... Args>
    size_type merge(size_type idx, size_type iter,  size_type arg1, size_type arg2, Args... args) const{
        return merge(idx + prod(size, iter+1)*arg1, iter+1, arg2, args...);
    }
    size_type merge(size_type idx, size_type iter,  size_type arg) const{
        return idx + arg;
    }

    template<typename... Args>
    T operator()(Args ... args) const{
        size_type idx = merge(0, 0, args...);
        return *data[idx];
    }
    template<typename... Args>
    T& operator()(Args ... args){
        size_type idx = merge(0, 0, args...);
        return *data[idx];
    }

    Grid<T, dim-1>& operator[](size_type idx){
        size_type start = idx*prod(size, 1), end = (idx+1)*prod(size, 1);
        T** darray = new T*[prod(size, 1)];
        for(size_type i = start; i < end; ++i){
            darray[i-start] = data[i];
        }
        Grid<T, dim-1>* slice = new Grid<T, dim-1>(darray, size+1, true);
        return *slice;
    }

    Grid<T, dim-1> operator[](size_type idx) const{
        size_type start = idx*prod(size, 1), end = (idx+1)*prod(size, 1);
        T** darray = new T*[prod(size, 1)];
        for(size_type i = start; i < end; ++i){
            T* el = new T(*data[i]);
            darray[i-start] = el;
        }
        size_type* size_arr = new size_type[dim-1];
        for(size_type i = 0; i < dim-1; ++i){
            size_arr[i] = size[i+1];
        }
        return Grid<T, dim-1>(darray, size_arr, false);
    }

	void show_sizes() const{
        for(size_type i = 0, end = dim; i != end; ++i){
			std::cout << size[i] << " ";
        }
        std::cout << std::endl;
    }
    size_type get_size(size_type n){
        return size[n];
    }
	void show(){
		for(size_type i = 0, end = prod(size, 0); i != end; ++i) 
			std::cout << *data[i] << std::endl;
	}

	~Grid(){
        if(!child){
        for(size_type i = 0, end = prod(size, 0); i != end; ++i) 
			delete data[i];
        delete[] data;
        delete[] size;
        }
        else delete[] data;
	}
};

class Cube{
	public:
	int numb;
	Cube() = delete;
	Cube(int n):numb(n){};
	friend std::ostream& operator<<(std::ostream& os, const Cube& obj);
};

std::ostream& operator<<(std::ostream& os, const Cube& obj) {
        os << obj.numb;
        return os;
    }

#include <cassert> 

int main(){
    int* a = new int[18]{1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18};
    Grid<float,3> g3(2, 3, 4, 1.0f); 
    g3[1][0][0] = 5;
}
/*
1(2) 3(4) 5(6) 
7(8) 9(10) 11(12) 
13(14) 15(16) 17(18)*/