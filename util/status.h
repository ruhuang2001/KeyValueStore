#ifndef FUNDB_STATUS_H_
#define FUNDB_STATUS_H_

#include <string>

namespace fundb {

class Status {
    private:
        int code_; // Status value
        std::string message1_;
        std::string message2_;

        int code() const { return code_; }

        enum Code {
            kOK = 0,
            kNotFound = 1,
            kDeleteOrder = 2,
            kInvalidArgument = 3,
            kIOError = 4,
            kDone = 5,
            kMultipartRequired = 6
        };

    public:
        Status(){ code_ = kOK; message1_ = ""; };
        ~Status();
        Status(int code) {
            code_ = code;
        }
        Status(int code, std::string message1, std::string message2)   
        {
        code_ = code;
        message1_ = message1;
        message2_ = message2;
        }

        static Status OK() { return Status(); }

        static Status Done() { return Status(kDone); }

        static Status MultipartRequired() { return Status(kMultipartRequired); }

        static Status DeleteOrder() { return Status(kDeleteOrder); }

        static Status NotFound(const std::string& message1, const std::string& message2="") {
            return Status(kNotFound, message1, message2);
        }

        static Status InvalidArgument(const std::string& message1, const std::string& message2="") {
            return Status(kInvalidArgument, message1, message2);
        }

        static Status IOError(const std::string& message1, const std::string& message2="") {
    return Status(kIOError, message1, message2);
  }
        
        bool IsOK() const { return (code() == kOK); }
        bool IsNotFound() const { return code() == kNotFound; }
        bool IsDeleteOrder() const { return code() == kDeleteOrder; }
        bool IsInvalidArgument() const { return code() == kInvalidArgument; }
        bool IsIOError() const { return code() == kIOError; }
        bool IsDone() const { return code() == kDone; }
        bool IsMultipartRequired() const { return code() == kMultipartRequired; }

        std::string ToString() const;
        
        }; // end class
    
} // namespace fundb


#endif // FUNDB_STATUS_H_
