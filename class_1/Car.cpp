#include "Car.h"
#include <iostream>

Car::Car()
{
    std::cout << "Car()" << std::endl;
}

Car::Car(float amount)
{
    fuel = amount;
}

Car::~Car()
{
    std::cout << "~Car()" << std::endl;
}

void Car::FillFuel(float amount)
{
    fuel = amount;
}

void Car::Accellerate()
{
    speed++;
    fuel -= 0.05f;
}

void Car::Brake()
{
    speed = 0;
}

void Car::AddPassengers(int count)
{
    passengers = count;
}

void Car::Dashboard()
{
    std::cout << "Fuel " << fuel << std::endl;
    std::cout << "Speed " << speed << std::endl;
    std::cout << "Passengers " << passengers << std::endl;

}
