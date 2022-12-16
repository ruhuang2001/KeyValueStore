#ifndef FUNDB_LOGGER_H_
#define FUNDB_LOGGER_H_

namespace fundb {

class Logger
{
    private:
        static bool is_syslog_open_;
        static int level_;
        static std::mutex mutex_;
        static int log_target_;
        static std::string syslog_ident_;
    public:
        Logger(/* args */);
        ~Logger();
};

Logger::Logger(/* args */)
{
}

Logger::~Logger()
{
}


}

#endif