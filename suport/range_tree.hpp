// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    07.11.2012 10:14:46 CET
// File:    range_tree.hpp

#ifndef __RANGE_TREE_HEADER
#define __RANGE_TREE_HEADER

//------------------- how to use -------------------
/* This header can be used with the arduino or in normal c++
 * 
 * One can define GCC in order to get the debug-output for normal c++ or
 * one can define SERIALCOM to get the debug-output via Serial communication
 * If none of them are defined, there will be no debug-output (i.e. Release-Version)
 * 
 * 
 * 
 * 
 */


#ifdef GCC
    #include <iostream>
#else
    #include "Arduino.h"
    #include "sensor.hpp"
#endif

//PRINT_OK is defined, if either GCC or SERIALCOM is defined
//if PRINT_OK isn't defined, no print-fct will be compiled
#ifdef GCC
    #define PRINT_OK
#else
    #ifdef SERIALCOM
        #define PRINT_OK
    #endif
#endif
typedef unsigned int uint;

namespace detail
{
    typedef uint set_type;

    //  +---------------------------------------------------+
    //  |                   INTERVALL_CLASS                 |
    //  +---------------------------------------------------+
    template<typename T>
    class intervall_class
    {
    public:
        intervall_class(T const & l, T const & h): tag(0), low_(l), high_(h), id_(k)
        {
        }
        static void reset_k()
        {
            k = 1;  // 1 = 2^0
        }
        static void incr_k()
        {
            k *= 2;
        }
        #ifdef PRINT_OK
            void print()
            {
                #ifdef GCC
                    std::cout << "Intervall: " << id_ << " (" << low_ << "/" << high_ << ") " << std::endl;
                #endif
                #ifdef SERIALCOM
                    Serial.print("Intervall: ");
                    Serial.print(id_);
                    Serial.print(" (");
                    Serial.print(low_);
                    Serial.print("/");
                    Serial.print(high_);
                    Serial.println(") ");
                #endif
            }
        #endif
        inline T const & low() //return lower bound
        {
            return low_;
        }
        inline T const & high() //return upper bound
        {
            return high_;
        }
        inline set_type const & id() //return id
        {
            return id_;
        }
        T tag; //each intervall gets the tag of the node it will be attached to.
    private:
        static set_type k;  //can be incremented with member-functions. If the ctor is called, id_ will be set to k
        T const low_;       //lower bound of the intervall
        T const high_;      //upper bound of the intervall
        set_type id_;       //identity of the intervall (a power of 2)
    };
    //------------------- operators to intervall -------------------
    template<typename T>
    bool low_bigger(intervall_class<T> * const a, intervall_class<T> * const b)
    {//compares lower bounds
        return a->low() > b->low();
    }
    template<typename T>
    bool high_bigger(intervall_class<T> * const a, intervall_class<T> * const b)
    {//compares upper bounds
        return a->high() < b->high();
    }
    template<typename T>
    void swap(intervall_class<T> * & a, intervall_class<T> * & b)
    {//only swap pointers, not actual class (therefor no copy-ctor in intervall_class)
        intervall_class<T> * temp(a);
        a = b;
        b = temp;
    }
    
    template<typename T>
    set_type intervall_class<T>::k = 1; //initialize k to 1 = 2^0
    
    
    //  +---------------------------------------------------+
    //  |                   TREE_NODE_CLASS                 |
    //  +---------------------------------------------------+
    template<typename T>
    struct tree_node_class
    {
        tree_node_class(T const & v): big(NULL), small(NULL), val(v), start(0), end(0), count(0)
        {}
        void deconstruct(tree_node_class<T> * & node) //deconstruct tree
        {//recursive deconstruction of all nodes below *this
            if(node->small != NULL)
                deconstruct(node->small);
            if(node->big != NULL)
                deconstruct(node->big);
                
            delete node; //all nodes are allocaded during runtime, therefor the delete
            node = NULL; //reset pointer
            
        }
        ~tree_node_class()
        {//call all deconstruct functions for nodes below
            if(small != NULL)
                deconstruct(small);
            if(big != NULL)
                deconstruct(big);
        }
        #ifdef PRINT_OK
            void print() const
            {
                static int level = 0;
                ++level;
                if(small != NULL)
                    small->print();
                --level;
                #ifdef GCC
                    std::cout << val << " on lv: " << level << " count: " << count << " start: " << start << std::endl;
                #endif
                #ifdef SERIALCOM
                    Serial.print(val);
                    Serial.print(" on lv: ");
                    Serial.print(level);
                    Serial.print(" count: ");
                    Serial.print(count);
                    Serial.print(" start: ");
                    Serial.println(start);
                    
                #endif
                
                ++level;
                if(big != NULL)
                    big->print();
                --level;
            }
        #endif
        tree_node_class * big;      //node from this pointer are all bigger then *this
        tree_node_class * small;    //node from this pointer are all smaller then *this
        T const val;                //value of the node (intervall-border)
        uint start;
        uint end;
        uint count;
    };
    
}

template<typename T>
void swap(T & a, T & b)
{//requires copy-ctor
    T temp(a);
    a = b;
    b = temp;
}
//  +---------------------------------------------------+
//  |                   RANGE_TREE_CLASS                |
//  +---------------------------------------------------+
template<typename T>
class range_tree_class
{
public:
    range_tree_class(): count(0), count2(0), master(0)
    {
        for(uint i = 0; i < max; ++i)
        {
            iv[i] = NULL;
            small[i] = NULL;
            big[i] = NULL;
        }
    }
    ~range_tree_class()
    {
        for(uint i = 0; i < max; ++i)
        {
            delete iv[i];
        }
        if(master != NULL)
            delete master;
    }
    void insert(T const & low, T const & high)
    {
        iv[count] = new detail::intervall_class<T>(low, high);
        ++count;

        node_val[count2] = low;
        node_val[count2 + 1] = high;
        count2 += 2;
        
        #ifdef GCC
        if(count == max)
            std::cout << "max in range-tree" << std::endl;
        #endif
        #ifdef SERIALCOM
        if(count == max)
            Serial.println("max in range-tree");
        #endif
    }
    #ifdef PRINT_OK
        void print() const
        {
            #ifdef GCC
                std::cout << "Range Tree" << std::endl;
                std::cout << "==========" << std::endl;
                uint i = 0;
                while(iv[i] != NULL and i != max)
                {
                    iv[i]->print();
                    ++i;
                }
                i = 0;
                while(i < count2)
                {
                    std::cout << node_val[i] << " ";
                    ++i;
                }
                std::cout << std::endl;
            #endif
            #ifdef SERIALCOM
                Serial.println("Range Tree");
                Serial.println("==========");
                uint i = 0;
                while(iv[i] != NULL and i != max)
                {
                    iv[i]->print();
                    ++i;
                }
                i = 0;
                while(i < count2)
                {
                    Serial.print(node_val[i]);
                    Serial.print(" ");
                    ++i;
                }
                Serial.println();
            #endif
        }
    #endif
    void create(uint pos1, uint pos2, detail::tree_node_class<T> * & node)
    {
        if(pos1 == pos2)
            return;
        
        uint pos = (pos2 + pos1)/2;

        node = new detail::tree_node_class<T>(node_val[pos]);
        if(pos1 != pos)
        {
            create(pos1, pos, node->small);
            create(pos+1, pos2, node->big);
        }
    }
    void incr(uint const & i, detail::tree_node_class<T> * const node)
    {
        if(iv[i]->low() <= node->val and iv[i]->high() >= node->val)
        {
            ++(node->count);
            iv[i]->tag = node->val;
        }
        else
        {
            if(iv[i]->low() > node->val)
                incr(i, node->big);
            if(iv[i]->high() < node->val)
                incr(i, node->small);
        }
    }
    void insert_intervalls_low(uint & s, detail::tree_node_class<T> * & node)
    {
        
        if(node->count != 0)
        {
            node->start = s;
            
            for(uint i = 0; i < count; ++i)
            {
                if(iv[i]->tag == node->val)
                {
                    small[s] = iv[i];
                    ++s;
                }
            }
            
            node->end = s;
        }
        
        if(node->small != NULL)
            insert_intervalls_low(s, node->small);
        
        if(node->big != NULL)
            insert_intervalls_low(s, node->big);
        
    }
    void insert_intervalls_high(uint & s, detail::tree_node_class<T> * & node)
    {
        
        if(node->count != 0)
        {
            node->start = s;
            
            for(uint i = 0; i < count; ++i)
            {
                if(iv[i]->tag == node->val)
                {
                    big[s] = iv[i];
                    ++s;
                }
            }
            
            node->end = s;
        }
        
        if(node->small != NULL)
            insert_intervalls_high(s, node->small);
        
        if(node->big != NULL)
            insert_intervalls_high(s, node->big);
        
    }
    void build_tree()
    {
        //sort intervalls low O(N^2) (i don't need O(N log(N)) here)
        for(uint i = 0; i < count; ++i)
        {
            for(uint j = i+1; j < count; ++j)
            {
                if(detail::low_bigger<T>(iv[i], iv[j]))
                    detail::swap<T>(iv[i], iv[j]);
            }
        }
        //------------------- sort nodes and make unique -------------------
        for(uint i = 0; i < 2*count; ++i)
        {
            for(uint j = i+1; j < 2*count; ++j)
            {
                if(node_val[i] == node_val[j])
                {
                    if(node_val[j] != 0)
                    {
                        node_val[j] = 0;
                        --count2;
                    }
                }
                if(node_val[i] < node_val[j])
                    swap<T>(node_val[i], node_val[j]);
            }
        }
        //------------------- reverse order in node_val -------------------
        for(uint i = 0; i < count2/2; ++i)
        {
            swap<T>(node_val[i], node_val[count2 - 1 - i]);
        }
        //------------------- build tree (recursion)-------------------
        create(0, count2, master);
        
        //------------------- tagging the intervalls with the nodes -------------------
        for(uint i = 0; i < count; ++i)
        {
            incr(i, master); //increment counter for later new [counter]
        }
        
        //------------------- insert intervalls-low -------------------
        uint s = 0;
        insert_intervalls_low(s, master);
        
        //------------------- sort intervalls high O(N^2) -------------------
        for(uint i = 0; i < count; ++i)
        {
            for(uint j = i+1; j < count; ++j)
            {
                if(detail::high_bigger<T>(iv[i], iv[j]))
                    detail::swap<T>(iv[i], iv[j]);
            }
        }
        
        //------------------- insert intervalls-high -------------------
        s = 0;
        insert_intervalls_high(s, master);
        
    }
    void search(T const & p, detail::set_type & res, detail::tree_node_class<T> * node) const
    {
        if(p > node->val)
        {
            for(uint i = node->start; i < node->end; ++i)
            {
                if(p <= big[i]->high())
                    res |= big[i]->id();
                else
                    i = node->end;
            }
            if(node->big != NULL)
                search(p, res, node->big);
        }
        else
            if(p < node->val)
            {
                for(uint i = node->start; i < node->end; ++i)
                {
                    if(p >= small[i]->low())
                        res |= small[i]->id();
                    else
                        i = node->end;
                }
                if(node->small != NULL)
                    search(p, res, node->small);
            }
            else // p == node->val
            {
                for(uint i = node->start; i < node->end; ++i)
                {
                    res |= small[i]->id(); //big also possible
                }
            }
    }
    detail::set_type query(T const & p) const //gives back the intervalls, that contain p
    {
        detail::set_type res = 0;
        search(p, res, master);
        return res;
    }
private:
    static const uint max = 20;
    detail::intervall_class<T>* iv[max];
    detail::intervall_class<T>* small[max];
    detail::intervall_class<T>* big[max];
    uint count;
    
    T node_val[2*max];
    uint count2;
    
    detail::tree_node_class<T> * master;
};


//  +---------------------------------------------------+
//  |                   VECTOR_TREE_CLASS               |
//  +---------------------------------------------------+
template<typename T, int N>
class vector_tree_class
{
public:
    vector_tree_class()
    {
        detail::intervall_class<T>::reset_k();
    }
    void insert(T low[N], T high[N])
    {
        for(uint i = 0; i < N; ++i)
        {
            if(low[i] <= high[i])
                tree[i].insert(low[i], high[i]);
            else
                tree[i].insert(high[i], low[i]);
        }
        detail::intervall_class<T>::incr_k();
    }
    void build_tree()
    {
        for(uint i = 0; i < N; ++i)
        {
            tree[i].build_tree();
        }
    }
    void print() const
    {
        #ifdef GCC
            std::cout << "Vector Tree" << std::endl;
            std::cout << "===========" << std::endl;
        #endif
        #ifdef SERIALCOM
            Serial.println("Vector Tree");
            Serial.println("===========");
        #endif
        for(uint i = 0; i < N; ++i)
        {
            #ifdef GCC
                std::cout << "sub-tree nr: " << i << std::endl;
            #endif
            #ifdef SERIALCOM
                Serial.print("sub-tree nr: ");
                Serial.println(i);
            #endif
            tree[i].print();
        }
    }
    detail::set_type query(T p[N]) const
    {
        detail::set_type res = tree[0].query(p[0]);
        for(uint i = 1; i < N; ++i)
        {
            res &= tree[i].query(p[i]);
        }
        return res;
    }
    #ifdef GCC
    #else 
    //case Arduino
    detail::set_type query(sensor_class const & p) const
    {
        detail::set_type res = tree[0].query(p[0]);
        for(uint i = 1; i < N; ++i)
        {
            res &= tree[i].query(p[i]);
        }
        return res;
    }
    #endif
private:
    range_tree_class<T> tree[N];
    
};



#endif //__RANGE_TREE_HEADER
