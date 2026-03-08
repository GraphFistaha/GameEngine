#include <cassert>
#include <fstream>
#include <stdexcept>

#include "FileStream.hpp"

namespace GameFramework
{
class StandardFileStream : public IBinaryFileReader,
                           public IBinaryFileWriter
{
public:
  explicit StandardFileStream(const std::filesystem ::path & path, bool isRead);
  virtual ~StandardFileStream() = default;

public: // IFileStream
  virtual std::filesystem::path FullPath() const override { return m_path; }
  /// get position of caret in file
  virtual size_t Tell() override { return m_stream.tellg(); }
  /// move writing caret in the file
  virtual void Seek(std::ptrdiff_t offset, SeekDirection dir) override;

public: // IBinaryFileReader
  /// read bytes from stream
  virtual size_t Read(std::span<std::byte> buffer) override;
  /// get size of file
  virtual size_t Size() const override { return m_fileSize; }

public: //IBinaryFileWriter
  /// write bytes into stream
  virtual void Write(std::span<const std::byte> data) override;
  /// finish all writing operations
  virtual void Flush() override { m_stream.flush(); }

private:
  std::filesystem::path m_path;
  std::fstream m_stream;
  size_t m_fileSize = 0;
};


StandardFileStream::StandardFileStream(const std::filesystem::path & path, bool isRead)
  : m_path(path)
{
  std::ios_base::openmode mode = std::ios::ate | std::ios_base::binary;
  if (isRead)
    mode |= std::ios::in;
  else
    mode |= std::ios::out;
  m_stream.open(path, mode);
  if (!m_stream.is_open() || m_stream.bad())
    throw std::runtime_error("Failed to open file");
  if (isRead)
  {
    m_fileSize = m_stream.tellg();
    m_stream.seekg(0, std::ios::beg);
  }
}

size_t StandardFileStream::Read(std::span<std::byte> buffer)
{
  m_stream.read(reinterpret_cast<char *>(buffer.data()), buffer.size_bytes());
  return m_stream.gcount();
}

void StandardFileStream::Seek(std::ptrdiff_t offset, SeekDirection dir)
{
  m_stream.seekg(offset, static_cast<std::ios::seekdir>(dir));
  m_stream.seekp(offset, static_cast<std::ios::seekdir>(dir));
}

void StandardFileStream::Write(std::span<const std::byte> data)
{
  m_stream.write(reinterpret_cast<const char *>(data.data()), data.size_bytes());
}


GAME_FRAMEWORK_API BinaryFileWriterUPtr OpenBinaryFileWrite(const std::filesystem::path & path)
{
  return std::make_unique<StandardFileStream>(path, false /*isRead*/);
}


GAME_FRAMEWORK_API BinaryFileReaderUPtr OpenBinaryFileRead(const std::filesystem::path & path)
{
  return std::make_unique<StandardFileStream>(path, true /*isRead*/);
}

} // namespace GameFramework


namespace GameFramework
{
class StandardTextFileStream : public ITextFileReader,
                               public ITextFileWriter
{
public:
  explicit StandardTextFileStream(const std::filesystem::path & path, bool isRead);
  virtual ~StandardTextFileStream() = default;

public: // IFileStream
  virtual std::filesystem::path FullPath() const override { return m_path; }
  /// get position of caret in file
  virtual size_t Tell() override { return m_stream.tellg(); }
  /// move writing caret in the file
  virtual void Seek(std::ptrdiff_t offset, SeekDirection dir) override;

public: //ITextFileReader
  /// get size of file
  virtual size_t Size() const override { return m_fileSize; }
  /// read bytes from stream
  virtual wchar_t ReadChar() override;
  /// watch next character from file, without reading
  virtual wchar_t PeekChar() override;
  /// read bytes from stream
  virtual size_t ReadLine(std::wstring & line, wchar_t delim) override;

public: // ITextFileWriter
  /// read next character from file
  virtual void WriteChar(wchar_t ch) override;
  /// reads line from file
  virtual void WriteLine(const std::wstring_view & line) override;
  /// finish all writing operations
  virtual void Flush() override { m_stream.flush(); }

private:
  std::filesystem::path m_path;
  std::wfstream m_stream;
  size_t m_fileSize = 0;
};

StandardTextFileStream::StandardTextFileStream(const std::filesystem::path & path, bool isRead)
  : m_path(path)
  , m_stream(path, isRead ? std::ios::in | std::ios::ate : std::ios::out | std::ios::ate)
{
  if (!m_stream.is_open() || m_stream.bad())
    throw std::runtime_error("Failed to open file");
  if (isRead)
  {
    m_fileSize = m_stream.tellg();
    m_stream.seekg(0, std::ios::beg);
  }
}

wchar_t StandardTextFileStream::ReadChar()
{
  wchar_t ch;
  m_stream.get(ch);
  return ch;
}

void StandardTextFileStream::Seek(std::ptrdiff_t offset, SeekDirection dir)
{
  m_stream.seekg(offset, static_cast<std::ios::seekdir>(dir));
  m_stream.seekp(offset, static_cast<std::ios::seekdir>(dir));
}

wchar_t StandardTextFileStream::PeekChar()
{
  return static_cast<wchar_t>(m_stream.peek());
}

size_t StandardTextFileStream::ReadLine(std::wstring & line, wchar_t delim)
{
  size_t oldLength = line.size();
  std::getline(m_stream, line, delim);
  return line.size() - oldLength;
}

void StandardTextFileStream::WriteChar(wchar_t ch)
{
  m_stream.put(ch);
  m_fileSize++;
}

void StandardTextFileStream::WriteLine(const std::wstring_view & line)
{
  m_stream << line << std::endl;
  m_fileSize += line.size();
}

GAME_FRAMEWORK_API TextFileReaderUPtr OpenTextFileRead(const std::filesystem::path & path)
{
  return std::make_unique<StandardTextFileStream>(path, true);
}

GAME_FRAMEWORK_API TextFileWriterUPtr OpenTextFileWrite(const std::filesystem::path & path)
{
  return std::make_unique<StandardTextFileStream>(path, false);
}
} // namespace GameFramework
