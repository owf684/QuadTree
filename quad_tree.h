#include <memory>
#include <list>
#include <vector>
#include <iostream>
#include <iterator>

#define TOP_LEFT_QUAD 0
#define BOTTOM_LEFT_QUAD 1
#define TOP_RIGHT_QUAD 2
#define BOTTOM_RIGHT_QUAD 3

// book keeping for data getting added

template <typename T>
struct QTIL{
    typename std::list<T*>* item_list;
    typename std::list<T*>::iterator item_it;

    QTIL(){}
};

template <typename T>
struct QTI {
    T item;
    QTIL<T> location;

    QTI(){}

};

struct Point {
    float x;
    float y;

    Point(float x_in, float y_in) {x = x_in; y = y_in;}
    Point() { x = 0; y = 0; }
};


template <typename T>
class QuadTree {


    public:

        // Constructors
        QuadTree() // create unit quad tree
        {
            width = 1;
            height = 1;
            area = 1;
        }


        // points defined by user 
        QuadTree(Point top_left_in, Point bottom_right_in){
            top_left = top_left_in;
            bottom_right = bottom_right_in;
            width = bottom_right.x - top_left.x;
            height = bottom_right.y - top_left.y;
            area = width*height;
            
            // create entire quad tree
            if (area >=1 )
            {
    
                // Populate children with nulls
                for(int i = 0; i < 4; i++)
                {
                    children.push_back(NULL);
                }
                
                // calculate top left quadrant
                children[TOP_LEFT_QUAD] = new QuadTree( top_left, Point ( ( top_left.x + bottom_right.x ) / 2.0, (top_left.y + bottom_right.y) / 2.0) );  
    
                // calulate bottom left quadrant
                children[BOTTOM_LEFT_QUAD] = new QuadTree( Point(top_left.x, (top_left.y + bottom_right.y) / 2.0 ), Point( ( top_left.x + bottom_right.x ) / 2.0, bottom_right.y ) ); 
    
                // calculate bottom right quadrant
                children[BOTTOM_RIGHT_QUAD] = new QuadTree( Point( (top_left.x + bottom_right.x) / 2.0, (top_left.y + bottom_right.y ) /2.0 ), bottom_right ); 
        
                // calculate top right quadrant
                children[TOP_RIGHT_QUAD] = new QuadTree( Point( (top_left.x + bottom_right.x) / 2.0, top_left.y ) , Point( bottom_right.x, (top_left.y + bottom_right.y) / 2.0 ) );
 
            }

        }


        /*
        * Function: bool contains(T& item)
        *
        * Arguments: T& item
        *
        * Assumptions: This function assumes that argument item has the functions getX(), getY(), getWidth(), getHeight()
        * These get functions are necessary. Please update object and include these functions.
        *
        * Purpose: This functions determines whether object item can be contained within any given Quadrant. 
        * returns true if it can and false if it cant
        */
        bool contains(T& item)
        {
            return ( item.getX() >= top_left.x && item.getX() < bottom_right.x &&
                     item.getX() + item.getWidth() > top_left.x && item.getX() + item.getWidth() <= bottom_right.x &&

                     item.getY() >= top_left.y && item.getY() < bottom_right.y &&
                     item.getY() + item.getHeight() > top_left.y && item.getY() + item.getHeight() <= bottom_right.y );

        }

        /*
        * Function: void insert(T& item)
        *
        * Arguments: T& item
        *
        * Purpose: This inserts item into 
        * the smallest quadrant available
        */
        void insert(T& item, QTI<T>& item_qti)
        {
            for (int i = 0; i < 4; i++) 
            {
            
                if (children[i]->contains(item) ) 
                {
                    
                    children[i]->insert(item,item_qti);
                    return;

                } else {

                    item_qti.location.item_list = &items;
                    items.push_back(&item);
                    item_qti.location.item_it = --(items.end());
                    return;
                }

            }
        }

        /*
        * Function: void search(T& item, std::list<T*>& item_list)
        *
        * Arguments: T& item, std::list<T*> &item_list
        *
        * Purpose: This function recurses through the quad
        * tree and finds the item by locating the smallest
        * possible quadrant it can fit into. The items list of
        * given quadrant is then inserted into list item_list
        */
        void search(T& item, std::list<T*>& item_list)
        {
            for (int i = 0; i < 4; i++) 
            {
            
                if (children[i]->contains(item) ) 
                {
                    
                    children[i]->search(item, item_list);
                    return;

                } else {
            
                    if (items.size() > 0){
                    item_list.insert(item_list.end(), items.begin(), items.end());
                    }
                    return;
                }

            }
            
        }

        /*
        * Function: void search_nearby(std::list<T*>& item_list
        *
        * Arguments: std::list<T*>& item_list
        *
        * Purpose: This function searches the children and the childrens
        * children for items list larger than 0. These lists are then
        * inserted into list item_list
        * 
        * Note: might not be needed... foolish dumb ass
        */
        void search_nearby(std::list<T*>& item_list) 
        {
            for ( int i = 0; i < 4; i ++ )
            {
                if ( children[i]->items.size() > 0 ) {
                    item_list.insert(item_list.end(),children[i]->items.begin(), children[i]->items.end());
                    children[i]->search_nearby(item_list);
                    return;
                }
            }

        }


        /*
        * Function: void clear()
        *
        * Purpose: This functions recursively clears the item list
        * of the parent and all of the children quads till there is 
        * nothing left   
        */
        void clear()
        {  
            if (area >= 1) 
            {
                items.clear();
            
                for(int i = 0; i < 4; ++i)
                {
                    children[i]->clear();
                }
            }
        }

    protected:
        float width;
        float height;
        float area; 
        Point top_left;
        Point bottom_right;
        std::vector<QuadTree*> children;
        std::list<T*>items;
};





// Wrapper for the QuadTree
template <typename T>
class QuadTreeContainer {


    public:
    QuadTree<T> root;
    std::list< QTI<T> > items;

    QuadTreeContainer(Point top_left, Point bottom_right) : root(top_left, bottom_right)
    {
    }


    void insert(T& item)
    {
        QTI<T> new_item;
        root.insert(item,new_item);
        new_item.item = item;
        items.push_back(new_item);

    }

    void remove(typename std::list<QTI<T> >::iterator& item_qti)
    {
        // remove from quad tree
        (*item_qti).location.item_list->erase((*item_qti).location.item_it);

        // remove from user list
        items.erase(item_qti);

    }

    void reinsert(typename std::list<QTI<T> >::iterator& item_qti)
    {
        // remove from quad tree
        (*item_qti).location.item_list->erase((*item_qti).location.item_it);
        QTI<T> old_item;
        root.insert((*item_qti).item,old_item);
        (*item_qti).location = old_item.location;

    }
    std::list<T*> search(T& item)
    {
        std::list<T*> items_found;
        root.search(item, items_found);
        return items_found;
    }

    std::list<T*> search(typename std::list<QTI<T> >::iterator& item_qti)
    {
        std::list<T*> items_found;
        root.search((*item_qti).item, items_found);
        return items_found;
    }

    int size()
    {
        return items.size();
    }

    typename std::list<QTI<T> >::iterator begin()
    {
        return items.begin();
    }

    typename std::list<QTI<T> >::iterator end()
    {   
        return items.end();
    }
    typename std::list<QTI<T> >::iterator front()
    {
        return items.front();
    }
    typename std::list<QTI<T> >::iterator back()
    {
        return items.back();
    }

    void pop_back()
    {
        auto item_qti = std::prev(items.end());
        (*item_qti).location.item_list->erase((*item_qti).location.item_it);
        items.pop_back();
    }

    void pop_front()
    {
        auto item_qti = items.begin();
        (*item_qti).location.item_list->erase((*item_qti).location.item_it);        
        items.pop_front();
    }
};




