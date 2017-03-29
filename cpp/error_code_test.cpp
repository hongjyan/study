#include <system_error>
#include <string>
#include <iostream>

class TPLErrorCategory : public std::error_category
{
    public:
        TPLErrorCategory(const std::string &scli_type, const std::string &scli_msg): msg_type(scli_type), msg_string(scli_msg) {};
        virtual const char* name() const noexcept
        {
            return msg_type.c_str();
        }

        virtual std::string message(int ev) const
        {
            return msg_string;
        }
        void set_type(const std::string &scli_type)
        {
            msg_type = scli_type;
        }

        void set_msg(const std::string &scli_msg)
        {
            msg_string = scli_msg;
        }

        ~TPLErrorCategory()
        {
            std::cout << "destructing" << std::endl;
        }
    private:
        std::string msg_type;
        std::string msg_string;
};

std::error_code generateErrCode()
{
    TPLErrorCategory errCategory("test", "my test");
    std::error_code ec(0, errCategory);
    return ec;
}

int main()
{
    std::error_code ec = generateErrCode();
    std::cout << "-----------" << std::endl;
    std::cout << ec.message() << std::endl;
    return 0;
}
