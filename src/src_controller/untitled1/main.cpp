#include <iostream>
#include "TMC5160.h"
int main() {
    std::cout << "Hello, World!" << std::endl;

    TMC5160 motor =  new TMC5160();


    motor.enable_motor()


    int i = 0;
    int target = 25600;
    while(1){
        target = 2560;
        motor.go_to(target);
        i = 0;
        while(motor.get_position() != target){
            std::cout << motor.get_position() << " " << i << std::endl;
            i++;
        }

        target = 0;
        motor.go_to(target);
        i = 0;
        while(motor.get_position() != target){
            std::cout << motor.get_position() << " " << i << std::endl;
            i++;
        }


    }

motor.disable_motor();


    return 0;
}
