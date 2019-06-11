#include "context.h"

bool context::increase_lineSize(){

    if(this->lineSize<2){
        this->lineSize=this->lineSize+1;
        return true;
    }
    return false;
}

bool context::decrease_lineSize(){
    if(this->lineSize>1){
        this->lineSize=this->lineSize-1;
        return true;
    }
    return false;
}