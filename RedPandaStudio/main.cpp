#include <RedPanda.h>

int main() {

    EngineFacade ef;

    ef.setWidth(1280);
    ef.setHeight(720);

    ef.init();

    getchar();

    return 0;

}