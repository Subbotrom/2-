#include <random>
#include <iostream>

#define M 6
#define N 1000000

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

class PenaltyDice: public Dice{
public:
    PenaltyDice(unsigned max, unsigned seed):Dice(max, seed){}

    unsigned roll() override{
        unsigned a = dstr(reng), b = dstr(reng);
        return (a<b?a:b);
    }
};

class BonusDice: public Dice{
public:
    BonusDice(unsigned max, unsigned seed):Dice(max, seed){}

    unsigned roll() override{
        unsigned a = dstr(reng), b = dstr(reng);
        return (a>b?a:b);
    }
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

double value_probability(unsigned value, ADice &d, unsigned number_of_rolls = 1){
    double count = 0;
    for(unsigned cnt = 0; cnt != number_of_rolls; ++cnt){ 
        count += d.roll()==value;
    }
    return count / static_cast<double>(number_of_rolls);
}

int main(){
    ThreeDicePool* cube = new ThreeDicePool(M, 10, 150, 209);
    for(unsigned i = 1; i <= 18; i++){
        std::cout << value_probability(i, *cube, N) << std::endl;
        delete cube;
        cube = new ThreeDicePool(M, 10, 150, 209);
    }
    delete cube;
}