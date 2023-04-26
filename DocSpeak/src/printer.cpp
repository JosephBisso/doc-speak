#include "printer.h"

using namespace docspeak;

Printer::Printer()
{
}

Printer::Printer(const std::filesystem::path& input_path, const std::filesystem::path& output_path)
{   
    auto success = false;
    success |= set_input_path(input_path).success;
    success |= set_output_path(output_path).success;

    if (!success)
        throw std::invalid_argument("Input path or Output path invalid! Check the given path");
}

Printer::~Printer()
{
}

Printer::StatusInfo Printer::__check_path(const std::filesystem::path& path, bool strict) {
    if (std::filesystem::exists(path)) {
        return StatusInfo(true);
    }

    if (!strict) {
        std::ofstream test_file(path.string());
        if (test_file.is_open()){
            test_file << "Writing this to a file.\n";
            test_file.close();

            std::filesystem::remove(path);
            return StatusInfo(true);

        }
    }

    auto msg = std::format("Path {} is either invalid or you don't have enough rigth to acces it", path.string());
    return StatusInfo(false, msg);

} 

bool Printer::__check_if_ready() {
    m_printer_ready =  
        (!m_input_path.empty()) &&
        (!m_font_path.empty()) &&
        (!m_output_path.empty());

    return m_printer_ready;
}

bool Printer::check_all() {
    m_printer_ready =  
        (!m_input_path.empty()) &&
        (!m_output_path.empty()) &&
        (!m_font_path.empty()) &&
        (!m_assets_folder_path.empty());


    return m_printer_ready;
}

Printer::StatusInfo Printer::set_input_path(const std::filesystem::path& path) {
    auto check_status = __check_path(path, true);
    if (!check_status.success)
        return check_status;
    
    m_input_path = path;
    __check_if_ready();
    return StatusInfo(true);
} 

Printer::StatusInfo Printer::set_font_path(const std::filesystem::path& path) {
    auto check_status = __check_path(path, true);
    if (!check_status.success)
        return check_status;
    
    m_font_path = path;
    __check_if_ready();
    return StatusInfo(true);
} 

Printer::StatusInfo Printer::set_assets_folder_path(const std::filesystem::path& path) {
    auto check_status = __check_path(path, true);
    if (!check_status.success)
        return check_status;
    
    m_assets_folder_path = path;
    __check_if_ready();
    return StatusInfo(true);
} 

Printer::StatusInfo Printer::set_output_path(const std::filesystem::path& path) {
    auto check_status = __check_path(path);
    if (!check_status.success)
        return check_status;

    if (std::filesystem::exists(path))
        std::filesystem::remove(path);
    
    m_output_path = path;
    __check_if_ready();
    return StatusInfo(true);
} 

Printer::StatusInfo Printer::print(const PrintJob& print_job) {
    if (!__check_if_ready()) {
        auto msg = std::string("Cannot print because printer is not ready. Specify all Paths first..");
        return StatusInfo(false, msg);
    }

    auto start = std::chrono::system_clock::now();
    bool success;
    std::string msg;

    PDFWriter pdfWriter;

    pdfWriter.ModifyPDF(
        m_input_path.string(),
        ePDFVersion13,
        m_output_path.string()
    );

    PDFModifiedPage modifiedPage(&pdfWriter,0);
    AbstractContentContext* contentContext = modifiedPage.StartContentContext();

    Printer::Printable::font = pdfWriter.GetFontForFile(m_font_path.string());

    try
    {
        for (auto i = 0; i < print_job.size(); i++) {
            if (print_job[i] -> x < 0 || print_job[i] -> y < 0)
                continue;
            print_job[i] -> print(contentContext);
        }
        success = true;
    }
    catch(const std::exception& exc)
    {
        success = false;
        msg = std::format("Error while printing test file. Exception: {}", exc.what());

    }
    
    modifiedPage.EndContentContext();
    modifiedPage.WritePage();

    success &= ( pdfWriter.EndPDF() == PDFHummus::eSuccess);

    clear_print_job();

    return StatusInfo(success, msg);

}

void Printer::clear_print_job() {
    m_print_job.clear();
}

Printer::StatusInfo Printer::test() {
    if (!check_all()) {
        auto msg = std::string("Cannot print because printer is not ready. Specify all Paths first..");
        return StatusInfo(false, msg);
    }

    auto start = std::chrono::system_clock::now();

    PDFWriter pdfWriter;

    pdfWriter.ModifyPDF(
        m_input_path.string(),
        ePDFVersion13,
        m_output_path.string()
    );

    PDFModifiedPage modifiedPage(&pdfWriter,0);
    AbstractContentContext* contentContext = modifiedPage.StartContentContext();
    PDFUsedFont* font = pdfWriter.GetFontForFile(m_font_path.string());

    AbstractContentContext::TextOptions opt_8(font,8,AbstractContentContext::eGray,0);
    AbstractContentContext::TextOptions opt_12(font,12,AbstractContentContext::eGray,0);
    AbstractContentContext::TextOptions opt_14(font,14,AbstractContentContext::eGray,0);
    AbstractContentContext::TextOptions opt_18(font,18,AbstractContentContext::eGray,0);
    AbstractContentContext::TextOptions opt_20(font,20,AbstractContentContext::eGray,0);

    contentContext->WriteText(x_left, y_top,"x top left",opt_8);
    contentContext->WriteText(x_rigth, y_bottom,"x bottom rigth",opt_8);

    PDFUsedFont::TextMeasures textDimensions;
    textDimensions = font->CalculateTextDimensions("DocSpeak Test START",20);
    contentContext->WriteText((double)x_mid - textDimensions.width / 2 ,y_top,"DocSpeak Test START",opt_20);

    textDimensions = font->CalculateTextDimensions("DocSpeak Test END",20);
    contentContext->WriteText((double)x_mid - textDimensions.width / 2 ,y_bottom,"DocSpeak Test END",opt_20);


    contentContext->WriteText(x_left ,y_top - empty_line ,"1. Text Block",opt_18);
    contentContext->WriteText(x_left ,y_top - empty_line - line_break ,"abcdefghijklmnopqrstuvwxyzßüöä",opt_12);
    contentContext->WriteText(x_left ,y_top - empty_line - 2*line_break ,"0123456789",opt_12);


    contentContext->WriteText(x_mid + padding ,y_top - empty_line ,"2. Text Block Size",opt_18);
    int i = 1;
    for (auto opt : {opt_8, opt_12, opt_14, opt_18, opt_20}) {
        auto msg = std::format("Test Size {}", opt.fontSize);
        contentContext->WriteText(x_mid + padding , y_top - empty_line - (i++) * line_break,msg,opt);
    }

    contentContext->WriteText(x_left ,y_top - 3 * empty_line ,"3. Image Block",opt_18);
    AbstractContentContext::ImageOptions opt_img;
    opt_img.transformationMethod = AbstractContentContext::eFit;
    opt_img.boundingBoxWidth = 200;
    opt_img.boundingBoxHeight = 200;
    contentContext->DrawImage(x_left,y_top - 3 * empty_line - line_break -  200,(m_assets_folder_path/"logo.jfif").string(), opt_img);

    contentContext->WriteText(x_mid + padding ,y_top - 6 * empty_line ,"4. Sign Block",opt_18);
    contentContext->WriteText(x_mid + padding , y_top - 6 * empty_line - line_break, "Signs follow:" ,opt_12);
    contentContext->WriteText(x_mid + padding , y_top - 6 * empty_line - 2 * line_break, "✗ ✓ ▢" ,opt_12);


    contentContext->WriteText(x_left ,y_top - 8 * empty_line ,"5. Rapport",opt_18);
    AbstractContentContext::TextOptions opt_blue(font,12,AbstractContentContext::eRGB, 	0x154c94); //Blue

    auto start_t = std::chrono::system_clock::to_time_t(start);
    std::string status = std::format ("Test start: {}", std::ctime(&start_t));
    contentContext->WriteText(x_left,y_top - 8 * empty_line - line_break, status,opt_blue);

    auto end = std::chrono::system_clock::now();
    auto end_t = std::chrono::system_clock::to_time_t(end);
    status = std::format ("Test end: {}", std::ctime(&end_t));
    contentContext->WriteText(x_left,y_top - 8 * empty_line - 2* line_break, status,opt_blue);

    status = std::format ("Test duration: {}", end - start);
    contentContext->WriteText(x_left,y_top - 8 * empty_line - 3 * line_break,status,opt_blue);


    modifiedPage.EndContentContext();
    modifiedPage.WritePage();


    PDFHummus::EStatusCode result;
    std::string msg;

    try {
        result = pdfWriter.EndPDF();
    } catch (const std::exception &exc) {
        msg = std::format("Error while printing test file. Exception: {}", exc.what());
    }

    return StatusInfo(result == PDFHummus::eSuccess, msg);
}

PDFUsedFont* Printer::Printable::font;

void Printer::Text::print(AbstractContentContext* context) const {
    if (!Printable::font) {
        auto msg = std::format("Cannot print text because no font has been set. Text: {}", this->text);
        throw std::invalid_argument(msg);
    }

    AbstractContentContext::TextOptions text_option(Printable::font, this->font_size ,AbstractContentContext::eRGB, this->color);

    auto x_adjust = 0.0;
    if (this->adjust_to_middle) {
        auto textDimensions = Printable::font->CalculateTextDimensions(this->text,this->font_size);
        x_adjust = (textDimensions.width / 2);
    }
    
    context -> WriteText(this->x - x_adjust, this->y, this->text, text_option);
}

void Printer::Image::print(AbstractContentContext* context) const {
    if (this->src_path.empty() || !std::filesystem::exists(this->src_path)) {
        auto msg = std::format("Cannot print image because no source path doesn't exist. Path: {}", this->src_path.string());
        throw std::invalid_argument(msg);
    }

    AbstractContentContext::ImageOptions image_option;
    image_option.transformationMethod = AbstractContentContext::eFit;
    image_option.boundingBoxWidth = this->width;
    image_option.boundingBoxHeight = this->heigth;
    context->DrawImage(this->x,this->y - this->heigth ,this->src_path.string(), image_option);
}