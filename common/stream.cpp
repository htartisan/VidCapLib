/*

    OpenPnp-Capture: a video capture subsystem.

    Platform independent stream code

    Copyright (c) 2017 Jason von Nieda, Niels Moseley.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
    
*/

#include <memory.h> // for memcpy
#include "stream.h"
#include "context.h"


// **********************************************************************
//   Stream
// **********************************************************************

Stream::Stream() :
    m_owner(nullptr),
    m_isOpen(false),
    m_frames(0),
    m_newFrame(false),
    m_lastFrameSize(0)
{
}

Stream::~Stream()
{
    LOG(LOG_DEBUG,"Stream::~Stream reports %d frames captured.\n", m_frames);
    //Note: close() should be called/handled by the PlatformStream!
}

bool Stream::hasNewFrame()
{
    m_bufferMutex.lock();
    bool ok = m_newFrame;
    m_bufferMutex.unlock();
    return ok;
}

bool Stream::captureFrame(uint8_t *RGBbufferPtr, uint32_t RGBbufferBytes, uint32_t &NumBytesCaptured)
{
    NumBytesCaptured = 0;

    if (!m_isOpen) 
        return false;

    if (RGBbufferPtr == nullptr || RGBbufferBytes < 1) 
        return false;

    bool bRet = false;

    m_bufferMutex.lock();    
    size_t frameBufSize = m_frameBuffer.size();
    size_t maxBytes = RGBbufferBytes <= frameBufSize ? RGBbufferBytes : frameBufSize;
    if (maxBytes != 0)
    {
        try
        { 
            memcpy(RGBbufferPtr, &m_frameBuffer, maxBytes);

            NumBytesCaptured = (uint32_t) m_lastFrameSize;

            bRet = true;
        }
        catch (...)
        {
            bRet = false;
        }
    }
    m_newFrame = false;
    m_bufferMutex.unlock();

    return true;
}

void * Stream::mapFrame(uint32_t& NumBytesCaptured)
{
    NumBytesCaptured = 0;

    if (!m_isOpen)
        return nullptr;

    void *pOut = nullptr;

    m_bufferMutex.lock();

    size_t frameBufSize = m_frameBuffer.size();
    if (frameBufSize != 0)
    {
        pOut = (void*) m_frameBuffer.data();

        NumBytesCaptured = (uint32_t) m_lastFrameSize;
    }

    return pOut;
}

bool Stream::unmapFrame()
{
    m_newFrame = false;

    m_bufferMutex.unlock();

    return true;
}

void Stream::submitBuffer(const uint8_t* ptr, size_t bytes)
{
    // sanity check
    if (ptr == nullptr)
    {
        return;
    }

    m_bufferMutex.lock();

    if (m_frameBuffer.size() == 0)
    {
        LOG(LOG_ERR, "ERROR: Stream: buffer size is 0 - cant store frames!\n");
    }

    if (m_frameBuffer.size() >= bytes)
    {
        if (bytes > 0)
        { 
            memcpy(&m_frameBuffer[0], ptr, bytes);

            m_newFrame = true;
            m_frames++;
        }
        else
        {
            if ((m_frames % 100) == 0)
            {
                // Generate warning every 100 frames if the frame buffer size = 0
                LOG(LOG_WARNING, "WARNING: captureFrame received buffer size: 0");
            }
        }
        m_lastFrameSize = (uint32_t) bytes;
    }
    else
    {
        if ((m_frames % 100) == 0)
        {
            // Generate warning every 100 frames if the frame buffer size = 0
            LOG(LOG_WARNING, "WARNING: captureFrame received size (%d) > buffer size\n", bytes);
        }
    }

    m_bufferMutex.unlock();
}


