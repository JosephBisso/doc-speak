#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "transcripter.h"
#include "printer.h"

#include <filesystem>
#include <map>

namespace docspeak
{
    template<typename T>
    class Protocol
    {
    private:
        std::shared_ptr<Printer> m_printer;
        std::shared_ptr<Transcripter> m_transcripter;
        std::filesystem::path m_output_folder;

    public:
        static std::filesystem::path s_template_pdf_path;
        static std::string s_current_template_name;
        static std::filesystem::path s_font_path;

        typedef std::map<std::string, Printer::Printable*> Template;

        static std::map<std::string, Template> s_templates;
    
    private:
        void __init() {            
            auto input_path_set = m_printer -> set_input_path(s_template_pdf_path);
            auto font_set = m_printer -> set_font_path(s_font_path);

            if (!(input_path_set.success && font_set.success)) 
                throw std::invalid_argument("Cannot create Protocol. Template path or Font path invalid! Check the given path");
        }

    public:
        Protocol(): m_printer(new Printer) {
            __init();
        }

        Protocol(const std::filesystem::path& template_pdf_path,const std::filesystem::path& font_path):
            s_template_pdf_path(template_pdf_path), s_font_path(font_path), m_printer(new Printer)
        {
            __init();            
        }

        ~Protocol() {}

        Printer::StatusInfo print() {m_printer.print();};
        std::string to_string() {return "Empty Protocoll";};

        inline void set_output_folder(const std::filesystem::path& output_folder) {m_output_folder = output_folder;}
        inline std::filesystem::path& get_output_folder() {return m_output_folder;}

        constexpr Template get_current_template()  {return s_templates.at(s_current_template_name);}
        Printer::Text& get_text_template_element(std::string element) {
            auto current_template = s_templates.at(s_current_template_name);
            return *(dynamic_cast<Printer::Text*>(current_template[element]));

        }
        Printer::Image& get_image_template_element(std::string element) {
            auto current_template = s_templates.at(s_current_template_name);
            return *(dynamic_cast<Printer::Image*>(current_template[element]));

        }

        constexpr std::shared_ptr<Printer> get_printer() {return m_printer;}

        void set_template_pdf_path(const std::filesystem::path template_pdf_path) {
            if (auto result = m_printer -> set_input_path(template_pdf_path); result.success)
                s_template_pdf_path = template_pdf_path;
            else 
                throw std::invalid_argument(result.error_message);
        }

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
    std::filesystem::path Protocol<T>::s_font_path;

    template<typename T> 
    std::string Protocol<T>::s_current_template_name = "default";

    

    
} // namespace docspeak


#endif