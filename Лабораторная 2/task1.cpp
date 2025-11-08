#include <iostream>

template <typename T>
class Grid final{
public:
	using value_type = T;
	using size_type = unsigned;
private:
	T** data;
	size_type y_size, x_size;

public:

    Grid(const Grid<T>& other) : data(new T*[other.y_size*other.x_size]), y_size(other.y_size), x_size(other.x_size){
		/*try-catch*/
		for(size_type i = 0, end = x_size*y_size; i != end; ++i){
			try{
				data[i] = new T(*other.data[i]);
			}catch(){
				for(size_type j = 0, j < i; ++j) delete data[i];
				delete data[];
			}
		}
    }
    Grid(Grid<T> && other):data(other.data),y_size(other.y_size),x_size(other.x_size){other.data = nullptr;other.y_size = 0;other.x_size = 0;}
	Grid<T>& operator=(Grid<T>const& other){
        for(size_type i = 0, end = x_size*y_size; i != end; ++i) 
			delete data[i];
        delete[] data;
        data = new T*[other.y_size*other.x_size];
        y_size = other.y_size;
		x_size = other.x_size;
        for(size_type i = 0, end = x_size*y_size; i != end; ++i){
			data[i] = new T(*other.data[i]);
	    }
        return *this;
    }
	/*поправить*/
    Grid<T>& operator=(Grid<T>&&other){
		std::swap(data, other.data);
		std::swap(x_size, other.x_size);
		std::swap(y_size, other.Y_size);
		return *this;
	}



	Grid(T *data_inp, size_type y_size, size_type x_size):data(new T*[y_size*x_size]),y_size(y_size),x_size(x_size){
        for(size_type i = 0, end = x_size*y_size; i != end; ++i){
			data[i] = new T;
            *data[i] = data_inp[i];
	    }
    }
	Grid(T const &t):data(new T*[1]), y_size(1), x_size(1){data[0] = new T(t);}
	Grid(size_type y_size, size_type x_size):data(new T*[y_size*x_size]), y_size(y_size), x_size(x_size){
        for(size_type i = 0, end = x_size*y_size; i != end; ++i){
			data[i] = new T();
	    }
    }
	Grid(size_type y_size, size_type x_size, T const &t):data(new T*[y_size*x_size]), y_size(y_size), x_size(x_size){
        for(size_type i = 0, end = x_size*y_size; i != end; ++i){
			data[i] = new T(t);
	    }
    }




	T operator()(size_type y_idx, size_type x_idx) const{
		return *data[y_idx * x_size + x_idx];
	}

	T& operator()(size_type y_idx, size_type x_idx){
		return *data[y_idx * x_size + x_idx];
	}
	Grid<T>& operator=(T const &t){
		for(auto it = data, end = data + x_size * y_size; it != end; ++it) **it = t;
		return *this;
	}





	size_type get_y_size() const{return y_size;}
	size_type get_x_size() const{return x_size;}
	void show(){
		for(size_type i = 0, end = x_size*y_size; i != end; ++i) 
			std::cout << *data[i] << std::endl;
	}

	~Grid(){
        for(size_type i = 0, end = x_size*y_size; i != end; ++i) 
			delete data[i];
        delete[] data;
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

int main(){
	int* ar = new int[10]{1, 2, 3, 4, 5, 6, 7, 9, 8, 10};
	Cube c(5);
	const Grid<int> g(ar, 2, 5);
    Grid<int> h = g;
	std::cout << h(1, 0);
}