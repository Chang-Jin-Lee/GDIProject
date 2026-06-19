#pragma once

#include <map>
#include <string>
#include <vector>

struct CsvRow
{
	std::map<std::string, std::string> Values;

	std::string Get(const std::string& key, const std::string& fallback = "") const;
};

class CsvTable
{
public:
	bool LoadFromFile(const std::wstring& path);
	bool LoadFromString(const std::string& text);

	const std::vector<CsvRow>& Rows() const { return m_rows; }
	const std::string& Error() const { return m_error; }

	static std::string Trim(const std::string& value);
	static std::vector<std::string> SplitLine(const std::string& line);

private:
	std::vector<CsvRow> m_rows;
	std::string m_error;
};
