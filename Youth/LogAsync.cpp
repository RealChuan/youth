/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LogAsync.cpp
 * Author: root
 *
 * Created on 2019年8月5日, 下午7:30
 */

#include "LogAsync.h"
#include "LogFile.h"
#include "LogOut.h"

using namespace youth;

static LogAsync* g_LogAsync = nullptr;

void asyncOutput(const char* msg, int len)
{
	g_LogAsync->append(msg, len);
}

LogAsync::LogAsync(int flushInterval)
	:refreshTime(flushInterval),
	  running(false),
      thread(std::bind(&LogAsync::threadFunc, this)),
      mutex(),
      cond(mutex),
      currentBuffer(new Buffer),
      nextBuffer(new Buffer),
      buffers()
{
    currentBuffer->bzero();
    nextBuffer->bzero();
    buffers.reserve(16);

	if (g_LogAsync)
	{
		delete g_LogAsync;
		g_LogAsync = this;
	}
	else
	{
		g_LogAsync = this;
	}
	Logging::setOutputFunc(asyncOutput);
}

LogAsync::~LogAsync()
{
	g_LogAsync = nullptr;
	if (running)
	{
		stop();
	}
}

void LogAsync::append(const char* buf, int len)
{
    MutexLock lock(mutex);
    if (currentBuffer->avail() > len)
	{
        currentBuffer->append(buf, len);
	}
	else
	{
        buffers.push_back(std::move(currentBuffer));

        if (nextBuffer)
		{
            currentBuffer = std::move(nextBuffer);
		}
		else
		{
            currentBuffer.reset(new Buffer); // Rarely happens
		}
        currentBuffer->append(buf, len);
        cond.notify();
	}
}

void LogAsync::threadFunc()
{
	assert(running == true);
	BufferPtr newBuffer1(new Buffer);
	BufferPtr newBuffer2(new Buffer);
	newBuffer1->bzero();
	newBuffer2->bzero();
	BufferVector buffersToWrite;
	buffersToWrite.reserve(16);
	while (running)
	{
		assert(newBuffer1 && newBuffer1->length() == 0);
		assert(newBuffer2 && newBuffer2->length() == 0);
		assert(buffersToWrite.empty());

		{
            MutexLock lock(mutex);
            if (buffers.empty()) // unusual usage!
			{
                cond.waitForSeconds(refreshTime);
			}

            buffers.push_back(std::move(currentBuffer));
            currentBuffer = std::move(newBuffer1);
            buffersToWrite.swap(buffers);
            if (!nextBuffer)
			{
                nextBuffer = std::move(newBuffer2);
			}
		}
		assert(!buffersToWrite.empty());

		if (buffersToWrite.size() > 25)
		{
			char buf[256];
			snprintf(buf, sizeof buf, "Dropped log messages at %s, %zd larger buffers\n",
					 myTime.getLogTime().c_str(), buffersToWrite.size() - 2);
			fputs(buf, stderr);
			LogFile::outputFunc(buf, static_cast<int> (strlen(buf)));

			//output.append(buf, static_cast<int> (strlen(buf)));
			buffersToWrite.erase(buffersToWrite.begin() + 2, buffersToWrite.end());
		}

		for (const auto& buffer : buffersToWrite)
		{
			// FIXME: use unbuffered stdio FILE ? or use ::writev ?
			LogFile::outputFunc(buffer->data(), buffer->length());
			//output.append(buffer->data(), buffer->length());
		}

		if (buffersToWrite.size() > 2)
		{
			// drop non-bzero-ed buffers, avoid trashing
			buffersToWrite.resize(2);
		}

		if (!newBuffer1)
		{
			assert(!buffersToWrite.empty());
			newBuffer1 = std::move(buffersToWrite.back());
			buffersToWrite.pop_back();
			newBuffer1->reset();
		}

		if (!newBuffer2)
		{
			assert(!buffersToWrite.empty());
			newBuffer2 = std::move(buffersToWrite.back());
			buffersToWrite.pop_back();
			newBuffer2->reset();
		}

		buffersToWrite.clear();
		LogFile::flushFunc();
		//output.flush();
	}
	LogFile::flushFunc();
	//output.flush();
}
