#ifndef PRINTER_H
#define PRINTER_H

#include <filesystem>
#include <iostream>
#include <fstream>
#include <format>
#include <ctime>
#include <memory>

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

        class Printable
        {
        public:
            double x, y;
            static PDFUsedFont* font;
            Printable(double xPos, double yPos): x(xPos), y(yPos) {}
            virtual void print(AbstractContentContext* context) const = 0; 
        };

        class Text: public Printable 
        {
        public:
            size_t font_size;
            long int color;
            std::string text;
            bool adjust_to_middle;
            Text(double xPos, double yPos, const std::string& print_text, size_t size = 12, long int text_color = 0x0) :
                Printable(xPos, yPos), text(print_text), font_size(size), color(text_color), adjust_to_middle(false)
            {}
            virtual void print(AbstractContentContext* context) const override; 
        };

        class Image: public Printable 
        {
        public:
            size_t width;
            size_t heigth;
            std::filesystem::path src_path;
            Image(double xPos, double yPos, const std::filesystem::path& path, size_t img_width = 50, size_t img_heigth = 50) :
                Printable(xPos, yPos), src_path(path), width(img_width), heigth(img_heigth) 
            {}
            virtual void print(AbstractContentContext* context) const override; 
        };

        typedef std::vector<Printable*> PrintJob;

    private:
        std::filesystem::path m_input_path;
        std::filesystem::path m_output_path;
        std::filesystem::path m_font_path;
        std::filesystem::path m_assets_folder_path;
        bool m_printer_ready = false;

        std::shared_ptr<PrintJob> m_print_job;

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

        StatusInfo print(const PrintJob& print_job);
        inline StatusInfo print() {return print(*m_print_job);}
        StatusInfo test() ;

        inline std::shared_ptr<PrintJob> get_print_job(){return m_print_job;}
        void clear_print_job();

    private:
        StatusInfo __check_path(const std::filesystem::path& path, bool strict = false);
        bool __check_if_ready();
    };
    
}

inline docspeak::Printer& operator<< (docspeak::Printer& stream, const docspeak::Printer::Printable& job) {
    if (dynamic_cast<const docspeak::Printer::Text*> (&job)) {
        auto printable_text = dynamic_cast<const docspeak::Printer::Text*> (&job);
        auto text = new docspeak::Printer::Text(printable_text->x, printable_text->y, printable_text->text, printable_text->font_size, printable_text->color);
        text -> adjust_to_middle = printable_text -> adjust_to_middle;
        stream.get_print_job() -> push_back(text);
    } else {
        auto printable_img = dynamic_cast<const docspeak::Printer::Image*> (&job);
        auto img = new docspeak::Printer::Image(printable_img->x, printable_img->y, printable_img->src_path, printable_img->width, printable_img->heigth);
        stream.get_print_job() -> push_back(img);
    }
    // stream.get_print_job() -> push_back((const_cast<docspeak::Printer::Printable&>(job)));
    return stream;
}
#endif