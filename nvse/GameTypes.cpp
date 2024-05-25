#include "nvse/GameTypes.h"

void String::Init(UInt32 bufSize)
{
	if (m_data) Game_HeapFree(m_data);
	m_bufLen = bufSize;
	m_data = Game_HeapAlloc(m_bufLen + 1);
	*m_data = 0;
	m_dataLen = 0;
}

void String::Set(const char *src)
{
	m_dataLen = StrLen(src);
	if (!m_dataLen)
	{
		if (!src)
		{
			if (m_data)
			{
				Game_HeapFree(m_data);
				m_data = NULL;
			}
			m_bufLen = 0;
		}
		else if (m_data) *m_data = 0;
		return;
	}
	if (m_bufLen < m_dataLen)
	{
		m_bufLen = m_dataLen;
		if (m_data) Game_HeapFree(m_data);
		m_data = Game_HeapAlloc(m_dataLen + 1);
	}
	COPY_BYTES(m_data, src, m_dataLen + 1);
}

void String::Append(const char *toAppend)
{
	UInt16 length = StrLen(toAppend);
	if (!length) return;
	UInt16 newLen = m_dataLen + length;
	if (m_bufLen < newLen)
	{
		m_bufLen = newLen;
		char *newStr = Game_HeapAlloc(m_bufLen + 1);
		if (m_data)
		{
			COPY_BYTES(newStr, m_data, m_dataLen);
			Game_HeapFree(m_data);
		}
		m_data = newStr;
	}
	COPY_BYTES(m_data + m_dataLen, toAppend, length + 1);
	m_dataLen = newLen;
}

void String::AppendChar(char toAppend)
{
	if (m_bufLen == m_dataLen)
	{
		m_bufLen++;
		char *newStr = Game_HeapAlloc(m_bufLen + 1);
		if (m_data)
		{
			COPY_BYTES(newStr, m_data, m_dataLen);
			Game_HeapFree(m_data);
		}
		m_data = newStr;
	}
	m_data[m_dataLen++] = toAppend;
	m_data[m_dataLen] = 0;
}

void String::InsertChar(char toInsert, UInt32 index)
{
	if (index > m_dataLen) return;
	if (m_bufLen == m_dataLen)
	{
		m_bufLen++;
		char *newStr = Game_HeapAlloc(m_bufLen + 1);
		if (m_data)
		{
			COPY_BYTES(newStr, m_data, m_dataLen);
			Game_HeapFree(m_data);
		}
		m_data = newStr;
	}
	if (index < m_dataLen)
		MemCopy(m_data + index + 1, m_data + index, m_dataLen - index);
	m_dataLen++;
	m_data[index] = toInsert;
	m_data[m_dataLen] = 0;
}

void String::EraseAt(UInt32 index)
{
	if (!m_data || (index >= m_dataLen)) return;
	m_dataLen--;
	if (m_dataLen)
	{
		UInt32 length = m_dataLen - index;
		if (!length)
			m_data[m_dataLen] = 0;
		else
			COPY_BYTES(m_data + index, m_data + index + 1, length + 1);
	}
	else *m_data = 0;
}

const char *String::CStr()
{
	return (m_data && m_dataLen) ? m_data : "";
}