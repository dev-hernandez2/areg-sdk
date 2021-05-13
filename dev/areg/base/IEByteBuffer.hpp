#ifndef AREG_BASE_IEBYTEBUFFER_HPP
#define AREG_BASE_IEBYTEBUFFER_HPP
/************************************************************************
 * \file        areg/base/IEByteBuffer.hpp
 * \ingroup     AREG SDK, Asynchronous Event Generator Software Development Kit 
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       AREG Platform, Byte Buffer interface.
 *              This is base class for classes supporting functionalities
 *              to control binary (Byte) buffer. Following classes are
 *              derived from this class: RawBuffer, SharedBuffer,
 *              FileBuffer
 *
 ************************************************************************/
/************************************************************************
 * Include files.
 ************************************************************************/
#include "areg/base/GEGlobal.h"
#include "areg/base/NEMemory.hpp"
#include "areg/base/IECursorPosition.hpp"

//////////////////////////////////////////////////////////////////////////
// IEByteBuffer pure virtual class declaration
//////////////////////////////////////////////////////////////////////////
/**
 * \brief       This is base pure virtual class to support 
 *              binary (Byte) buffer basic functionalities. 
 *              All derived classes should overwrite pure virtual functions
 * 
 * \details     Byte Buffer is defined in namespace NEMemory (NEMemory::sByteBuffer).
 *              This class is an interface of basic functionalities.
 *              For more information about basic functionalities
 *              see description of functions
 *
 **/
class AREG_API IEByteBuffer
{
//////////////////////////////////////////////////////////////////////////
// Defined static constants and types
//////////////////////////////////////////////////////////////////////////
protected:
    /**
     * \brief   IEByteBuffer::MAX_BUF_LENGTH
     *          Maximum length of byte buffer. It is defined as 1 Mb.
     **/
    static const unsigned int   MAX_BUF_LENGTH  /*= static_cast<unsigned int>(67108864)*/;  /*0x04000000*/

//////////////////////////////////////////////////////////////////////////
// Constructor / Destructor
//////////////////////////////////////////////////////////////////////////
protected:
    /**
     * \brief   Protected default constructor
     **/
    IEByteBuffer( NEMemory::sByteBuffer & byteBuffer );

public:
    /**
     * \brief   Destructor 
     **/
    virtual ~IEByteBuffer( void );

//////////////////////////////////////////////////////////////////////////
// Attributes and operations
//////////////////////////////////////////////////////////////////////////
public:
/************************************************************************/
// IEByteBuffer overrides
/************************************************************************/

    /**
     * \brief   Adding reference in the byte buffer reference counter.
     *          This is valid only in case of shared buffer.
     *          In case of raw buffer, the reference counter should not be changed.
     *          The reference counter prevents that the byte buffer is deleted / freed
     *          by any other object. For example, if buffer structure is attached to 
     *          byte buffer object instance, until reference counter is not reaching
     *          to zero, it should not be deleted, since it might be used by other class
     *          The reference counter of invalid buffer should not be changed at all.
     *          To check buffer validation, call function isValid()
     **/
    virtual void addReference( void ) = 0;

    /**
     * \brief   Remove reference in the byte buffer reference counter.
     *          This is valid only in case of shared buffer.
     *          In case of raw buffer, the reference counter should not be changed.
     *          If reference counter reaches zero, the buffer is not valid anymore 
     *          and should be invalidated / deleted. The buffer of Invalid Buffer object
     *          should not be deleted / freed at all. To check buffer validation,
     *          call function isValid().
     **/
    virtual void removeReference( void ) = 0;

    /**
     * \brief   Invalidates the buffer. Removes reference, assigns to invalid buffer,
     *          invalidates writing and reading positions.
     **/
    virtual void invalidate( void ) = 0;

    /**
     * \brief   Returns true if buffer is shared between several byte buffer instances.
     **/
    virtual bool isShared( void ) const = 0;

    /**
     * \brief   Returns true if buffer can be shared.
     *          The Raw Buffer object should return false.
     *          The Shared Buffer object should return true.
     **/
    virtual bool canShare( void ) const = 0;

/************************************************************************/
// IEByteBuffer Attributes and operations
/************************************************************************/

    /**
     * \brief	Reserves space for byte buffer structure, if needed, 
     *          copies existing data and the size of buffer available to write.
     *          1.  If requested reserved space is more than the length of data buffer, 
     *              the size of data buffer is increased.
     *          2.  If total data buffer spaces is more than IEByteBuffer::MAX_BUF_LENGTH,
     *              reserves the maximum IEByteBuffer::MAX_BUF_LENGTH.
     *          3.  If size is zero, calls RemoveReference() to free all space.
     *          4.  If buffer is shared (i.e. reference count is more than 1)
     *              the operation is ignored and function returns total remaining space
     *              to write data. Because if data is shared between different instances
     *              of byte-buffer, the size of buffer should not be changed.
     *
     * \param	size	Size in bytes to reserve
     * \param	copy    If true and the existing buffer is valid, it will copy data
     * \return	Returns the size available to use (i.e. remaining space).
     **/
    virtual unsigned int resize(unsigned int size, bool copy);

    /**
     * \brief   Returns reference to the byte buffer.
     **/
    const NEMemory::sByteBuffer & getByteBuffer( void)  const;

    /**
     * \brief   Returns reference to the byte buffer.
     **/
    NEMemory::sByteBuffer & getByteBuffer( void );

    /**
     * \brief   Returns true if buffer is either empty or is invalid.
     **/
    bool isEmpty( void ) const;

    /**
     * \brief   Returns the use
     **/
    unsigned int getSizeUsed( void ) const;
    
    /**
     * \brief   Returns the constant pointer to the data buffer of byte buffer
     **/
    const unsigned char * getBuffer( void ) const;

    /**
     * \brief   Returns pointer to the data buffer of byte buffer
     **/
    unsigned char * getBuffer( void );

    /**
     * \brief   Checks whether the buffer is valid or not.
     *          Byte buffer is invalid if it is equal to NEMemory::InvalidBuffer
     *          No read / write should be performed on invalid buffer.
     *          The memory used by invalid buffer should not be freed / deleted.
     **/
    inline bool isValid( void ) const;
    
    /**
     * \brief   Returns the size in byte of byte buffer
     *          If empty, returns zero
     **/
    inline unsigned int getSizeAvailable( void ) const;

    /**
     * \brief   Returns type of byte buffer. 
     *          If valid, it should be either internal or external (for RPC)
     **/
    inline NEMemory::eBufferType getType( void ) const;

//////////////////////////////////////////////////////////////////////////
// Protected internal members
//////////////////////////////////////////////////////////////////////////
protected:

    /**
     * \brief	Sets the used size value in byte buffer object.
     *          It should not be more than the length of buffer.
     * \param	newSize	The new size in bytes to set in byte buffer object.
     **/
    inline void setSizeUsed(unsigned int newSize);

    /**
     * \brief   Returns read-only end-of-buffer, i.e. end of used space. The end of buffer means 
     *          there is no data to read. Can be only used to manipulate with cursor.
     **/
    inline const unsigned char * getEndOfBuffer( void ) const;

    /**
     * \brief   Returns end-of-buffer, i.e. end of used space. So that, if length of buffer is
     *          bigger than used space, there can be still data written. 
     **/
    inline unsigned char * getEndOfBuffer( void );

/************************************************************************/
// IEByteBuffer protected overrides
/************************************************************************/

    /**
     * \brief	Initialize passed buffer making it as byte buffer and
     *          returns the current writing position in initialized byte buffer.
     *          If needed, it will copy existing binary data to passed buffer.
     *          The function is called from child classes when new byte buffer
     *          is created and the existing data should be passed to new buffer.
     * \param	newBuffer	The buffer to initialize. If NULL, the buffer object. 
     * \param	bufLength	The length of entire buffer, i.e. length of complete byte buffer.
     * \param	makeCopy	If 'true' it will make copy of existing buffer
     * \return	Returns the current writing position in initialized buffer.
     *          If buffer is invalid, it will return INVALID_CURSOR_POSITION.
     *          If no data is copied, it will return position at the beginning of buffer.
     *          If data is copied, will return the position of written data.
     **/
    virtual unsigned int initBuffer(unsigned char * newBuffer, unsigned int bufLength, bool makeCopy) const;

    /**
     * \brief   Returns the size to align the buffer. By default it is sizeof(NEMemory::uAlign)
     **/
    virtual unsigned int getAlignedSize( void ) const;

    /**
     * \brief   Returns the offset value from the beginning of byte buffer, which should be set
     **/
    virtual int getDataOffset( void ) const = 0;

    /**
     * \brief   Returns the size of data byte structure (header and one byte).
     *          This is a minimum size of byte buffer to reserve when initializing buffer.
     *          The size can differ for shared and remote message.
     **/
    virtual int getHeaderSize( void ) const = 0;

//////////////////////////////////////////////////////////////////////////
// Member variables
//////////////////////////////////////////////////////////////////////////

    /**
     * \brief   Pointer to Byte Buffer structure. Must not be NULL. When invalidated, points to NEMemory::InvalidBuffer object.
     **/
    mutable NEMemory::sByteBuffer * mByteBuffer;

//////////////////////////////////////////////////////////////////////////
// Forbidden calls
//////////////////////////////////////////////////////////////////////////
private:
    IEByteBuffer( void );
    IEByteBuffer( const IEByteBuffer & /*src*/ );
    const IEByteBuffer & operator = ( const IEByteBuffer & /*src*/ );
};

//////////////////////////////////////////////////////////////////////////
// IEByteBuffer class inline function implementation
//////////////////////////////////////////////////////////////////////////

inline const NEMemory::sByteBuffer & IEByteBuffer::getByteBuffer(void) const
{
    ASSERT(mByteBuffer != NULL);
    return (*mByteBuffer);
}

inline NEMemory::sByteBuffer & IEByteBuffer::getByteBuffer(void)
{
    ASSERT(mByteBuffer != NULL);
    return (*mByteBuffer);
}

inline bool IEByteBuffer::isEmpty( void ) const
{
    return (isValid() == false) || (mByteBuffer->bufHeader.biUsed == 0);
}

inline unsigned int IEByteBuffer::getSizeUsed( void ) const
{
    return (isValid() ? mByteBuffer->bufHeader.biUsed : 0);
}

inline const unsigned char* IEByteBuffer::getBuffer( void ) const
{
    return NEMemory::getBufferDataRead(mByteBuffer);
}

inline unsigned char* IEByteBuffer::getBuffer( void )
{
    return NEMemory::getBufferDataWrite(mByteBuffer);
}

inline bool IEByteBuffer::isValid( void ) const
{
    return NEMemory::isBufferValid(mByteBuffer);
}

inline unsigned int IEByteBuffer::getSizeAvailable( void ) const
{
    ASSERT(mByteBuffer != NULL);
    return (isValid() ? mByteBuffer->bufHeader.biLength : 0);
}

inline NEMemory::eBufferType IEByteBuffer::getType( void ) const
{
    ASSERT(mByteBuffer != NULL);
    return mByteBuffer->bufHeader.biBufType;
}

inline void IEByteBuffer::setSizeUsed(unsigned int newSize)
{
    if (isValid() && newSize <= getSizeAvailable())
        mByteBuffer->bufHeader.biUsed = newSize;
}

const unsigned char * IEByteBuffer::getEndOfBuffer(void) const
{
    return (isValid() ? NEMemory::getBufferDataRead(mByteBuffer) + mByteBuffer->bufHeader.biUsed : NULL);    
}

unsigned char * IEByteBuffer::getEndOfBuffer(void)
{
    return (isValid() ? NEMemory::getBufferDataWrite(mByteBuffer) + mByteBuffer->bufHeader.biUsed : NULL);    
}

#endif  // AREG_BASE_IEBYTEBUFFER_HPP
