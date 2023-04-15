#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <nlohmann/json.hpp>

#include <string>
#include <chrono>
#include <map>
#include <vector>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <format>
#include <ranges>
#include <filesystem>
#include <fstream>

using json = nlohmann::json;

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

        virtual void save(const std::filesystem::path& path) = 0;
        virtual void load(const std::filesystem::path& path) = 0;

    };

    template<typename T>
    class Book: public Serializable
    {
    private:        

        std::vector<std::shared_ptr<T>> m_elements;

        static std::shared_ptr<Book<T>> s_instance;
    public:
        Book(){};
        ~Book() {};

        void save(const std::filesystem::path&) override;
        void load(const std::filesystem::path&) override;

        static std::weak_ptr<Book<T>> get_instance() {return s_instance;}

        static std::weak_ptr<Book<T>> get_book();
        static void init_book();
        static void clear_book();

        void add(std::shared_ptr<T> element);

        static std::vector<std::shared_ptr<T>>& get_all();
        static std::vector<std::weak_ptr<T>> find(const T& to_find);
        static void clear();
        static size_t size();
    };
    
}


#include "serializable.impl"
#endif