#ifndef PRINTER_H
#define PRINTER_H

#include <filesystem>
#include <iostream>
#include <fstream>
#include <format>
#include <ctime>

#include <PDFWriter.h>
#include <PDFPage.h>
#include <PDFModifiedPage.h>
#include <EStatusCode.h>
#include <AbstractContentContext.h>
#include <PageContentContext.h>
#include <PDFUsedFont.h>
#include <UnicodeString.h>

namespace docspeak {
    class Printer
    {
    private:
        std::filesystem::path m_input_path;
        std::filesystem::path m_output_path;
        std::filesystem::path m_font_path;
        std::filesystem::path m_assets_folder_path;
        bool m_printer_ready = false;
    public:
        struct [[nodiscard]] StatusInfo {
            bool success = false;
            std::string error_message = "";
            explicit StatusInfo(bool status, std::string message=""): success(status) {
                error_message = success ? "No Error" : message;
            }
        };

        enum PDFPosition {
            padding = 75,
            empty_line = 60,
            line_break = 30,
            x_left = 0 + padding,
            x_rigth = 612 - padding,
            y_bottom = 0 + padding,
            y_top = 792 - padding,
            x_mid = (x_rigth + x_left) / 2,
            y_mid = (y_top + y_bottom) / 2
        };

    public:
        Printer();
        explicit Printer(const std::filesystem::path& input_path, const std::filesystem::path& output_path = std::filesystem::current_path());
        ~Printer();

        inline std::filesystem::path& input_path() {return m_input_path;}
        StatusInfo set_input_path(const std::filesystem::path& file_path);
        
        inline std::filesystem::path& output_path() {return m_output_path;}
        StatusInfo set_output_path(const std::filesystem::path& file_path);

        inline std::filesystem::path& font_path() {return m_font_path;}
        StatusInfo set_font_path(const std::filesystem::path& file_path);

        inline std::filesystem::path& assets_folder_path() {return m_assets_folder_path;}
        StatusInfo set_assets_folder_path(const std::filesystem::path& file_path);


        inline bool ready() {return m_printer_ready;}
        bool check_all();

        void print();
        StatusInfo test() ;

    private:
        StatusInfo __check_path(const std::filesystem::path& path, bool strict = false);
        bool __check_if_ready();
    };
    
    
}
#endif