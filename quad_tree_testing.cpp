#include "quad_tree.h"
#include <iostream>


struct Item {
    float x;
    float y;
    float width;
    float height;


    Item(float x_in, float y_in, float width_in, float height_in) {
        x = x_in;
        y = y_in;
        width = width_in;
        height = height_in;
    }
    Item(){ x= 1; y = 1; width = 1; height = 1;}
    float getX(){
        return x;
    }

    float getY(){
        return y;
    }

    float getWidth(){
        return width;
    }

    float getHeight(){
        return height;
    }
};



int main() {

    Item new_item1(3,3,2,2);
    Item new_item2(2,3,2,3);
    Item new_item3(3,1,3,3);

    QuadTreeContainer<Item> new_tree(Point(0,0), Point(16,16));

    new_tree.insert(new_item1);
    new_tree.insert(new_item2);
    new_tree.insert(new_item3);

    //new_tree.pop_front();

    std::list<QTI<Item> >::iterator first_item = new_tree.begin();

    //new_tree.remove(last_item);

    std::list<Item*> item_list;

    item_list = new_tree.search(first_item);
    for(auto it = item_list.begin(); it != item_list.end(); it++)
    {
        std::cout << "x: " << (*it)->x << " y: " << (*it)->y << std::endl;
    }

    /*first_item->item.x = 4;
    first_item->item.y = 4;;
    new_tree.reinsert(first_item);

    item_list.clear();
    item_list = new_tree.search(first_item->item);

    for(auto it = item_list.begin(); it != item_list.end(); it++)
    {
        std::cout << "x: " << (*it)->x << " y: " << (*it)->y << std::endl;
    }*/
    

    //item_list = new_tree.search(new_item3);

    //std::cout<< "item_list size= " << item_list.size() << std::endl;

    return 0;
}


