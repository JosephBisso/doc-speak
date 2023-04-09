#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <string>
#include <chrono>
#include <map>
#include <vector>
#include <iostream>
#include <memory>

namespace docspeak {
    class Serializable
    {
    private:
        unsigned int m_id;
        static int number_of_elements;
    public:
        Serializable();
        ~Serializable();

        inline unsigned int get_id() {return m_id;} 

        virtual void save() = 0;
        virtual void load() = 0;
    };
}

#endif