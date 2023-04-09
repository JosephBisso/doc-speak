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

        static void save_all(const std::filesystem::path& path);
    };

    static std::map<unsigned int, Serializable*> s_serializables;

}

#endif