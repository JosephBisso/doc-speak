#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <nlohmann/json.hpp>
#include <plog/Log.h>

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
    public:
        virtual void save(const std::filesystem::path& path) = 0;
        virtual void load(const std::filesystem::path& path) = 0;

    };

    template <typename T>
    class Item {
        private:
            unsigned int m_id = 0;
            static size_t s_number_of_items;
        public:
            Item(){m_id = ++s_number_of_items; PLOGV << std::format("New Item. id = {}", m_id);}

            inline void set_id(unsigned int id) {m_id = id;}
            inline unsigned int get_id() const {return m_id;}
            static inline size_t get_number_of_items() {return s_number_of_items;}

            virtual bool is_like(const T& other_book_item) const = 0;
            virtual bool equals(const T& other_book_item) const {return m_id == other_book_item.m_id;}

    };

    template<class T, class U>
    concept BookItem = std::is_base_of<Item<T>, T>::value || std::is_base_of<Item<U>, T>::value; 

    template<class T, class U = T>
    requires BookItem<T, U>
    class Book: public Serializable
    {
    private:        

        std::vector<std::shared_ptr<T>> m_elements;

        static std::unique_ptr<Book<T, U>> s_instance;
    public:
        Book(){};
        ~Book() {};

        void save(const std::filesystem::path&) override {

        }

        void load(const std::filesystem::path&) override {

        }

        // static std::weak_ptr<Book<T, U>> get_instance() {return s_instance;}

        // static std::weak_ptr<Book<T, U>> get_book() {
        //     if (!s_instance) 
        //         s_instance = std::shared_ptr<Book>(new Book<T,U>);
            
        //     return s_instance;
        // }

        static void init_book() {
            if (!s_instance) 
                s_instance = std::unique_ptr<Book>(new Book<T,U>);
        }

        static void clear_book() {
            if (!s_instance) 
                return;
            
            s_instance->clear();
        }

        static void add(std::shared_ptr<T> element);

        static std::vector<std::shared_ptr<T>>& get_all() {
            if (!s_instance) 
                init_book();
            
            return s_instance->m_elements;
        }

        static std::vector<std::weak_ptr<T>> find(const T& to_find) {
            std::vector<std::weak_ptr<T>> results;
            auto all_elements = get_all();

            std::copy_if(all_elements.begin(), all_elements.end(),
                        std::back_inserter(results), [&to_find] (auto element){ 
                    return element -> is_like(to_find);
            });

            return results;
        }

        static std::vector<std::weak_ptr<T>> find(unsigned int id) {
            std::vector<std::weak_ptr<T>> results;
            auto all_elements = get_all();

            std::copy_if(all_elements.begin(), all_elements.end(),
                        std::back_inserter(results), [&id] (auto element){ 
                    
                    auto e_id = element -> get_id(); 
                    return e_id == id || std::to_string(e_id).find(std::to_string(id)) != std::string::npos;
            });

            return results;
        }

        static void clear() {
            if (!s_instance) 
                return;
            
            s_instance->m_elements.clear();
        }

        static size_t size() {
            if (!s_instance) 
                return 0;
            
            return s_instance->m_elements.size();
        }
    };
}

#include "serializable.hpp"
#endif