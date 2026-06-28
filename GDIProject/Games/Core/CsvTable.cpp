#include "CsvTable.h"

#include <filesystem>
#include <fstream>
#include <sstream>

std::string CsvRow::Get(const std::string& key, const std::string& fallback) const
{
	const auto it = Values.find(key);
	return it == Values.end() ? fallback : it->second;
}

bool CsvTable::LoadFromFile(const std::wstring& path)
{
	std::ifstream file{ std::filesystem::path(path) };
	if (!file.is_open())
	{
		m_error = "file open failed";
		m_rows.clear();
		return false;
	}

	std::ostringstream buffer;
	buffer << file.rdbuf();
	return LoadFromString(buffer.str());
}

bool CsvTable::LoadFromString(const std::string& text)
{
	m_rows.clear();
	m_error.clear();

	std::istringstream input(text);
	std::string line;
	std::vector<std::string> headers;

	while (std::getline(input, line))
	{
		if (!line.empty() && line.back() == '\r')
		{
			line.pop_back();
		}

		if (Trim(line).empty())
		{
			continue;
		}

		if (headers.empty())
		{
			headers = SplitLine(line);
			if (headers.empty())
			{
				m_error = "missing header";
				return false;
			}
			continue;
		}

		const std::vector<std::string> cells = SplitLine(line);
		if (cells.size() != headers.size())
		{
			m_error = "column count mismatch";
			m_rows.clear();
			return false;
		}

		CsvRow row;
		for (size_t i = 0; i < headers.size(); ++i)
		{
			row.Values[headers[i]] = cells[i];
		}
		m_rows.push_back(row);
	}

	if (headers.empty())
	{
		m_error = "empty csv";
		return false;
	}

	return true;
}

std::string CsvTable::Trim(const std::string& value)
{
	size_t begin = 0;
	while (begin < value.size() && static_cast<unsigned char>(value[begin]) <= ' ')
	{
		++begin;
	}

	size_t end = value.size();
	while (end > begin && static_cast<unsigned char>(value[end - 1]) <= ' ')
	{
		--end;
	}

	return value.substr(begin, end - begin);
}

std::vector<std::string> CsvTable::SplitLine(const std::string& line)
{
	std::vector<std::string> cells;
	std::string cell;
	std::istringstream stream(line);
	while (std::getline(stream, cell, ','))
	{
		cells.push_back(Trim(cell));
	}

	if (!line.empty() && line.back() == ',')
	{
		cells.emplace_back();
	}

	return cells;
}
