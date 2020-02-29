#include "LogOut.h"
#include "LogFile.h"
#include "CurrentThread.h"
#include "LogAsync.h"
#include "ProcessMsg.h"

using namespace youth;

LogOut::LogOut(const char* pLevel, const char* File, int Line)
	: line(Line)
	, file(File)
    , time()
    , logStream()
{
    CurrentThread::tid();
	//在构造的时候会填充日志头(其实就是时间和报警等级)
	//首先，输出打印日志时间到LogStream中的Buffer
	//其次，输出日志等级
    logStream << time.getMicroSToString() << " " << CurrentThread::tidString() << " " << pLevel;
}

LogOut::~LogOut()
{
}

LogStream &LogOut::getLogStream()
{
    return logStream;
}

void LogOut::finishLog()
{
	//根据我们的日志格式安排，最后输出的是
	//文件名即行号。这个方法是在Logger析构
	//时候调用的，表示一条日志流的完成，可以
	//输出到标准输出或文件中去。
    logStream << " - " << file << ":" << line << "\n";
}

const char *LogOut::getLogStreamBuff()
{
    return logStream.GetStreamBuff();
}

int LogOut::getLogStreamBuffLen()
{
    return logStream.GetStreamBuffLen();
}
//---------------------------------------------------------------------------

const static char *arrLevel[] = {"[DEBUG] ", "[INFO] ", "[WARN] ", "[ERROR] ", "[FATAL] "};

Logging::LogLevel static g_LogLevel = Logging::INFO; //默认的日志等级为INFO
int static g_OutputMode = LOGGER_MODE_STDOUT; //默认的输出模式为输出到标准输出

void DefauleOutout(const char *pMsg, int Len)
{
	::fwrite(pMsg, 1, static_cast<size_t>(Len), stdout);
}

void DefauleFlush(void)
{
	::fflush(stdout);
}

Logging::outputFunc static g_OutputFunc = DefauleOutout; //默认的日志输出到标准输出
Logging::flushFunc static g_FlushFunc = DefauleFlush;

Logging::Logging(LogLevel Level, const char* _File, int Line)
	: m_LogOut(new LogOut(arrLevel[Level], _File, Line))
	, logLevel(Level)
{
}

Logging::~Logging()
{
	//当调用析构的时候，日志流填充下文件名和行号，就是完整的一条日志了
	m_LogOut->finishLog();
	g_OutputFunc(m_LogOut->getLogStreamBuff(), m_LogOut->getLogStreamBuffLen());
	if (logLevel == FATAL)
	{
		//如果发生了FATAL错误，那么就终止程序。
		//以便之后重启程序。
		DefauleFlush(); //在此之前先冲刷缓冲区
		abort();
	}
}

LogStream &Logging::getLogStream()
{
	return m_LogOut->getLogStream();
}

Logging::LogLevel Logging::getLogLevel()
{
	return g_LogLevel;
}

void Logging::setLogLevel(LogLevel level)
{
	g_LogLevel = level;
}

void Logging::setOutputMode(int iMode)
{
	g_OutputMode = iMode;

	if (g_OutputMode == LOGGER_MODE_STDOUT)
	{
		//事实上，默认的输出就是输出到标准输出，不做处理
		return;
	}
	else if (g_OutputMode == LOGGER_MODE_LOGFILE)
	{
		//仅输出到日志文件
        setOutputFunc(LogFile::outputFunc);
        setFlushFunc(LogFile::flushFunc);
	}
	else if (g_OutputMode == (LOGGER_MODE_STDOUT | LOGGER_MODE_LOGFILE))
	{
		//输出到标准输出和log中去
		setOutputFunc(outputOutAndLog);
		setFlushFunc(flushAll);
	}
	else
	{
		//nothing
	}
}

void Logging::setFileBaseName(const char* _basename)
{
    std::string baseName = ProcessMsg::fileBaseName(_basename);
	baseName += ".";
    LogFile::instance().setBaseFileName(baseName);
}

void Logging::setOutputFunc(outputFunc Output)
{
	g_OutputFunc = Output;
}

void Logging::setFlushFunc(flushFunc Flush)
{
	g_FlushFunc = Flush;
}

void Logging::outputOutAndLog(const char *pMsg, int Len)
{
    DefauleOutout(pMsg, Len);   //输出到标准输出
    LogFile::instance().outputLogFile(pMsg, Len);   //输出到日志中
}

void Logging::flushAll()
{
	DefauleFlush();
    LogFile::instance().flushLogFile();
}
