#include <random>
#include <iostream>

#define M 10
#define N 200

class ADice {
public:
    virtual unsigned roll() = 0;
};

class Dice: public ADice{
public:
    Dice(unsigned max, unsigned seed): dstr(1, max), reng(seed), max(max){}

    unsigned roll() override{
        return dstr(reng);
    }
protected:
    unsigned max;
    std::uniform_int_distribution<unsigned> dstr;
    std::default_random_engine reng;
};

class ThreeDicePool: public ADice{
public:
    ThreeDicePool(unsigned max, unsigned seed_1, unsigned seed_2, 
        unsigned seed_3): d1(new Dice(max, seed_1)), 
        d2(new Dice(max, seed_2)), d3(new Dice(max, seed_3)){}
    unsigned roll() override{
        return d1->roll()+d2->roll()+d3->roll();
    }
private:
    ADice* d1;
    ADice* d2;
    ADice* d3;
};

double expected_value(ADice & d, unsigned number_of_rolls = 1){
    auto accum = 0llu;
    for(unsigned cnt = 0; cnt != number_of_rolls; ++cnt){ 
        accum += d.roll();
    }
    return static_cast<double>(accum) / static_cast<double>(number_of_rolls); 
}

int main(){
    Dice cube(M, 1);
    ThreeDicePool super_cube(M, 2, 3, 4);
    std::cout << expected_value(cube, N) << std::endl;
    std::cout << expected_value(super_cube, N) << std::endl;
    return 0;
}