#include <random>
#include <iostream>

#define M 10
#define N 2000

class ADice {
public:
    virtual unsigned roll() = 0;
    virtual ~ADice(){}
};

class Dice: public ADice{
public:
    Dice(unsigned max, unsigned seed): dstr(1, max), reng(seed){}

    unsigned roll() override{
        return dstr(reng);
    }
protected:
    std::uniform_int_distribution<unsigned> dstr;
    std::default_random_engine reng;
};

class ThreeDicePool: public ADice{
public:
    ThreeDicePool(unsigned max, unsigned seed_1, unsigned seed_2, 
        unsigned seed_3): d1(new Dice(max, seed_1)), 
        d2(new Dice(max, seed_2)), d3(new Dice(max, seed_3)){}
    ThreeDicePool(ADice* d_1, ADice* d_2, ADice* d_3):d1(d_1),d2(d_2),d3(d_3){}
    unsigned roll() override{
        return d1->roll()+d2->roll()+d3->roll();
    }
    ~ThreeDicePool() override{
        delete d1;
        delete d2;
        delete d3;
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
    Dice* cube_1 = new Dice(M, 1), * cube_2 = new Dice(M, 2), * cube_3 = new Dice(M, 3);
    ThreeDicePool* super_cube_1 = new ThreeDicePool(cube_1, cube_2, cube_3), * super_cube_2 = new ThreeDicePool(M, 1, 2, 3);
    std::cout << expected_value(*super_cube_1, N) << std::endl;
    std::cout << expected_value(*super_cube_2, N) << std::endl;
    delete &super_cube_1;
    delete &super_cube_2;
    return 0;
}