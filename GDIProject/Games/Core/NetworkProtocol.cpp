#include "NetworkProtocol.h"

#include "CsvTable.h"

#include <sstream>

NetworkLine ParseNetworkLine(const std::string& line)
{
	NetworkLine result;
	std::stringstream stream(line);
	std::string token;

	if (!std::getline(stream, token, '|'))
	{
		result.Error = "empty line";
		return result;
	}

	result.Name = CsvTable::Trim(token);
	if (result.Name.empty())
	{
		result.Error = "missing name";
		return result;
	}

	while (std::getline(stream, token, '|'))
	{
		const size_t equals = token.find('=');
		if (equals == std::string::npos || equals == 0)
		{
			result.Error = "malformed field";
			return result;
		}

		const std::string key = CsvTable::Trim(token.substr(0, equals));
		const std::string value = CsvTable::Trim(token.substr(equals + 1));
		if (key.empty())
		{
			result.Error = "empty key";
			return result;
		}
		result.Fields[key] = value;
	}

	result.Valid = true;
	return result;
}

std::string BuildNetworkLine(const std::string& name, const std::map<std::string, std::string>& fields)
{
	std::ostringstream out;
	out << name;
	for (const auto& pair : fields)
	{
		out << '|' << pair.first << '=' << pair.second;
	}
	return out.str();
}
