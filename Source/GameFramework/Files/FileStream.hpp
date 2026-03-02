#pragma once
#include <GameFramework_def.h>

#include <filesystem>
#include <memory>
#include <span>
#include <string>
#include <type_traits>

namespace GameFramework
{

enum class SeekDirection
{
  Begin,
  Current,
  End
};
struct IBinaryFileReader;
struct IBinaryFileWriter;
struct ITextFileReader;
struct ITextFileWriter;

template<typename T>
concept BinaryReadable = requires(IBinaryFileReader & stream, T & obj) {
  { T::ReadBinary(stream, obj) } -> std::convertible_to<size_t>;
};

template<typename T>
concept BinaryWritable = requires(IBinaryFileWriter & stream, const T & obj) {
  { T::WriteBinary(stream, obj) };
};

template<typename T>
concept TextReadable = requires(ITextFileReader & stream, T & obj) {
  { T::ReadText(stream, obj) } -> std::convertible_to<size_t>;
};

template<typename T>
concept TextWriteable = requires(ITextFileWriter & stream, T & obj) {
  { T::WriteText(stream, obj) };
};

struct IFileStream
{
  virtual ~IFileStream() = default;
  virtual std::filesystem::path FullPath() const = 0;
  /// get position of caret in file
  virtual size_t Tell() = 0;
  /// move writing caret in the file
  virtual void Seek(std::ptrdiff_t offset, SeekDirection dir) = 0;
};

struct IBinaryFileReader : public IFileStream
{
  virtual ~IBinaryFileReader() = default;
  /// read bytes from stream
  virtual size_t Read(std::span<std::byte> buffer) = 0;
  /// get size of file
  virtual size_t Size() const = 0;

  template<typename T>
  size_t ReadValue(T & val);

  /// reads trivially copyable value from stream
  template<typename T>
    requires(std::is_trivial_v<T>)
  size_t ReadValue(T & value)
  {
    return Read(std::as_writable_bytes(std::span{&value, 1}));
  }

  template<typename StrT>
    requires(std::is_convertible_v<StrT, std::string_view> ||
             std::is_convertible_v<StrT, std::wstring_view>)
  size_t ReadValue(StrT & val)
  {
    return Read(std::as_writable_bytes(std::span{val}));
  }

  template<BinaryReadable T>
  size_t ReadValue(T & value)
  {
    return T::ReadBinary(*this, value);
  }
};

struct ITextFileReader : public IFileStream
{
  /// get size of file
  virtual size_t Size() const = 0;
  /// read next character from file
  virtual wchar_t ReadChar() = 0;
  /// watch next character from file, without reading
  virtual wchar_t PeekChar() = 0;
  /// reads line from file
  virtual size_t ReadLine(std::wstring & line, wchar_t delim = L'\n') = 0;

  template<TextReadable T>
  size_t ReadValue(T & obj)
  {
    return T::ReadText(*this, obj);
  }
};

struct IBinaryFileWriter : public IFileStream
{
  virtual ~IBinaryFileWriter() = default;
  /// write bytes into stream
  virtual void Write(std::span<const std::byte> data) = 0;
  /// finish all writing operations
  virtual void Flush() = 0;

  template<typename T>
  void WriteValue(const T & value);

  template<typename T>
    requires(std::is_trivial_v<T>)
  void WriteValue(const T & value)
  {
    Write(std::as_bytes(std::span{&value, 1}));
  }

  template<BinaryWritable T>
  void WriteValue(const T & object)
  {
    T::WriteBinary(*this, object);
  }

  template<typename StrT>
    requires(std::is_convertible_v<StrT, std::string_view> ||
             std::is_convertible_v<StrT, std::wstring_view>)
  void WriteValue(const StrT & value)
  {
    Write(std::as_bytes(std::span{value}));
  }
};

struct ITextFileWriter : public IFileStream
{
  /// read next character from file
  virtual void WriteChar(wchar_t ch) = 0;
  /// reads line from file
  virtual void WriteLine(const std::wstring_view & line) = 0;
  /// finish all writing operations
  virtual void Flush() = 0;

  template<TextWriteable T>
  void WriteValue(const T & obj)
  {
    return T::WriteText(*this, obj);
  }
};

using BinaryFileReaderUPtr = std::unique_ptr<IBinaryFileReader>;
using BinaryFileWriterUPtr = std::unique_ptr<IBinaryFileWriter>;
using TextFileReaderUPtr = std::unique_ptr<ITextFileReader>;
using TextFileWriterUPtr = std::unique_ptr<ITextFileWriter>;

} // namespace GameFramework

namespace GameFramework
{

GAME_FRAMEWORK_API BinaryFileWriterUPtr OpenBinaryFileWrite(const std::filesystem::path & path);
GAME_FRAMEWORK_API BinaryFileReaderUPtr OpenBinaryFileRead(const std::filesystem::path & path);
GAME_FRAMEWORK_API TextFileReaderUPtr OpenTextFileRead(const std::filesystem::path & path);
GAME_FRAMEWORK_API TextFileWriterUPtr OpenTextFileWrite(const std::filesystem::path & path);

} // namespace GameFramework
