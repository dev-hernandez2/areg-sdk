/************************************************************************
 * \file        areg/base/private/FileBase.cpp
 * \ingroup     AREG SDK, Asynchronous Event Generator Software Development Kit 
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief      
 *
 ************************************************************************/

#include "areg/base/FileBase.hpp"
#include "areg/base/NEUtilities.hpp"
#include "areg/base/WideString.hpp"
#include "areg/base/IEByteBuffer.hpp"
#include "areg/base/DateTime.hpp"
#include "areg/base/Process.hpp"

const char * const  FileBase::FILE_MASK_TIMESTAMP       = "%time%";
const char * const  FileBase::TIMESTAMP_FORMAT          = "%04d_%02d_%02d_%02d_%02d_%02d_%03d"; // yyyy_mm_dd_hh_mm_ss_ms
const char * const  FileBase::FILE_MASK_APPNAME         = "%appname%";
const int           FileBase::FILE_MASK_TIMESTAMP_LEN   = NEString::getStringLength<char>(FileBase::FILE_MASK_TIMESTAMP);
const int           FileBase::FILE_MASK_APPNAME_LEN     = NEString::getStringLength<char>(FileBase::FILE_MASK_APPNAME);

//////////////////////////////////////////////////////////////////////////
// Local methods.
//////////////////////////////////////////////////////////////////////////
template<typename CharType, class ClassType>
inline int _readString(const FileBase & file, ClassType & outValue)
{
    static const unsigned int maxBufSize = 1024;

    int result = 0;
    outValue.clear();
    if ((file.isOpened() == false) || (file.canRead() == false))
    {
        OUTPUT_ERR("Either file is not opened or forbidden to read data. Data cannot be read.");
        return result;
    } 

    CharType * buffer = DEBUG_NEW CharType[maxBufSize];
    if (buffer != NULL)
    {
        int strLength       = maxBufSize - 1;
        CharType * context  = NULL;
        int length          = 0;
        do 
        {
            buffer[0]           = NEString::EndOfString;
            int curPos          = file.getPosition();
            int readLength      = file.read(reinterpret_cast<unsigned char *>(buffer), strLength * sizeof(CharType)) / sizeof(CharType);
            readLength          = MACRO_MIN(strLength, readLength);
            buffer[readLength]  = NEString::EndOfString;

            length              = readLength;
            if ( readLength > 0 )
            {
                const CharType * str = NEString::getPrintable<CharType>( buffer, readLength, &context );
                length = context != NULL ? MACRO_ELEM_COUNT( buffer, context ) : readLength;

                outValue+= str;
                result  += length;
                file.setPosition(curPos + result * sizeof(CharType), IECursorPosition::POSITION_BEGIN);
                if ( context != buffer + readLength)
                    length = 0; // break loop
            }
        } while ( length > 0 );

        delete [] buffer;
    }

    return result;
}

template<typename CharType, class ClassType>
inline int _readLine(const FileBase & file, ClassType & outValue)
{
    static const unsigned int maxBufSize = 1024;

    int result = 0;
    outValue.clear();
    if ((file.isOpened() == false) || (file.canRead() == false))
    {
        OUTPUT_ERR("Either file is not opened or forbidden to read data. Data cannot be read.");
        return result;
    } 


    CharType * buffer = DEBUG_NEW CharType[maxBufSize];
    if (buffer != NULL)
    {
        int strLength       = maxBufSize - 1;
        CharType * context  = NULL;
        int length          = 0;
        do 
        {
            buffer[0]           = NEString::EndOfString;
            int curPos          = file.getPosition();
            int readLength      = file.read(reinterpret_cast<unsigned char *>(buffer), strLength * sizeof(CharType)) / sizeof(CharType);
            readLength          = MACRO_MIN(strLength, readLength);
            buffer[readLength]  = NEString::EndOfString;

            length              = readLength;
            if ( readLength > 0 )
            {
                const CharType * str = NEString::getLine<CharType>( buffer, readLength, &context );
                length   = context != NULL ? MACRO_ELEM_COUNT(buffer, context) : readLength;
                outValue+= str;
                result  += length;
                file.setPosition(curPos + result * sizeof(CharType), IECursorPosition::POSITION_BEGIN);
                if ( context != buffer + readLength)
                    length = 0; // break loop
            }
        } while ( length > 0 );

        delete [] buffer;
    }

    return result;
}

template<typename CharType>
inline int _readString(const FileBase & file, CharType * buffer, int elemCount)
{
    int result = 0;
    if ((file.isOpened() == false) || (file.canRead() == false))
    {
        OUTPUT_ERR("Either file is not opened or forbidden to read data. Data cannot be read.");
    } 
    else if (buffer != NULL && elemCount > 0)
    {
        int strLength       = elemCount - 1;
        buffer[0]           = NEString::EndOfString;
        int curPos          = file.getPosition();
        CharType * context  = NULL;
        int readLength      = file.read(reinterpret_cast<unsigned char *>(buffer), strLength * sizeof(CharType)) / sizeof(CharType);
        readLength          = MACRO_MIN(strLength, readLength);
        buffer[readLength]  = NEString::EndOfString;

        if ( readLength > 0 )
        {
            NEString::getPrintable<CharType>( buffer, elemCount, &context );
            ASSERT(context == NULL || context > buffer);
            result = context != NULL ? MACRO_ELEM_COUNT( buffer, context ) : readLength;
            file.setPosition(curPos + result * sizeof(CharType), IECursorPosition::POSITION_BEGIN);
        }
    }
    else
    {
        OUTPUT_ERR("The required buffer to fill is empty, cannot read string");
    }

    return result;
}

template<typename CharType>
inline int _readLine(const FileBase & file, CharType * buffer, int elemCount)
{
    int result = 0;
    if ((file.isOpened() == false) || (file.canRead() == false))
    {
        OUTPUT_ERR("Either file is not opened or forbidden to read data. Data cannot be read.");
    } 
    else if (buffer != NULL && elemCount > 0)
    {
        int strLength       = elemCount - 1;
        buffer[0]           = NEString::EndOfString;
        int curPos          = file.getPosition();
        CharType * context  = NULL;
        int readLength      = file.read(reinterpret_cast<unsigned char *>(buffer), strLength * sizeof(CharType)) / sizeof(CharType);
        readLength          = MACRO_MIN(strLength, readLength);
        buffer[readLength]  = NEString::EndOfString;
        if ( readLength > 0 )
        {
            NEString::getLine<CharType>(buffer, elemCount, &context);
            ASSERT(context == NULL || context > buffer);
            result = context != NULL ? MACRO_ELEM_COUNT(buffer, context) : readLength;
            file.setPosition(curPos + result * sizeof(CharType), IECursorPosition::POSITION_BEGIN);
        }
    }
    else
    {
        OUTPUT_ERR("The required buffer to fill is empty, cannot read string");
    }
    return result;
}

template<typename CharType>
inline bool _writeString(FileBase & file, const CharType * buffer)
{
    bool result = false;
    if (file.isOpened() && file.canWrite())
    {
        if (buffer != NULL)
        {
            unsigned int len = NEString::getStringLength<CharType>(buffer);
            len += file.isBinaryMode() ? 1 : 0;
            len *= sizeof(CharType);

            result = file.write( reinterpret_cast<const unsigned char *>(buffer), len ) == len;
        }
        else
        {
            OUTPUT_ERR("The buffer is NULL, nothing to write.");
        }
    }
    else
    {
        OUTPUT_ERR("Either file is not opened or data cannot be written.");
    }

    return result;
}

template<typename CharType>
inline  bool _writeLine(FileBase & file, const CharType * buffer)
{
    bool result = false;
    if (file.isOpened() && file.canWrite())
    {
        if (buffer != NULL)
        {
            unsigned int len = 0;
            while (NEString::isEndOfLine<CharType>(buffer[len]) == false)
                ++ len;

            len *= sizeof(CharType);
            if ( file.write(reinterpret_cast<const unsigned char *>(buffer), len) == len )
                result = file.writeChar( TEString<CharType>::NewLine );
        }
        else
        {
            OUTPUT_ERR("The buffer is NULL, nothing to write.");
        }
    }
    else
    {
        OUTPUT_ERR("Either file is not opened or data cannot be written.");
    }

    return result;
}

//////////////////////////////////////////////////////////////////////////
// Local defines.
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// FileBase class implementation.
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Constructor / Destructor
//////////////////////////////////////////////////////////////////////////
FileBase::FileBase( void )
    : IEIOStream        ( )
    , IECursorPosition  ( )

    , mFileName         (String::EmptyString)
    , mFileMode         (static_cast<unsigned int>(FO_MODE_INVALID))
    , mReadConvert      (static_cast<IEInStream &>(self()), static_cast<IECursorPosition &>(self()) )
    , mWriteConvert     (static_cast<IEOutStream &>(self()), static_cast<IECursorPosition &>(self()) )
{
    ; // do nothing
}

FileBase::~FileBase( void )
{
    ; // do nothing
}

//////////////////////////////////////////////////////////////////////////
// Methods
//////////////////////////////////////////////////////////////////////////

unsigned int FileBase::normalizeMode(unsigned int mode) const
{
    if (mode != FO_MODE_INVALID)
        mode |= FO_MODE_READ;

    if (mode & FOB_ATTACH)
    {
        mode &= ~(FOB_DETACH | FOB_TRUNCATE | FOB_TEMP_FILE | FOB_SHARE_WRITE | FOB_WRITE);
        mode |= FO_MODE_ATTACH;
    }

    if (mode & FOB_DETACH)
    {
        mode &= ~(FOB_ATTACH | FOB_TEMP_FILE | FOB_SHARE_WRITE);
        mode |= FO_MODE_DETACH;
    }

    if (mode & FOB_TEMP_FILE)
    {
        mode &= ~(FOB_FOR_DELETE | FOB_EXIST | FOB_ATTACH | FOB_DETACH | FOB_SHARE_READ | FOB_SHARE_WRITE);
        mode |= FO_MODE_CREATE_TEMP;
    }

    if (mode & FOB_TEXT)
    {
        mode &= ~FOB_BINARY;
        mode |= FO_MODE_TEXT;
    }
    else
    {
        mode |= FO_MODE_BINARY;
    }

    if (mode & FOB_BINARY)
    {
        mode &= ~FOB_TEXT;
        mode |= FO_MODE_BINARY;
    }

    if ((mode & FOB_WRITE) == 0 && (mode & FOB_READ) != 0)
        mode |= FOB_EXIST;
    else if (mode & FOB_CREATE)
        mode |= FO_MODE_CREATE;

    if (mode & FOB_EXIST)
        mode |= FO_MODE_EXIST;
    else
        mode &= ~FOB_TRUNCATE;

    if (mode & FOB_TRUNCATE)
        mode |= FO_MODE_TRUNCATE;

    if (mode & FOB_WRITE_DIRECT)
        mode |= FO_MODE_WRITE_DIRECT;

    return mode;
}

int FileBase::append( const unsigned char* buffer, int length )
{
    int result = 0;
    if (buffer != NULL && isOpened() && canWrite())
    {
        moveToEnd();
        result = write(buffer, length);
    }
    return result;
}

int FileBase::readInvert( unsigned char* buffer, int length ) const
{
    int result = 0;
    if (isOpened() && canRead())
    {
        result = read(buffer, length);
        for (int i = 0; i < result / 2; ++ i)
        {
            unsigned char ch = buffer[i];
            buffer[i] = buffer[length - 1 - i];
            buffer[length - 1 - i] = ch;
        }
    }
    return result;
}

int FileBase::writeInvert( const unsigned char* buffer, int length )
{
    int result = 0;
    if (isOpened() && canWrite())
    {
        unsigned char* temp = buffer != NULL && length > 0 ? DEBUG_NEW unsigned char[length] : NULL;
        if (temp != NULL)
        {
            for (int i = 0; i < length; ++ i)
                temp[i] = buffer[length - 1 - i];
            result = write(temp, length);
            delete [] temp;
        }
    }
    return result;
}

int FileBase::readString( char * buffer, int elemCount) const
{
    return _readString<char>(self(), buffer, elemCount);
}

int FileBase::readString( wchar_t * buffer, int elemCount ) const
{
    return _readString<wchar_t>(self(), buffer, elemCount);
}

int FileBase::readString(String & outValue ) const
{
    return _readString<char, String>(self(), outValue);
}

int FileBase::readString(WideString & outValue) const
{
    return _readString<char, WideString>(self(), outValue);
}

int FileBase::readLine( char * buffer, int elemCount) const
{
    return _readLine<char>(self(), buffer, elemCount);
}

int FileBase::readLine( wchar_t * buffer, int elemCount ) const
{
    return _readLine<wchar_t>(self(), buffer, elemCount);
}

int FileBase::readLine( String & outValue) const
{
    return _readLine<char, String>(self(), outValue);
}

int FileBase::readLine(WideString & outValue) const
{
    return _readLine<wchar_t, WideString>(self(), outValue);
}

bool FileBase::writeString( const char* inValue )
{
    return _writeString<char>(self(), inValue);
}

bool FileBase::writeString( const wchar_t* inValue )
{
    return _writeString<wchar_t>(self(), inValue);
}

bool FileBase::writeLine( const char* inValue )
{
    return _writeLine<char>(self(), inValue);
}

bool FileBase::writeLine( const wchar_t* inValue )
{
    return _writeLine<wchar_t>(self(), inValue);
}

unsigned int FileBase::resizeAndFill( int newSize, unsigned char fillValue )
{
    unsigned int curPos = getPosition();
    unsigned int newPos = reserve(newSize);
    if (newPos != IECursorPosition::INVALID_CURSOR_POSITION)
    {
        if (newPos > curPos)
        {
            setPosition(curPos, IECursorPosition::POSITION_BEGIN);
            for (unsigned int i = 0; i < newPos; ++ i)
                write(&fillValue, sizeof(unsigned char));
            ASSERT(getPosition() == newPos);
        }
    }
    return newPos;
}

void FileBase::resetCursor(void) const
{
    setPosition(0, IECursorPosition::POSITION_BEGIN);
}

unsigned int FileBase::read(IEByteBuffer & buffer) const
{
    unsigned int result = 0;
    buffer.invalidate();

    if ( isOpened() && canRead() )
    {
        int sizeReserve = 0;
        int sizeRead    = 0;

        if (readInt(sizeReserve) && (sizeReserve > 0))
        {
            sizeRead = buffer.resize(sizeReserve, false);
            unsigned char * data = sizeRead != 0 ? buffer.getBuffer() : NULL;
            if ( (data != NULL) && (read(data, static_cast<unsigned int>(sizeRead)) == static_cast<unsigned int>(sizeRead)) )
            {
                result = sizeRead + sizeof(int);
            }
            else
            {
                OUTPUT_ERR("Either was not able to reserve [ %d ] bytes of space, or failed read file [ %s ].", sizeReserve, mFileName.getString());
            }
        }
        else
        {
            OUTPUT_ERR("Unable to read the size of byte-buffer, the file [ %s ]", mFileName.getString());
        }
    }
    else
    {
        OUTPUT_ERR("Either file [ %s ] is not opened [ %s ], or reading mode is not set (mode = [ %d ]).", mFileName.getString(), isOpened() ? "true" : "false", mFileMode);
    }

    return result;
}

unsigned int FileBase::read(String & asciiString) const
{
    return readString(asciiString);
}

unsigned int FileBase::read(WideString & wideString) const
{
    return readString(wideString);
}

unsigned int FileBase::write(const IEByteBuffer & buffer)
{
    unsigned int result = 0;

    if ( isOpened() && canWrite() )
    {
        const unsigned char * data  = buffer.getBuffer();
        int sizeUsed                = static_cast<int>(buffer.getSizeUsed());

        if (writeInt(sizeUsed) && (write(data, static_cast<unsigned int>(sizeUsed)) == static_cast<unsigned int>(sizeUsed)))
        {
            result = sizeUsed + sizeof(int);
        }
        else
        {
            OUTPUT_ERR("Failed to write [ %d ] bytes of data in file [ %s ]", sizeUsed + sizeof(int), mFileName.getString());
        }
    }
    else
    {
        OUTPUT_ERR("Either files [ %s ] is not opened or it is not opened to write file", mFileName.getString());
    }

    return result;
}

unsigned int FileBase::write(const String & asciiString)
{
    const char * buffer = asciiString.getString();
    unsigned int space  = isTextMode() != 0 ? asciiString.getLength() * sizeof(char) : asciiString.getUsedSpace();

    return write(reinterpret_cast<const unsigned char *>(buffer), space);
}

unsigned int FileBase::write(const WideString & wideString)
{
    const wchar_t * buffer  = wideString.getString();
    unsigned int space      = isTextMode() != 0 ? wideString.getLength() * sizeof(wchar_t) : wideString.getUsedSpace();

    return write(reinterpret_cast<const unsigned char *>(buffer), space);
}

bool FileBase::write(const char * asciiString)
{
    return this->writeString(asciiString);
}

bool FileBase::write(const wchar_t * wideString)
{
    return this->writeString(wideString);
}

void FileBase::flush(void)
{
    ; // do nothing
}

void FileBase::normalizeName(String & name)
{
    // replace all "%time%"
    char fmt[32];
    NEUtilities::sSystemTime st;
    DateTime::getNow(st, true);
    String::formatString(fmt, 32, FileBase::TIMESTAMP_FORMAT, st.stYear, st.stMonth, st.stDay, st.stHour, st.stMinute, st.stSecond, st.stMillisecs);

    do 
    {
        NEString::CharPos index = name.findFirstOf(FileBase::FILE_MASK_TIMESTAMP, 0, false);
        if (index == NEString::InvalidPos)
            break;

        name.replace(fmt, index, FileBase::FILE_MASK_TIMESTAMP_LEN );
    } while (true);


    // replace all "%appname%"
    String appName = Process::getInstance().getAppName();
    do 
    {
        NEString::CharPos index = name.findFirstOf(FileBase::FILE_MASK_APPNAME, 0, false);
        if (index == NEString::InvalidPos)
            break;

        name.replace(appName, index, FileBase::FILE_MASK_APPNAME_LEN );
    } while (true);

}