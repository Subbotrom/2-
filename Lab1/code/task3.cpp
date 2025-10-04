#include <random>
#include <iostream>

#define M 100
#define N 50000

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

class PenaltyDice: virtual public Dice{
public:
    PenaltyDice(unsigned max, unsigned seed):Dice(max, seed){};
    PenaltyDice(Dice& d):Dice(d){};
    unsigned roll() override{
        unsigned a = dstr(reng), b = dstr(reng);
        return (a<b?a:b);
    }
};

class BonusDice: virtual public Dice{
public:
    BonusDice(unsigned max, unsigned seed):Dice(max, seed){};
    BonusDice(Dice& d):Dice(d){};
    unsigned roll() override{
        unsigned a = dstr(reng), b = dstr(reng);
        return (a>b?a:b);
    }
};

class ThreeDicePool: public ADice{
public:
    ThreeDicePool(ADice* d_1, ADice* d_2, ADice* d_3):d1(d_1),d2(d_2),d3(d_3){}
    unsigned roll() override{
        return d1->roll()+d2->roll()+d3->roll();
    }
    ~ThreeDicePool(){}
private:
    ADice* d1;
    ADice* d2;
    ADice* d3;
};

class DoubleDice: public PenaltyDice, public BonusDice{
public:
    DoubleDice(Dice& d):Dice(d), PenaltyDice(d), BonusDice(d){}
    unsigned roll() override{
        return PenaltyDice::roll()+BonusDice::roll();
    }
};

class DDice: virtual public Dice{
public:
    DDice(unsigned max, unsigned seed):Dice(max, seed){};
    DDice(Dice& d):Dice(d){};
    unsigned roll() override{
        unsigned a, b, s = 0;
        a = dstr(reng);
        b = dstr(reng);
        s += (a>b?a:b);
        a = dstr(reng);
        b = dstr(reng);
        s += (a<b?a:b);
        return s;
    }
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
    Dice cube(M, 45);
    DDice d_cube(cube);
    std::cout << expected_value(d_cube, N);
    return 0;
}