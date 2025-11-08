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

    class Row{
        public:
            using value_type = T;
            using size_type = unsigned;
        private:
            T** data;
            size_type size;
        public:
            Row(Grid<T>const & owner, size_type row):data(new T*[owner.x_size]), size(owner.x_size){
                if(row < owner.y_size){
                    for(size_type i = 0, end = size; i != end; ++i){
                        data[i] = owner.data[row * size + i];
                    }
                }
            }
            T& operator[](size_type idx){
                return *data[idx];
            }
            T operator[](size_type idx) const{
                return *data[idx];
            }
            ~Row(){}
    };

    Grid(const Grid<T>& other) : data(new T*[other.y_size*other.x_size]), y_size(other.y_size), x_size(other.x_size){
        for(size_type i = 0, end = x_size*y_size; i != end; ++i){
			data[i] = new T;
            *data[i] = *other.data[i];
	    }
    }
    Grid(Grid<T> && other):data(other.data),y_size(other.y_size),x_size(other.x_size){other.data = nullptr;other.y_size = 0;other.x_size = 0;}
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


    Row operator[](size_type y_idx){
        return Row(*this, y_idx);
    }
    const Row operator[](size_type y_idx)const{
        return Row(*this, y_idx);
    }
    /*+const*/


	size_type get_y_size() const{return y_size;}
	size_type get_x_size() const{return x_size;}
	void show() const{
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

#include <cassert> 

int main(){
	int* ar = new int[10]{1, 2, 3, 4, 5, 6, 7, 9, 8, 10};
    /*
	Grid<float> g(3, 2, 0.0f);
    assert(3 == g.get_y_size());
    assert(2 == g.get_x_size());
    using gsize_t = Grid<float>::size_type;
    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
        for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx) 
            assert(0.0f == g[y_idx][x_idx]);
    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
        for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
            g[y_idx][x_idx] = 1.0f;
    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
        for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx) 
            assert(1.0f == g[y_idx][x_idx]);
            */
    Grid<int> g1(ar, 2, 5);
    Grid<int> const g2 = Grid<int>(2, 5, 1);
    std::cout << g2[0][0] << " ";
    g2.show();
    return 0;
}