#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "transcripter.h"
#include "printer.h"

#include <filesystem>
#include <map>

namespace docspeak
{
    struct TemplateElement {
        enum Type {Text, Image, PageDimension};
        TemplateElement(int id, size_t x, size_t y, std::string text, size_t size, Type type = Type::Text, size_t heigth = 100, long int color = 0x0):
        x(x), y(y), width_or_size(size), heigth(heigth), text_or_path(text),  element_id(id), type(type), color(color)
        {}
        TemplateElement(size_t width, size_t heigth):
        width_or_size(width), heigth(heigth), element_id(0), type(PageDimension)
        {}
        TemplateElement(){}
        size_t x, y, width_or_size, heigth;
        std::string text_or_path;
        long int color;
        int element_id;
        Type type = Type::Text;
    };

    template<typename T>
    class Protocol
    {
    private:
        std::shared_ptr<Printer> m_printer;
        std::shared_ptr<Transcripter> m_transcripter;
        std::filesystem::path m_output_folder;
        std::filesystem::path m_template_pdf_path;
        size_t m_current_template_index = -1;

    public:
        static std::filesystem::path s_assets_path;
        static std::filesystem::path s_font_path;
        static size_t s_current_template_index;
        static std::filesystem::path s_output_folder;
        static std::filesystem::path s_template_pdf_path;
        
        typedef std::map<std::string, TemplateElement> Template;
        static std::vector<Template> s_templates;
    
    private:
        void __init() {
            auto template_pdf_path = get_template_pdf_path();      
            if(template_pdf_path.empty() && s_font_path.empty())
                return;
                
            auto input_path_set = m_printer -> set_input_path(template_pdf_path);
            auto font_set = m_printer -> set_font_path(s_font_path);

            if (!(input_path_set.success && font_set.success)) {
                PLOGW << "Cannot create Protocol. Template path or Font path invalid! Check the given path";
                throw std::invalid_argument("Cannot create Protocol. Template path or Font path invalid! Check the given path");
            }
        }

    public:
        Protocol(): m_printer(new Printer), m_transcripter(new Transcripter) {
            __init();
        }

        Protocol(const std::filesystem::path& template_pdf_path,const std::filesystem::path& font_path):
            m_template_pdf_path(template_pdf_path), s_font_path(font_path), m_printer(new Printer), m_transcripter(new Transcripter)
        {
            __init();            
        }

        ~Protocol() {}

        StatusInfo print() {return m_printer.print();};
        StatusInfo transcript() {return m_transcripter.transcript();}
        std::string to_string() {return "Empty Protocoll";};

        constexpr std::shared_ptr<Printer> get_printer() {return m_printer;}
        constexpr std::shared_ptr<Transcripter> get_transcripter() {return m_transcripter;}


        inline std::filesystem::path& get_output_folder() {return m_output_folder.empty() ? s_output_folder : m_output_folder;}
        void set_specific_output_folder(const std::filesystem::path& output_folder) {
            if (output_folder.empty())
                return;

            if (std::filesystem::exists(output_folder)) {
                PLOGI << std::format("Setting output folder to {}", output_folder.string());
                m_output_folder = output_folder;
                return;
            } 

            std::error_code ec;
            auto created = std::filesystem::create_directory(output_folder, ec);
            if (created) {
                PLOGI << std::format("Setting output folder to {}", output_folder.string());
                m_output_folder = output_folder;
            }
            else {
                PLOGW << std::format("Cannot create folder at path {}. Error: {}", output_folder.string(), ec.message());
                throw std::invalid_argument(ec.message());
            }
        }

        static void set_output_folder(const std::filesystem::path& output_folder) {
            if (output_folder.empty())
                return;

            if (std::filesystem::exists(output_folder)) {
                PLOGI << std::format("Setting output folder to {}", output_folder.string());
                s_output_folder = output_folder;
                return;
            } 

            std::error_code ec;
            auto created = std::filesystem::create_directory(output_folder, ec);
            if (created) {
                PLOGI << std::format("Setting output folder to {}", output_folder.string());
                s_output_folder = output_folder;
            }
            else {
                PLOGW << std::format("Cannot create folder at path {}. Error: {}", output_folder.string(), ec.message());
                throw std::invalid_argument(ec.message());
            } 
        }

        static std::pair<size_t, Template&> new_template(size_t index_template_to_copy = 0){
            size_t index = index_template_to_copy;
            if (index >= s_templates.size())
                index = 0;

            s_templates.push_back(std::map (s_templates.at(index)));
            
            auto new_template_index = s_templates.size() -1;
            s_current_template_index = new_template_index;
            
            PLOGV << std::format("New Template at index {}", new_template_index);
            return { new_template_index, s_templates.at(new_template_index)};
        }

        inline bool set_current_template_index(size_t template_index) {
            if (template_index >= s_templates.size())
                return false;

            m_current_template_index = template_index;
            return true;        
        }

        constexpr Template get_current_template()  {return s_templates.at(get_current_template_index());}
        constexpr size_t get_current_template_index()  {return m_current_template_index = -1 ? s_current_template_index : m_current_template_index;}

        void set_template_pdf_path(const std::filesystem::path template_pdf_path) {
            if (auto result = m_printer -> set_input_path(template_pdf_path); result.success)
                m_template_pdf_path = template_pdf_path;
            else 
                throw std::invalid_argument(result.error_message);
        }
        inline std::filesystem::path get_template_pdf_path() {return m_template_pdf_path.empty() ? s_template_pdf_path : m_template_pdf_path;}

        void set_font_path(const std::filesystem::path font_path) {
            if (auto result = m_printer -> set_font_path(font_path); result.success)
                s_font_path = font_path;
            else 
                throw std::invalid_argument(result.error_message);
        }

    };

    template<typename T> 
    std::filesystem::path Protocol<T>::s_template_pdf_path;

    template<typename T> 
    std::filesystem::path Protocol<T>::s_output_folder;

    template<typename T> 
    std::filesystem::path Protocol<T>::s_assets_path;

    template<typename T> 
    std::filesystem::path Protocol<T>::s_font_path;

    template<typename T> 
    size_t Protocol<T>::s_current_template_index = 0;


} // namespace docspeak


#endif